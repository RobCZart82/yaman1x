# Absolute H8 bus completion time

Recorded 2026-10-28, after merged CPU/SCI receive #66.
The CPU now supplies its validated absolute slice end to the bus adapter.
An optional host bus_cycle(uint64_t) callback observes each bus operation after
its existing two-cycle charge and before its memory callback. Completion time
is slice_end minus signed remaining cycles, so a negative remainder deliberately
places completion beyond the current slice end. The next slice repays that
debt. Checked unsigned arithmetic rejects overflow; no timestamp is clamped.

This is a standalone host integration contract, not a generated MAME body or a
new claim about physical AN1x bus phases. Hosts without bus_cycle retain existing
callbacks and cycle accounting. A timed direct bus adapter must receive
set_slice_end before use. The callback must not re-enter the CPU, mutate its
counter or change scheduling deadlines. Callback failure is fail-stop: the cycle
charge remains, but the memory operation is not invoked. The host must discard
its failed CPU. Bus/controller/host lifetime requirements remain unchanged.

Independent evidence: all five access kinds with expected timestamps
97/99/101/103/105, 201 CPU split histories compared with uninterrupted execution,
one timestamp per memory callback, overflow and observer failure. Existing
memory/decode/reference tests separately check unchanged instruction behavior.

This milestone is the timestamp prerequisite. General CPU-driven SCI TX is
still unsupported by the bounded receive fixture. Next: publish new peripheral
deadlines after CPU MMIO writes and test a CPU-configured transmit frame against
an independent expected wire sequence. Serving due events and changing a
deadline inside an access needs explicit ordering tests first; bus_cycle alone
does not provide that scheduler integration. No firmware boot/audio acceptance.
