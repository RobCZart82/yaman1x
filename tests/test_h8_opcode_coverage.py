import json
from pathlib import Path
import sys
import tempfile
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from h8_opcode_coverage import inventory,build
from check_h8_first_instructions import NAMES,H8H_NAMES


class CoverageTests(unittest.TestCase):
    def test_profiles_methods_and_lines(self):
        raw=b'# comment\nmacro fetch\n\tignored\n10000 state_reset\n0200 fff0 0 stc ccr r8l\n0210 fff0 0 stc exr r8l s20\n7900 fff0 0 mov.w imm16 r16l h\n8000 f000 0 other - - o\n'
        r=inventory(raw,{'stc_ccr_r8l'})
        self.assertEqual(r['counts'],{'excluded_profile':2,'missing_handler':1,'selected_handler':1})
        self.assertEqual(r['instructions'][0]['line'],5)
        self.assertEqual(r['instructions'][2]['method'],'mov_w_imm16_r16l')
        self.assertFalse(r['decoder_proven'])

    def test_unknown_and_changed_input_rejected(self):
        for raw in [b'garbage',b'0200 fff0 0 stc ccr r8l unknown']:
            with self.assertRaises(ValueError):inventory(raw,set())
        with tempfile.TemporaryDirectory() as temp:
            p=Path(temp)/'bad.lst';p.write_bytes(b'wrong')
            with self.assertRaises(ValueError):build(p)

    def test_committed_report_selection_and_identity(self):
        root=Path(__file__).resolve().parents[1]
        report=json.loads((root/'research/h8-opcode-coverage.json').read_text())
        lock=json.loads((root/'research/h8-generator-lock.json').read_text())
        self.assertEqual(report['source_sha256'],lock['inputs']['src/devices/cpu/h8/h8.lst'])
        self.assertEqual(report['upstream_commit'],lock['upstream_commit'])
        selected=set(NAMES+H8H_NAMES)
        for r in report['instructions']:
            if r['profile'] in ('common','h'):
                self.assertEqual(r['status']=='selected_handler',r['method'] in selected)
        from collections import Counter
        self.assertEqual(report['counts'],dict(Counter(r['status'] for r in report['instructions'])))
