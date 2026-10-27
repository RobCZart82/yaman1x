import hashlib
from pathlib import Path
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from check_h8_generation import verify_inputs, verify_output


class H8GenerationTests(unittest.TestCase):
    def test_input_identity_is_exact(self):
        raw = b'// synthetic\n'
        lock = {'inputs': {'source': hashlib.sha256(raw).hexdigest()}}
        verify_inputs({'source': raw}, lock)
        with self.assertRaises(ValueError):
            verify_inputs({'source': raw.replace(b'\n', b'\r\n')}, lock)

    def test_generated_text_normalizes_crlf_but_rejects_content_changes(self):
        raw = b'// generated\n'
        expected = {'lf_bytes': len(raw), 'lf_sha256': hashlib.sha256(raw).hexdigest()}
        self.assertEqual(verify_output(raw.replace(b'\n', b'\r\n'), expected), raw)
        for changed in (raw+b'\n', raw.replace(b'generated', b'corrupted')):
            with self.subTest(changed=changed), self.assertRaises(ValueError):
                verify_output(changed, expected)
