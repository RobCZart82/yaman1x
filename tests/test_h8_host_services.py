import hashlib
from pathlib import Path
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from h8_host_services import scan


class HostServiceTests(unittest.TestCase):
    def test_ignored_literals_comments_and_exact_lines(self):
        raw = b'''// machine total_cycles
/* m_program\nread16 */
const char* a="read8 \\\" machine";
auto b=R"tag(write16\nm_icount)tag";
char c='x';
read8(0); m_program.read_word(0); // read8
'''
        r = scan({'cpu.cpp': raw})
        refs = r['files'][0]['references']
        self.assertEqual({x['token']: x['lines'] for x in refs}, {'read8': [8], 'm_program': [8]})
        self.assertEqual(r['token_reference_counts']['memory_bus'], 2)
        self.assertEqual(r['files'][0]['sha256'], hashlib.sha256(raw).hexdigest())

    def test_token_boundaries_and_duplicate_line_counts(self):
        r = scan({'a.h': b'read8x _read8 read8 read8\nread8\n'})
        ref = r['files'][0]['references'][0]
        self.assertEqual((ref['token'], ref['count'], ref['lines']), ('read8', 3, [1, 2]))

    def test_deterministic_order_and_changed_source_identity(self):
        files = {'z.h': b'total_cycles', 'a.cpp': b'execute_run'}
        self.assertEqual(scan(files), scan(dict(reversed(list(files.items())))))
        changed = dict(files); changed['a.cpp'] += b' '
        self.assertNotEqual(scan(files)['files'], scan(changed)['files'])

    def test_empty_unknown_and_honest_scope(self):
        r = scan({'empty.h': b'unknown_framework_service();'})
        self.assertEqual(r['files'][0]['references'], [])
        self.assertFalse(any(r['token_reference_counts'].values()))
        for key in ['complete_compiler_closure', 'symbol_resolution', 'cpu_compiled', 'firmware_execution', 'license_clearance']:
            self.assertFalse(r[key])

    def test_invalid_encoding(self):
        with self.assertRaises(UnicodeDecodeError):
            scan({'bad.h': b'\xff'})

    def test_raw_literal_and_character_escapes(self):
        r = scan({'x.cpp': b'''u8R"(machine)" L"read8" '\\'';\nset_irq();'''} )
        self.assertEqual([x['token'] for x in r['files'][0]['references']], ['set_irq'])

    def test_changed_archive_never_executes_generator(self):
        import tempfile
        from unittest.mock import patch
        from h8_host_services import build
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp)/'changed.zip'; path.write_bytes(b'not the locked archive')
            with patch('h8_host_services.generate') as generator:
                with self.assertRaises(ValueError):
                    build(path, Path(temp)/'work')
                generator.assert_not_called()
                self.assertFalse((Path(temp)/'work').exists())

    def test_recorded_report_matches_rules_and_generator_identities(self):
        import json
        from h8_host_services import SERVICES
        root = Path(__file__).resolve().parents[1]
        report = json.loads((root/'research/h8-host-services.json').read_text())
        source = json.loads((root/'research/mame-source-lock.json').read_text())
        generated = json.loads((root/'research/h8-generator-lock.json').read_text())
        self.assertEqual(report['categories'], SERVICES)
        self.assertEqual(report['archive_sha256'], source['archive_sha256'])
        self.assertEqual(report['upstream_commit'], source['commit'])
        self.assertEqual(len(report['files']), 34)
        counts = dict.fromkeys(SERVICES, 0)
        for row in report['files']:
            for ref in row['references']:
                self.assertIn(ref['token'], SERVICES[ref['category']])
                self.assertGreaterEqual(ref['count'], len(ref['lines']))
                self.assertEqual(ref['lines'], sorted(set(ref['lines'])))
                self.assertGreater(min(ref['lines']), 0)
                counts[ref['category']] += ref['count']
        self.assertEqual(counts, report['token_reference_counts'])
        actual = {r['path']: r for r in report['files'] if r['path'].startswith('generated/')}
        self.assertEqual(set(actual), {'generated/'+n for n in generated['outputs']})
        for name, identity in generated['outputs'].items():
            self.assertEqual(actual['generated/'+name]['sha256'], identity['lf_sha256'])
            self.assertEqual(actual['generated/'+name]['bytes'], identity['lf_bytes'])
