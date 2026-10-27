"""Synthetic source trees exercise preparation without downloading MAME or ROMs."""
import difflib
import hashlib
import os
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path
import zipfile
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from prepare_mame_research import prepare_checkout, patch_preflight, check_archive, safe_path


class ResearchPreparationTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.source, self.repo = self.root / "upstream", self.root / "project"
        for root in (self.source, self.repo):
            (root / "src").mkdir(parents=True)
        (self.repo / "research").mkdir()
        self.original = b"// synthetic upstream\nint value = 0;\n"
        self.updated = self.original.replace(b"= 0", b"= 1")
        self.helper = b"// synthetic project helper\n"
        (self.source / "src/driver.cpp").write_bytes(self.original)
        (self.repo / "src/helper.h").write_bytes(self.helper)
        patch = "".join(difflib.unified_diff(
            self.original.decode().splitlines(True), self.updated.decode().splitlines(True),
            fromfile="a/src/driver.cpp", tofile="b/src/driver.cpp"))
        patch += "".join(difflib.unified_diff(
            [], self.helper.decode().splitlines(True), fromfile="/dev/null", tofile="b/src/helper.h"))
        (self.repo / "research/first.patch").write_bytes(patch.encode())
        self.git("init", "-q")
        self.git("config", "core.autocrlf", "false")
        self.git("add", "src/driver.cpp")
        self.git("-c", "user.name=Test", "-c", "user.email=test@example.invalid", "commit", "-qm", "Synthetic")
        self.lock = dict(commit=self.git("rev-parse", "HEAD"),
                         inputs={"src/driver.cpp": hashlib.sha256(self.original).hexdigest()},
                         patches=["first.patch"], generated_headers={"src/helper.h": "src/helper.h"})

    def git(self, *args):
        return subprocess.run(["git", "-C", str(self.source), *args], check=True,
                              capture_output=True, text=True).stdout.strip()

    def test_preflight_is_read_only_and_preparation_installs_tested_helpers(self):
        prepared = patch_preflight(self.source, self.repo, self.lock)
        self.assertEqual(prepared["src/driver.cpp"], self.updated)
        self.assertEqual((self.source / "src/driver.cpp").read_bytes(), self.original)
        self.assertEqual(self.git("status", "--porcelain"), "")
        prepare_checkout(self.source, self.repo, self.lock)
        self.assertEqual((self.source / "src/helper.h").read_bytes(), self.helper)
        self.assertEqual((self.source / "src/driver.cpp").read_bytes(), self.updated)
        with self.assertRaisesRegex(ValueError, "dirty"):
            prepare_checkout(self.source, self.repo, self.lock)

    def test_late_patch_or_helper_failure_leaves_checkout_untouched(self):
        (self.repo / "research/bad.patch").write_bytes(b"not a patch\n")
        self.lock["patches"].append("bad.patch")
        with self.assertRaisesRegex(ValueError, "does not apply"):
            prepare_checkout(self.source, self.repo, self.lock)
        self.lock["patches"].pop()
        (self.repo / "src/helper.h").write_bytes(b"// different helper\n")
        with self.assertRaisesRegex(ValueError, "differs from tested"):
            prepare_checkout(self.source, self.repo, self.lock)
        self.assertEqual(self.git("status", "--porcelain"), "")

    def test_global_autocrlf_does_not_change_prepared_bytes(self):
        with patch.dict(os.environ, {"GIT_CONFIG_COUNT": "1",
                                      "GIT_CONFIG_KEY_0": "core.autocrlf",
                                      "GIT_CONFIG_VALUE_0": "true"}):
            prepared = patch_preflight(self.source, self.repo, self.lock)
        self.assertEqual(prepared["src/helper.h"], self.helper)
        self.assertEqual(prepared["src/driver.cpp"], self.updated)

    def test_wrong_revision_modified_inputs_and_unsafe_paths_are_rejected(self):
        self.lock["commit"] = "0" * 40
        with self.assertRaisesRegex(ValueError, "commit differs"):
            prepare_checkout(self.source, self.repo, self.lock)
        (self.source / "src/driver.cpp").write_bytes(b"// modified\n")
        with self.assertRaisesRegex(ValueError, "input differs"):
            patch_preflight(self.source, self.repo, self.lock)
        for name in ("../escape.cpp", "/escape.cpp", "C:/escape.cpp", "src\\escape.cpp"):
            with self.subTest(name=name), self.assertRaises(ValueError):
                safe_path(self.source, name)

    def test_archive_extracts_only_locked_inputs_and_checks_archive_identity(self):
        archive = self.root / "source.zip"
        with zipfile.ZipFile(archive, "w") as output:
            output.writestr("mame-master/src/driver.cpp", self.original)
            output.writestr("mame-master/roms/not-an-input.bin", b"synthetic excluded bytes")
        self.lock["archive_sha256"] = hashlib.sha256(archive.read_bytes()).hexdigest()
        prepared = check_archive(archive, self.repo, self.lock)
        self.assertEqual(set(prepared), {"src/driver.cpp", "src/helper.h"})
        self.assertEqual(hashlib.sha256(archive.read_bytes()).hexdigest(), self.lock["archive_sha256"])
        self.lock["archive_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "archive SHA-256"):
            check_archive(archive, self.repo, self.lock)
