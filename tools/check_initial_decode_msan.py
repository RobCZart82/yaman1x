"""Controlled constructor-only MSan experiment against original and candidate headers."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ORIGINAL_SHA256 = "83de64a33c17f9334ebbf2b19e707e5a7027b4e1729146ce37150714166da76e"


def run_probe(binary):
    env = os.environ.copy()
    env["MSAN_OPTIONS"] = "exit_code=86:halt_on_error=1"
    # Some ASLR placements cannot map MSan shadow memory. Retry only that
    # explicit runtime startup failure, never a diagnostic or a program crash.
    for attempt in range(5):
        result = subprocess.run([str(binary)], env=env, capture_output=True, text=True, timeout=30)
        if "MemorySanitizer can not mmap the shadow memory" not in result.stderr:
            return result
    raise RuntimeError("MSan shadow mapping unavailable after five attempts")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="clang++-18")
    parser.add_argument("--work", type=Path, required=True)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    original = (repo / "third_party/vop3/src/yss236_core.h").read_bytes()
    if hashlib.sha256(original).hexdigest() != ORIGINAL_SHA256:
        raise RuntimeError("Expected the unchanged reviewed upstream header")
    candidate = original.replace(b"\top ops[STEPS];", b"\top ops[STEPS]{};")
    if candidate == original:
        raise RuntimeError("Candidate initialization edit did not apply")
    args.work.mkdir(parents=True, exist_ok=False)
    results = {}
    for name, header in (("original", original), ("candidate", candidate)):
        directory = args.work / name
        directory.mkdir()
        (directory / "yss236_core.h").write_bytes(header)
        binary = directory / "probe"
        subprocess.run([args.compiler, "-std=c++20", "-O0", "-g", "-fno-omit-frame-pointer",
                        "-fsanitize=memory", "-fsanitize-memory-track-origins=2", "-fPIE", "-pie",
                        "-DYAMANIX_INIT_STANDALONE", "-I", str(directory),
                        str(repo / "tests/core_init_check.cpp"), "-o", str(binary)], check=True)
        result = run_probe(binary.resolve())
        if name == "original":
            if (result.returncode != 86 or "MemorySanitizer: use-of-uninitialized-value" not in result.stderr
                    or "memcmp" not in result.stderr.lower()):
                raise RuntimeError("Original did not produce the expected memcmp diagnostic:\n" + result.stderr)
            print(result.stderr)
            results[name] = "expected uninitialized memcmp read reproduced"
        else:
            if result.returncode or "MemorySanitizer:" in result.stderr or not result.stdout.startswith("PASS:"):
                raise RuntimeError("Candidate did not pass:\n" + result.stdout + result.stderr)
            print(result.stdout)
            results[name] = "clean construction/decode probe"
    print(json.dumps(results))


if __name__ == "__main__":
    main()
