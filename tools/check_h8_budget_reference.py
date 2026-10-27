"""Optional isolated original execute_run arithmetic comparison; no CPU opcodes."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import zipfile


def check(archive, work, compiler='c++'):
    root = Path(__file__).resolve().parents[1]
    lock = json.loads((root/'research/mame-source-lock.json').read_text())
    identity = json.loads((root/'research/h8-execution-budget-provenance.json').read_text())
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
        members = [i for i in z.infolist() if i.filename=='mame-master/'+name]
        if len(members)!=1:
            raise ValueError('Missing or duplicate CPU source')
        raw = z.read(members[0])
    expected = identity['inputs'][name]
    if len(raw)!=expected['bytes'] or hashlib.sha256(raw).hexdigest()!=expected['sha256']:
        raise ValueError('CPU source differs from lock')
    text = raw.decode('utf-8'); methods=[]
    for method in ['recompute_bcount', 'execute_run']:
        matches = re.findall(r'^void h8_device::'+method+r'\([^\n]*\)\n\{\n.*?^\}', text, re.M|re.S)
        if len(matches)!=1:
            raise ValueError('Missing or duplicate CPU method')
        methods.append(matches[0].replace('h8_device::',''))
    work=work.resolve(); work.mkdir(parents=True, exist_ok=False)
    source=work/'budget-reference.cpp'
    source.write_text('''// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
#include "h8_execution_budget.h"
#include <cstdint>
#include <cstdio>
using u64=uint64_t;
struct Upstream {
 int m_icount=0,m_bcount=0,m_count_before_instruction_step=0;
 int m_inst_substate=0,m_inst_state=0x10000,m_PPC=0,m_NPC=0;
 u64 end=0; unsigned actions=0;
 u64 total_cycles(){return end-u64(m_icount);}
 bool debugger_enabled(){return false;}
 void debugger_instruction_hook(int){}
 void internal_update(u64){recompute_bcount(0);}
 void do_exec_full(){m_icount-=2;++actions;}
 void do_exec_partial(){m_icount-=1;++actions;m_inst_substate=0;}
'''+ '\n'.join(methods)+'''
};
int main(){
 Upstream ref; yamanix::H8ExecutionBudget b;
 u64 start=0;unsigned actions=0,random=123;
 for(unsigned n=0;n<200000;++n){
  random=random*1664525u+1013904223u;int size=int(random%19);
  b.begin_slice(start,size);b.recompute_deadline(0);b.apply_carry();
  while(b.full_step_allowed()){b.counter()-=2;++actions;}
  b.finish_slice();ref.end=start+size;ref.m_icount=size;ref.execute_run();
  if(b.remaining()!=ref.m_icount || b.debt()!=ref.m_count_before_instruction_step || actions!=ref.actions)return 1;
  start+=size;
 }
 // Compare event-boundary arithmetic separately, without claiming an IRQ runner.
 for(unsigned n=0;n<200000;++n){
  int size=int(n%31);u64 start=1000000+n;
  yamanix::H8ExecutionBudget c;c.begin_slice(start,size);
  u64 event=n%7==0?0:start-10+(n%50);
  c.recompute_deadline(event);ref.end=start+size;ref.m_icount=size;ref.recompute_bcount(event);
  if(c.boundary()!=ref.m_bcount)return 2;
 }
 std::puts("Original execute_run: 200000 synthetic no-event slices; recompute_bcount: 200000 deadlines; zero differences");
}
''')
    exe=work/'budget-reference'
    subprocess.run([compiler,'-std=c++20','-O2','-I'+str(root/'src'),str(source),'-o',str(exe)],check=True,timeout=120)
    subprocess.run([str(exe)],check=True,timeout=60)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--work',type=Path,required=True)
    p.add_argument('--compiler',default='c++')
    a=p.parse_args();check(a.archive,a.work,a.compiler)


if __name__=='__main__':
    main()
