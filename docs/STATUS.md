# Development status

Recorded: 2026-10-27. Product: YamAN1x. Repository: `yaman1x`. First public research snapshot: 2026-10-26.

The repository contains a ROM-free C++/Python development foundation.
No VST3, AU, GUI or AN1x audio output is available yet.

Current baseline: merged PR #44, `b644b6e1c471f8632ea086d75bd37d20b5ebbeb4`.
The [first-sound plan](FIRST_REAPER_SOUND_PLAN.md) tracks M0 as partial, M1 as
prepared and M2–M6 as research/planned. Completed helper components do not
establish a complete CPU or firmware boot. Changes and their provenance are
indexed in the [development ledger](DEVELOPMENT_LOG.md).

Current verified suite: **27 CTest groups, 74 Python cases**. PRs #39–#44 are merged and each final head has five successful GitHub checks; see the [closure ledger](DEVELOPMENT_LOG.md). Earlier component counts and pending-CI notes describe their historical snapshots. Full CPU opcode execution, M0 closure and firmware/audio acceptance remain open.

## Current standalone engine increment

The [H83002 SCI MMIO/IRQ adapter](h83002-sci-mmio-irq.md), PR #39, connects both asynchronous channels to the standalone interrupt controller. Byte/word lane, DMA priority, debugger side effects, reset and isolation tests pass locally; the ROM-free suite now has 27 groups. CPU scheduling and firmware integration remain open. See the dated entry in the [development plan](FIRST_REAPER_SOUND_PLAN.md).

## Observed and tested

- Standalone H8 digital ports, internal IRQ arbitration and IRQ register/pin
  adapters are tested. SCI now has one shared asynchronous TX/RX owner,
  integer-cycle scheduling, an SSR latch and compatibility facades. Synthetic
  byte/format, error, timing, partitioning and isolation tests pass. SCI MMIO and IRQ routing are implemented as a standalone adapter;
  CPU integration and the full machine scheduler remain open.
- The eleven-patch MAME research target has a pinned source identity and a
  separate ROM-free macOS arm64 build/registration CI. This is distinct from
  local Yamaha-firmware execution and from the future embedded H8 port.

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
  A generated note-on/off MIDI file produced 21 RXD0 line transitions in a
  30-second MAME run with corrected byte-lane taps. SCI0 receive-data register reads remained zero;
  the only observed SCR write was `0x00` (receiver disabled). This confirms the
  serial pin route, not UART byte reception, firmware event handling or sound.

## Material limitations

- Direct, side-effect-suppressed SCI snapshots now independently observe both
  H8 serial devices. Paired 12-second passive and DSP-experiment runs first show
  SCI1 SCR=0x70 at six seconds, with 30 TDR writes and no RDR reads. SCI0 remains
  disabled at all recorded instants. This motivates PKS protocol research;
  a causal link to MIDI startup has not been established. See
  [serial-state observations](sci-state-milestone.md).

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

Original downloaded inputs remain external. The vendored VOP3 header has one
reviewed, reversible first-decode initialization fix (#22); provenance records
both its original identity and the exact local edit. Other recorded upstream
files retain their locked contents. The EG target-hold discrepancy is unchanged.
Older reports in this repository are historical observations, not claims that
all device responses or the complete synthesizer are hardware validated.

## Repository validation

The ROM-free CMake/CTest suite also passes in GitHub Actions on Linux x64,
macOS Apple Silicon and Windows x64. These builds do not validate AN1x audio,
hardware timing or DAW/plugin-host behavior.

The current CMake suite has **27 CTest groups**, including the Python unittest
group, license coverage, provenance and normal/optimized ABI checks. The audit
of the baseline passed all 22 groups and 56 Python cases locally on macOS arm64;
the merged bridge-auditor regression fix adds four Python cases in the same CTest group.
The combined PR #39 tree therefore contains 60 Python cases.
Default checks use no Yamaha data. Two upstream ROM-dependent tests remain
excluded, without substituting successful results for missing fixtures.

Fresh scope and evidence: [repository audit](REPOSITORY_AUDIT_2026-10-27.md).

## Current scheduling increment

The [bounded cycle dispatcher](cycle-dispatcher.md) provides monotonic, resumable event processing with explicit budgets and callback-failure handling. It is now combined with merged SCI integration PR #39. This branch has 27 CTest groups and 60 Python cases. A two-channel SCI event owner is now implemented; CPU integration and hardware-validated tie ordering remain open.

## SCI event-owner increment

The [two-channel SCI cycle timeline](sci-cycle-timeline.md) combines immutable timestamped RX input and both UART clocks with explicit budgets. Whole/partitioned/resumed traces match for 256 dual-channel duplex patterns. Wire-before-clock and SCI0-before-SCI1 tie ordering is a software convention. CPU instruction/bus integration remains open.

## CPU host-boundary preparation

The [H8 host-services report](h8-host-services-boundary.md) records reviewed token references across 30 candidate sources and four verified generated outputs. Two independent regenerations match byte-for-byte. Word-address alignment and execution-budget carry are documented requirements for the future host adapter. This is source evidence, not compiler/linker closure; no new CPU execution is claimed. The source-only suite remains 25 CTest groups and now includes 68 Python cases.

## H8H bus-access increment

The [standalone bus helpers](h8h-bus-access.md) preserve two-cycle pre-access charging, separate instruction/data paths and aligned word access. Local comparison against five original pinned MAME method bodies: 200,000 mixed operations, zero differences. Native tests cover all 65,536 word values and failure/isolation contracts. The source-only suite now has 26 CTest groups and 72 Python cases. No full CPU or Yamaha firmware execution is established.

## H8 execution-budget increment

The [execution budget](h8-execution-budget.md) isolates inter-slice debt and peripheral-deadline arithmetic. Original-method comparison: 200,000 synthetic no-event slices plus 200,000 deadline calculations, zero differences. Native bus/budget tests cover 1,000 partition histories and boundary/error/isolation cases. The suite now has 27 CTest groups and 74 Python cases. Actual full/partial opcode execution and complete CPU/peripheral scheduling remain open.
