import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from check_h8_runner_reference import check

class RunnerReferenceTests(unittest.TestCase):
    def test_wrong_archive_rejected_before_work_or_compiler(self):
        with tempfile.TemporaryDirectory() as temp:
            a=Path(temp)/'wrong.zip';a.write_bytes(b'not locked source')
            w=Path(temp)/'work'
            with patch('check_h8_runner_reference.subprocess.run') as run:
                with self.assertRaises(ValueError):check(a,w)
                run.assert_not_called()
            self.assertFalse(w.exists())

    def test_runner_source_identity_matches_reviewed_budget(self):
        root=Path(__file__).resolve().parents[1]
        a=json.loads((root/'research/h8-execution-runner-provenance.json').read_text())
        b=json.loads((root/'research/h8-execution-budget-provenance.json').read_text())
        self.assertEqual(a['inputs'],b['inputs'])
        self.assertEqual(a['upstream_commit'],b['upstream_commit'])
