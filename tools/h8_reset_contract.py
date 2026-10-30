"""Read-only reset-method identity audit;not reset emulation or hardware proof."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import zipfile

METHODS={
 'h8.cpp':['h8_device::device_reset'],
 'h83002.cpp':['h83002_device::device_reset'],
 'h8_intc.cpp':['h8_intc_device::device_reset','h8h_intc_device::device_reset'],
 'h8_sci.cpp':['h8_sci_device::device_reset'],
 'h8_port.cpp':['h8_port_device::device_reset'],
}

def method_identity(raw,name):
    # Exact original formatting;closed method brace must begin at column zero.
    pattern=r'^void '+re.escape(name)+r'\(\)\n\{.*?^\}'
    found=re.findall(pattern,raw.decode('utf-8'),re.M|re.S)
    if len(found)!=1:
        raise ValueError('Expected exactly one reset method: '+name)
    body=found[0].encode('utf-8')
    return dict(name=name,bytes=len(body),sha256=hashlib.sha256(body).hexdigest())

def report(files,commit):
    result=[]
    for leaf,names in METHODS.items():
        path='src/devices/cpu/h8/'+leaf
        raw=files[path]
        result.append(dict(path=path,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
                           methods=[method_identity(raw,n) for n in names]))
    return dict(schema_version=1,upstream_commit=commit,files=result,
                scope='Six original device_reset method identities only;not execution,global ordering,generated CPU reset,hardware fidelity or RAM retention proof.')

def from_archive(path,lock):
    digest=hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda:stream.read(1024*1024),b''):
            digest.update(chunk)
    if digest.hexdigest()!=lock['archive_sha256']:
        raise ValueError('Archive differs from locked source')
    files={}
    with zipfile.ZipFile(path) as archive:
        for leaf in METHODS:
            key='src/devices/cpu/h8/'+leaf
            members=[n for n in archive.namelist() if n.endswith('/'+key)]
            if len(members)!=1:
                raise ValueError('Expected one archive member: '+key)
            files[key]=archive.read(members[0])
    return report(files,lock['commit'])

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive',type=Path,required=True)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--check',action='store_true')
    a=parser.parse_args()
    root=Path(__file__).resolve().parents[1]
    lock=json.loads((root/'research/mame-source-lock.json').read_text())
    result=from_archive(a.archive,lock)
    if a.check:
        expected=json.loads((root/'research/h83002-reset-contract.json').read_text())
        if result!=expected:
            raise ValueError('Reset identity report differs from recorded evidence')
        print('Five pinned source files/six reset methods match;global reset semantics unverified')
    elif a.output:
        with a.output.open('x',encoding='utf-8') as output:
            json.dump(result,output,indent=2);output.write('\n')
    else:
        print(json.dumps(result,indent=2))

if __name__=='__main__':
    main()
