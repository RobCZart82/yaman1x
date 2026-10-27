"""Synthetic M1 access captures; no firmware or private traces."""
import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from sci_access_trace import BEGIN, parse_sci_access
from run_h8_probe import summarize_trace


def event(seq, ns, channel=0, op="W", reg=2, data="10", mask="ff"):
    return (f"YAMANIX SCI_ACCESS seq={seq} time_ns={ns} channel={channel} "
            f"op={op} reg={reg} data={data} mask={mask} pc=000100")


def capture(events):
    return ["YAMANIX " + BEGIN, *events,
            f"YAMANIX SCI_ACCESS_END events={len(events)} dropped=0 hardware_validated=0"]


class SciAccessTests(unittest.TestCase):
    def test_research_patch_embeds_the_tested_native_helper(self):
        root = Path(__file__).resolve().parents[1]
        patch = (root / "research/mame-sci-access-observation.patch").read_text()
        section = patch.split("+++ b/src/mame/yamaha/yamanix_sci_access_trace.h\n", 1)[1]
        embedded = "\n".join(line[1:] for line in section.splitlines()
                               if line.startswith("+")) + "\n"
        self.assertEqual(embedded, (root / "src/sci_access_trace.h").read_text())

    def test_short_enable_window_and_independent_channels(self):
        lines = capture([event(0, 10), event(1, 11, data="00"),
                         event(2, 11, channel=1, reg=3, data="5a"),
                         event(3, 12, op="R", reg=5, data="40")])
        result = parse_sci_access(lines)
        self.assertEqual(result["channels"][0]["receiver_enable_writes"][0]["time_ns"], 10)
        self.assertEqual(result["channels"][0]["rdr_reads"], 1)
        self.assertEqual(result["channels"][1]["tdr_writes"], 1)
        self.assertFalse(result["firmware_note_handling_verified"])
        self.assertFalse(result["pks_handshake_verified"])
        self.assertEqual(summarize_trace(lines)["sci_access_observation"], result)

    def test_legacy_empty_and_masked_write(self):
        self.assertIsNone(parse_sci_access(["YAMANIX PC tick=0 pc=000000"]))
        self.assertEqual(parse_sci_access(capture([]))["events"], [])
        result = parse_sci_access(capture([event(0, 0, mask="01")]))
        self.assertEqual(result["channels"][0]["receiver_enable_writes"], [])

    def test_corrupt_and_incomplete_streams_are_rejected(self):
        valid = capture([event(0, 10), event(1, 20)])
        broken = [valid[1:], valid[:-1], valid + [valid[-1]],
                  valid[:1] + valid, capture([event(1, 10)]),
                  capture([event(0, 20), event(1, 10)]),
                  capture([event(0, 0, mask="00")]),
                  capture([event(0, 0, op="R", reg=4)]),
                  capture([event(0, 0, data="100")]),
                  [line.replace("events=2", "events=3") for line in valid],
                  [line.replace("dropped=0", "dropped=1") for line in valid],
                  [line.replace("version=1", "version=2") for line in valid]]
        for lines in broken:
            with self.subTest(lines=lines), self.assertRaises(ValueError):
                parse_sci_access(lines)

    def test_capture_bound_is_enforced(self):
        events = [event(seq, seq) for seq in range(4096)]
        self.assertEqual(len(parse_sci_access(capture(events))["events"]), 4096)
        with self.assertRaises(ValueError):
            parse_sci_access(capture(events + [event(4096, 4096)]))
