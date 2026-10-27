import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from check_h8_bus_reference import METHODS, SOURCE, extract, check


def fixture():
    return '\n'.join('u16 h8_device::'+name+'(u32 a)\n{\n\treturn 0;\n}' for name in METHODS).encode()


class BusReferenceTests(unittest.TestCase):
    def test_exact_method_order_and_qualifier_removal(self):
        result = extract(fixture())
        self.assertNotIn('h8_device::', result)
        self.assertEqual([result.index(name+'(') for name in METHODS], sorted(result.index(name+'(') for name in METHODS))

    def test_missing_and_duplicate_method_rejected(self):
        with self.assertRaises(ValueError):
            extract(b'')
        raw = fixture()
        with self.assertRaises(ValueError):
            extract(raw+b'\n'+raw)

    def test_archive_rejected_before_compile_or_work_creation(self):
        with tempfile.TemporaryDirectory() as temp:
            archive = Path(temp)/'wrong.zip'; archive.write_bytes(b'wrong source')
            work = Path(temp)/'work'
            with patch('check_h8_bus_reference.subprocess.run') as run:
                with self.assertRaises(ValueError):
                    check(archive, work)
                run.assert_not_called()
                self.assertFalse(work.exists())

    def test_source_identity_agrees_with_host_services_report(self):
        root = Path(__file__).resolve().parents[1]
        lock = json.loads((root/'research/h8h-bus-provenance.json').read_text())
        report = json.loads((root/'research/h8-host-services.json').read_text())
        self.assertEqual(lock['upstream_commit'], report['upstream_commit'])
        row = next(r for r in report['files'] if r['path']==SOURCE)
        self.assertEqual(lock['inputs'][SOURCE]['sha256'], row['sha256'])
        self.assertEqual(lock['inputs'][SOURCE]['bytes'], row['bytes'])


class BudgetReferenceTests(unittest.TestCase):
    def test_budget_archive_rejected_before_compile(self):
        from check_h8_budget_reference import check as budget_check
        with tempfile.TemporaryDirectory() as temp:
            archive = Path(temp)/'wrong.zip'; archive.write_bytes(b'wrong source')
            work = Path(temp)/'work'
            with patch('check_h8_budget_reference.subprocess.run') as run:
                with self.assertRaises(ValueError):
                    budget_check(archive, work)
                run.assert_not_called()
                self.assertFalse(work.exists())

    def test_budget_and_bus_share_exact_source_identity(self):
        root = Path(__file__).resolve().parents[1]
        bus = json.loads((root/'research/h8h-bus-provenance.json').read_text())
        budget = json.loads((root/'research/h8-execution-budget-provenance.json').read_text())
        self.assertEqual(bus['inputs'], budget['inputs'])
        self.assertEqual(bus['upstream_commit'], budget['upstream_commit'])
