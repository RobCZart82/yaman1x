import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from an1x_table_rom_audit import (
    FIRMWARE_SIZE,
    FIRMWARE_TABLE_BASE,
    TABLE_ROM_SIZE,
    audit,
    read_image,
)


class TableRomAuditTests(unittest.TestCase):
    def setUp(self):
        self.firmware = bytearray(FIRMWARE_SIZE)
        self.table = bytearray(TABLE_ROM_SIZE)
        # Distinct deterministic word sequences make byte-order and region offsets observable.
        regions = [
            (0x0000, 0x0000, 0x1000),
            (0x1020, 0x1010, 0x2000),
            (0x3020, 0x3010, 0x1000),
            (0x4020, 0x4010, 0x1001),
        ]
        for table_word, firmware_word, count in regions:
            start = FIRMWARE_TABLE_BASE + firmware_word * 2
            data = bytes((i * 17 + table_word) & 0xFF for i in range(count * 2))
            self.firmware[start:start + len(data)] = data
            table_start = table_word * 2
            self.table[table_start:table_start + len(data)] = data
        start = FIRMWARE_TABLE_BASE + 0x1000 * 2
        powers = bytes(range(0x20))
        self.firmware[start:start + len(powers)] = powers
        expanded = b"".join(powers[i:i + 2] * 2 for i in range(0, len(powers), 2))
        self.table[0x1000 * 2:0x1020 * 2] = expanded

    def test_matches_firmware_table_regions_and_duplicated_power_words(self):
        result = audit(self.firmware, self.table)
        self.assertTrue(result["all_shipped_voice_table_regions_match"])
        self.assertTrue(all(x["byte_match"] for x in result["regions"].values()))
        self.assertFalse(result["address_index_scale_verified"])
        self.assertFalse(result["hardware_validated_by_this_audit"])

    def test_changed_table_byte_is_reported(self):
        self.table[0x3020 * 2] ^= 1
        result = audit(self.firmware, self.table)
        self.assertFalse(result["regions"]["sine"]["byte_match"])
        self.assertFalse(result["all_shipped_voice_table_regions_match"])

    def test_rejects_wrong_image_sizes(self):
        with self.assertRaises(ValueError):
            audit(self.firmware[:-1], self.table)
        with self.assertRaises(ValueError):
            audit(self.firmware, self.table[:-1])

    def test_zip_member_selection_is_read_only(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "images.zip"
            with zipfile.ZipFile(path, "w", compression=zipfile.ZIP_DEFLATED) as archive:
                archive.writestr("firmware.bin", bytes(FIRMWARE_SIZE))
                archive.writestr("table.bin", bytes(TABLE_ROM_SIZE))
            with self.assertRaises(ValueError):
                read_image(path, None, FIRMWARE_SIZE, "firmware")
            self.assertEqual(
                len(read_image(path, "firmware.bin", FIRMWARE_SIZE, "firmware")),
                FIRMWARE_SIZE,
            )
            self.assertEqual(
                len(read_image(path, "table.bin", TABLE_ROM_SIZE, "table ROM")),
                TABLE_ROM_SIZE,
            )


if __name__ == "__main__":
    unittest.main()
