"""Audit observed H8 SCI state and keyboard-port traffic; no hardware claims."""
import argparse
import json
from pathlib import Path
import re

STATE = re.compile(r"SCI_STATE tick=(\d+) channel=([01]) smr=([0-9a-f]{2}) brr=([0-9a-f]{2}) scr=([0-9a-f]{2}) ssr=([0-9a-f]{2}) hardware_validated=0$")
TX = re.compile(r"SCI1_TDR write=(\d+) data=([0-9a-f]{2}) pc=([0-9a-f]{6})$")
END = re.compile(r"SCI1_END tdr_writes=(\d+) rdr_reads=(\d+) hardware_validated=0$")


def parse_sci_observation(lines):
    """Return None for older captures; reject incomplete new observations."""
    snapshots, tx, footer = [], [], None
    for line in lines:
        if "YAMANIX " not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        if not body.startswith(("SCI_STATE", "SCI1_TDR", "SCI1_END")):
            continue
        if footer is not None:
            raise ValueError("SCI observations after completion footer")
        if body.startswith("SCI_STATE"):
            match = STATE.fullmatch(body)
            if not match:
                raise ValueError("Malformed SCI state snapshot")
            tick, channel = map(int, match.groups()[:2])
            expected_channel = len(snapshots) % 2
            expected_tick = (len(snapshots) // 2) * 100
            if channel != expected_channel or tick != expected_tick:
                raise ValueError("Missing, repeated or unordered SCI state snapshot")
            snapshots.append(dict(tick=tick, channel=channel,
                                  **dict(zip(("smr", "brr", "scr", "ssr"),
                                             (int(x, 16) for x in match.groups()[2:])))))
        elif body.startswith("SCI1_TDR"):
            match = TX.fullmatch(body)
            if not match or int(match.group(1)) != len(tx) + 1 or len(tx) >= 128:
                raise ValueError("Malformed or incomplete SCI1 transmit prefix")
            tx.append(dict(data=int(match.group(2), 16), pc=int(match.group(3), 16)))
        else:
            match = END.fullmatch(body)
            if not match:
                raise ValueError("Malformed SCI1 completion footer")
            footer = dict(tdr_writes=int(match.group(1)), rdr_reads=int(match.group(2)))
    if not snapshots and not tx and footer is None:
        return None
    if not snapshots or len(snapshots) % 2 or footer is None:
        raise ValueError("Incomplete SCI observation")
    if len(tx) != min(footer["tdr_writes"], 128):
        raise ValueError("SCI1 transmit count disagrees with captured prefix")
    channels = []
    for channel in (0, 1):
        states = [s for s in snapshots if s["channel"] == channel]
        enabled = [s for s in states if s["scr"] & 0x10]
        channels.append(dict(channel=channel, snapshots=len(states),
                             last_state=states[-1],
                             first_receiver_enabled_tick=enabled[0]["tick"] if enabled else None))
    return dict(channels=channels, keyboard_port=dict(**footer, tx_prefix=tx,
                tx_prefix_complete=footer["tdr_writes"] <= 128),
                sample_interval_ns=1_000_000_000, timer_tick_ns=10_000_000,
                hardware_validated=False, firmware_note_handling_verified=False,
                note="Snapshots are observations at one-second intervals; TDR writes do not prove transmitted wire bytes or a PKS handshake.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path)
    args = parser.parse_args()
    try:
        manifest = json.loads((args.directory / "run.json").read_text())
        if manifest.get("returncode") != 0 or not manifest["rom_identity"]["matches_known_an1x_104"]:
            raise ValueError("Expected a successful known AN1x 1.04 run")
        result = parse_sci_observation((args.directory / "trace.log").read_text().splitlines())
        if result is None:
            raise ValueError("SCI observation patch is required")
        print(json.dumps(result, indent=2))
    except (OSError, ValueError, KeyError) as error:
        parser.exit(2, f"SCI audit error: {error}\n")


if __name__ == "__main__":
    main()
