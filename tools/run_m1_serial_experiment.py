"""Run paired local SCI observations with generated MIDI; never certify boot or note handling."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import zipfile
from rom_inspect import read_input, identify


def midi_probe():
    # SMF format 0, 480 ticks/quarter, explicit 500000 us tempo. Note 60 on
    # channel 1 at t=0, velocity 64; note-off at t=2s. MAME delays playback 10s.
    track = bytes.fromhex('00 ff 51 03 07 a1 20 00 90 3c 40 8f 00 80 3c 00 00 ff 2f 00')
    return (b'MThd' + (6).to_bytes(4, 'big') + bytes.fromhex('00 00 00 01 01 e0')
            + b'MTrk' + len(track).to_bytes(4, 'big') + track)


def summarize_pair(manifests, midi_sha256):
    rows = []
    reference = None
    for profile in ('passive', 'firmware-contract'):
        run = manifests[profile]
        if (run['returncode'] != 0 or not run['h8_cpu_progress_observed']
                or not run['rom_identity']['matches_known_an1x_104']):
            raise ValueError('Expected a completed known-firmware CPU observation')
        if (run['dsp_profile'] != profile or run['dms_board'] != 'absent'
                or run['eg_readback'] != 'zero' or run['dsp_sample_clock'] != 'disabled'
                or run['vop3_bridge'] is not None or run['dsp_sample_execution']):
            raise ValueError('Unexpected experiment configuration')
        identity = (run['rom_identity']['sha256'], run['mame_sha256'],
                    run['midi_input']['sha256'])
        if identity[2] != midi_sha256 or (reference is not None and identity != reference):
            raise ValueError('Paired input or executable identities differ')
        reference = identity
        access = run.get('sci_access_observation')
        if not access or not access.get('complete') or not run.get('sci_observation'):
            raise ValueError('Complete SCI state and access observations required')
        channels = access['channels']
        if [entry['channel'] for entry in channels] != [0, 1]:
            raise ValueError('Both SCI channels required')
        edges = run.get('midi_rx_transitions')
        if not isinstance(edges, int) or isinstance(edges, bool) or edges < 0:
            raise ValueError('MIDI transition completion counter required')
        keyboard = run['sci_observation']['keyboard_port']
        if (channels[0]['rdr_reads'] != run['sci0']['rdr_reads']
                or channels[1]['rdr_reads'] != keyboard['rdr_reads']
                or channels[1]['tdr_writes'] != keyboard['tdr_writes']):
            raise ValueError('SCI access events disagree with independent completion counters')
        rows.append(dict(profile=profile, channels=channels,
                         midi_rx_transitions=edges,
                         sci0_rdr_reads=channels[0]['rdr_reads']))
    return dict(suite_complete=True, rows=rows, rom_sha256=reference[0],
                mame_sha256=reference[1], midi_sha256=midi_sha256,
                hardware_validated=False, complete_an1x_boot=False,
                firmware_note_handling_verified=False, pks_handshake_verified=False,
                note='Only the DSP status profile differs. RDR reads/receiver enable do not prove valid MIDI bytes or note handling; inspect private traces and firmware control flow.')


def run_suite(mame, rom, work, member=None, run_command=None):
    repo = Path(__file__).resolve().parents[1]
    mame, rom, work = mame.resolve(strict=True), rom.resolve(strict=True), work.resolve()
    if not mame.is_file():
        raise ValueError('Expected a local MAME executable')
    if work.is_relative_to(repo):
        first = work.relative_to(repo).parts[:1]
        if not first or not (first[0] in ('private_runs', 'work') or first[0].startswith('build')):
            raise ValueError('Inside the repository, use ignored private_runs/, work/ or build*/ output')
    identity = identify(read_input(rom, member))
    if not identity['matches_known_an1x_104']:
        raise ValueError('Expected unchanged known AN1x 1.04 firmware')
    work.mkdir(parents=True, exist_ok=False)
    midi = work / 'synthetic-note.mid'
    midi.write_bytes(midi_probe())
    midi_sha256 = hashlib.sha256(midi.read_bytes()).hexdigest()
    env = os.environ.copy()
    cleared = sorted(key for key in env if key.startswith('YAMANIX_') or key == 'VOP3_INTERP')
    for key in cleared:
        del env[key]
    revision = subprocess.run(['git', '-C', str(repo), 'rev-parse', 'HEAD'],
                              capture_output=True, text=True, check=True).stdout.strip()
    dirty = subprocess.run(['git', '-C', str(repo), 'status', '--porcelain'],
                           capture_output=True, text=True, check=True).stdout.strip()
    report = dict(suite_complete=False, project_revision=revision,
                  project_dirty=bool(dirty),
                  experiment_version=1, seconds_per_run=14, cleared_environment_keys=cleared,
                  expected_file_note_on_seconds=0, expected_file_note_off_seconds=2,
                  modeled_mame_playback_delay_seconds=10, runs_completed=[])
    report_file = work / 'suite.json'
    report_file.write_text(json.dumps(report, indent=2) + '\n')
    manifests = {}
    runner = run_command or subprocess.run
    try:
        for profile in ('passive', 'firmware-contract'):
            destination = work / profile
            command = [sys.executable, str(repo / 'tools/run_h8_probe.py'),
                       '--mame', str(mame), '--rom', str(rom), '--work', str(destination),
                       '--seconds', '14', '--midi-file', str(midi),
                       '--dsp-profile', profile, '--dms-board', 'absent',
                       '--eg-readback', 'zero', '--dsp-sample-clock', 'disabled']
            if member is not None:
                command += ['--member', member]
            with (work / (profile + '-launcher.log')).open('w') as log:
                result = runner(command, cwd=repo, env=env, stdout=log,
                                stderr=subprocess.STDOUT, timeout=150, check=False)
            if result.returncode:
                raise ValueError(f'{profile} probe failed; inspect its local launcher/console logs')
            manifests[profile] = json.loads((destination / 'run.json').read_text())
            report['runs_completed'].append(profile)
            report_file.write_text(json.dumps(report, indent=2) + '\n')
        report.update(summarize_pair(manifests, midi_sha256))
    except (OSError, ValueError, KeyError, TypeError, subprocess.TimeoutExpired) as error:
        report['error'] = str(error)
        report_file.write_text(json.dumps(report, indent=2) + '\n')
        raise
    report_file.write_text(json.dumps(report, indent=2) + '\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mame', type=Path, required=True)
    parser.add_argument('--rom', type=Path, required=True)
    parser.add_argument('--member')
    parser.add_argument('--work', type=Path, required=True)
    args = parser.parse_args()
    try:
        report = run_suite(args.mame, args.rom, args.work, args.member)
        print(json.dumps(report, indent=2))
    except (OSError, ValueError, KeyError, TypeError, subprocess.SubprocessError, zipfile.BadZipFile) as error:
        parser.exit(2, f'M1 local experiment failed: {error}\n')


if __name__ == '__main__':
    main()
