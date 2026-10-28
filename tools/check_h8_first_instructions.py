"""Verify selected generated H8 bodies; optionally compare against original dispatch."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import zipfile
from check_h8_generation import check as generate, verify_output

NAMES = ('state_reset', 'nop', 'mov_b_imm8_r8u', 'bt_rel8')
PREFIX = ('// license:BSD-3-Clause\n// copyright-holders:Olivier Galibert\n'
          '// Selected pinned generated h8.hxx methods; only class qualifiers removed.\n'
          '// Included inside H8FirstInstructions. See research/h8-first-instructions-provenance.json.\n')


def generated_method(text, name, device='h8_device'):
    pattern = r'^void '+device+r'::'+re.escape(name)+r'\(\)\n'
    matches = list(re.finditer(pattern, text, re.M))
    if len(matches) != 1:
        raise ValueError('Expected one generated method: '+name)
    start = matches[0].start()
    end = text.find('\nvoid ', start+1)
    result = text[start:end if end >= 0 else len(text)].rstrip()
    if not result.endswith('}'):
        raise ValueError('Incomplete generated method')
    return result.replace(device+'::', '', 1)


def render(raw):
    text = raw.decode('utf-8')
    return PREFIX+'\n\n'.join(generated_method(text,n+'_'+s) for n in NAMES for s in ('full','partial'))+'\n'


def verify_selected(raw, root):
    lock = json.loads((root/'research/h8-generator-lock.json').read_text())
    normalized = verify_output(raw, lock['outputs']['h8.hxx'])
    selected = render(normalized)
    if selected != (root/'src/h8_first_handlers.h').read_text(encoding='utf-8'):
        raise ValueError('Selected handlers differ from locked generated bodies')
    return selected


def original_method(text, name):
    found = re.findall(r'^(?:void|u16) h8_device::'+name+r'\([^\n]*\)\n\{\n.*?^\}',text,re.M|re.S)
    if len(found) != 1:
        raise ValueError('Expected one original host method: '+name)
    return found[0].replace('h8_device::','',1)


def check(archive, work, cmake='cmake'):
    root = Path(__file__).resolve().parents[1]
    work = work.resolve()
    if work.exists():
        raise ValueError('Reference work directory must be new')
    generate(archive,work/'gen') # Archive and generator hashes checked before execution/output.
    selected = verify_selected((work/'gen/0-h8.hxx').read_bytes(),root)
    lock = json.loads((root/'research/h8-first-instructions-provenance.json').read_text())
    generator_lock = json.loads((root/'research/h8-generator-lock.json').read_text())
    if lock['upstream_commit'] != generator_lock['upstream_commit']:
        raise ValueError('Source revisions disagree')
    with zipfile.ZipFile(archive) as z:
        sources = {}
        for name, expected in lock['inputs'].items():
            members = [i for i in z.infolist() if i.filename=='mame-master/'+name]
            if len(members)!=1:
                raise ValueError('Missing or duplicate source')
            raw = z.read(members[0])
            if len(raw)!=expected['bytes'] or hashlib.sha256(raw).hexdigest()!=expected['sha256']:
                raise ValueError('Source identity differs')
            sources[Path(name).name] = raw.decode('utf-8')
    host = '\n'.join(original_method(sources['h8.cpp'],n) for n in
                     ('read16i','set_nzv8','prefetch_done','prefetch_done_noirq','recompute_bcount','execute_run'))
    header = sources['h8.h']
    inline = []
    for name in ('r8_w','prefetch_switch'):
        pattern = (r'\tinline void r8_w\([^\n]*\) \{\n.*?^\t\}' if name=='r8_w'
                   else r'\tvoid prefetch_switch\([^\n]*\) \{[^\n]*\}')
        found = re.findall(pattern,header,re.M|re.S)
        if len(found)!=1:
            raise ValueError('Expected one inline host helper')
        inline.append(found[0])
    raw = (work/'gen/0-h8h.hxx').read_bytes()
    verify_output(raw,generator_lock['outputs']['h8h.hxx'])
    dispatch = '\n'.join(generated_method(raw.decode(),n,'h8h_device') for n in ('do_exec_full','do_exec_partial'))
    supported = {n+'_'+s for n in NAMES for s in ('full','partial')}|{'do_exec_full','do_exec_partial'}
    unsupported = sorted(set(re.findall(r'\b(\w+_(?:full|partial))\(\)',dispatch))-supported)
    stubs = '\n'.join('void '+n+'(){throw std::runtime_error("Unsupported reference instruction");}' for n in unsupported)
    fixture = (root/'tests/h8_first_reference.cpp').read_text(encoding='utf-8')
    for marker,value in [('SELECTED',selected),('HOST',host+'\n'+'\n'.join(inline)),('DISPATCH',dispatch),('STUBS',stubs)]:
        fixture = fixture.replace('/* '+marker+' */',value)
    (work/'reference.cpp').write_text('// license:BSD-3-Clause\n// copyright-holders:Olivier Galibert\n'+fixture,encoding='utf-8')
    (work/'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.20)\nproject(reference LANGUAGES CXX)\nset(CMAKE_CXX_STANDARD 20)\nadd_executable(reference reference.cpp)\ntarget_include_directories(reference PRIVATE "'+(root/'src').as_posix()+'")\n',encoding='utf-8')
    subprocess.run([cmake,'-S',str(work),'-B',str(work/'b')],check=True,timeout=120)
    subprocess.run([cmake,'--build',str(work/'b'),'--config','Release'],check=True,timeout=120)
    exe = next(p for p in (work/'b/Release/reference.exe',work/'b/reference',work/'b/reference.exe') if p.is_file())
    subprocess.run([str(exe)],check=True,timeout=60)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--generated',type=Path)
    p.add_argument('--archive',type=Path)
    p.add_argument('--work',type=Path)
    p.add_argument('--cmake',default='cmake')
    a=p.parse_args()
    if a.generated and not a.archive:
        verify_selected(a.generated.read_bytes(),Path(__file__).resolve().parents[1])
        print('Selected generated H8 handlers match locked output')
    elif a.archive and a.work and not a.generated:
        check(a.archive,a.work,a.cmake)
    else:p.error('Use --generated, or --archive with --work')
