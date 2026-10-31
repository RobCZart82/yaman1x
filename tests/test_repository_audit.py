import hashlib
import difflib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from repository_audit import audit


class RepositoryAuditTests(unittest.TestCase):
    def test_local_edit_must_reverse_to_the_recorded_upstream(self):
        source = self.root/'third_party/vop3/src/core.cpp'
        original = source.read_bytes()
        reviewed = original.replace(b'synthetic', b'reviewed')
        source.write_bytes(reviewed)
        patch = self.root/'third_party/patches/core.patch'
        patch.parent.mkdir()
        patch.write_bytes(''.join(difflib.unified_diff(
            original.decode().splitlines(True), reviewed.decode().splitlines(True),
            fromfile='a/third_party/vop3/src/core.cpp',
            tofile='b/third_party/vop3/src/core.cpp')).encode())
        manifest = self.root/'third_party/provenance.json'
        data = json.loads(manifest.read_text())
        data['files'][0].update(bytes=len(reviewed), sha256=hashlib.sha256(reviewed).hexdigest())
        edit = dict(path='third_party/vop3/src/core.cpp',
                    before='synthetic', after='reviewed', upstream_bytes=len(original),
                    upstream_sha256=hashlib.sha256(original).hexdigest(),
                    patch='third_party/patches/core.patch',
                    patch_sha256=hashlib.sha256(patch.read_bytes()).hexdigest())
        data['local_edits'] = [edit]
        manifest.write_text(json.dumps(data))
        result = audit(self.root)
        self.assertTrue(result['upload_check_passed'], result)
        self.assertEqual(result['reviewed_local_edits_verified'], 1)
        edit['upstream_sha256'] = '0'*64
        manifest.write_text(json.dumps(data))
        self.assertFalse(audit(self.root)['upload_check_passed'])
        edit['upstream_sha256'] = hashlib.sha256(original).hexdigest()
        patch.write_bytes(patch.read_bytes().replace(b'core.cpp', b'missing.cpp'))
        edit['patch_sha256'] = hashlib.sha256(patch.read_bytes()).hexdigest()
        manifest.write_text(json.dumps(data))
        self.assertFalse(audit(self.root)['upload_check_passed'])

    def test_exact_illustration_exception_rejects_changes_and_other_images(self):
        from unittest.mock import patch
        raw=b'\x89PNG\r\n\x1a\nsynthetic\0image'
        target=self.root/'docs/images/an1x-hardware.png'
        target.parent.mkdir(parents=True);target.write_bytes(raw)
        with patch('repository_audit.APPROVED_IMAGES', {target.relative_to(self.root).as_posix():hashlib.sha256(raw).hexdigest()}):
            self.assertTrue(audit(self.root)['upload_check_passed'])
            target.write_bytes(raw+b'changed')
            self.assertFalse(audit(self.root)['upload_check_passed'])
            target.write_bytes(raw)
            (target.parent/'other.png').write_bytes(raw)
            self.assertFalse(audit(self.root)['upload_check_passed'])

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
