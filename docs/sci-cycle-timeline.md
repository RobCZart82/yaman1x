# Two-channel SCI cycle timeline

Recorded: 2026-10-27. Base: merged #40,
`76787a6ee5450fa29a7403b658fc861ef908a297`.
This M0 increment connects the reviewed SCI adapter to the bounded cycle
dispatcher. It does not integrate CPU instruction execution or run Yamaha firmware.

## Ownership and input

`SciCycleTimeline` references a machine-owned `H83002SciMmio` and a borrowed,
immutable `span<const RxEvent>`. Each event contains a cycle, channel and RX level.
The adapter, its IRQ controller and event storage must outlive the timeline.
Never mutate/reallocate input storage during that lifetime. Construction checks
nondecreasing timestamps, channels 0/1 and initial-time consistency before any
peripheral mutation. No dynamic input insertion, storage allocation, reset,
standby or save-state implementation is included. Copying is disabled through
the contained noncopyable dispatcher.

Initialize peripherals before constructing the timeline. During advancement,
callbacks may not re-enter the timeline or mutate its adapter/controller/input.
Between calls, any MMIO intervention must use the timeline's current cycle;
while a horizon is incomplete, finish it before applying a CPU-side action.
After a completed horizon, MMIO would follow the already-serviced events at that
cycle. This is a documented host convention, not CPU bus-timing integration.
An adapter deadline moved behind current time fails on the next advance.

## Event order and budget

The next deadline is the minimum of queued input and the two UART clocks.
SCI's zero-as-no-clock sentinel is converted to the dispatcher's `nullopt`;
external cycle-zero inputs remain valid. At equal cycles, all queued RX changes
run in source order, followed by one SCI0 edge and then one SCI1 edge. Each input
change or channel edge consumes one budget unit. Incomplete work preserves the
pending event and can resume even in the middle of tied events. This explicit
software ordering has not been verified on physical AN1x hardware.

Sink methods are `tx(channel,cycle,level)`, `clock(channel,cycle,level)` and
`bool dma_request(vector,cycle)`. The wrapper supplies the actual event cycle,
even when the requested horizon is far ahead. IRQ qualification continues to
use the existing adapter's DMA-first rule. This is not a DMA transfer engine.
Callback or stale-deadline failures poison the dispatcher; discard timeline and
peripheral owner afterward because side effects cannot be rolled back. Caller
horizon errors remain recoverable according to the dispatcher contract.
No real-time deadline guarantee is made for user callbacks or input storage.

## Synthetic evidence

- 256 byte patterns, both channels transmitting and receiving concurrently;
  whole-run, single-cycle partition and one-event-budget traces agree exactly.
  Both RDR results are checked independently against known 8N1 bytes.
- Equal-cycle RX transitions preserve input order before the exact RX sample;
  equal-deadline TX output orders SCI0 before SCI1.
- DMA-consumed interrupt requests retain received data without latching CPU IRQs.
- Unsorted/stale/invalid input, stale initial clock, zero-cycle/zero-budget
  continuation, idle maximum horizon, machine isolation and sink failures.

All data are synthetic. The existing framing/parity/overrun/MMIO tests remain
separate unchanged checks. Local macOS arm64 Release: 25/25 CTest groups and 60 Python cases PASS.
The new native regression also passes ASan/UBSan. Platform CI is tied to the final PR revision.

## Next gate

Close the H8 framework/generated-source boundary and establish CPU instruction
stepping, memory-bus timing and interrupt-taking contracts. CPU/MMIO ordering,
PKS firmware behavior, clock calibration and actual MIDI reception must be
validated separately. M0 remains partial; audio and REAPER acceptance are open.
