# Development status

Recorded: 2026-10-28. Product: YamAN1x. Repository: `yaman1x`. First public research snapshot: 2026-10-26.

The repository contains a ROM-free C++/Python development foundation.
No VST3, AU, GUI or AN1x audio output is available yet.

Verified engine baseline: merged PR #57, `f8b6aa20834f209931e3e9bb9463cc47b5b7a540`. Later documentation commits do not change this tested engine baseline.
The [first-sound plan](FIRST_REAPER_SOUND_PLAN.md) tracks M0 as partial, M1 as
prepared and M2–M6 as research/planned. Completed helper components do not
establish a complete CPU or firmware boot. Changes and their provenance are
indexed in the [development ledger](DEVELOPMENT_LOG.md).

Merged #57 suite: **39 CTest groups, 84 Python cases**, all five final-head jobs PASS.
Final tested head: `bf3e7158a9a4dac6edb36ea4deb659bdf3c54cb1`.
[Platform CI](https://github.com/RobCZart82/yaman1x/actions/runs/40685365642),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40685365647),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/40685365644).
Local Windows: 38 runnable CTest groups / 83 Python cases PASS; one symlink-privilege
case excluded (WinError 1314). There is no open engine candidate recorded in the ledger.

Current subset: 366 selected original full/partial handlers, including normal H8H
stack/control families and synthetic RTE frame restore. Next: remaining H8H opcode/
addressing coverage, IRQ/TRAPA entry/filter integration, then common CPU/peripheral
scheduling and machine integration. Full CPU, firmware MIDI, audio and REAPER
acceptance remain open; M0 stays partial.

## Historical component evidence — current state is above

The following dated increments retain their preparation-time counts, open tasks
and pending-CI notes. They are not current blockers; completed later work is
summarized above and in the closure ledger.

### SCI MMIO/IRQ adapter — historical #39 increment

The [H83002 SCI MMIO/IRQ adapter](h83002-sci-mmio-irq.md), PR #39, connects both asynchronous channels to the standalone interrupt controller. Byte/word lane, DMA priority, debugger side effects, reset and isolation tests pass locally; the historical SCI-MMIO increment added one group. CPU scheduling and firmware integration remain open. See the dated entry in the [development plan](FIRST_REAPER_SOUND_PLAN.md).

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

The current CMake suite has **39 CTest groups**, including the Python unittest
group, license coverage, provenance and normal/optimized ABI checks. The audit
of the earlier #37 baseline passed all 22 groups and 56 Python cases locally on macOS arm64;
the merged bridge-auditor regression fix adds four Python cases in the same CTest group.
That historical #39 increment contained 60 Python cases; current discovery has 84.
Default checks use no Yamaha data. Two upstream ROM-dependent tests remain
excluded, without substituting successful results for missing fixtures.

Fresh scope and evidence: [repository audit](REPOSITORY_AUDIT_2026-10-28.md).

## Current scheduling increment

The [bounded cycle dispatcher](cycle-dispatcher.md) provides monotonic, resumable event processing with explicit budgets and callback-failure handling. It is now combined with merged SCI integration PR #39. At this increment the historical Python count was 60; current counts are in the summary above. A two-channel SCI event owner is now implemented; CPU integration and hardware-validated tie ordering remain open.

## SCI event-owner increment

The [two-channel SCI cycle timeline](sci-cycle-timeline.md) combines immutable timestamped RX input and both UART clocks with explicit budgets. Whole/partitioned/resumed traces match for 256 dual-channel duplex patterns. Wire-before-clock and SCI0-before-SCI1 tie ordering is a software convention. CPU instruction/bus integration remains open.

## CPU host-boundary preparation

The [H8 host-services report](h8-host-services-boundary.md) records reviewed token references across 30 candidate sources and four verified generated outputs. Two independent regenerations match byte-for-byte. Word-address alignment and execution-budget carry are documented requirements for the future host adapter. This is source evidence, not compiler/linker closure; no new CPU execution is claimed. At #42, the historical suite contained 25 CTest groups and 68 Python cases.

## H8H bus-access increment

The [standalone bus helpers](h8h-bus-access.md) preserve two-cycle pre-access charging, separate instruction/data paths and aligned word access. Local comparison against five original pinned MAME method bodies: 200,000 mixed operations, zero differences. Native tests cover all 65,536 word values and failure/isolation contracts. At #43, the historical suite contained 26 CTest groups and 72 Python cases. No full CPU or Yamaha firmware execution is established.

## H8 execution-budget increment

The [execution budget](h8-execution-budget.md) isolates inter-slice debt and peripheral-deadline arithmetic. Original-method comparison: 200,000 synthetic no-event slices plus 200,000 deadline calculations, zero differences. Native bus/budget tests cover 1,000 partition histories and boundary/error/isolation cases. At #44, the historical suite contained 27 CTest groups and 74 Python cases. Actual full/partial opcode execution and complete CPU/peripheral scheduling remain open.

## H8 execution-order historical candidate — 2026-10-28

The [runner](h8-execution-runner.md) composes initial update, carry, peripheral catch-up and full/partial callbacks. A fail-stop callback bound detects nonprogress. Original-method comparison across 200,000 eventful synthetic slices has zero differences. Actual generated handlers, CPU state, IRQ entry and firmware/audio acceptance remain open; M0 stays partial.

## First generated H8 instructions — historical #47 candidate

The [limited execution profile](h8-first-instructions.md) executes real generated reset, NOP, MOV.B immediate and BRA rel8 handlers with owned CPU register/prefetch/substate storage. Other opcodes fail explicitly; IRQ/DMA/trace/retry/standby are disabled. Original H8H decoder/handlers and execute_run agree across 200,000 synthetic slices and 899,400 instruction reads, with a detected negative control. This is first opcode execution, not a complete CPU or firmware boot. M0 remains partial. Next: memory instructions, conditional branches, IRQ entry and CPU/SCI ordering.

## H8H byte memory and rel8 conditions — historical #48 candidate

The [expanded profile](h8-memory-branches.md) executes all rel8 conditions, MOV.L immediate32 and indirect byte loads/stores through full ER registers. Both generated selections are locked and retained with notices. Original-reference comparison: 200,000 mixed slices, zero differences in full state, instruction/data trace and memory. Word/long operations, arithmetic coverage, IRQ entry and real CPU/SCI scheduling remain open; M0 is partial and firmware/audio acceptance unchanged.

## H8H word/long memory — historical #49 candidate

The [word/long extension](h8-word-long-memory.md) adds MOV.W immediate, indirect word/long data accesses and the original 0100 prefix state. All 56 selected full/partial bodies match locked generated outputs. Original-code comparison: 200,000 slices, zero differences in diagnostic state, memory and ordered instruction/byte/word traces. Arithmetic coverage, IRQ entry and CPU/SCI integration remain open. The #48 automated review was still running at merge; it is not counted as completed evidence.

## H8H arithmetic and comparison — historical #50 candidate

The [arithmetic extension](h8-arithmetic-flags.md) executes selected ADD/SUB/CMP forms at 8/16/32 bits. A fixed-profile unsigned helper preserves H/N/Z/V/C, including half-carry at bits 4/12/28. Independent mathematical controls and original-code comparison agree, and CMP leaves registers unchanged. Register-wide/carry-input arithmetic, logical/shift operations, stack/IRQ and CPU/SCI integration remain open; M0 is partial and firmware/audio acceptance unchanged.

## Carry/register/logic — 2026-10-28 candidate

The [carry/register/logic extension](h8-carry-register-logic.md) retains 124 generated methods, adding sticky-Z carry byte arithmetic, wide register arithmetic, logic and immediate CCR storage. Over one million carry cases and 20,000 multi-byte programs agree with independent math; original-code comparison has zero differences across 200,000 slices. LDC stores I/CCR but does not implement IRQ input/entry. Wide register logic, shift/bit/stack and IRQ/SCI integration remain open.

## Wide register logic — historical #52 preparation snapshot

The [word/long register logic](h8-wide-register-logic.md) adds OR/XOR/AND and the original 01f0 prefix, with strict internal-state guards. 138 generated full/partial methods are retained. All 65536 extensions are checked; original-code comparison across 200000 slices has zero differences. M0 remains partial; shift/bit/stack/IRQ and CPU/SCI integration remain open.

## Single-bit shifts and rotates — historical #53 preparation snapshot

The [shift/rotate extension](h8-shift-rotate.md) adds all eight single-bit operations at byte/word/long width (186 retained generated methods). Independent bit-lane and signed-range controls, partial/debt and original-code comparisons pass. Two-bit forms, bit operations, stack/IRQ and CPU/SCI integration remain open; M0 is partial.

## Register bit operations — historical #54 preparation snapshot

The [register-bit extension](h8-register-bit-operations.md) retains 222 generated methods, adding byte register bit modification/test and carry boolean/load/store forms. Independent controls and 200000 original-code slices agree. Two-bit shifts are H8S/2000 forms, absent in the pinned H8H decoder, so they are excluded from this CPU profile. Memory bit forms, stack/control flow, IRQ and CPU/SCI integration remain open; M0 stays partial.

## Indirect memory bit operations — historical #55 preparation snapshot

The [indirect byte bit extension](h8-memory-bit-operations.md) retains 262 generated methods, original 7c/7d prefixes and 18 memory bit groups. Tests cover flags, addresses, rejected extensions, exact read/prefetch/write order, failures and partitioned execution. M0 remains partial; absolute forms, stack/control flow and IRQ/SCI integration remain open.

## Absolute memory bit operations — historical #56 preparation snapshot

The [absolute byte bit extension](h8-absolute-bit-operations.md) retains 302 generated methods, original 7e/7f prefixes and 18 abs8 bit groups. All short addresses, guarded extensions, flags and ordered bus effects are checked. M0 remains partial; stack/control flow and IRQ/SCI integration remain open.

## Stack and control flow — historical #57 preparation snapshot

The [stack/control extension](h8-stack-control-flow.md) completes the normal fixed-profile H8H instruction families: pre/post MOV at all widths, all JMP/JSR forms, BSR rel8/16, all rel16 branches and RTS. Synthetic RTE frame restore is implemented; IRQ/TRAPA entry is separate and unsupported. 366 generated methods retained. M0 remains partial; full CPU and CPU/SCI integration remain open.

## Repository review — 2026-10-28

Audit baseline #58 (`863d21abcde448d746b3b44c39a1eb61be455a15`): fresh local macOS arm64 Release 39/39 CTest and 84/84 Python PASS; original-code comparison 200000 slices with zero differences; four focused CPU ASan/UBSan groups PASS. Source/notice/link/ledger checks pass. No reproducible engine regression found in the exercised scope. [Full audit and remaining coverage limits](REPOSITORY_AUDIT_2026-10-28.md).
