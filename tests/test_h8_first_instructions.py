import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from check_h8_first_instructions import H8H_NAMES,NAMES,render,verify_selected,check


def fixture():
    return '\n'.join('void h8_device::'+n+'_'+s+'()\n{\nswitch (1) {\ncase 1:;\n}\n m_inst_substate = 0;\n}' for n in NAMES for s in ('full','partial')).encode()


class FirstInstructionTests(unittest.TestCase):
    def test_nested_partial_body_is_not_truncated(self):
        text=render(fixture())
        self.assertEqual(text.count('m_inst_substate = 0;'),2*len(NAMES))
        self.assertNotIn('h8_device::',text)
        self.assertLess(text.index('state_reset_full'),text.index('nop_full'))

    def test_missing_and_duplicate_methods_rejected(self):
        for raw in (b'',fixture()+b'\n'+fixture()):
            with self.assertRaises(ValueError):render(raw)

    def make_root(self,root):
        (root/'research').mkdir();(root/'src').mkdir()
        raw=fixture()
        lock={'outputs':{'h8.hxx':{'lf_bytes':len(raw),'lf_sha256':hashlib.sha256(raw).hexdigest()}}}
        (root/'research/h8-generator-lock.json').write_text(json.dumps(lock))
        (root/'src/h8_first_handlers.h').write_text(render(raw),encoding='utf-8')
        return raw

    def test_lf_and_crlf_match_locked_selection(self):
        with tempfile.TemporaryDirectory() as t:
            root=Path(t);raw=self.make_root(root)
            self.assertEqual(verify_selected(raw,root),verify_selected(raw.replace(b'\n',b'\r\n'),root))

    def test_changed_generated_and_selected_bodies_rejected(self):
        with tempfile.TemporaryDirectory() as t:
            root=Path(t);raw=self.make_root(root)
            with self.assertRaises(ValueError):verify_selected(raw+b'\n',root)
            (root/'src/h8_first_handlers.h').write_text('changed')
            with self.assertRaises(ValueError):verify_selected(raw,root)

    def test_recorded_identities_and_selected_content(self):
        root=Path(__file__).resolve().parents[1]
        lock=json.loads((root/'research/h8-generator-lock.json').read_text())
        p=json.loads((root/'research/h8-first-instructions-provenance.json').read_text())
        self.assertEqual(p['upstream_commit'],lock['upstream_commit'])
        self.assertEqual(p['generated_lf_sha256'],lock['outputs']['h8.hxx']['lf_sha256'])
        self.assertEqual(p['methods'],[n+'_'+s for n in NAMES for s in ('full','partial')])
        raw=(root/'src/h8_first_handlers.h').read_bytes().replace(b'\r\n',b'\n')
        self.assertEqual(hashlib.sha256(raw).hexdigest(),p['selected_lf_sha256'])

    def test_bad_archive_rejected_before_output_and_execution(self):
        with tempfile.TemporaryDirectory() as t:
            a=Path(t)/'bad.zip';a.write_bytes(b'wrong archive');w=Path(t)/'out'
            with patch('check_h8_first_instructions.subprocess.run') as run:
                with self.assertRaises(ValueError):check(a,w)
                run.assert_not_called()
            self.assertFalse(w.exists())

    def test_h8h_selection_uses_separate_locked_output(self):
        with tempfile.TemporaryDirectory() as t:
            root=Path(t);self.make_root(root)
            # Use the dedicated H8H names rather than a base-method fallback.
            raw='\n'.join('void h8h_device::'+n+'_'+s+'()\n{\n} ' for n in H8H_NAMES for s in ('full','partial')).encode()
            lock=json.loads((root/'research/h8-generator-lock.json').read_text())
            lock['outputs']['h8h.hxx']={'lf_bytes':len(raw),'lf_sha256':hashlib.sha256(raw).hexdigest()}
            (root/'research/h8-generator-lock.json').write_text(json.dumps(lock))
            (root/'src/h8_first_h8h_handlers.h').write_text(render(raw,H8H_NAMES,'h8h_device'),encoding='utf-8')
            self.assertIn('mov_l_imm32',verify_selected(raw,root,True))
            with self.assertRaises(ValueError):verify_selected(raw,root,False)

    def test_recorded_h8h_identities(self):
        root=Path(__file__).resolve().parents[1]
        p=json.loads((root/'research/h8-first-instructions-provenance.json').read_text())
        lock=json.loads((root/'research/h8-generator-lock.json').read_text())
        self.assertEqual(p['h8h_methods'],[n+'_'+s for n in H8H_NAMES for s in ('full','partial')])
        self.assertEqual(p['h8h_generated_lf_sha256'],lock['outputs']['h8h.hxx']['lf_sha256'])
        raw=(root/'src/h8_first_h8h_handlers.h').read_bytes().replace(b'\r\n',b'\n')
        self.assertEqual(p['h8h_selected_lf_sha256'],hashlib.sha256(raw).hexdigest())
