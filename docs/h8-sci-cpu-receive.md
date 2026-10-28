# ROM-free CPU/SCI interrupt-driven receive composition

Recorded: 2026-10-28. Candidate after IRQ/NMI entry (#65).
No CPU or UART implementation changes: this milestone composes their existing
interfaces in a deliberately bounded synthetic host.

Both H83002 SCI channels receive all 256 byte values as timed start/data/stop
edges. The preconfigured asynchronous channel requests RXI vector 53 or 57;
the original CPU handlers enter the ISR, read SSR then RDR, store the byte at
RAM 0x400, clear RDRF using the read latch, and execute RTE. Independent checks
require one correct IRQ/DMA-decline request, the expected byte, cleared RDRF,
no remaining eligible IRQ and restored stack. All 1602 split positions (two
channels, 0..800 cycles) compare CPU snapshots, bus trace, RAM, notifications,
request sequence and SSR with uninterrupted execution.

The fixture merges external pin and UART deadlines. Its explicit equal-time
rule is external edge before UART sample, channel0 before channel1. This is a
reproducible test convention, not evidence of physical AN1x tie ordering.
Instruction and data storage remain synthetic and split; there is no production
AN1x memory map. UART configuration happens before execution. CPU MMIO writes
are limited to SSR receive acknowledgement, whose tested operation needs no
new timed transmit event. Other UART writes throw rather than inventing an
access timestamp. The last update callback time is therefore not presented as
a generally correct bus-access time. General CPU-driven TX/configuration needs
an absolute bus-cycle timestamp and deadline rescheduling integration first.

No original ROMs, generated bodies or sound engine changed. No firmware boot,
MIDI acceptance or REAPER sound claim. Predecessor merge and final-head CI gates
apply. Next: expose/test bus-cycle time and reschedule deadlines after MMIO
writes; then replace this bounded host with a reviewed machine bus adapter.
