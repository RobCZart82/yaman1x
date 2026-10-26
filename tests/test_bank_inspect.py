import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from bank_inspect import parse_bank


def bulk(address, payload):
    count = len(payload)
    body = bytes([count >> 7, count & 127, *address]) + payload
    return b"\xf0\x43\x00\x5c" + body + bytes([-sum(body) & 127, 247])


class BankTests(unittest.TestCase):
    def test_complete_bank_and_names(self):
        data = b"".join(bulk((17, slot, 0), b"TestVoice " + bytes(1932)) for slot in range(128))
        report = parse_bank(data)
        self.assertEqual(report["complete_voice_banks"], [0])
        self.assertEqual(report["voices"][-1]["slot"], 128)
        self.assertFalse(report["factory_content_verified"])

    def test_corruption_and_truncation(self):
        good = bulk((0, 0, 0), bytes(28))
        corrupt = bytearray(good)
        corrupt[9] ^= 1
        for data in (b"", good[:-1], good + b"\x00", bytes(corrupt), good + good):
            with self.subTest(data_length=len(data)), self.assertRaises(ValueError):
                parse_bank(data)

    def test_partial_dump_not_claimed_complete(self):
        result = parse_bank(bulk((17, 0, 0), b"Partial   " + bytes(1932)))
        self.assertEqual(result["complete_voice_banks"], [])
        unknown = parse_bank(bulk((16, 0, 0), bytes(10)))
        self.assertEqual(unknown["kinds"], {"unclassified_bulk": 1})
