"""Strict, read-only AN1x raw SysEx bulk-dump inspector (not MIDI/AN1)."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path


def parse_bank(data):
    packets = []
    offset = 0
    seen = set()
    voices = []
    patterns = []
    while offset < len(data):
        if data[offset] != 0xF0:
            raise ValueError(f"Expected F0 at byte {offset}")
        end = data.find(b"\xf7", offset + 1)
        if end < 0:
            raise ValueError(f"Unterminated message at byte {offset}")
        packet = data[offset:end + 1]
        if len(packet) < 11 or any(x > 127 for x in packet[1:-1]):
            raise ValueError(f"Invalid message bytes at {offset}")
        if packet[1] != 0x43 or packet[2] > 15 or packet[3] != 0x5C:
            raise ValueError(f"Unsupported manufacturer, bulk type or model at {offset}")
        count = (packet[4] << 7) | packet[5]
        if count != len(packet) - 11:
            raise ValueError(f"Length mismatch at {offset}")
        if sum(packet[4:-1]) % 128:
            raise ValueError(f"Checksum mismatch at {offset}")
        address = tuple(packet[6:9])
        key = (packet[2], address)
        if key in seen:
            raise ValueError(f"Duplicate device/address at {offset}: {address}")
        seen.add(key)
        payload = packet[9:-2]
        item = {"offset": offset, "device": packet[2], "address": list(address),
                "payload_bytes": len(payload), "sha256": hashlib.sha256(payload).hexdigest()}
        # Recognize complete stored blocks only; partial/edit-buffer dumps remain listed.
        if address[0] == 0x11 and address[2] == 0 and count == 1942:
            name = payload[:10]
            if any(x < 32 or x > 126 for x in name):
                raise ValueError(f"Invalid voice name at {offset}")
            item.update(kind="user_voice", slot=address[1] + 1,
                        name=name.decode("ascii").rstrip())
            voices.append({"device": packet[2], "slot": item["slot"], "name": item["name"]})
        elif address[0] == 1 and address[2] == 0 and count == 70:
            item.update(kind="user_pattern", slot=address[1] + 1)
            patterns.append((packet[2], address[1] + 1))
        elif address == (0, 0, 0) and count == 28:
            item["kind"] = "system"
        else:
            item["kind"] = "unclassified_bulk"
        packets.append(item)
        offset = end + 1
    if not packets:
        raise ValueError("Empty bank")
    devices = sorted({p["device"] for p in packets})
    return {"bytes": len(data), "sha256": hashlib.sha256(data).hexdigest(),
            "packets": len(packets), "kinds": dict(Counter(p["kind"] for p in packets)),
            "complete_voice_banks": [d for d in devices if {v["slot"] for v in voices if v["device"] == d} == set(range(1, 129))],
            "complete_pattern_banks": [d for d in devices if {s for dev, s in patterns if dev == d} == set(range(1, 129))],
            "factory_content_verified": False, "voices": voices, "blocks": packets}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bank", type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(parse_bank(args.bank.read_bytes()), indent=2, ensure_ascii=False))
    except (OSError, ValueError) as error:
        parser.exit(2, f"Bank input error: {error}\n")


if __name__ == "__main__":
    main()
