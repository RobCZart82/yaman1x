"""Read-only comparison of a local AN1x VOP3 table image and firmware copy.

The firmware and table-ROM payloads remain external inputs; this tool never
extracts, writes, patches, or uploads them.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile

FIRMWARE_SIZE = 0x100000
TABLE_ROM_SIZE = 0x20000
FIRMWARE_TABLE_BASE = 0xE496C
REFERENCE_TABLE_SHA256 = "97eff8e560cb5e644b8567601a72fbe353fd0eb1770c368b717aa51f76055944"


def read_image(path, member, expected_size, label):
    path = Path(path)
    if path.suffix.lower() == ".zip":
        with zipfile.ZipFile(path) as archive:
            entries = [entry for entry in archive.infolist() if not entry.is_dir()]
            if member is None:
                if len(entries) != 1:
                    raise ValueError(f"{label} ZIP must contain one file or specify its member")
                entry = entries[0]
            else:
                matches = [entry for entry in entries if entry.filename == member]
                if len(matches) != 1:
                    raise ValueError(f"{label} ZIP member missing or ambiguous")
                entry = matches[0]
            if entry.file_size != expected_size:
                raise ValueError(f"Expected {expected_size} {label} bytes, got {entry.file_size}")
            return archive.read(entry)
    if member is not None:
        option = "--table-rom-member" if label == "table ROM" else "--firmware-member"
        raise ValueError(f"{option} is only supported for ZIP input")
    if path.stat().st_size != expected_size:
        raise ValueError(f"Expected {expected_size} {label} bytes, got {path.stat().st_size}")
    return path.read_bytes()


def compare_region(firmware, table_rom, table_word, firmware_word, count):
    table_start = table_word * 2
    firmware_start = FIRMWARE_TABLE_BASE + firmware_word * 2
    size = count * 2
    return {
        "table_rom_word_start": f"0x{table_word:04X}",
        "firmware_word_start": f"0x{firmware_word:04X}",
        "word_count": count,
        "byte_match": table_rom[table_start:table_start + size]
        == firmware[firmware_start:firmware_start + size],
    }


def audit(firmware, table_rom):
    if len(firmware) != FIRMWARE_SIZE:
        raise ValueError(f"Expected {FIRMWARE_SIZE} firmware bytes, got {len(firmware)}")
    if len(table_rom) != TABLE_ROM_SIZE:
        raise ValueError(f"Expected {TABLE_ROM_SIZE} table-ROM bytes, got {len(table_rom)}")
    if FIRMWARE_TABLE_BASE + 0x5022 * 2 > len(firmware):
        raise ValueError("Firmware table copy extends outside the firmware image")

    regions = {
        "exponential": compare_region(firmware, table_rom, 0x0000, 0x0000, 0x1000),
        "decay": compare_region(firmware, table_rom, 0x1020, 0x1010, 0x2000),
        "sine": compare_region(firmware, table_rom, 0x3020, 0x3010, 0x1000),
        "clipper": compare_region(firmware, table_rom, 0x4020, 0x4010, 0x1001),
    }
    table_power = table_rom[0x1000 * 2:0x1020 * 2]
    firmware_power = firmware[
        FIRMWARE_TABLE_BASE + 0x1000 * 2:FIRMWARE_TABLE_BASE + 0x1010 * 2
    ]
    regions["powers_of_two"] = {
        "table_rom_word_start": "0x1000",
        "firmware_word_start": "0x1000",
        "word_count": 0x20,
        "byte_match": table_power == b"".join(
            firmware_power[i:i + 2] * 2 for i in range(0, len(firmware_power), 2)
        ),
    }

    table_sha256 = hashlib.sha256(table_rom).hexdigest()
    return {
        "firmware_bytes": len(firmware),
        "firmware_sha256": hashlib.sha256(firmware).hexdigest(),
        "table_rom_bytes": len(table_rom),
        "table_rom_sha256": table_sha256,
        "matches_referenced_vop3_image": table_sha256 == REFERENCE_TABLE_SHA256,
        "table_copy_base_in_firmware": f"0x{FIRMWARE_TABLE_BASE:X}",
        "regions": regions,
        "all_shipped_voice_table_regions_match": all(
            region["byte_match"] for region in regions.values()
        ),
        "address_index_scale_verified": False,
        "hardware_validated_by_this_audit": False,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("firmware", type=Path)
    parser.add_argument("table_rom", type=Path)
    parser.add_argument("--firmware-member")
    parser.add_argument("--table-rom-member")
    args = parser.parse_args()
    try:
        firmware = read_image(args.firmware, args.firmware_member, FIRMWARE_SIZE, "firmware")
        table_rom = read_image(args.table_rom, args.table_rom_member, TABLE_ROM_SIZE, "table ROM")
        result = audit(firmware, table_rom)
        print(json.dumps(result, indent=2))
        return 0 if result["all_shipped_voice_table_regions_match"] else 1
    except (OSError, ValueError, zipfile.BadZipFile, KeyError, RuntimeError) as error:
        parser.exit(2, f"Table-ROM input error: {error}\n")


if __name__ == "__main__":
    raise SystemExit(main())
