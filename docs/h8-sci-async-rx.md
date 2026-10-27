# H8 SCI asynchronous RX owner

Current implementation note (2026-10-27): this API now forwards to the
[shared asynchronous channel](h8-sci-async-channel.md). The extraction evidence
below records the earlier standalone milestone; current integration gates are
listed in the shared-channel document.

Date: 2026-10-27. Baseline: main `389594251b9b4521286c94c232f5ce262b5de396`.
Source: the pinned MAME `h8_sci.cpp` and header recorded in
`research/h8-sci-rx-provenance.json`. BSD-3-Clause/Olivier Galibert notices and
reviewed license coverage are retained.

## Implemented boundary

`src/h8_sci_async_rx.h` composes the reviewed SSR latch and cycle clock with
asynchronous line sampling, a receive shift register, RDR and RXI/ERI event
requests. It consumes timed line levels, not injected completed bytes. RE enables
falling-edge start detection; enabling RE while input is already low does not
start reception. Disabling RE aborts active reception. The sixteen-phase counter
samples at phase eight; transitions at phase one/fifteen reset that counter.
A high start sample is discarded without a receive/error interrupt.

SMR selects 7/8 data bits and the upstream PE/OE parity convention. RX samples
one stop bit even when SMR requests two transmit stop bits. These are preserved
reference semantics, not newly proven AN1x hardware behavior. Sync/MP framing,
external clocks and TX enable/interrupt controls are explicitly rejected before
register mutation in this RX-only API. Other receive-control bits retain their
stored values without inventing multiprocessor behavior.

The status/error rules follow upstream ordering:

- A good frame sets RDRF and updates RDR unless an unread byte causes ORER.
- New parity errors reject that frame's data. Framing error takes precedence
  over a new parity error at the stop sample.
- Existing FER blocks later RDR updates until acknowledged. Existing PER/ORER
  can remain set while a later otherwise good byte is stored if RDRF is clear;
  the interrupt remains ERI while any receive error is set.
- RIE gates completion requests. Raising RIE with RDRF and an error already set
  publishes RXI then ERI, matching upstream SCR write order.
- Ordinary RDR reads retain RDRF; qualified DMA reads clear RDRF, while debugger
  DMA reads do not. SSR read-before-clear rules use the existing latch adapter.
- Reset restores register/event defaults but retains the external RX line and
  SSR read latch as in upstream. Fresh construction starts with an idle-high input.

## Scheduler and ownership contract

Supply input transitions at explicit machine-cycle times and service individual
`next_event` deadlines with a bounded loop. The caller defines equal-time wire
transition/clock ordering; the native fixture chooses wire changes before an
edge at the same cycle. This is a test convention, not final hardware ordering.
The line-start time argument is the upstream receiver's now-as-cycles view.
One `service_one` call handles at most one due edge even when late.

Sinks provide `rx_irq`, `error_irq` and `clock(bool)` without re-entering the
owner. Requests are not connected to the IRQ controller. Callback failures or
cycle overflow can partially advance this compound owner; discard/reinitialize
it. No save-state or real-time safety claim is made. Standby scheduling is not
modeled; input writes here apply to an awake receiver.

The TX-only and RX-only facades each own an independent channel instance. Use
`H8SciAsyncChannel` directly for one full-duplex channel; do not wire two facade
instances as the two halves of one SCI. IRQ/CPU MMIO integration remains open.

## Verification

Local Windows MSVC Release: 4,096 data/format combinations pass with first-stop
sample IRQ timing, plus false starts, resynchronization, sticky errors, overrun,
DMA/debugger reads, interrupt enabling, receiver abort, invalid-mode rejection,
reset/input retention, block partitioning and instance isolation.

A temporary local fixture uses original upstream RX/register method bodies,
the already reviewed cycle scheduler, an awake CPU stub and no TX/sync services.
It compares 4,096 good frames, 96 error-control frames and 200,000 mixed/late
operations: 896,623 comparisons, 663,805 RX ticks, 4,128 RXI and 65 ERI requests,
zero event/state mismatches. It is not a fully independent MAME/CPU run. Fixtures
and generated builds stay in ignored scratch storage. Final tested revision and
platform results are attached to the associated PR checks.

Next: IRQ/CPU MMIO integration of the shared channel, followed by actual
firmware/MIDI acceptance. PKS protocol, audio and plugin
acceptance remain open. No Yamaha ROM/bank/private trace or EG target-hold change.
