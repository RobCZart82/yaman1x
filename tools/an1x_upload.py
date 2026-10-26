"""AN1x 512-row image upload adapter and comparison with a captured firmware upload.

Image format: 512 big-endian rows of tag, table offset, coefficient, five program words.
No image data is bundled. No audio or hardware-accuracy claim is made.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from dsp_trace import parse_capture, reconstruct, export_events, capture_profile


def decode_image(blob):
    if len(blob) != 8192:
        raise ValueError("Expected the 8192-byte AN1x voice image")
    rows = list(struct.iter_unpack(">8H", blob))
    slots = []
    for i in range(128):
        nonzero = {row[1] for row in rows[4*i:4*i+4] if row[1]}
        if len(nonzero) > 1:
            raise ValueError(f"Ambiguous table offsets in slot {i}")
        slots.append(next(iter(nonzero), 0))
    if any(row[0] > 255 for row in rows):
        raise ValueError("Tag does not fit the modeled byte register")
    return rows, slots


def image_events(blob, chip=0):
    if chip not in (0, 1):
        raise ValueError("Chip must be 0 or 1")
    rows, slots = decode_image(blob)
    events = []
    def write(reg, data):
        events.append(dict(seq=len(events), time_ns=0, op="W", chip=chip,
                           reg=reg, data=data, mask=65535, pc=0))
    for step, row in enumerate(rows):
        write(0, step)
        write(11, row[2])
        write(12, row[0])
        for word, value in enumerate(row[3:]):
            write(10-word, value)
    for slot, offset in enumerate(slots):
        write(0, slot)
        write(13, offset >> 9)
        write(14, offset & 511)
    return events


def verify_upload(blob, events, chip=0):
    decode_image(blob)
    # Find the final full five-word upload, before runtime changes to words 3 and 4.
    candidates = [i for i, e in enumerate(events)
                  if e["chip"] == chip and e["op"] == "W" and 8 <= e["reg"] <= 10]
    if not candidates:
        raise ValueError("No complete program-word upload found")
    end = candidates[-1] + 1
    tail = []
    while end < len(events):
        e = events[end]
        if e["chip"] == chip and e["op"] == "W":
            if e["reg"] == 0:
                break
            tail.append(e["reg"])
        end += 1
    if tail != [7, 6]:
        raise ValueError("Cannot identify the last complete program row")
    slots_end = end + 128*3
    if slots_end > len(events):
        raise ValueError("Truncated slot upload")
    for slot in range(128):
        triple = events[end+3*slot:end+3*slot+3]
        if [(e["chip"], e["op"], e["reg"]) for e in triple] != [(chip, "W", 0), (chip, "W", 13), (chip, "W", 14)]:
            raise ValueError("Unexpected slot upload order")
        if triple[0]["data"] != slot:
            raise ValueError("Unexpected slot selection")
    expected = reconstruct(image_events(blob, chip))[0]
    actual = reconstruct(events[:slots_end])[0]
    a, b = actual[chip*7170:(chip+1)*7170], expected[chip*7170:(chip+1)*7170]
    if a != b:
        mismatch = next(i for i, (x, y) in enumerate(zip(a, b)) if x != y)
        raise ValueError(f"Captured image differs at host snapshot byte {mismatch}")
    return dict(chip=chip, matches=True, program_words=2560, coefficients=512, tags=512,
                slots=128, checkpoint_events=slots_end, image_sha256=hashlib.sha256(blob).hexdigest(),
                chip_snapshot_sha256=hashlib.sha256(a).hexdigest(), hardware_validated=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("image", type=Path)
    parser.add_argument("--trace", type=Path)
    parser.add_argument("--chip", type=int, choices=(0, 1), default=0)
    parser.add_argument("--events-out", type=Path)
    args = parser.parse_args()
    try:
        blob = args.image.read_bytes()
        events = image_events(blob, args.chip)
        report = dict(image_sha256=hashlib.sha256(blob).hexdigest(), synthetic_upload_events=len(events),
                      audio_rendered=False, hardware_validated=False)
        if args.trace:
            lines = args.trace.read_text(encoding="utf-8-sig").splitlines()
            report.update(verify_upload(blob, parse_capture(lines), args.chip))
            report["capture_profile"] = capture_profile(lines)
        if args.events_out:
            args.events_out.write_text(export_events(events))
        print(json.dumps(report, indent=2))
    except (OSError, ValueError) as error:
        parser.exit(2, f"AN1x upload error: {error}\n")


if __name__ == "__main__":
    main()
