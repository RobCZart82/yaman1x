import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from eg_readback_audit import characterize


def scan(values):
    events=[]
    for index in range(16):
        events.extend([dict(chip=0,op='W',reg=0,data=index,mask=65535,pc=0),
                       dict(chip=0,op='R',reg=0x22,data=values.get(index,0),mask=65535,pc=0x54676)])
    return events


class EgAuditTests(unittest.TestCase):
    def test_signed_range_and_channel_14_exception(self):
        result=characterize(scan({0:0x4000,1:0x4001,2:0xff00,3:0xfeff,14:0xc000}))
        self.assertEqual(result['chips'][0]['range_error_decisions'],2)
        self.assertEqual(result['chips'][0]['diagnostic_scans'],1)
        self.assertFalse(result['hardware_validated'])

    def test_invalid_or_incomplete_scan_rejected(self):
        with self.assertRaises(ValueError): characterize(scan({})[:-2])
        bad=scan({});bad[1]['mask']=255
        with self.assertRaises(ValueError): characterize(bad)
        bad=scan({});bad[4]['data']=0
        with self.assertRaises(ValueError): characterize(bad)

    def test_special_read_is_a_scalar_comparison(self):
        events=[dict(chip=1,op='W',reg=0,data=14,mask=65535,pc=0),
                dict(chip=1,op='R',reg=0x22,data=0xc000,mask=65535,pc=0x516c6)]
        self.assertTrue(characterize(events)['chips'][1]['special_channel_reads'][0]['matches_expected_c000'])
        events[-1]['data']=0
        self.assertFalse(characterize(events)['chips'][1]['special_channel_reads'][0]['matches_expected_c000'])
