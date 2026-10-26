"""Audit a complete live register-bridge run against independent upload reconstruction.

Only SELECT/program/coef/tag/slot fields are compared. Timed runs additionally
check their explicit experimental profile and sample counters; this does not
audit audio, other internal chip state, or physical hardware correctness.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from dsp_trace import parse_capture, capture_profile, reconstruct


def audit(directory, replay=None):
    manifest = json.loads((directory / 'run.json').read_text())
    if manifest.get('returncode') != 0 or not manifest.get('vop3_bridge'):
        raise ValueError('Expected a successful, explicitly enabled live bridge run')
    timed = manifest.get('dsp_sample_execution') is True
    sample_clock = manifest.get('dsp_sample_clock', 'disabled')
    if timed:
        if sample_clock != 'assumed-48k' or manifest.get('dsp_sample_rate_hz') != 48000:
            raise ValueError('Unsupported or mismatched experimental sample-clock profile')
    elif sample_clock != 'disabled':
        raise ValueError('Sample-clock metadata contradicts register-only execution')
    lines = (directory / 'trace.log').read_text().splitlines()
    events = parse_capture(lines)
    profile = capture_profile(lines)
    if profile['name'] != 'firmware-contract' or manifest.get('dsp_profile') != profile['name']:
        raise ValueError('Unsupported or mismatched control profile')
    starts = [line for line in lines if 'YAMANIX BRIDGE state=' in line]
    if timed:
        if len(starts) != 1 or not re.search(r'state=timed-model-clock abi=2 hardware_validated=0$', starts[0]):
            raise ValueError('Missing or ambiguous timed bridge marker')
        clocks = [re.search(r'YAMANIX DSP_SAMPLE_CLOCK profile=assumed-48k rate_hz=48000 hardware_validated=0$', line)
                  for line in lines if 'YAMANIX DSP_SAMPLE_CLOCK' in line]
        if len(clocks) != 1 or not clocks[0]:
            raise ValueError('Missing or ambiguous experimental sample-clock marker')
    elif len(starts) != 1 or not re.search(r'state=live-register-only abi=[12] dsp_samples=0 hardware_validated=0$', starts[0]):
        raise ValueError('Missing or ambiguous live bridge marker')
    boards = [re.search(r'YAMANIX DMS_BOARD state=(absent|installed) port9_input=(ff|fb) hardware_validated=0$', line)
              for line in lines if 'YAMANIX DMS_BOARD' in line]
    if len(boards) != 1 or not boards[0]:
        raise ValueError('Missing or ambiguous DMS board marker')
    board, level = boards[0].groups()
    if board != manifest.get('dms_board') or level != {'absent': 'ff', 'installed': 'fb'}[board]:
        raise ValueError('Inconsistent DMS board metadata')
    eg_profile = manifest.get('eg_readback', 'zero')
    eg_markers = [re.search(r'YAMANIX EG_READBACK profile=(zero|an1x-level-experiment) scale=64 hardware_validated=0$', line)
                  for line in lines if 'YAMANIX EG_READBACK' in line]
    if eg_profile not in ('zero', 'an1x-level-experiment'):
        raise ValueError('Unknown EG readback profile')
    if 'abi=2' in starts[0]:
        if len(eg_markers) != 1 or not eg_markers[0] or eg_markers[0].group(1) != eg_profile:
            raise ValueError('Missing or inconsistent EG readback marker')
    elif eg_markers or eg_profile != 'zero':
        raise ValueError('Legacy ABI cannot claim experimental EG readback')
    metrics = []
    sample_passes = manifest.get('dsp_sample_passes', [0, 0])
    if not isinstance(sample_passes, list) or len(sample_passes) != 2 or any(not isinstance(x, int) or x < 0 for x in sample_passes):
        raise ValueError('Malformed per-chip DSP sample counters')
    for line in lines:
        if 'YAMANIX BRIDGE_END' not in line:
            continue
        match = re.search(r'YAMANIX BRIDGE_END chip=([01]) writes=(\d+) reads=(\d+) unsupported_reads=(\d+) sample_passes=(\d+)$', line)
        if not match:
            raise ValueError('Malformed bridge footer')
        chip, writes, reads, unsupported, samples = map(int, match.groups())
        if chip != len(metrics):
            raise ValueError('Missing or duplicated bridge chip footer')
        subset = [event for event in events if event['chip'] == chip]
        # Register 1 status is supplied by the explicit experimental control profile.
        expected_samples = sample_passes[chip] if timed else 0
        if timed and expected_samples <= 0:
            raise ValueError('Timed bridge did not advance this VOP3 chip')
        expected = (sum(e['op'] == 'W' for e in subset),
                    sum(e['op'] == 'R' and e['reg'] != 1 for e in subset),
                    sum(e['op'] == 'R' and e['reg'] not in ((1, 0x21, 0x22) if eg_profile != 'zero' else (1, 0x21)) for e in subset),
                    expected_samples)
        if (writes, reads, unsupported, samples) != expected:
            raise ValueError('Bridge counters disagree with complete captured bus events')
        metrics.append(dict(chip=chip, writes=writes, reads=reads,
                            unsupported_reads=unsupported, sample_passes=samples))
    if len(metrics) != 2:
        raise ValueError('Missing bridge completion footers')
    expected, _ = reconstruct(events)
    actual = (directory / 'live-vop3-state.bin').read_bytes()
    if actual != expected:
        raise ValueError('Live upload state differs from independent Python reconstruction')
    if replay is not None and replay.read_bytes() != actual:
        raise ValueError('Offline native replay differs from live state')
    return dict(events=len(events), dms_board=board, bridge_metrics=metrics, eg_readback=eg_profile,
                dsp_sample_clock=sample_clock, dsp_hardware_validated=False,
                experimental_eg_level_reads=sum(e['op'] == 'R' and e['reg'] == 0x22 for e in events) if eg_profile != 'zero' else 0,
                snapshot_bytes=len(actual), snapshot_sha256=hashlib.sha256(actual).hexdigest(),
                live_matches_python=True, live_matches_native_replay=True if replay else None,
                audited_fields=['select', 'program_words', 'coefficients', 'tags', 'slot_offsets'],
                hardware_validated=False, audio_rendered=False, complete_an1x_boot=False,
                note=('Register 0x01 and any selected-channel EG level response use experimental contracts; '
                      'the 48 kHz clock is an unverified assumption. No audio is rendered and this does not '
                      'establish complete firmware boot or physical chip behavior.'))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    parser.add_argument('--replay-snapshot', type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(audit(args.directory, args.replay_snapshot), indent=2))
    except (OSError, ValueError, KeyError) as error:
        parser.exit(2, f'Live bridge audit error: {error}\n')


if __name__ == '__main__':
    main()
