# Development status

Recorded: 2026-10-30. Product: YamAN1x. Repository: `yaman1x`. First public research snapshot: 2026-10-26.

The repository contains a ROM-free C++/Python development foundation.
No VST3, AU, GUI or AN1x audio output is available yet.

Verified engine baseline: [#122](https://github.com/RobCZart82/yaman1x/pull/122),
`804c8a5100ac281945f643c4c9f8f969f263da14`;tested head `32b36079e0a37f420193b410320f7705d728e29e`.
78 CTest groups/87 Python cases;CPU handlers unchanged:562 upstream-derived methods
(558 unchanged,four prior adaptations);264 selected/0 missing/135 excluded encoding rows.
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044271), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044353), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044283):all five jobs passed.
[Automated code review](https://github.com/RobCZart82/yaman1x/pull/122#issuecomment-6512135987) completed on the exact tested head with no findings.
This is code-review evidence;no security review result is recorded.
No active engine candidate. [Bounded masked port6](an1x-gpio6.md) adds DDR/pins byte MMIO
with default DDR80/mask80 and separate owner alongside port4. Independent pin,
reserved-bit suppression,callback identity/time,CPU flags and partitions pass.
Next:port8 DDR/data with pinned default F0/mask E0,then remaining GPIO groups.
IRQ-control/SYSCR modes need semantic tests;full machine reset remains open.
Full machine,LCD/DSP/MEG,physical timing,firmware/MIDI/PKS/audio and REAPER acceptance
remain open. Full handler selection is not full CPU/machine acceptance.

M0 partial;M1 prepared but unaccepted. One PR at a time.

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
  Synthetic CPU/SCI integration is implemented; the full AN1x machine map/scheduler remains open.
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

The merged #57 CMake suite has **39 CTest groups**, including the Python unittest
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

## Coverage / CCR candidate

[Coverage inventory and CCR](h8-coverage-and-ccr.md): 181 selected encoding rows, 83 missing and 135 excluded; 376 full/partial handlers. Candidate suite 40 CTest groups / 87 Python cases; IRQ/filter/firmware acceptance still open.

## Register-transfer candidate

[MOV register family](h8-register-transfers.md): 382 selected full/partial methods, coverage rows 184 selected / 80 missing / 135 excluded. Local candidate suite 41/41 CTest and 87 Python PASS; native sanitizer and 200000 original-reference slices PASS. IRQ and firmware acceptance unchanged.

#60 closure: 376 handlers / 40 CTest groups / 87 Python cases; final head e8dee6975054c4143abe36b3b8731b7cea510cf4 passed all five CI jobs. #61 is the current register-MOV candidate (382 handlers / 41 groups), rebased on the merged main; its own CI is pending.

## Small-arithmetic candidate

[INC/DEC and ADDS/SUBS](h8-small-arithmetic.md): 414 full/partial methods; encoding rows 200 selected / 64 missing / 135 excluded. Local candidate suite 42 CTest groups / 87 Python cases; reference/sanitizer checks recorded in component document. CPU IRQ and firmware remain open.

#61 closure: register MOV merged as 9cedcdf92d0330103fbeac24932c5c13e3924b3a after all five CI jobs succeeded. #62 small arithmetic is now the next candidate, rebased onto main; own final-head CI pending. Earlier pending descriptions are historical.

## Unary/extension candidate

[NOT/NEG/EXTU/EXTS](h8-unary-extension.md): 434 full/partial handlers; encoding rows 210 selected / 54 missing / 135 excluded. Local stacked candidate 43/43 CTest, 87 Python, native sanitizer and reachable original-reference checks PASS. Parent merges and final CI required.

#62 closed: merged f0b37f3e10e02248b8c9d0f52b3a555ae944c8d5 after all five CI jobs passed. #63 unary/extension is the next candidate, rebased onto main with flat-dispatch Windows portability preserved. Earlier pending descriptions are historical.

## Synchronous TRAPA candidate

[Fixed reset-SYSCR trap entry](h8-trapa-entry.md): 436 handlers, 211 selected / 53 missing / 135 excluded encoding rows; 44/44 local CTest groups, 87 Python, native sanitizer and original-reference checks PASS. Stacked PC follows pinned m_NPC behavior and is not hardware-validated. External IRQ/NMI and mutable SYSCR are not yet integrated.

#63 merged as dfa7af9bc21c8fe2b91367ad88b91bd7a56ba498 with five successful checks. #64 TRAPA is next, rebased onto main with the Windows flat-dispatch fix; #65/#66 remain dependent candidates. Earlier pending paragraphs are historical.

## External IRQ/NMI candidate

[Fixed reset-SYSCR entry](h8-external-irq.md) binds an optional controller to the CPU, preserving noirq prefetch boundaries and original entry/acknowledgement order. 438 selected methods; instruction counts remain 211/53/135. Dedicated ROM-free IRQ/NMI, stack-fault and partition checks plus 200000 original-method reference slices. Still a stacked candidate after #64; full CPU/peripheral scheduling and firmware/audio acceptance remain open.

## CPU/SCI receive composition candidate

[Interrupt-driven receive fixture](h8-sci-cpu-receive.md) tests both channels/all bytes and 1602 execution splits through RXI, CPU ISR, RAM storage, RDRF clear and RTE. No production machine bus or general timed MMIO write support is claimed. Follows #65; ROM boot and audio remain open.

## Bus cycle time candidate

#66 receive composition merged as 817e9ce91db22c770291fe2acf666f86f29e1896 with four successful CI checks. [Absolute bus completion timestamps](h8-bus-cycle-time.md) are the next candidate; optional host observation includes slice overrun/debt. CPU-driven UART writes still need explicit deadline rescheduling. No firmware/audio acceptance.

## CPU SCI TX candidate

[CPU-configured transmit](h8-sci-cpu-transmit.md), after #68: optional next_bus_event publishes newly created MMIO deadlines into both runner and handler boundaries. Synthetic BRR=0 TX checks both channels/all bytes and all 1602 split positions. Full firmware, concurrent I/O and audio remain open.

## CPU SCI duplex candidate

#69 merged as 298b8465aee4e74cfe7ec8e1459402b21f2ce9aa after all five CI jobs succeeded. [Overlapping TX/RX checks](h8-sci-cpu-duplex.md) cover all four channel pairings, independent wire/data/IRQ checks and 3604 execution splits. This is an integration-test increment; firmware boot/audio and receive-error matrices remain open.

## CPU framing-error candidate

#70 duplex tests merged as 860815eb62afb2d9b55e1e2cc8318a85967d4d04 with four successful checks. [ERI/FER and valid-frame recovery](h8-sci-cpu-framing.md) add CPU status capture/clear/RTE evidence and subsequent RXI reception. Fixed synthetic UART profile only; parity/overrun and firmware/audio remain open.

## CPU SCI parity candidate

#71 framing/recovery merged as 8e9c431f1a0629cc20db136c50c56eba81854202 with four successful checks. [Parity status/IRQ checks](h8-sci-cpu-parity.md) cover SMR0x20/0x30, good/bad input, CPU RXI/ERI acknowledgement and transmitted parity. Source-profile polarity is explicitly distinguished from physical hardware validation. Overrun, full CPU/firmware and audio remain open.

## CPU SCI overrun candidate

[Unread RDR and pending IRQ checks](h8-sci-cpu-overrun.md) verify ORER/RDRF and first-byte preservation across masked CPU time, then ERI and latched RXI service after unmasking. Synthetic fixed-profile evidence; depends on #72. Firmware/audio and combined-error/repeated-overrun coverage remain open.

## Absolute memory candidate

#73 overrun tests merged as 8446d10b65c698b5fbc749279f696d69c100e5fc with four successful checks. [Abs8 byte/abs16 byte-word MOV](h8-absolute-memory.md) adds six original groups (450 methods); encoding rows217 selected/47 missing/135 excluded. Abs32/displacements and full firmware/audio acceptance remain open.

## Absolute32 memory candidate

#74 merged as 758ad981253f8d3b5c72915f2fe5b60df560de97 with five successful checks. [Byte/word abs32 MOV](h8-absolute32-memory.md) adds four original groups (458 methods),221 selected/43 missing/135 excluded encoding rows. Full32-bit bus arguments and24-bit host mapping are checked separately. Long-width/displacement forms and firmware/audio remain open.

## Long absolute memory candidate — 2026-10-29

[Abs16/abs32 MOV.L](h8-long-absolute-memory.md) adds four original groups and a four-word instruction buffer. Candidate counts:466 methods,225 selected/39 missing/135 excluded encoding rows. The verified merged baseline above remains #75 until this candidate passes its own CI and merges.
