"""Read-only H83002 source dependency inventory, not a compiler dependency closure."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import zipfile

PREFIX = 'src/devices/cpu/h8/'
INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"\n]+)"', re.M)


def inventory(files):
    pending = [PREFIX + 'h83002.cpp', PREFIX + 'h83002.h']
    seen, records, boundaries = set(), [], []
    while pending:
        path = pending.pop()
        if path in seen:
            continue
        seen.add(path)
        if path not in files:
            raise ValueError('Required H8 source missing: ' + path)
        raw = files[path]
        text = raw.decode('utf-8')
        includes = INCLUDE.findall(text)
        notices = [line.strip() for line in text.splitlines()[:20]
                   if 'license:' in line or 'copyright-holders:' in line]
        records.append(dict(path=path, bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest(),
                            notices=notices, quoted_includes=includes))
        for include in includes:
            name = PurePosixPath(include)
            if name.is_absolute() or '..' in name.parts or '\\' in include or ':' in include:
                raise ValueError('Unsafe include path')
            local = PREFIX + include
            if local in files:
                pending.append(local)
            else:
                boundaries.append(dict(source=path, include=include,
                                       kind='generated-or-external'))
        # Pair implementation/header files conservatively; headers alone do
        # not describe runtime dependencies. This deliberately over-approximates.
        paired = str(PurePosixPath(path).with_suffix('.cpp' if path.endswith('.h') else '.h'))
        if path.endswith(('.h', '.cpp')) and paired in files:
            pending.append(paired)
    return dict(schema_version=1, roots=[PREFIX+'h83002.cpp', PREFIX+'h83002.h'],
                files=sorted(records, key=lambda row: row['path']),
                boundaries=sorted(boundaries, key=lambda row: (row['source'], row['include'])),
                complete_compiler_closure=False, license_clearance=False,
                note='Quoted-include traversal plus matching cpp/h pairs only. Conditional branches are not evaluated; generated code, framework services, symbols, build rules and transitive external dependencies require separate review.')


def from_archive(path, expected_sha256):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024*1024), b''):
            digest.update(chunk)
    if digest.hexdigest() != expected_sha256:
        raise ValueError('Archive differs from locked source')
    with zipfile.ZipFile(path) as archive:
        files = {}
        for entry in archive.infolist():
            name = entry.filename.removeprefix('mame-master/')
            if name.startswith(PREFIX) and name.endswith(('.h', '.cpp', '.lst', '.py')):
                if name in files:
                    raise ValueError('Duplicate source member')
                files[name] = archive.read(entry)
    result = inventory(files)
    result['archive_sha256'] = expected_sha256
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[1]
    lock = json.loads((repo/'research/mame-source-lock.json').read_text())
    try:
        result = from_archive(args.archive, lock['archive_sha256'])
        result['upstream_commit'] = lock['commit']
        text = json.dumps(result, indent=2) + '\n'
        if args.output:
            with args.output.open('x', encoding='utf-8', newline='\n') as output:
                output.write(text)
        else:
            print(text, end='')
    except (OSError, ValueError, zipfile.BadZipFile) as error:
        parser.exit(2, f'H8 inventory failed: {error}\n')


if __name__ == '__main__':
    main()
