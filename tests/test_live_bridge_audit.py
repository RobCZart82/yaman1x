"""Synthetic complete bridge captures; no Yamaha firmware or program data."""
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from dsp_trace import parse_capture, reconstruct
from live_bridge_audit import audit


class LiveBridgeAuditTests(unittest.TestCase):
    def make_run(self, marker, clock=None, timed=False, samples=None):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        manifest = dict(returncode=0, vop3_bridge='synthetic-bridge',
                        dsp_profile='firmware-contract', dms_board='installed',
                        eg_readback='zero', dsp_sample_execution=timed,
                        dsp_sample_clock='assumed-48k' if timed else 'disabled',
                        dsp_sample_rate_hz=48000 if timed else None)
        if samples is not None:
            manifest['dsp_sample_passes'] = samples
        lines = ['YAMANIX PROFILE dsp=firmware-contract hardware_validated=0',
                 'YAMANIX DMS_BOARD state=installed port9_input=fb hardware_validated=0',
                 'YAMANIX BRIDGE ' + marker]
        if 'abi=2' in marker:
            lines.append('YAMANIX EG_READBACK profile=zero scale=64 hardware_validated=0')
        if clock:
            lines.append('YAMANIX DSP_SAMPLE_CLOCK ' + clock)
        lines += ['YAMANIX DSP_BEGIN version=1 clock_hz=1000000000 limit=200000',
                  'YAMANIX DSP seq=0 time_ns=0 op=W chip=0 reg=00 data=0000 mask=ffff pc=000000',
                  'YAMANIX DSP seq=1 time_ns=0 op=W chip=1 reg=00 data=0000 mask=ffff pc=000000']
        for chip in (0, 1):
            count = samples[chip] if timed and samples is not None else 0
            lines.append(f'YAMANIX BRIDGE_END chip={chip} writes=1 reads=0 unsupported_reads=0 sample_passes={count}')
        lines.append('YAMANIX DSP_END events=2 dropped=0')
        (directory / 'run.json').write_text(json.dumps(manifest))
        (directory / 'trace.log').write_text('\n'.join(lines) + '\n')
        (directory / 'live-vop3-state.bin').write_bytes(reconstruct(parse_capture(lines))[0])
        return directory

    def test_current_register_only_capture(self):
        run = self.make_run('state=live-register-only abi=2 hardware_validated=0',
                            'profile=disabled rate_hz=0 hardware_validated=0')
        self.assertTrue(audit(run)['live_matches_python'])

    def test_legacy_abi_markers(self):
        for abi in (1, 2):
            with self.subTest(abi=abi):
                run = self.make_run(f'state=live-register-only abi={abi} dsp_samples=0 hardware_validated=0')
                self.assertEqual(audit(run)['events'], 2)

    def test_timed_capture(self):
        run = self.make_run('state=timed-model-clock abi=2 hardware_validated=0',
                            'profile=assumed-48k rate_hz=48000 hardware_validated=0',
                            timed=True, samples=[48, 48])
        self.assertEqual(audit(run)['bridge_metrics'][1]['sample_passes'], 48)

    def test_register_only_contradictions_fail(self):
        marker = 'state=live-register-only abi=2 hardware_validated=0'
        for clock, samples in ((None, None),
                               ('profile=assumed-48k rate_hz=48000 hardware_validated=0', None),
                               ('profile=disabled rate_hz=0 hardware_validated=0', [1, 0])):
            with self.subTest(clock=clock, samples=samples):
                with self.assertRaises(ValueError):
                    audit(self.make_run(marker, clock, samples=samples))
