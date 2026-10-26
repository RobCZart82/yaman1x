from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from an1x_upload import decode_image, image_events, verify_upload


def image():
    return b"".join(struct.pack(">8H", step % 8, (step//4 + 1) if step % 4 == (step//4)%4 else 0,
                                 step, step, step+1, step+2, step+3, step+4) for step in range(512))


class ImageTests(unittest.TestCase):
    def test_distributed_offsets_and_complete_upload(self):
        blob = image()
        rows, slots = decode_image(blob)
        self.assertEqual(slots, list(range(1, 129)))
        report = verify_upload(blob, image_events(blob))
        self.assertTrue(report["matches"])
        self.assertEqual(report["program_words"], 2560)

    def test_conflict_truncation_and_wrong_data(self):
        blob = image()
        conflict = bytearray(blob)
        conflict[18:20] = b"\x12\x34"
        with self.assertRaises(ValueError):
            decode_image(conflict)
        with self.assertRaises(ValueError):
            decode_image(blob[:-1])
        events = image_events(blob)
        events[10]["data"] ^= 1
        with self.assertRaises(ValueError):
            verify_upload(blob, events)
        with self.assertRaises(ValueError):
            verify_upload(blob, image_events(blob)[:-1])
