"""Compile isolated pinned MAME bus method bodies against the candidate adapter."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import zipfile

METHODS = ('read16i', 'read8', 'write8', 'read16', 'write16')
SOURCE = 'src/devices/cpu/h8/h8.cpp'


def extract(raw):
    text = raw.decode('utf-8')
    bodies = []
    for name in METHODS:
        pattern = r'^(?:u8|u16|void) h8_device::' + name + r'\([^\n]*\)\n\{\n.*?^\}'
        matches = re.findall(pattern, text, re.M | re.S)
        if len(matches) != 1:
            raise ValueError('Expected one reviewed bus method: '+name)
        bodies.append(matches[0].replace('h8_device::', ''))
    return '\n'.join(bodies)


def check(archive, work, compiler='c++'):
    root = Path(__file__).resolve().parents[1]
    source_lock = json.loads((root/'research/mame-source-lock.json').read_text())
    lock = json.loads((root/'research/h8h-bus-provenance.json').read_text())
    if lock['upstream_commit'] != source_lock['commit']:
        raise ValueError('Bus and source locks disagree')
    digest = hashlib.sha256()
    with archive.open('rb') as f:
        for chunk in iter(lambda: f.read(1024*1024), b''):
            digest.update(chunk)
    if digest.hexdigest() != source_lock['archive_sha256']:
        raise ValueError('Archive differs from locked source')
    with zipfile.ZipFile(archive) as z:
        members = [i for i in z.infolist() if i.filename == 'mame-master/'+SOURCE]
        if len(members) != 1:
            raise ValueError('Missing or duplicate bus source')
        raw = z.read(members[0])
    identity = lock['inputs'][SOURCE]
    if len(raw) != identity['bytes'] or hashlib.sha256(raw).hexdigest() != identity['sha256']:
        raise ValueError('Bus source differs from lock')
    bodies = extract(raw)
    work = work.resolve(); work.mkdir(parents=True, exist_ok=False)
    header = work/'upstream-bus.h'
    header.write_text('''// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
struct Upstream {
    struct Cache { Bus& bus; u16 read_word(u32 a){return bus.read_instruction_word(a);} };
    Bus& m_program;
    Cache m_cache;
    bool m_has_exr=false;
    int m_icount=0;
    explicit Upstream(Bus& b): m_program(b),m_cache{b} {}
'''+bodies+'\n};\n')
    exe = work/'bus-reference-check'
    subprocess.run([compiler, '-std=c++20', '-O2', '-I'+str(root/'src'),
                    '-DYAMANIX_H8_BUS_REFERENCE_HEADER="'+str(header)+'"',
                    str(root/'tests/h8h_bus_access_check.cpp'), '-o', str(exe)], check=True, timeout=120)
    subprocess.run([str(exe)], check=True, timeout=60)
    return dict(upstream_commit=lock['upstream_commit'], source_sha256=identity['sha256'],
                methods=list(METHODS), operations=200000, differences=0,
                scope='H8H bus helpers only; not full CPU, firmware or hardware validation')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive', type=Path, required=True)
    p.add_argument('--work', type=Path, required=True)
    p.add_argument('--compiler', default='c++')
    a = p.parse_args()
    print(json.dumps(check(a.archive, a.work, a.compiler), indent=2))


if __name__ == '__main__':
    main()
