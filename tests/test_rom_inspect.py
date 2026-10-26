import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from rom_inspect import RomView, identify, read_input, SIZE


class RomTests(unittest.TestCase):
    def test_word_swap_and_vector(self):
        source = bytearray.fromhex("1200563400000000")
        view = RomView(source)
        source[0] = 255  # Caller changes cannot mutate the ROM view.
        self.assertEqual([view.read8(i) for i in range(4)], [0, 0x12, 0x34, 0x56])
        self.assertEqual(view.read32be(0), 0x00123456)
        for address in (-1, 8):
            with self.assertRaises(ValueError):
                view.read8(address)
        with self.assertRaises(ValueError):
            view.read32be(6)

    def test_unknown_image_rejected_by_identity(self):
        self.assertFalse(identify(bytes(SIZE))["matches_known_an1x_104"])

    def test_zip_selection_and_size(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "rom.zip"
            data = bytes(SIZE)
            with zipfile.ZipFile(path, "w", compression=zipfile.ZIP_DEFLATED) as archive:
                archive.writestr("image.bin", data)
                archive.writestr("note.txt", "not a ROM")
            with self.assertRaises(ValueError):
                read_input(path)
            self.assertEqual(read_input(path, "image.bin"), data)
            with self.assertRaises(ValueError):
                read_input(path, "note.txt")
            with self.assertRaises(ValueError):
                read_input(path, "missing")


if __name__ == "__main__":
    unittest.main()
