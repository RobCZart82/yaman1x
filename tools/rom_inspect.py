"""Read-only AN1x 1.04 identification and MAME-compatible ROM memory view.

No ROM payload is embedded, extracted to disk, patched or transmitted.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile
import zlib

SIZE = 0x100000
CRC32 = "f03b8c30"
SHA1 = "778b669a450660f2b15cc385f156dc6676c437f1"


def read_input(path, member=None):
    path = Path(path)
    if path.suffix.lower() == ".zip":
        with zipfile.ZipFile(path) as archive:
            candidates = [x for x in archive.infolist() if not x.is_dir()]
            if member is None:
                if len(candidates) != 1:
                    raise ValueError("ZIP must contain one file; otherwise specify --member")
                entry = candidates[0]
            else:
                matches = [x for x in candidates if x.filename == member]
                if len(matches) != 1:
                    raise ValueError("ZIP member missing or ambiguous")
                entry = matches[0]
            if entry.file_size != SIZE:
                raise ValueError(f"Expected {SIZE} ROM bytes, got {entry.file_size}")
            return archive.read(entry)
    if member is not None:
        raise ValueError("--member is only supported for ZIP input")
    if path.stat().st_size != SIZE:
        raise ValueError(f"Expected {SIZE} ROM bytes, got {path.stat().st_size}")
    return path.read_bytes()


def identify(raw):
    crc = f"{zlib.crc32(raw):08x}"
    sha = hashlib.sha1(raw).hexdigest()
    return {"bytes": len(raw), "crc32": crc, "sha1": sha,
            "sha256": hashlib.sha256(raw).hexdigest(),
            "matches_known_an1x_104": len(raw) == SIZE and crc == CRC32 and sha == SHA1}


class RomView:
    """Immutable CPU byte view for MAME ROM_LOAD16_WORD_SWAP, not a CPU emulator."""
    def __init__(self, raw):
        if not raw or len(raw) % 2:
            raise ValueError("ROM must contain complete 16-bit words")
        self._raw = bytes(raw)

    def read8(self, address):
        if not 0 <= address < len(self._raw):
            raise ValueError(f"Address outside ROM: {address:#x}")
        return self._raw[address ^ 1]

    def read32be(self, address):
        return int.from_bytes(bytes(self.read8(address + i) for i in range(4)), "big")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("rom", type=Path)
    parser.add_argument("--member")
    args = parser.parse_args()
    try:
        raw = read_input(args.rom, args.member)
        result = identify(raw)
        if result["matches_known_an1x_104"]:
            view = RomView(raw)
            vector = view.read32be(0)
            result.update(reset_vector_word=f"0x{vector:08x}",
                          reset_target_24bit=f"0x{vector & 0xffffff:06x}",
                          reset_target_inside_rom=(vector & 0xffffff) < len(raw))
        result["firmware_executed"] = False
        print(json.dumps(result, indent=2))
        return 0 if result["matches_known_an1x_104"] else 2
    except (OSError, ValueError, zipfile.BadZipFile, KeyError, RuntimeError) as error:
        parser.exit(2, f"ROM input error: {error}\n")


if __name__ == "__main__":
    raise SystemExit(main())
