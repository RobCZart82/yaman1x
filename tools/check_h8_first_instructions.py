"""Verify selected generated H8 bodies; optionally compare against original dispatch."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import zipfile
from check_h8_generation import check as generate, verify_output

BIT_REG = ('bset','bnot','bclr','btst')
BIT_IMM = BIT_REG + ('bst','bist','bor','bior','bxor','bixor','band','biand','bld','bild')
STACK_CONTROL = ('rts', 'bsr_rel8', 'bsr_rel16', 'jmp_r32h', 'jmp_abs24e', 'jmp_abs8i', 'jsr_r32h', 'jsr_abs24e', 'jsr_abs8i', 'rte', 'mov_b_r32ph_r8l', 'mov_b_r8l_pr32h', 'mov_w_r32ph_r16l', 'mov_w_r16l_pr32h', 'mov_l_r32ph_r32l', 'mov_l_r32l_pr32h')
SHIFTS = ('shll','shal','shlr','shar','rotxl','rotl','rotxr','rotr')
BRANCHES = ('bt','bf','bhi','bls','bcc','bcs','bne','beq','bvc','bvs','bpl','bmi','bge','blt','bgt','ble')
ABSOLUTE_MEMORY = ('mov_b_abs8_r8u','mov_b_r8u_abs8','mov_b_abs16_r8l','mov_b_r8l_abs16','mov_w_abs16_r16l','mov_w_r16l_abs16')
NAMES = ABSOLUTE_MEMORY + ('not_b_r8l','neg_b_r8l','inc_b_one_r8l','dec_b_one_r8l','mov_b_r8h_r8l','mov_w_r16h_r16l','stc_ccr_r8l','ldc_r8l_ccr','orc_imm8_ccr','xorc_imm8_ccr','andc_imm8_ccr','state_reset', 'nop', 'mov_b_imm8_r8u','mov_w_imm16_r16l','dispatch_0100','dispatch_01f0','add_b_imm8_r8u','cmp_b_imm8_r8u','add_b_r8h_r8l','sub_b_r8h_r8l','cmp_b_r8h_r8l','ldc_imm8_ccr','add_w_r16h_r16l','sub_w_r16h_r16l','cmp_w_r16h_r16l','addx_b_imm8_r8u','subx_b_imm8_r8u','addx_b_r8h_r8l','subx_b_r8h_r8l','and_b_imm8_r8u','or_b_imm8_r8u','xor_b_imm8_r8u','and_b_r8h_r8l','or_b_r8h_r8l','xor_b_r8h_r8l') + tuple(n+'_rel8' for n in BRANCHES) + tuple(n+'_b_r8l' for n in SHIFTS) + tuple(n+'_r8h_r8l' for n in BIT_REG) + tuple(n+'_imm3_r8l' for n in BIT_IMM) + ('dispatch_7c00','dispatch_7d00','dispatch_7e00','dispatch_7f00') + tuple(n+'_r8h_abs8' for n in BIT_REG) + tuple(n+'_imm3_abs8' for n in BIT_IMM)
ABSOLUTE32_MEMORY = ('mov_b_abs32_r8l','mov_b_r8l_abs32','mov_w_abs32_r16l','mov_w_r16l_abs32')
LONG_ABSOLUTE_MEMORY = ('mov_l_abs16_r32l','mov_l_r32l_abs16','mov_l_abs32_r32l','mov_l_r32l_abs32')
H8H_NAMES = LONG_ABSOLUTE_MEMORY + ABSOLUTE32_MEMORY + ('state_irq','trapa_imm2','not_w_r16l','not_l_r32l','extu_w_r16l','extu_l_r32l','neg_w_r16l','neg_l_r32l','exts_w_r16l','exts_l_r32l','inc_w_one_r16l','inc_w_two_r16l','inc_l_one_r32l','inc_l_two_r32l','dec_w_one_r16l','dec_w_two_r16l','dec_l_one_r32l','dec_l_two_r32l','adds_l_one_r32l','adds_l_two_r32l','adds_l_four_r32l','subs_l_one_r32l','subs_l_two_r32l','subs_l_four_r32l','mov_l_r32h_r32l','mov_l_imm32_r32l','mov_b_r32ih_r8l','mov_b_r8l_r32ih','mov_w_r32ih_r16l','mov_w_r16l_r32ih','mov_l_r32ih_r32l','mov_l_r32l_r32ih','add_w_imm16_r16l','sub_w_imm16_r16l','cmp_w_imm16_r16l','add_l_imm32_r32l','sub_l_imm32_r32l','cmp_l_imm32_r32l','add_l_r32h_r32l','sub_l_r32h_r32l','cmp_l_r32h_r32l','and_w_imm16_r16l','or_w_imm16_r16l','xor_w_imm16_r16l','and_l_imm32_r32l','or_l_imm32_r32l','xor_l_imm32_r32l','or_w_r16h_r16l','xor_w_r16h_r16l','and_w_r16h_r16l','or_l_r32h_r32l','xor_l_r32h_r32l','and_l_r32h_r32l') + tuple(n+'_'+size+'_'+reg for n in SHIFTS for size,reg in (('w','r16l'),('l','r32l'))) + tuple(n+'_r8h_r32ihh' for n in BIT_REG) + tuple(n+'_imm3_r32ihh' for n in BIT_IMM) + STACK_CONTROL + tuple(n+'_rel16' for n in BRANCHES)
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


def render(raw, names=NAMES, device='h8_device'):
    text = raw.decode('utf-8')
    prefix = PREFIX if device=='h8_device' else PREFIX.replace('h8.hxx','h8h.hxx')
    return prefix+'\n\n'.join(generated_method(text,n+'_'+s,device) for n in names for s in ('full','partial'))+'\n'


def verify_selected(raw, root, h8h=False):
    lock = json.loads((root/'research/h8-generator-lock.json').read_text())
    name = 'h8h.hxx' if h8h else 'h8.hxx'
    normalized = verify_output(raw, lock['outputs'][name])
    selected = render(normalized,H8H_NAMES,'h8h_device') if h8h else render(normalized)
    path = 'src/h8_first_h8h_handlers.h' if h8h else 'src/h8_first_handlers.h'
    if selected != (root/path).read_text(encoding='utf-8'):
        raise ValueError('Selected handlers differ from locked generated bodies')
    return selected


def original_method(text, name):
    found = re.findall(r'^(?:void|int|u8|u16|u32) h8_device::'+name+r'\([^\n]*\)\n\{\n.*?^\}',text,re.M|re.S)
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
    selected += verify_selected((work/'gen/0-h8h.hxx').read_bytes(),root,True)
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
                     ('do_inc8','do_inc16','do_inc32','do_dec8','do_dec16','do_dec32','read16i','read8','write8','read16','write16','set_nzv8','set_nzv16','set_nzv32','do_add8','do_sub8','do_addx8','do_subx8','do_add16','do_sub16','do_add32','do_sub32','internal','take_interrupt','prefetch_done_notrace','prefetch_done','prefetch_done_noirq','recompute_bcount','execute_run') + tuple('do_'+n+str(bits) for n in SHIFTS for bits in (8,16,32)))
    host += '\n'+original_method(sources['h83002.cpp'].replace('h83002_device::','h8_device::'),'trapa_setup')
    inline = []
    for filename,name in [('h8.h','r8_w'),('h8.h','r8_r'),('h8.h','r16_w'),('h8.h','r16_r'),('h8.h','prefetch_switch'),('h8h.h','r32_w'),('h8h.h','r32_r')]:
        header = sources[filename]
        if name in ('r8_w','r8_r'):
            pattern = r'\tinline (?:void|u8) '+name+r'\([^\n]*\) \{\n.*?^\t\}'
        else:
            pattern = r'\t(?:inline )?(?:void|u16|u32) '+name+r'\([^\n]*\)(?: const)? \{[^\n]*\}'
        found = re.findall(pattern,header,re.M|re.S)
        if len(found)!=1:
            raise ValueError('Expected one inline host helper: '+name)
        inline.append(found[0])
    raw = (work/'gen/0-h8h.hxx').read_bytes()
    verify_output(raw,generator_lock['outputs']['h8h.hxx'])
    dispatch = '\n'.join(generated_method(raw.decode(),n,'h8h_device') for n in ('do_exec_full','do_exec_partial'))
    supported = {n+'_'+s for n in NAMES+H8H_NAMES for s in ('full','partial')}|{'do_exec_full','do_exec_partial'}
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
    p.add_argument('--h8h',action='store_true',help='Verify an h8h.hxx selection with --generated')
    p.add_argument('--archive',type=Path)
    p.add_argument('--work',type=Path)
    p.add_argument('--cmake',default='cmake')
    a=p.parse_args()
    if a.generated and not a.archive:
        verify_selected(a.generated.read_bytes(),Path(__file__).resolve().parents[1],a.h8h)
        print('Selected generated H8 handlers match locked output')
    elif a.archive and a.work and not a.generated:
        check(a.archive,a.work,a.cmake)
    else:p.error('Use --generated, or --archive with --work')
