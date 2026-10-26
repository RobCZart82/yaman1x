"""Validate a complete passive MAME DSP trace and reconstruct host-upload state.

This models uploaded words, not chip execution, timing or firmware boot success.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import struct

EVENT = re.compile(r"DSP seq=(\d+) time_ns=(\d+) op=([RW]) chip=([01]) reg=([0-9a-f]{2}) data=([0-9a-f]{4}) mask=([0-9a-f]{4}) pc=([0-9a-f]{6})$")


def capture_profile(lines):
    profiles = []
    for line in lines:
        if "YAMANIX PROFILE" in line:
            body = line.split("YAMANIX ", 1)[1].strip()
            match = re.fullmatch(r"PROFILE dsp=(passive|firmware-contract) hardware_validated=0", body)
            if not match:
                raise ValueError("Unknown or invalid capture profile")
            profiles.append(match.group(1))
    if len(profiles) > 1:
        raise ValueError("Repeated or mixed capture profiles")
    profile = profiles[0] if profiles else "unlabelled"
    return dict(name=profile, experimental=profile == "firmware-contract", hardware_validated=False)


def parse_capture(lines):
    events = []
    began = ended = False
    last_time = 0
    for line in lines:
        if "YAMANIX DSP" not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        if body == "DSP_BEGIN version=1 clock_hz=1000000000 limit=200000":
            if began or ended:
                raise ValueError("Repeated capture header")
            began = True
        elif body.startswith("DSP_END"):
            match = re.fullmatch(r"DSP_END events=(\d+) dropped=(\d+)", body)
            if not began or ended or not match:
                raise ValueError("Invalid capture footer")
            count, dropped = map(int, match.groups())
            if dropped or count != len(events):
                raise ValueError("Capture truncated, dropped events or count mismatch")
            ended = True
        else:
            match = EVENT.fullmatch(body)
            if not began or ended or not match:
                raise ValueError("Invalid event or event outside capture")
            seq, time_ns, op, chip, reg, data, mask, pc = match.groups()
            item = dict(seq=int(seq), time_ns=int(time_ns), op=op, chip=int(chip),
                        reg=int(reg, 16), data=int(data, 16), mask=int(mask, 16), pc=int(pc, 16))
            if item["seq"] != len(events) or item["time_ns"] < last_time:
                raise ValueError("Event sequence gap or time reversed")
            if item["reg"] >= 128 or item["mask"] != 65535:
                raise ValueError("Unsupported register range or partial-word transaction")
            last_time = item["time_ns"]
            events.append(item)
    if not began or not ended or not events:
        raise ValueError("Missing header/footer or no captured events")
    return events


def reconstruct(events):
    chips = [dict(select=0, prog=[[0] * 5 for _ in range(512)], coef=[0] * 512,
                  tag=[0] * 512, slot=[[0, 0] for _ in range(128)]) for _ in range(2)]
    touched = [set(), set()]
    for e in events:
        if e["op"] != "W":
            continue
        c = chips[e["chip"]]
        reg, data = e["reg"], e["data"]
        if reg == 0:
            c["select"] = data
            continue
        step = c["select"] & 511
        if c["select"] & 0x8000:
            c["select"] = (c["select"] & ~511) | ((step + 1) & 511)
        if 6 <= reg <= 10:
            c["prog"][step][10 - reg] = data
            touched[e["chip"]].add((step, 10 - reg))
        elif reg == 11:
            c["coef"][step] = data
        elif reg == 12:
            c["tag"][step] = data & 255
        elif reg in (13, 14):
            c["slot"][step & 127][reg - 13] = data
    snapshot = bytearray()
    for c in chips:
        snapshot += struct.pack(">H", c["select"])
        for row in c["prog"]:
            snapshot += struct.pack(">5H", *row)
        snapshot += struct.pack(">512H", *c["coef"])
        snapshot += bytes(c["tag"])
        for row in c["slot"]:
            snapshot += struct.pack(">2H", *row)
    return bytes(snapshot), [len(t) for t in touched]


def analyze(events):
    snapshot, touched = reconstruct(events)
    chips = []
    for chip in range(2):
        subset = [e for e in events if e["chip"] == chip]
        chips.append(dict(chip=chip, reads=sum(e["op"] == "R" for e in subset),
                          writes=sum(e["op"] == "W" for e in subset),
                          program_word_writes=sum(e["op"] == "W" and 6 <= e["reg"] <= 10 for e in subset),
                          distinct_program_words=touched[chip], expected_program_words=2560,
                          registers=dict(Counter(f'{e["op"]}:{e["reg"]:02x}' for e in subset))))
    return dict(events=len(events), first_time_ns=events[0]["time_ns"], last_time_ns=events[-1]["time_ns"],
                snapshot_bytes=len(snapshot), snapshot_sha256=hashlib.sha256(snapshot).hexdigest(),
                audited_fields=["select", "program_words", "coefficients", "tags", "slot_offsets"],
                chips=chips, audio_rendered=False, complete_an1x_boot=False,
                note="Passive trace: captured reads are from the incomplete MAME machine, not hardware reference responses.")


def export_events(events):
    header = "# YamANIx DSP events v1: seq time_ns op chip reg data mask pc (decimal)\n"
    return header + "".join(" ".join(str(e[k]) for k in ("seq", "time_ns", "op", "chip", "reg", "data", "mask", "pc")) + "\n" for e in events) + f"# END events={len(events)}\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("trace", type=Path)
    parser.add_argument("--events-out", type=Path)
    parser.add_argument("--snapshot-out", type=Path)
    args = parser.parse_args()
    try:
        lines = args.trace.read_text(encoding="utf-8-sig").splitlines()
        events = parse_capture(lines)
        profile = capture_profile(lines)
        if args.events_out:
            args.events_out.write_text(export_events(events))
        if args.snapshot_out:
            args.snapshot_out.write_bytes(reconstruct(events)[0])
        result = analyze(events)
        result["capture_profile"] = profile
        if profile["experimental"]:
            result["note"] = "Experimental firmware-contract responses used for register 0x01; no hardware reference or audio validation."
            result["control_status_assumptions"] = {"request_command": 1, "stopped_status": 32768,
                                                    "release_command": 0, "released_status": 0,
                                                    "acknowledgement_timing_validated": False,
                                                    "other_status_bits_validated": False}
        print(json.dumps(result, indent=2))
    except (ValueError, OSError) as error:
        parser.exit(2, f"DSP trace error: {error}\n")


if __name__ == "__main__":
    main()
