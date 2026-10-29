# Fixed-profile H8H SLEEP and wake

Recorded:2026-10-29. Merged engine baseline:#112;current increment is a candidate.

## Scope and explicit exclusions

Two unchanged pinned MAME `sleep_full`/`sleep_partial` methods,commit
706dbb73281320c54d7c2f16262c1c3786f2be01;class qualifiers removed only.
562 upstream-derived methods (558 unchanged/four earlier arithmetic adaptations).
All264 eligible H8H encoding rows now select handlers;135 other-profile rows
excluded. This is an encoding inventory,not complete CPU/machine acceptance.

Exact0180 dispatch uses existing budget/deadline runner. SLEEP prefetches the next
instruction once,then consumes budget up to peripheral deadlines until a selected
IRQ/NMI vector is available. Continuation3 retains sleep across calls. Before each
sleep continuation the optional interrupt controller's currently qualified vector
is sampled;masked IRQ stays asleep,NMI bypasses I. Original `prefetch_done` enters
the existing IRQ path and RTE restores execution at the prefetched next PC.
No data/register/CCR mutation occurs simply from waiting. With no controller or
no deadline,sleep consumes the slice without instruction fetching or busy looping.
Peripherals continue through the existing host update/deadline contract.

Fixed reset-profile `m_standby_pending` is constexpr false. Original unreachable
standby references have fail-closed host placeholders;no public API enables them.
Clock-stopping standby,DMA/DTC,trace,retry,dynamic SYSCR profiles and real hardware
sleep/oscillator timing remain unsupported. No new dependency or Yamaha input.
Existing snapshot instruction/substate represents sleep;the cached IRQ vector is
recomputed before use rather than independently serialized.

## Independent tests

`tests/h8_sleep_check.cpp` checks512 IRQ/NMI/allCCR mask cases and644 every-cycle
partitions (both interrupt types,masked/unmasked I). Complete snapshots,RAM,
ordered bus histories,peripheral tick histories,IRQ state and exception history
must match. Wake stack contains loaded CCR and next PC010a;RTE restores SP and
the prefetched marker executes in the correct byte lane.

Additional cases cover repeated sleeping slices without refetch,peripheral events
while sleeping,no-event budget consumption,optional absent IRQ owner,already
pending IRQ without a future deadline,and prefetch-fault poisoning/no-resume.
The initial fixture expected marker F85A in the high byte;the independent expected
lane was corrected to R0L. Engine semantics did not change for that correction.

## Original reference and negative control

Original decoder/unchanged handlers/execute_run match200000 split slices:
689757 instruction reads,24390 byte reads,38002 byte writes,39726 word reads,
39771 word writes;zero differences. Original-reference IRQ fixture now reaches
SLEEP and raises a delayed event at cycle55,including an NMI case. An address-bound
guard requires SLEEP execution. Candidate-only ignored mutation forcing the sleep
IRQ sample to zero mismatches at seed13/slice7 (expected exit1);original reference
unchanged,tracked candidate correct.

Local Release74/74 CTest and87 Python PASS;targeted ASan/UBSan PASS.
Final-head CI recorded in log/PR before merge. Linux CI includes the target under UBSan. Own final CI/review pending.
M0 remains partial:complete AN1x memory/MMIO/scheduler integration,isolation,
firmware/MIDI/PKS,physical timing,DSP/audio and plugin/REAPER acceptance remain open.
