"""Check the candidate repository for excluded data and changed vendored sources.

Uses Git's tracked + non-ignored untracked file list when a .git entry exists.
It never prints matching secret content. This is a project-specific upload
check, not a general license, authenticity or credential-security guarantee.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import subprocess

ALLOWED = {'.cpp', '.h', '.hpp', '.inc', '.py', '.md', '.txt', '.json', '.patch',
           '.cmake', '.yaml', '.yml', '.natvis'}
NAMES = {'CMakeLists.txt', 'LICENSE', '.gitignore', '.gitattributes', '.gitkeep'}
EXCLUDED_DIRS = {'work', 'private_runs', 'roms', 'vendor', '__pycache__'}
PATTERNS = [
    ('personal absolute path', re.compile(r'/Users/[A-Za-z0-9._-]+/|[A-Za-z]:\\Users\\')),
    ('possible GitHub credential', re.compile(r'\b(?:gh[pousr]_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{30,})')),
    ('private key', re.compile(r'-----BEGIN (?:[A-Z ]+ )?PRIVATE KEY-----')),
    ('binary patch payload', re.compile(r'^GIT binary patch$', re.M)),
]


def candidate_paths(root):
    if (root/'.git').exists():
        result = subprocess.run(['git', '-C', str(root), 'ls-files', '-z', '--cached', '--others', '--exclude-standard'],
                                capture_output=True, check=True)
        return sorted(set(Path(name.decode('utf-8')) for name in result.stdout.split(b'\0') if name))
    # Before initial Git staging, inspect all candidate files except generated caches/builds.
    return sorted(p.relative_to(root) for p in root.rglob('*')
                  if (p.is_file() or p.is_symlink()) and '.git' not in p.relative_to(root).parts
                  and '__pycache__' not in p.relative_to(root).parts
                  and not any(part.startswith('build') for part in p.relative_to(root).parts[:-1]))


def audit(root):
    root = root.resolve()
    problems = []
    def report(path, reason): problems.append({'path': str(path), 'reason': reason})
    paths = candidate_paths(root)
    for relative in paths:
        p = root/relative
        if relative.is_absolute() or '..' in relative.parts:
            report(relative, 'path outside repository'); continue
        if p.is_symlink() or root not in p.resolve().parents:
            report(relative, 'symlink or path outside repository'); continue
        if any(part in EXCLUDED_DIRS or part.startswith('build') for part in relative.parts[:-1]):
            report(relative, 'local data/build directory'); continue
        if relative.name not in NAMES and relative.suffix.lower() not in ALLOWED:
            report(relative, 'unapproved file type'); continue
        if not p.is_file():
            report(relative, 'missing candidate file'); continue
        raw = p.read_bytes()
        if len(raw) > 2_000_000 or b'\0' in raw:
            report(relative, 'binary or oversized content'); continue
        try: content = raw.decode('utf-8')
        except UnicodeDecodeError:
            report(relative, 'non-UTF-8 content'); continue
        for reason, pattern in PATTERNS:
            if pattern.search(content): report(relative, reason)
    verified = 0
    try:
        manifest = json.loads((root/'third_party/provenance.json').read_text())
        listed = set()
        for record in manifest['files']:
            name = PurePosixPath(record['path'])
            if name.is_absolute() or '..' in name.parts or name.parts[:2] != ('third_party', 'vop3'):
                raise ValueError('unsafe upstream manifest path')
            if name.as_posix() in listed: raise ValueError('duplicate upstream manifest path')
            listed.add(name.as_posix())
            path = root/name
            if path.is_symlink() or root not in path.resolve().parents: raise ValueError('unsafe upstream file')
            raw = path.read_bytes()
            if len(raw) != record['bytes'] or hashlib.sha256(raw).hexdigest() != record['sha256']:
                report(name, 'vendored source differs from recorded snapshot')
            else: verified += 1
        actual = {p.relative_to(root).as_posix() for p in (root/'third_party/vop3').rglob('*') if p.is_file()}
        if actual != listed: report('third_party/vop3', 'unlisted or missing upstream files')
    except (OSError, ValueError, KeyError, TypeError) as error:
        report('third_party/provenance.json', 'invalid or incomplete upstream provenance')
    return dict(checked_files=len(paths), upstream_files_verified=verified, problems=problems,
                upload_check_passed=not problems)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    try: result = audit(args.root)
    except (OSError, subprocess.CalledProcessError) as error:
        parser.exit(2, 'Repository file listing failed\n')
    print(json.dumps(result, indent=2))
    if not result['upload_check_passed']: parser.exit(1)


if __name__ == '__main__': main()
