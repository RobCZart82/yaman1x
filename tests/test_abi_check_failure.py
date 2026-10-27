"""The ABI smoke checks must reject an incompatible library even under -O."""
from pathlib import Path
import subprocess
import sys
import unittest


class AbiCheckFailureTests(unittest.TestCase):
    def test_incompatible_abi_is_rejected(self):
        wrapper = '''import ctypes, runpy, sys
class Fn:
    def __call__(self, *args): return 999
class BadABI:
    def __getattr__(self, name): return Fn()
ctypes.CDLL = lambda path: BadABI()
sys.argv = [sys.argv[1], 'synthetic-incompatible-library']
runpy.run_path(sys.argv[0], run_name='__main__')
'''
        for name in ('check_vop3_bridge.py', 'check_vop3_timing.py'):
            for options in ([], ['-O']):
                with self.subTest(script=name, options=options):
                    result = subprocess.run(
                        [sys.executable, *options, '-c', wrapper,
                         str(Path(__file__).with_name(name))],
                        capture_output=True, text=True, timeout=30)
                    self.assertNotEqual(result.returncode, 0)
                    self.assertIn('AssertionError: VOP3 ABI check failed', result.stderr)
