"""Validate bounded, timestamped SCI bus observations, without PKS/boot claims."""
import re

BEGIN = "SCI_ACCESS_BEGIN version=1 capacity=4096 hardware_validated=0"
EVENT = re.compile(r"SCI_ACCESS seq=(\d+) time_ns=(\d+) channel=([01]) op=([WR]) reg=([0-5]) data=([0-9a-f]{2}) mask=([0-9a-f]{2}) pc=([0-9a-f]{6})$")
END = re.compile(r"SCI_ACCESS_END events=(\d+) dropped=(\d+) hardware_validated=0$")
REGISTERS = ("SMR", "BRR", "SCR", "TDR", "SSR", "RDR")


def parse_sci_access(lines):
    """Return None for legacy captures; reject loss, truncation and bad ordering."""
    begun, ended, events = False, False, []
    for line in lines:
        if "YAMANIX " not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        if not body.startswith("SCI_ACCESS"):
            continue
        if ended:
            raise ValueError("SCI access record after completion")
        if body == BEGIN:
            if begun:
                raise ValueError("Repeated SCI access header")
            begun = True
            continue
        if not begun:
            raise ValueError("SCI access header missing or malformed")
        match = END.fullmatch(body)
        if match:
            total, dropped = map(int, match.groups())
            if dropped or total != len(events):
                raise ValueError("SCI access capture is lossy or incomplete")
            ended = True
            continue
        match = EVENT.fullmatch(body)
        if not match:
            raise ValueError("Malformed SCI access record")
        seq, ns, channel = map(int, match.groups()[:3])
        op = match.group(4)
        reg = int(match.group(5))
        data, mask, pc = (int(value, 16) for value in match.groups()[5:])
        if seq != len(events) or seq >= 4096:
            raise ValueError("Missing, repeated or excessive SCI access event")
        if not mask or (op == "R" and reg != 5):
            raise ValueError("SCI access mask/register outside observer contract")
        if events and ns < events[-1]["time_ns"]:
            raise ValueError("SCI access time moved backwards")
        events.append(dict(seq=seq, time_ns=ns, channel=channel, op=op,
                           reg=reg, register=REGISTERS[reg], data=data, mask=mask, pc=pc))
    if not begun:
        return None
    if not ended:
        raise ValueError("SCI access completion footer missing")
    channels = []
    for channel in (0, 1):
        own = [event for event in events if event["channel"] == channel]
        writes = [event for event in own if event["op"] == "W"]
        channels.append(dict(
            channel=channel,
            config_writes=[event for event in writes if event["reg"] <= 2],
            receiver_enable_writes=[event for event in writes if event["reg"] == 2
                                    and event["mask"] & 0x10 and event["data"] & 0x10],
            tdr_writes=sum(event["reg"] == 3 for event in writes),
            rdr_reads=sum(event["op"] == "R" for event in own)))
    return dict(events=events, channels=channels, complete=True, hardware_validated=False,
                firmware_note_handling_verified=False, pks_handshake_verified=False,
                note="Bus accesses and observed CPU PCs only; RDR reads may be stale. No wire-byte, full instruction-flow, successful MIDI handling or PKS causality claim.")
