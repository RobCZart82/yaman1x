import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from repository_audit import audit


class RepositoryAuditTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        source = self.root/'third_party/vop3/src/core.cpp'
        source.parent.mkdir(parents=True); source.write_text('// synthetic source\n')
        (self.root/'third_party/provenance.json').write_text(json.dumps({'files':[
            {'path':'third_party/vop3/src/core.cpp', 'bytes':source.stat().st_size,
             'sha256':hashlib.sha256(source.read_bytes()).hexdigest()}]}))
        (self.root/'README.md').write_text('Synthetic repository\n')

    def test_clean_candidate_and_changed_dependency(self):
        self.assertTrue(audit(self.root)['upload_check_passed'])
        (self.root/'third_party/vop3/src/core.cpp').write_text('// changed\n')
        self.assertFalse(audit(self.root)['upload_check_passed'])

    def test_disguised_payload_paths_and_token_are_rejected_without_echo(self):
        bad = self.root/'capture.md'
        for content in ('payload\0data', '/Users/'+'example/private/', 'gh'+'p_'+'a'*40):
            bad.write_text(content)
            result = audit(self.root)
            self.assertFalse(result['upload_check_passed'])
            self.assertNotIn(content, json.dumps(result))
        bad.unlink()
        (self.root/'firmware.bin').write_bytes(bytes(4096))
        self.assertFalse(audit(self.root)['upload_check_passed'])

    def test_symlink_and_unlisted_dependency_rejected(self):
        (self.root/'third_party/vop3/src/extra.cpp').write_text('// unlisted\n')
        self.assertFalse(audit(self.root)['upload_check_passed'])
        (self.root/'third_party/vop3/src/extra.cpp').unlink()
        (self.root/'linked.md').symlink_to(self.root/'README.md')
        self.assertFalse(audit(self.root)['upload_check_passed'])

    def test_force_tracked_rom_is_rejected_even_if_ignored(self):
        subprocess.run(['git','init','-q',str(self.root)],check=True)
        (self.root/'.gitignore').write_text('*.bin\n')
        (self.root/'firmware.bin').write_bytes(bytes(4096))
        self.assertTrue(audit(self.root)['upload_check_passed'])
        subprocess.run(['git','-C',str(self.root),'add','-f','firmware.bin'],check=True)
        self.assertFalse(audit(self.root)['upload_check_passed'])
