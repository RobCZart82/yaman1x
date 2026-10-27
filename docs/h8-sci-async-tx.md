# H8 SCI asynchronous TX owner

Current implementation note (2026-10-27): this API now forwards to the
[shared asynchronous channel](h8-sci-async-channel.md). The extraction evidence
below records the earlier standalone milestone; current integration gates are
listed in the shared-channel document.

Date: 2026-10-27. Baseline: main `6e5da15024e33cf11cff56dbc51854e0050cad31`.
The intervening #33 changes LICENSE line wrapping only; its licensing status
is preserved. Source identities and BSD-3-Clause attribution are recorded in
`research/h8-sci-tx-provenance.json` and the reviewed license-coverage manifest.

## Implemented boundary

`src/h8_sci_async_tx.h` owns an asynchronous transmit shift state, TDR,
SMR/BRR/SCR transmit controls, the existing SSR latch and internal cycle clock.
The sink receives line levels, optional output-clock levels, and TXI/TEI event
requests. Those requests are not yet connected to an interrupt controller.
This is a TX-only peripheral owner, not a CPU-integrated SCI or plugin.

A CPU TDR write alone does not launch a frame. An observed TDRE acknowledgement
through SSR, or a qualified DMA write with TE enabled, starts an idle transmitter.
Starting transfers TDR into the shift register, raises TDRE and emits enabled TXI.
A second TDR can be queued while the first shift register is active. The start
bit, 7/8 data bits (LSB first), optional parity and one/two stop bits are stepped
at sixteen clock events per bit; a final full-bit interval precedes idle/TEND.

The raw SMR OE parity initialization is retained exactly from upstream, including
its output-bit convention. The test oracle is the upstream equation, not a claim
of verified conventional odd/even framing on AN1x hardware. Hardware evidence
would be needed before changing it. Multiprocessor and synchronous framing are
rejected before state mutation; external SCR clocks are likewise rejected.

Preserved upstream corner behavior: disabling TE does not abort an active frame;
the stop-state queue load does not recheck TE, while the last-tick restart does.
TEI is requested at last-tick completion before a possible restart and before
setting TEND. Rising TIE/TEIE can publish already-set TDRE/TEND. Active divider
changes retain the existing clock step until restart. Reset restores idle-high
lines/register defaults and retains the SSR read latch as in upstream.

## Time and callback contract

Call register writes with the owner's current total-cycle view. `next_event`
returns the next scheduled peripheral edge. The machine scheduler should advance
to that edge and call `service_one` there, draining edges in a bounded loop before
advancing further. One call services at most one edge even when late; a late
restart uses the supplied current CPU cycle view, preserving the reference clock
contract. Standby, a shared RX clock, external pins and CPU MMIO are not integrated.
Host block boundaries must not substitute for individual peripheral deadlines.

Sink callbacks must not re-enter or reconfigure the owner. They should record
line/IRQ events for the machine scheduler. Exceptions or cycle overflow after a
callback can leave a partially advanced peripheral; discard/reinitialize the
owner, as documented by the underlying cycle-clock adapter. No real-time safety
or save-state support is claimed yet. RE/RIE concern the future RX owner and do
not cause receive activity in this TX-only implementation.

## Evidence and remaining gates

Local Windows MSVC Release: all 4,096 byte/format combinations pass, with exact
cycle/line sequences. Native sequences cover queueing, DMA qualification, idle
and completion flags, interrupt enables, clock output, invalid modes, reset,
block-partition stepping and instance isolation. The original upstream TX and
register method bodies were also exercised in a temporary local fixture:
200,000 deterministic operations, 82,931 TX ticks, zero output/state mismatches.
The fixture uses the already-reviewed cycle scheduler and suppresses RX services;
it is not a full independent MAME machine or firmware run. Temporary fixtures
and their generated builds are excluded from the repository.

Final revision/platform results are recorded on PR checks. The full MAME research
build remains a separate reference smoke test, not integration of this owner.
Next: connect the shared channel interrupt requests and CPU MMIO, then perform
firmware/MIDI acceptance. RX/RDR and common asynchronous ownership now exist
as documented in the shared-channel milestone. PKS communication, actual hardware parity behavior,
audio and plugin acceptance remain open. No Yamaha ROM/bank/trace is included;
EG target-hold is unchanged.
