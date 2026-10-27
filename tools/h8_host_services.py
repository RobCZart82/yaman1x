"""Lexical H8 host-service evidence; not compiler or linker dependency closure."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import zipfile
from h8_source_inventory import from_archive
from check_h8_generation import check as generate, verify_output

# Reviewed search vocabulary. Counts are token references, NOT resolved calls.
SERVICES = {
    'device_ownership': ['cpu_device', 'device_t', 'machine_config', 'required_device', 'optional_device_array', 'DEFINE_DEVICE_TYPE'],
    'memory_bus': ['address_space_config', 'memory_access', 'address_map', 'space', 'm_cache', 'm_program', 'read8', 'read16', 'read16i', 'write8', 'write16'],
    'cycle_accounting': ['total_cycles', 'now_as_cycles', 'm_icount', 'm_bcount', 'recompute_bcount', 'internal_update', 'add_event'],
    'machine_time': ['attotime', 'machine', 'as_ticks', 'm_cycles_base'],
    'execution_lifecycle': ['execute_run', 'device_start', 'device_reset', 'do_exec_full', 'do_exec_partial', 'm_inst_state', 'm_inst_substate'],
    'interrupt_dma': ['set_irq', 'trigger_dma', 'interrupt_taken', 'update_irq_filter', 'irq_setup', 'standard_irq_callback', 'access_is_dma', 'set_current_dtc'],
    'peripheral_callbacks': ['devcb_read8', 'devcb_read16', 'devcb_write8', 'devcb_write_line', 'do_sci_tx', 'do_sci_clk', 'do_read_port', 'do_write_port'],
    'standby': ['suspend', 'resume', 'suspended', 'SUSPEND_REASON_CLOCK', 'notify_standby', 'standby_time'],
    'state_persistence': ['save_item', 'save_pointer', 'state_add', 'device_nvram_interface', 'nvram_read', 'nvram_write', 'nvram_default'],
    'debugger_diagnostics': ['debugger_instruction_hook', 'debugger_exception_hook', 'side_effects_disabled', 'logerror', 'fatalerror', 'create_disassembler'],
}
# Mask literals/comments while retaining exact offsets and newlines. This lexer
# does not preprocess C++, resolve macros, types or calls, or parse header names.
IGNORED = re.compile(r'//[^\n]*|/\*.*?\*/|(?:u8|u|U|L)?R"(?P<delim>[^ ()\\\t\r\n]{0,16})\(.*?\)(?P=delim)"|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
TOKEN = re.compile(r'\b[A-Za-z_][A-Za-z_0-9]*\b')


def scan(files):
    reverse = {token: category for category, tokens in SERVICES.items() for token in tokens}
    records = []
    totals = {category: 0 for category in SERVICES}
    for name, raw in sorted(files.items()):
        text = raw.decode('utf-8')
        masked = IGNORED.sub(lambda m: ''.join('\n' if c == '\n' else ' ' for c in m.group()), text)
        hits = {}
        for match in TOKEN.finditer(masked):
            token = match.group()
            if token in reverse:
                line = masked.count('\n', 0, match.start()) + 1
                hits.setdefault(token, []).append(line)
                totals[reverse[token]] += 1
        records.append(dict(path=name, bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest(),
                            references=[dict(category=reverse[t], token=t, count=len(lines), lines=sorted(set(lines)))
                                        for t, lines in sorted(hits.items())]))
    return dict(schema_version=1, categories=SERVICES, files=records, token_reference_counts=totals,
                complete_compiler_closure=False, symbol_resolution=False,
                cpu_compiled=False, firmware_execution=False, license_clearance=False,
                limitations='Reviewed token vocabulary only; comments/literals excluded, preprocessing and semantic resolution absent. Declarations, definitions and inactive branches count. Unknown services and transitive framework dependencies are not detected.')


def build(archive, work, repo=None):
    repo = repo or Path(__file__).resolve().parents[1]
    lock = json.loads((repo/'research/mame-source-lock.json').read_text())
    generation_lock = json.loads((repo/'research/h8-generator-lock.json').read_text())
    inventory = from_archive(archive, lock['archive_sha256'])
    files = {}
    with zipfile.ZipFile(archive) as zipped:
        for row in inventory['files']:
            members = [i for i in zipped.infolist() if i.filename == 'mame-master/'+row['path']]
            if len(members) != 1:
                raise ValueError('Missing or duplicate source member')
            raw = zipped.read(members[0])
            if hashlib.sha256(raw).hexdigest() != row['sha256']:
                raise ValueError('Inventory/source mismatch')
            files[row['path']] = raw
    generation = generate(archive, work)  # Hash-verified generator, two passes.
    for name, expected in generation_lock['outputs'].items():
        files['generated/'+name] = verify_output((work/('0-'+name)).read_bytes(), expected)
    result = scan(files)
    result.update(upstream_commit=lock['commit'], archive_sha256=lock['archive_sha256'],
                  generation=generation, quoted_include_boundaries=inventory['boundaries'])
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive', type=Path, required=True)
    p.add_argument('--work', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    if a.output.exists():
        p.error('Output already exists')
    result = build(a.archive, a.work)
    with a.output.open('x', encoding='utf-8', newline='\n') as f:
        f.write(json.dumps(result, indent=2)+'\n')
    print(json.dumps(dict(files=len(result['files']), counts=result['token_reference_counts'],
                          cpu_compiled=False, complete_compiler_closure=False), indent=2))


if __name__ == '__main__':
    main()
