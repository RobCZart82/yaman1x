"""Locked H8 instruction-list selection inventory, not complete decoder proof."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
from check_h8_first_instructions import NAMES, H8H_NAMES


def inventory(raw, selected):
    rows=[]
    for number,line in enumerate(raw.decode('utf-8').splitlines(),1):
        if not line or line[0].isspace() or line.startswith('#') or line.startswith('macro '):continue
        t=line.split()
        if len(t) in (2,3):continue  # Internal states, not instruction encodings.
        if len(t) not in (6,7) or not re.fullmatch('[0-9a-f]+',t[0]) or not re.fullmatch('[0-9a-f]+',t[1]) or len(t[0])!=len(t[1]):
            raise ValueError('Unrecognized instruction header')
        profile=t[6] if len(t)==7 else 'common'
        if profile not in ('common','o','h','s20','s26','g'):raise ValueError('Unknown source profile')
        name=t[3].replace('.','_')+''.join('_'+p for p in t[4:6] if p!='-')
        eligible=profile in ('common','h')
        status=('selected_handler' if name in selected else 'missing_handler') if eligible else 'excluded_profile'
        rows.append(dict(line=number,value=t[0],mask=t[1],skip=int(t[2]),mnemonic=t[3],operands=t[4:6],method=name,profile=profile,status=status))
    return dict(schema_version=1,source_sha256=hashlib.sha256(raw).hexdigest(),profile='advanced H8H, fixed reset-SYSCR research host',
                counts=dict(sorted(Counter(r['status'] for r in rows).items())),instructions=rows,
                complete_cpu=False,decoder_proven=False,
                limits='Encoding rows, not opcode-word coverage percentages. Selected means full/partial bodies are selected; dispatch, prefix continuation and semantics require separate tests. Internal states/macros excluded. H8S/GT913/base-only rows excluded from H8H.')


def build(path):
    root=Path(__file__).resolve().parents[1]
    lock=json.loads((root/'research/h8-generator-lock.json').read_text())
    raw=path.read_bytes()
    if hashlib.sha256(raw).hexdigest()!=lock['inputs']['src/devices/cpu/h8/h8.lst']:raise ValueError('Instruction list differs from lock')
    result=inventory(raw,set(NAMES+H8H_NAMES));result['upstream_commit']=lock['upstream_commit']
    return result


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--list',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();result=build(a.list)
    with a.output.open('x',encoding='utf-8',newline='\n') as f:f.write(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result['counts'],indent=2))
