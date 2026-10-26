import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from run_h8_probe import summarize_trace


class ProbeTests(unittest.TestCase):
    def test_startup_message_is_not_cpu_execution(self):
        result = summarize_trace(["YAMANIX research harness: H8 active", "YAMANIX PC tick=0 pc=000000"])
        self.assertFalse(result["h8_cpu_progress_observed"])

    def test_loop_is_observed_without_claiming_boot(self):
        result = summarize_trace(["YAMANIX PC tick=0 pc=000000", "YAMANIX PC tick=1 pc=0009d8",
                                  "YAMANIX PC tick=2 pc=03f678", "YAMANIX PC tick=3 pc=03f678",
                                  "YAMANIX READ pc=03f678 address=ffffd2 data=ff00 mask=ff00"])
        self.assertTrue(result["h8_cpu_progress_observed"])
        self.assertEqual(result["pc_samples"], 4)
        self.assertEqual(result["read_addresses_observed"], 1)
        self.assertNotIn("complete_an1x_boot", result)

    def test_lcd_counters_and_dsp_addresses_are_observations_only(self):
        result = summarize_trace([
            "YAMANIX LCD_SUMMARY tick=100 busy_reads=42 ready_reads=7 commands=4 data=3",
            "YAMANIX LCD_SUMMARY tick=200 busy_reads=54 ready_reads=9 commands=6 data=5",
            "YAMANIX WRITE pc=05654e address=440016 data=d555 mask=ffff",
            "YAMANIX WRITE pc=05654e address=480016 data=d555 mask=ffff",
            "YAMANIX READ pc=03f676 address=ffffd6 data=0000 mask=ff00"])
        self.assertEqual(result["dsp_mmio_addresses_observed"], [0x440016, 0x480016])
        self.assertEqual(result["lcd_counters"]["busy_reads"], 54)
        self.assertNotIn("complete_an1x_boot", result)
