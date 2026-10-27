"""Generated MIDI and paired-run guards, with synthetic manifests only."""
import copy
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from run_m1_serial_experiment import midi_probe, run_suite, summarize_pair


def manifest(profile, midi_sha256):
    return dict(returncode=0, h8_cpu_progress_observed=True,
                rom_identity=dict(matches_known_an1x_104=True, sha256='synthetic-rom'),
                mame_sha256='synthetic-executable', midi_input=dict(sha256=midi_sha256),
                dsp_profile=profile, dms_board='absent', eg_readback='zero',
                dsp_sample_clock='disabled', dsp_sample_execution=False, vop3_bridge=None,
                midi_rx_transitions=21, sci0={'rdr_reads': 1},
                sci_observation={'keyboard_port': {'rdr_reads': 0, 'tdr_writes': 0}},
                sci_access_observation=dict(complete=True, channels=[
                    dict(channel=0, rdr_reads=1, tdr_writes=0),
                    dict(channel=1, rdr_reads=0, tdr_writes=0)]))


class M1SerialExperimentTests(unittest.TestCase):
    def test_midi_has_explicit_tempo_and_two_second_note(self):
        raw = midi_probe()
        self.assertEqual(raw[:14], b'MThd\x00\x00\x00\x06\x00\x00\x00\x01\x01\xe0')
        self.assertEqual(raw[14:18], b'MTrk')
        self.assertEqual(int.from_bytes(raw[18:22], 'big'), len(raw)-22)
        track = raw[22:]
        self.assertEqual(track[:11], bytes.fromhex('00 ff 51 03 07 a1 20 00 90 3c 40'))
        ticks = ((track[11] & 127) << 7) | track[12]
        self.assertEqual(ticks * 500000 / 480, 2_000_000)
        self.assertEqual(track[13:], bytes.fromhex('80 3c 00 00 ff 2f 00'))

    def test_pair_does_not_claim_midi_acceptance_and_rejects_mixed_inputs(self):
        sha = hashlib.sha256(midi_probe()).hexdigest()
        runs = {profile: manifest(profile, sha) for profile in ('passive', 'firmware-contract')}
        report = summarize_pair(runs, sha)
        self.assertTrue(report['suite_complete'])
        self.assertFalse(report['firmware_note_handling_verified'])
        self.assertFalse(report['complete_an1x_boot'])
        for field, value in [('dms_board', 'installed'), ('eg_readback', 'an1x-level-experiment'),
                             ('dsp_sample_clock', 'assumed-48k'), ('vop3_bridge', 'unexpected'),
                             ('returncode', 1), ('sci_access_observation', None),
                             ('mame_sha256', 'different-executable'),
                             ('midi_rx_transitions', None), ('sci0', {'rdr_reads': 2})]:
            broken = copy.deepcopy(runs)
            broken['firmware-contract'][field] = value
            with self.subTest(field=field), self.assertRaises(ValueError):
                summarize_pair(broken, sha)
        with self.assertRaises(ValueError):
            summarize_pair(runs, 'different-midi')

    def test_suite_uses_fresh_runs_clears_profiles_and_records_failure(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            mame, rom = root/'fake-mame', root/'fake-input'
            mame.write_bytes(b'synthetic executable placeholder')
            rom.write_bytes(b'synthetic firmware placeholder')
            commands = []
            def fake_run(command, **kwargs):
                profile = command[command.index('--dsp-profile')+1]
                self.assertNotIn('YAMANIX_VOP3_BRIDGE', kwargs['env'])
                self.assertNotIn('VOP3_INTERP', kwargs['env'])
                destination = Path(command[command.index('--work')+1])
                destination.mkdir()
                midi = Path(command[command.index('--midi-file')+1])
                data = manifest(profile, hashlib.sha256(midi.read_bytes()).hexdigest())
                (destination/'run.json').write_text(json.dumps(data))
                commands.append(command)
                return subprocess.CompletedProcess(command, 0)
            with patch('run_m1_serial_experiment.read_input', return_value=b'synthetic'), \
                 patch('run_m1_serial_experiment.identify', return_value={'matches_known_an1x_104': True}), \
                 patch.dict(os.environ, {'YAMANIX_VOP3_BRIDGE': 'stale', 'VOP3_INTERP': '1'}):
                report = run_suite(mame, rom, root/'success', run_command=fake_run)
                self.assertTrue(report['suite_complete'])
                self.assertEqual(len(commands), 2)
                self.assertEqual(commands[0][commands[0].index('--seconds')+1], '14')
                self.assertEqual(report['runs_completed'], ['passive', 'firmware-contract'])
                with self.assertRaises(FileExistsError):
                    run_suite(mame, rom, root/'success', run_command=fake_run)
                failed = root/'failure'
                with self.assertRaises(ValueError):
                    run_suite(mame, rom, failed,
                              run_command=lambda command, **kwargs: subprocess.CompletedProcess(command, 1))
                saved = json.loads((failed/'suite.json').read_text())
                self.assertFalse(saved['suite_complete'])
                self.assertIn('error', saved)

    def test_unknown_rom_is_rejected_before_output_creation(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            executable, unknown = root/'mame', root/'unknown'
            executable.write_bytes(b'synthetic')
            unknown.write_bytes(bytes(0x100000))
            with self.assertRaises(ValueError):
                run_suite(executable, unknown, root/'not-created')
            self.assertFalse((root/'not-created').exists())
