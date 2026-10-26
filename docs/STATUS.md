# Development status

Recorded: 2026-10-26. Product: YamAN1x. Intended repository: `yaman1x`.

The first GitHub snapshot contains a ROM-free C++/Python development foundation.
No VST3, AU, GUI or AN1x audio output is available yet.

## Observed and tested

- Synthetic DSP serial loopback: interpreter/JIT equality, 4096 input samples.
- Two-chip native register replay agrees with independent Python reconstruction.
- LCD bus wiring and the separate MAME H8 research harness execute the unchanged
  local 1.04 firmware far enough to load both complete VOP3 programs, using an
  explicitly experimental upload-control status contract.
- A service-schematic-derived P92 input enables the DMS panel's detection;
  the firmware itself sets its RAM presence flag.
- The firmware's 0x22 reads are selected-channel signed levels, distinct from
  the VOP3 core's encoded 0x21 completion status. A 64x readback profile is
  experimental; rounding, saturation and physical level mapping are unverified.
- The integer sample clock and actual VOP3 execution agree across different
  call partitions; the synthetic timing probe runs 2400 samples per chip.

## Material limitations

- The H8 harness currently performs register operations only. No VOP3 sample
  clock is connected to firmware MMIO or periodic execution yet.
- Upload acknowledgements, their latency and other status bits are experimental.
- Upstream `Vop3::run()` continues towards aim after crossing target, while its
  description says the segment stops at target. The synthetic reproduction
  gives 0x7FBF after 50 ms instead of the target's 0x2000. This is a
  code/description discrepancy; physical hardware behavior remains unverified.
- The library's 48 kHz model is not verification of the AN1x clock. Table-ROM
  scaling/addressing, inter-chip audio, MEG effects and MIDI/PKS handling remain
  incomplete. The original PKS ROM is still unavailable in the checked sources.

The original downloaded files and the upstream VOP3 snapshot were not modified.
Older reports in this repository are historical observations, not claims that
all device responses or the complete synthesizer are hardware validated.

## Repository validation

Default tests need no Yamaha data. The eight existing project test groups are
retained; a ninth repository audit verifies the source-only upload boundary
and pinned upstream files. Two upstream ROM-dependent tests are excluded,
without substituting success or skip results for their missing fixtures.
