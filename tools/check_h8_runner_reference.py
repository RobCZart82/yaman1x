"""Optional locked-source execute_run ordering comparison; synthetic handlers only."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import zipfile


def methods(archive, root):
    lock = json.loads((root/'research/mame-source-lock.json').read_text())
    identity = json.loads((root/'research/h8-execution-runner-provenance.json').read_text())
    if identity['upstream_commit'] != lock['commit']:
        raise ValueError('Source revisions disagree')
    digest = hashlib.sha256()
    with archive.open('rb') as f:
        for chunk in iter(lambda: f.read(1024*1024), b''):
            digest.update(chunk)
    if digest.hexdigest() != lock['archive_sha256']:
        raise ValueError('Archive differs from locked source')
    name = 'src/devices/cpu/h8/h8.cpp'
    with zipfile.ZipFile(archive) as z:
        entries = [i for i in z.infolist() if i.filename == 'mame-master/'+name]
        if len(entries) != 1:
            raise ValueError('Missing or duplicate CPU source')
        raw = z.read(entries[0])
    expected = identity['inputs'][name]
    if len(raw) != expected['bytes'] or hashlib.sha256(raw).hexdigest() != expected['sha256']:
        raise ValueError('CPU source differs from lock')
    result = []
    for name in ['recompute_bcount', 'execute_run']:
        found = re.findall(r'^void h8_device::'+name+r'\([^\n]*\)\n\{\n.*?^\}', raw.decode(), re.M|re.S)
        if len(found) != 1:
            raise ValueError('Missing or duplicate CPU method')
        result.append(found[0].replace('h8_device::', ''))
    return '\n'.join(result)


def check(archive, work, cmake='cmake'):
    root = Path(__file__).resolve().parents[1]
    original = methods(archive, root)  # Identity checks precede all output/compilation.
    work = work.resolve()
    work.mkdir(parents=True, exist_ok=False)
    fixture = (root/'tests/h8_execution_runner_reference.cpp').read_text()
    (work/'reference.cpp').write_text('// license:BSD-3-Clause\n// copyright-holders:Olivier Galibert\n'+fixture.replace('/* ORIGINAL_METHODS */', original), encoding='utf-8')
    (work/'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.20)\nproject(reference LANGUAGES CXX)\nset(CMAKE_CXX_STANDARD 20)\nadd_executable(reference reference.cpp)\ntarget_include_directories(reference PRIVATE "'+(root/'src').as_posix()+'")\n')
    subprocess.run([cmake, '-S', str(work), '-B', str(work/'b')], check=True, timeout=120)
    subprocess.run([cmake, '--build', str(work/'b'), '--config', 'Release'], check=True, timeout=120)
    candidates = [work/'b/Release/reference.exe', work/'b/reference', work/'b/reference.exe']
    exe = next(p for p in candidates if p.is_file())
    subprocess.run([str(exe)], check=True, timeout=60)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive', type=Path, required=True)
    p.add_argument('--work', type=Path, required=True)
    p.add_argument('--cmake', default='cmake')
    a = p.parse_args()
    check(a.archive, a.work, a.cmake)
