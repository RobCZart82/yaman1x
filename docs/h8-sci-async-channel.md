# Shared asynchronous H8 SCI channel

Date: 2026-10-27. Baseline: main `e7257ebe2bbc192ac8f7efc5c43f72b4779998d2`.
Source identities: `research/h8-sci-channel-provenance.json`; original
BSD-3-Clause/Olivier Galibert attribution and license coverage retained.

## One owner per SCI channel

`src/h8_sci_async_channel.h` owns one SMR/BRR/SCR/TDR/RDR set, one SSR read
latch, one internal cycle clock, and separate TX/RX shift states and phase
counters. Both directions can be active simultaneously. The previous TX-only
and RX-only APIs are compatibility facades over this same implementation;
framing/error logic is no longer duplicated. Two facade objects are still two
independent instances, so use the shared channel API for full duplex.

The ported scope is internal-clock asynchronous 7/8-bit framing with optional
raw upstream PE/OE parity and one/two transmit stop bits (RX checks one stop).
Sync/MP framing and external clocks remain rejected before register mutation.
All TX/RX enable and interrupt bits are accepted by the shared API; the RX-only
facade retains its narrower TX-control rejection. Original queueing, sticky
errors, debugger/DMA acknowledgement and reset/input retention rules remain.

RX joins an active TX clock (and vice versa) without replacing its event or
step. Only the new side's phase counter is initialized. Active BRR/CKS changes
retain the existing step until the clock restarts. Stopping one side leaves
the other running. One SSR snapshot can acknowledge receive flags and launch
queued transmit data, with the upstream post-write TX-start behavior.

## Scheduler and event contract

One `service_one` call handles one shared edge: TX runs first, then RX if it
is still active, followed by the idle output-clock notification if both stop.
Native tests explicitly check simultaneous TEI/RXI order, including line and
clock callbacks. This preserves upstream internal dispatch; it does not settle
AN1x hardware ordering between unrelated external wire/CPU events.

A sink supplies `tx(bool)`, `clock(bool)`, `tx_irq`, `end_irq`, `rx_irq` and
`error_irq`. Callbacks record events without re-entering or reconfiguring this
owner. For loopback/cross-channel wiring, queue line events and apply them under
the machine scheduler's explicit equal-time policy. The native external-wire
fixtures use wire-before-edge ordering as a test convention only.

Supply TX register starts with the current total-cycle view, and RX transitions
with the receiver's now-as-cycles view. Drain individual `next_event` deadlines
with a bounded loop; host block ends must not replace peripheral deadlines.
Late service still executes at most one edge per call. Reset with an attached
sink before connecting the machine to publish idle-high TX/clock levels.
Callback failures/cycle overflow can partially advance this compound owner;
discard/reinitialize it. No save-state, real-time, CPU MMIO or standby integration
is claimed. Interrupt callbacks are requests, not an IRQ-controller connection.

## Evidence

Local Windows MSVC Release: 4,096 full-duplex byte/format combinations pass,
including independent incoming/outgoing data, exact TX cycles and combined SSR.
Focused sequences check equal-cycle TX/TEI/clock/RXI order, joining an old step
after BRR changes, RE abort while TX continues, one SSR acknowledgement affecting
both directions, host block partitioning and two-channel/reset isolation. The
unchanged earlier 4,096-case TX and RX suites also pass through the new facades.

A temporary reference fixture uses original upstream TX/RX, register and clock
method bodies, with an independent original event/step calculation (not the new
cycle-clock helper). Across 4,096 duplex frames plus 300,000 mixed/late operations:
1,153,468 comparisons, 804,316 shared edges, zero event/state mismatches. Logging
is ignored; CPU rescheduling/timer services are no-ops, and sync/standby are not
exercised. This is not a complete MAME/CPU run or hardware validation. The Windows
scratch build used a shorter build directory after MSBuild hit its path-length
limit; the product build and its native tests were unaffected. Final tested
revision/platform CI results are recorded on the associated PR checks.

## Next acceptance gate

Route the channel's four interrupt requests into the reviewed H8H interrupt
controller with per-channel vector mapping; expose the six SCI registers through
an explicit bus adapter and test both SCI channels independently. Then integrate
the CPU/peripheral scheduler and exercise actual firmware/MIDI reception. The
full MAME research CI remains a separate reference smoke, not this CPU integration.
M0 remains partial, M1/PKS and audio/plugin acceptance remain open. Original parity
semantics are preserved pending hardware evidence; EG target-hold is unchanged.
No Yamaha ROM/bank/private trace is included.
