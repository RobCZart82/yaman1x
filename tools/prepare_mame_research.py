"""Preflight the locked source-only research patch chain; optionally prepare a checkout."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import tempfile
import zipfile


def safe_path(root, name):
    path = PurePosixPath(name)
    if path.is_absolute() or ".." in path.parts or "\\" in name or ":" in name:
        raise ValueError("Unsafe source path")
    target = root / path
    if target.is_symlink() or root.resolve() not in target.resolve().parents:
        raise ValueError("Source path escapes the work directory")
    return target


def validate_inputs(root, lock):
    for name, digest in lock["inputs"].items():
        source = safe_path(root, name)
        if hashlib.sha256(source.read_bytes()).hexdigest() != digest:
            raise ValueError(f"Source input differs from lock: {name}")


def patch_preflight(root, repo, lock):
    """Only mutate a temporary subset; a late patch failure leaves source untouched."""
    validate_inputs(root, lock)
    with tempfile.TemporaryDirectory(prefix="yamanix-patch-check-") as temporary:
        stage = Path(temporary)
        for name in lock["inputs"]:
            target = safe_path(stage, name)
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(safe_path(root, name), target)
        for name in lock["patches"]:
            patch = safe_path(repo / "research", name)
            result = subprocess.run(
                ["git", "-c", "core.autocrlf=false", "-c", "core.eol=lf",
                 "apply", "--unsafe-paths", "--whitespace=error", str(patch.resolve())],
                cwd=stage, capture_output=True, text=True)
            if result.returncode:
                raise ValueError(f"Research patch does not apply: {name}\n{result.stderr}")
        for name, original in lock["generated_headers"].items():
            if safe_path(stage, name).read_bytes() != safe_path(repo, original).read_bytes():
                raise ValueError(f"Injected helper differs from tested source: {name}")
        expected = set(lock["inputs"]) | set(lock["generated_headers"])
        actual = {p.relative_to(stage).as_posix() for p in stage.rglob("*") if p.is_file()}
        if actual != expected:
            raise ValueError("Patch outputs differ from the reviewed file set")
        return {name: safe_path(stage, name).read_bytes() for name in sorted(expected)}


def prepare_checkout(root, repo, lock):
    root = root.resolve()
    def git(*args):
        return subprocess.run(["git", "-C", str(root), *args], check=True,
                              capture_output=True, text=True).stdout.strip()
    if Path(git("rev-parse", "--show-toplevel")).resolve() != root:
        raise ValueError("Expected the MAME checkout root")
    if git("rev-parse", "HEAD") != lock["commit"]:
        raise ValueError("MAME checkout commit differs from lock")
    if git("status", "--porcelain", "--untracked-files=all"):
        raise ValueError("Refusing to patch a dirty MAME checkout")
    prepared = patch_preflight(root, repo, lock)
    for name in prepared:
        target = safe_path(root, name)
        if name in lock["generated_headers"] and target.exists():
            raise ValueError("Generated helper already exists")
    # All patches and helpers passed before modifying any checkout source.
    for name, contents in prepared.items():
        target = safe_path(root, name)
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(contents)
    return prepared


def check_archive(archive, repo, lock):
    with archive.open("rb") as stream:
        hasher = hashlib.sha256()
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(chunk)
        digest = hasher.hexdigest()
    if digest != lock["archive_sha256"]:
        raise ValueError("MAME archive SHA-256 differs from lock")
    with tempfile.TemporaryDirectory(prefix="yamanix-source-check-") as temporary:
        root = Path(temporary)
        with zipfile.ZipFile(archive) as source:
            # Extract only four reviewed text inputs, never a firmware/ROM member.
            for name in lock["inputs"]:
                target = safe_path(root, name)
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(source.read("mame-master/" + name))
        return patch_preflight(root, repo, lock)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    choice = parser.add_mutually_exclusive_group(required=True)
    choice.add_argument("--archive", type=Path, help="Check the original locked ZIP without changing it")
    choice.add_argument("--source", type=Path, help="Prepare a clean checkout at the locked Git commit")
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    lock = json.loads((repo / "research/mame-source-lock.json").read_text())
    try:
        prepared = (check_archive(args.archive, repo, lock) if args.archive else
                    prepare_checkout(args.source, repo, lock))
        print(json.dumps(dict(commit=lock["commit"], patches=len(lock["patches"]),
                              prepared_files=len(prepared), helpers_verified=True,
                              checkout_modified=args.source is not None)))
    except (OSError, ValueError, KeyError, subprocess.CalledProcessError, zipfile.BadZipFile) as error:
        parser.exit(2, f"Research preparation failed: {error}\n")


if __name__ == "__main__":
    main()
