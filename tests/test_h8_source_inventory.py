import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
from h8_source_inventory import inventory, PREFIX


class H8InventoryTests(unittest.TestCase):
    def test_cycles_pairs_and_unresolved_boundaries(self):
        files = {PREFIX+'h83002.cpp': b'// license:BSD-3-Clause\n#include "h83002.h"\n#include "emu.h"\n',
                 PREFIX+'h83002.h': b'#include "sci.h"\n',
                 PREFIX+'sci.h': b'#include "h83002.h"\n',
                 PREFIX+'sci.cpp': b'#include "generated.hxx"\n'}
        result = inventory(files)
        self.assertEqual(len(result['files']), 4)
        self.assertEqual({row['include'] for row in result['boundaries']}, {'emu.h', 'generated.hxx'})
        self.assertFalse(result['complete_compiler_closure'])
        self.assertFalse(result['license_clearance'])
        self.assertEqual(result, inventory(dict(reversed(list(files.items())))))
        changed = dict(files)
        changed[PREFIX+'sci.h'] += b'// changed\n'
        self.assertNotEqual(inventory(changed)['files'], result['files'])

    def test_missing_roots_and_escaping_includes_fail(self):
        with self.assertRaises(ValueError):
            inventory({})
        for include in ('../escape.h', '/escape.h', 'C:/escape.h', 'bad\\file.h'):
            with self.subTest(include=include), self.assertRaises(ValueError):
                inventory({PREFIX+'h83002.cpp': ('#include "'+include+'"').encode(),
                           PREFIX+'h83002.h': b''})
