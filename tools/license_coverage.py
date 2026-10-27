"""Check reviewed third-party source/notice coverage; not legal or release clearance."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re

from repository_audit import candidate_paths


def audit(root):
    root = Path(root).resolve()
    problems, covered, terms = [], set(), set()

    def read(name):
        path = PurePosixPath(name)
        if not name or path.is_absolute() or '..' in path.parts or '\\' in name or ':' in name:
            raise ValueError('unsafe coverage path')
        target = root / name
        if target.is_symlink() or root not in target.resolve().parents:
            raise ValueError('coverage path escapes root')
        return target.read_bytes()

    def notices(record):
        text = read(record['path']).decode('utf-8')
        for notice in record['required_notices']:
            if notice not in text:
                problems.append({'path': record['path'], 'reason': 'required notice missing: ' + notice})

    try:
        manifest = json.loads(read('third_party/license-coverage.json'))
        if manifest['schema_version'] != 1 or not manifest['components']:
            raise ValueError('unsupported or empty coverage manifest')
        ids = set()
        for component in manifest['components']:
            if component['id'] in ids or not component['license'] or not component['source']:
                raise ValueError('invalid component identity')
            ids.add(component['id'])
            if not component['files'] or not component['license_files']:
                raise ValueError('component missing files or terms')
            for record in component['license_files']:
                if hashlib.sha256(read(record['path'])).hexdigest() != record['sha256']:
                    problems.append({'path': record['path'], 'reason': 'license text differs from reviewed copy'})
                terms.add(record['path'])
            for record in component['files']:
                if record['path'] in covered:
                    raise ValueError('duplicate covered file')
                covered.add(record['path'])
                notices(record)
        for record in manifest['notice_documents']:
            notices(record)
        for path in candidate_paths(root):
            name = path.as_posix()
            third_party_code = name.startswith('third_party/vop3/') or (
                name.startswith('third_party/') and path.suffix in {'.cpp', '.h', '.hpp', '.inc', '.patch', '.cmake'})
            research_patch = name.startswith('research/') and path.suffix == '.patch'
            marked_adaptation = False
            if path.parts[0] in {'src', 'cmake'} and path.suffix in {'.h', '.cpp', '.hpp', '.cmake', '.txt'}:
                text = read(name).decode('utf-8')
                marked_adaptation = bool(re.search(r'license:|SPDX-License-Identifier:|adapted from', text, re.I))
            if (third_party_code or research_patch or marked_adaptation) and name not in covered:
                problems.append({'path': name, 'reason': 'file missing from reviewed license coverage'})
    except (OSError, ValueError, KeyError, TypeError) as error:
        problems.append({'path': 'third_party/license-coverage.json', 'reason': str(error)})
    return dict(covered_files=len(covered), license_texts=len(terms), problems=problems,
                source_notice_coverage_passed=not problems,
                legal_clearance=False, binary_distribution_verified=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    result = audit(parser.parse_args().root)
    print(json.dumps(result, indent=2))
    if not result['source_notice_coverage_passed']:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
