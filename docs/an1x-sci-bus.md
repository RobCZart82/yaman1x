# Timed SCI composition on the AN1x memory map

Recorded:2026-10-30. Merged #118 as `6f9e8b8f66d962e968af8d6f9cd4135d2c4d1ab3`;tested head `85baf0a93896468e35f019845d9f9116763ecb29`.

## Ownership and scheduling contract

Project-owned `An1xSciBus<Sink>` composes the existing bounded memory map,one IRQ
controller,two-channel H83002 SCI adapter and SciCycleTimeline. It owns ROM/RAM,
immutable copied RX-edge storage and peripherals;the caller-owned sink must
outlive it. The CPU binds the same IRQ owner. Instances are noncopyable/nonmovable.
No existing upstream-derived peripheral or instruction body changes.
Pinned MAME reference:`706dbb73281320c54d7c2f16262c1c3786f2be01`. Address/vector
identities remain in `research/h8-sci-mmio-provenance.json`;see the
[SCI adapter evidence](h83002-sci-mmio-irq.md),[timeline evidence](sci-cycle-timeline.md)
and [memory-map facts and source hashes](an1x-memory-map.md).

All bus addresses normalize to24 bits. SCI byte/word registers route to the
existing adapter;ROM/RAM use the common map. Instruction fetch remains memory
only. Historical #118 scope:unknown MMIO,including DSP/MEG/GPIO/LCD/IRQ-control registers,failed
closed. No whole-machine boot or firmware MIDI acceptance claim.

CPU `bus_cycle` advances pending RX edges and SCI events through the charged
memory-access completion time before performing MMIO. `next_bus_event` exposes
newly created MMIO deadlines to the existing CPU budget refresh. The original
software tie policy is retained:RX input order,then SCI0,then SCI1. This is not
measured hardware arbitration. Input at cycle0 is consumed by initial update.
A completed bus access may overrun a slice;the next update does not rewind the
peripheral timeline while the CPU repays debt. Time/side effects already charged
remain committed. Sink receives explicit cycle timestamps for TX,clock and DMA
requests;optional CPU exception notification is forwarded.

Each advance has a configurable finite event-work limit (default65536). Exhaustion,
callback failure,reentry or invalid access poison the bus;discard it and its CPU.
No rollback/resume is promised after failure. Pure inspection does not recover it.
Callbacks may not mutate the controller/peripherals through retained references.
The CPU owns all execution;no concurrent threading support is introduced.

Cold construction independently initializes memory/SCI/IRQ owners. Exact-size
NVRAM export/import permits caller-managed persistence before execution. This
is not an in-place hardware reset API;reconstruct owners for a fresh research
instance. Full board reset/peripheral wiring remains open.

## Independent composition tests

`tests/an1x_sci_bus_check.cpp` checks512 TX frames and512 RX frames (both channels,
all256 bytes). Project-generated ROM configures SCI via CPU MMIO. TX wire levels
and32-cycle bit spacing are independently checked. Scheduled RX input wakes CPU
SLEEP through RXI;the ISR writes the byte to mapped NVRAM,clears RDRF and RTE
restores the internal-RAM stack. Request/exception vectors and exact stored byte
are checked. Mutating caller RX storage after construction cannot affect input.

3204 every-cycle partitions compare CPU snapshots,NVRAM,TX/clock/request/exception
histories,consumed inputs and final SSR,including bus-overrun/debt boundaries.
Additional controls cover big-endian SCI words,24-bit aliasing,instance isolation,
fresh-owner NVRAM restore without changing SCI,invalid input/order/channel,
event-budget failure,callback reentry (also when sink suppresses the inner error),poisoned-owner rejection and unmapped DSP.
The initial reentry fixture only enabled TE without starting transmission,so no
callback occurred;it now launches a byte and services the actual callback.

A candidate-only ignored mutation hiding all peripheral deadlines is required to
fail the independent RX/wake oracles and partition checks:2738 failures observed;
tracked code unchanged.
Existing source-level original MAME comparisons remain component evidence;this
is a new synthetic composition check,not full-machine MAME equivalence.

Full76 CTest and87 Python PASS;targeted ASan/UBSan PASS. Linux CI includes this target under UBSan. Own final-head CI:all five checks PASS (runs40973239180,40973239137,40973239160).
No recorded automated review result at inspection;none claimed. GPIO/IRQ-control/LCD/DSP/MEG wiring,complete reset/isolation,
physical timing,firmware/MIDI/PKS,audio and plugin/REAPER acceptance remain open.

Merged follow-ups now map byte GPIO for [port4](an1x-gpio4.md), [port6](an1x-gpio6.md),
[port7](an1x-gpio7.md), [port8](an1x-gpio8.md), [port9](an1x-gpio9.md) and [portA](an1x-gpioa.md).
Merged #132 [portB byte MMIO](an1x-gpiob.md) extends this map. Earlier omissions above describe #118.
GPIO word semantics, IRQ/SYSCR and other peripheral wiring remain open.
