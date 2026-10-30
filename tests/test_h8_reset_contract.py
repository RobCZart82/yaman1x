import importlib.util
from pathlib import Path
import unittest
spec=importlib.util.spec_from_file_location('reset_contract',Path(__file__).resolve().parents[1]/'tools/h8_reset_contract.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
class ResetIdentityTests(unittest.TestCase):
 def test_identity_and_mutation(self):
  raw=b'void cpu::device_reset()\n{\n if(x){\n  f();\n }\n}\n'
  a=m.method_identity(raw,'cpu::device_reset')
  self.assertNotEqual(a['sha256'],m.method_identity(raw.replace(b'f();',b'g();'),'cpu::device_reset')['sha256'])
  self.assertEqual(a['bytes'],len(raw)-1)
 def test_missing_and_duplicate(self):
  raw=b'void cpu::device_reset()\n{\n}\n'
  for bad in [b'',raw+raw]:
   with self.assertRaises(ValueError):m.method_identity(bad,'cpu::device_reset')
 def test_nonreset_cannot_substitute(self):
  with self.assertRaises(ValueError):m.method_identity(b'void cpu::device_start()\n{\n}\n','cpu::device_reset')
 def test_archive_hash_checked_before_zip(self):
  import tempfile
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/'invalid.zip';p.write_bytes(b'not source')
   with self.assertRaisesRegex(ValueError,'Archive differs'):m.from_archive(p,{'archive_sha256':'0'*64,'commit':'test'})
