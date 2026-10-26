"""Synthetic capture integrity cases; no Yamaha data."""
import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from sci_trace import parse_sci_observation


def state(tick, channel, scr="00"):
    return f"[:] YAMANIX SCI_STATE tick={tick} channel={channel} smr=00 brr=0f scr={scr} ssr=84 hardware_validated=0"


class SciTraceTests(unittest.TestCase):
    def capture(self, count=1):
        return [state(0, 0), state(0, 1), state(100, 0), state(100, 1, "70"),
                "[:] YAMANIX SCI1_TDR write=1 data=5a pc=000100",
                f"[:] YAMANIX SCI1_END tdr_writes={count} rdr_reads=0 hardware_validated=0"]

    def test_separate_channels_and_legacy_capture(self):
        self.assertIsNone(parse_sci_observation(["YAMANIX PC tick=0 pc=000000"]))
        result = parse_sci_observation(self.capture())
        self.assertIsNone(result["channels"][0]["first_receiver_enabled_tick"])
        self.assertEqual(result["channels"][1]["first_receiver_enabled_tick"], 100)
        self.assertEqual(result["keyboard_port"]["tx_prefix"][0]["data"], 0x5a)
        self.assertFalse(result["firmware_note_handling_verified"])

    def test_incomplete_or_duplicate_observations_fail(self):
        source = self.capture()
        for lines in (source[:-1], source[:1] + source[2:], source[:2] + source,
                      source + [source[-1]], self.capture(2),
                      [source[0].replace("smr=00", "smr=100")] + source[1:]):
            with self.subTest(lines=lines), self.assertRaises(ValueError):
                parse_sci_observation(lines)

    def test_bounded_transmit_prefix(self):
        lines = [state(0, 0), state(0, 1)]
        lines += [f"YAMANIX SCI1_TDR write={i} data=5a pc=000100" for i in range(1, 129)]
        lines += ["YAMANIX SCI1_END tdr_writes=200 rdr_reads=4 hardware_validated=0"]
        result = parse_sci_observation(lines)
        self.assertFalse(result["keyboard_port"]["tx_prefix_complete"])
        self.assertEqual(result["keyboard_port"]["rdr_reads"], 4)
        with self.assertRaises(ValueError):
            parse_sci_observation(lines[:-2] + lines[-1:])


if __name__ == "__main__":
    unittest.main()
