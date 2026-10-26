import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from dsp_trace import parse_capture, reconstruct, analyze, capture_profile

HEADER = "YAMANIX DSP_BEGIN version=1 clock_hz=1000000000 limit=200000"
EVENT = "YAMANIX DSP seq=0 time_ns=1 op=W chip=0 reg=00 data=01ff mask=ffff pc=000100"
FOOTER = "YAMANIX DSP_END events=1 dropped=0"


class CaptureTests(unittest.TestCase):
    def test_experimental_profiles_cannot_be_silently_mixed(self):
        baseline = "YAMANIX PROFILE dsp=passive hardware_validated=0"
        experiment = "YAMANIX PROFILE dsp=firmware-contract hardware_validated=0"
        self.assertTrue(capture_profile([experiment])["experimental"])
        self.assertFalse(capture_profile([baseline])["experimental"])
        self.assertEqual(capture_profile([])["name"], "unlabelled")
        with self.assertRaises(ValueError):
            capture_profile([baseline, experiment])
        with self.assertRaises(ValueError):
            capture_profile([experiment.replace("validated=0", "validated=1")])

    def test_complete_and_partial_upload_distinguished(self):
        events = parse_capture([HEADER, EVENT, FOOTER])
        self.assertEqual(analyze(events)["chips"][0]["distinct_program_words"], 0)
        self.assertEqual(len(reconstruct(events)[0]), 14340)

    def test_capture_damage_rejected(self):
        for lines in ([HEADER, EVENT], [EVENT, FOOTER], [HEADER, EVENT, FOOTER, EVENT],
                      [HEADER, EVENT.replace("seq=0", "seq=2"), FOOTER],
                      [HEADER, EVENT, FOOTER.replace("dropped=0", "dropped=1")],
                      [HEADER, EVENT.replace("mask=ffff", "mask=00ff"), FOOTER],
                      [HEADER, EVENT, FOOTER.replace("events=1", "events=2")]):
            with self.subTest(lines=lines), self.assertRaises(ValueError):
                parse_capture(lines)

    def test_timestamp_order_and_repeated_writes_preserved(self):
        second = EVENT.replace("seq=0", "seq=1").replace("time_ns=1", "time_ns=0")
        with self.assertRaises(ValueError):
            parse_capture([HEADER, EVENT, second, FOOTER.replace("events=1", "events=2")])
        repeated = parse_capture([HEADER, EVENT, EVENT.replace("seq=0", "seq=1"), FOOTER.replace("events=1", "events=2")])
        self.assertEqual(len(repeated), 2)
