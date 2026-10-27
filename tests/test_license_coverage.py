import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from license_coverage import audit


class LicenseCoverageTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.put('third_party/demo/a.h', '// SPDX-License-Identifier: Zlib\n// Author: Example\n')
        self.put('licenses/demo.txt', 'reviewed license terms\n')
        self.put('THIRD_PARTY.md', 'Example source attribution\n')
        self.manifest = dict(schema_version=1, components=[dict(
            id='demo', license='Zlib', source='https://example.test/source',
            files=[dict(path='third_party/demo/a.h', required_notices=['Author: Example'])],
            license_files=[dict(path='licenses/demo.txt', sha256=hashlib.sha256(
                (self.root/'licenses/demo.txt').read_bytes()).hexdigest())])],
            notice_documents=[dict(path='THIRD_PARTY.md', required_notices=['Example'])])
        self.save()

    def put(self, name, text):
        p = self.root/name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding='utf-8')

    def save(self):
        self.put('third_party/license-coverage.json', json.dumps(self.manifest))

    def test_complete_fixture_is_not_legal_or_binary_clearance(self):
        result = audit(self.root)
        self.assertTrue(result['source_notice_coverage_passed'], result)
        self.assertFalse(result['legal_clearance'])
        self.assertFalse(result['binary_distribution_verified'])

    def test_removed_license_fails(self):
        (self.root/'licenses/demo.txt').unlink()
        self.assertFalse(audit(self.root)['source_notice_coverage_passed'])

    def test_altered_license_fails(self):
        self.put('licenses/demo.txt', 'different terms')
        self.assertFalse(audit(self.root)['source_notice_coverage_passed'])

    def test_removed_source_notice_fails(self):
        self.put('third_party/demo/a.h', '// SPDX-License-Identifier: Zlib\n')
        self.assertFalse(audit(self.root)['source_notice_coverage_passed'])

    def test_unregistered_new_sources_fail(self):
        for name in ('third_party/demo/new.h', 'src/new.h', 'research/new.patch'):
            with self.subTest(name=name):
                self.put(name, '// Adapted from an upstream source\n')
                self.assertFalse(audit(self.root)['source_notice_coverage_passed'])
                (self.root/name).unlink()

    def test_missing_summary_attribution_fails(self):
        self.put('THIRD_PARTY.md', 'No attribution')
        self.assertFalse(audit(self.root)['source_notice_coverage_passed'])

    def test_duplicate_or_escaping_manifest_paths_fail(self):
        record = self.manifest['components'][0]['files'][0]
        self.manifest['components'][0]['files'].append(dict(record))
        self.save()
        self.assertFalse(audit(self.root)['source_notice_coverage_passed'])
        self.manifest['components'][0]['files'] = [dict(path='../outside.h', required_notices=[])]
        self.save()
        self.assertFalse(audit(self.root)['source_notice_coverage_passed'])


if __name__ == '__main__':
    unittest.main()
