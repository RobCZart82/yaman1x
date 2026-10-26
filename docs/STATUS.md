# Development status

Recorded: 2026-10-26. Product: YamAN1x. Repository: `yaman1x`. First public research snapshot: 2026-10-26.

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
- The timed 5-second firmware trace now passes the live bridge audit: all 25,310
  bus events parse, and the loaded DSP state agrees with independent Python
  reconstruction. Each chip produced 28 complete 16-channel EG scans; all
  values were zero, so the separate channel-14 `0xC000` firmware comparison
  still fails under the experimental profile. That timed audit run sent no
  MIDI note and rendered no audio.
- The service manual pin table identifies H8 RXD0 as MIDI IN and RXD1 as
  keyboard data. An opt-in MAME patch wires its MIDI input image to SCI0/RXD0.
  Generated note-on/off MIDI files produced 21 RXD0 line transitions in both
  12- and 30-second MAME runs. SCI0 received-data register reads remained zero;
  the only observed SCR write was `0x00` (receiver disabled). This confirms the
  serial pin route, not UART byte reception, firmware event handling or sound.

## Material limitations

- The H8 harness remains register-only by default. A separate, opt-in research
  patch now advances both VOP3 models from MAME simulation time at the core's
  assumed 48 kHz rate before DSP MMIO. It compiled and ran against the local
  firmware for 1 and 5 MAME seconds, advancing both chips 48,799 and 240,799
  samples respectively. The 799-sample excess is MAME's frame-granularity
  overshoot; the AN1x clock remains unverified. This is not a complete boot.
- Upload acknowledgements, their latency and other status bits are experimental.
- Upstream `Vop3::run()` continues towards aim after crossing target, while its
  description says the segment stops at target. The synthetic reproduction
  gives 0x7FBF after 50 ms instead of the target's 0x2000. This is a
  code/description discrepancy; physical hardware behavior remains unverified.
- The library's 48 kHz model is not verification of the AN1x clock. Table-ROM
  index scaling, inter-chip audio, MEG effects and MIDI/PKS handling remain
  incomplete. A 128 KiB AN1x VOP3 mask-ROM image is present in the downloaded
  VOP3 research archive and its shipped-voice tables match the main firmware's
  table copy; this does not establish that both board ROMs match or verify
  address scaling; see [AN1x clock and table-ROM findings](an1x-clock-and-table-rom.md).
  The original PKS ROM is still unavailable in the checked
  sources.

The original downloaded files and the upstream VOP3 snapshot were not modified.
Older reports in this repository are historical observations, not claims that
all device responses or the complete synthesizer are hardware validated.

## Repository validation

The ROM-free CMake/CTest suite also passes in GitHub Actions on Linux x64,
macOS Apple Silicon and Windows x64. These builds do not validate AN1x audio,
hardware timing or DAW/plugin-host behavior.

Default tests need no Yamaha data. The eight existing project test groups are
retained; a ninth repository audit verifies the source-only upload boundary
and pinned upstream files. Two upstream ROM-dependent tests are excluded,
without substituting success or skip results for their missing fixtures.
