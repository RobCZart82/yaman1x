"""Verify and execute the pinned upstream H8 generator without firmware inputs."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import zipfile


def verify_inputs(files, lock):
    for name, expected in lock['inputs'].items():
        if hashlib.sha256(files[name]).hexdigest() != expected:
            raise ValueError('Generator input differs from lock: ' + name)


def verify_output(raw, expected):
    normalized = raw.replace(b'\r\n', b'\n')
    if (len(normalized) != expected['lf_bytes'] or
            hashlib.sha256(normalized).hexdigest() != expected['lf_sha256']):
        raise ValueError('Generated output differs from lock')
    return normalized


def check(archive, work):
    repo = Path(__file__).resolve().parents[1]
    lock = json.loads((repo/'research/h8-generator-lock.json').read_text())
    source = json.loads((repo/'research/mame-source-lock.json').read_text())
    if lock['upstream_commit'] != source['commit']:
        raise ValueError('Generator and research source revisions disagree')
    digest = hashlib.sha256()
    with archive.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024*1024), b''):
            digest.update(chunk)
    if digest.hexdigest() != source['archive_sha256']:
        raise ValueError('Archive differs from locked source')
    # Literal reviewed member set: never extract arbitrary archive paths.
    names = ('src/devices/cpu/h8/h8make.py', 'src/devices/cpu/h8/h8.lst', 'scripts/src/cpu.lua')
    if set(lock['inputs']) != set(names):
        raise ValueError('Unexpected generator input set')
    with zipfile.ZipFile(archive) as zipped:
        files = {}
        for name in names:
            members = [i for i in zipped.infolist() if i.filename == 'mame-master/'+name]
            if len(members) != 1:
                raise ValueError('Missing or duplicate generator member')
            files[name] = zipped.read(members[0])
    verify_inputs(files, lock)  # No external code executes before hash checks.
    expected_jobs = {'h8.hxx': ('s', 'o'), 'h8h.hxx': ('s', 'h'),
                     'h8d.hxx': ('d', 'o'), 'h8hd.hxx': ('d', 'h')}
    if set(lock['outputs']) != set(expected_jobs):
        raise ValueError('Unexpected generated output set')
    work = work.resolve()
    work.mkdir(parents=True, exist_ok=False)
    for name, raw in files.items():
        (work/Path(name).name).write_bytes(raw)
    for filename, (mode, kind) in expected_jobs.items():
        expected = lock['outputs'][filename]
        if (expected['mode'], expected['type']) != (mode, kind):
            raise ValueError('Generator arguments differ from reviewed jobs')
        # Two independent output paths detect unstable generation. Normalize
        # only CRLF in generated text; inputs stay byte-exact.
        for repeat in range(2):
            output = work/(str(repeat)+'-'+filename)
            subprocess.run([sys.executable, str(work/'h8make.py'), str(work/'h8.lst'),
                            mode, kind, str(output)], cwd=work, check=True, timeout=60)
            output.write_bytes(verify_output(output.read_bytes(), expected))
    report = dict(upstream_commit=source['commit'], inputs_verified=3,
                  outputs_verified=4, independent_passes=2,
                  compiler_execution=False, firmware_execution=False)
    (work/'generation-report.json').write_text(json.dumps(report, indent=2)+'\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--work', type=Path, required=True)
    args = parser.parse_args()
    try:
        print(json.dumps(check(args.archive, args.work), indent=2))
    except (OSError, ValueError, KeyError, subprocess.SubprocessError, zipfile.BadZipFile) as error:
        parser.exit(2, f'H8 generation check failed: {error}\n')


if __name__ == '__main__':
    main()
