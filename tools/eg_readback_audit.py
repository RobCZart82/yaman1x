"""Characterize AN1x 1.04 register 0x22 consumers without asserting hardware values."""
import argparse
from collections import Counter
import json
from pathlib import Path
from dsp_trace import parse_capture


def characterize(events):
    selected = [0, 0]
    scans, special, others = [[], []], [[], []], []
    for e in events:
        chip, reg = e['chip'], e['reg']
        if e['op'] == 'W':
            if reg == 0:
                selected[chip] = e['data']
            elif selected[chip] & 0x8000:
                selected[chip] = (selected[chip] & ~511) | ((selected[chip] + 1) & 511)
            continue
        if reg != 0x22:
            continue
        if e['mask'] != 65535:
            raise ValueError('Partial 0x22 read is unsupported')
        index = selected[chip] & 511
        if e['pc'] == 0x54676:
            if index != len(scans[chip]) % 16:
                raise ValueError('Unexpected diagnostic channel selection order')
            signed = e['data'] if e['data'] < 32768 else e['data'] - 65536
            # Signed comparisons in 0x05468e..0x0546b2; channel 14 skips the lower bound.
            would_report = signed > 0x4000 or (index != 14 and signed < -256)
            scans[chip].append(dict(index=index, value=e['data'], would_report_range_error=would_report))
        elif e['pc'] in (0x51668, 0x516c6):
            if chip != (e['pc'] == 0x516c6) or index != 14:
                raise ValueError('Unexpected special-channel read context')
            special[chip].append(dict(index=index, value=e['data'], matches_expected_c000=e['data'] == 0xc000))
        else:
            others.append(dict(chip=chip, index=index, pc=e['pc'], value=e['data']))
    if any(len(scan) % 16 for scan in scans):
        raise ValueError('Truncated 16-channel scan')
    chips = []
    for chip in (0, 1):
        chips.append(dict(chip=chip, diagnostic_scans=len(scans[chip])//16,
                          indexed_scan_reads=len(scans[chip]),
                          scan_indices=dict(Counter(e['index'] for e in scans[chip])),
                          scan_value_counts=dict(Counter(e['value'] for e in scans[chip])),
                          range_error_decisions=sum(e['would_report_range_error'] for e in scans[chip]),
                          special_channel_reads=special[chip]))
    return dict(chips=chips, other_reads=others, hardware_validated=False,
                firmware_contract='Selected signed level; diagnostic bounds [-256,16384], channel 14 has no lower bound. Separate channel-14 comparison expects 0xc000.',
                level_scale_evidence='Firmware 0x0514a6..0x0514c4 compares readback with a RAM level multiplied by 64; conversion details are still experimental.')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    args = parser.parse_args()
    try:
        manifest = json.loads((args.directory/'run.json').read_text())
        if manifest.get('returncode') != 0 or not manifest['rom_identity']['matches_known_an1x_104']:
            raise ValueError('Expected a successful known AN1x 1.04 run')
        result = characterize(parse_capture((args.directory/'trace.log').read_text().splitlines()))
        result['eg_readback'] = manifest.get('eg_readback', 'zero')
        print(json.dumps(result, indent=2))
    except (OSError, ValueError, KeyError) as error:
        parser.exit(2, f'EG audit error: {error}\n')


if __name__ == '__main__':
    main()
