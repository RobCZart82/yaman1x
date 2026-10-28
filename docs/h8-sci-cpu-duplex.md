# Overlapping CPU SCI transmit and interrupt-driven receive

Candidate after merged #69. Integration checks only; no engine or ROM change.
A synthetic CPU program configures both UART channels (BRR=0, SCR=0x70),
starts one transmit frame, and enables interrupts. Independent external RX
edges start at cycle160 while TX is active. RXI enters a CPU ISR that reads
SSR/RDR, stores the received byte in RAM, clears RDRF and returns through RTE.
ISR address/data registers are separate from those used by the main program.

All four TX/RX channel pairings are tested, including same-channel duplex.
For each pairing all 256 transmitted bytes use received=transmitted XOR 0xa5:
1024 cases. Expected TX levels are computed independently, with start, eight
LSB-first data bits, stop and final idle at 32-cycle intervals. Explicit checks
require TX to begin before RX start and finish after it. Expected received byte,
RXI vector, cleared RDRF, no eligible pending IRQ and restored stack are checked.
Every split0..900 for all four pairings compares CPU snapshot, RAM, bus trace,
wire timestamps/levels and exception sequence: 3604 split histories.

Host tie convention remains external pin before SCI service and channel0 before
channel1; this is reproducibility, not physical AN1x arbitration evidence.
Frames overlap, but no claim is made that RXI preempts the active TX frame:
in this fixture the received frame completes after TX. No chained TX frames,
receive overrun/error matrix, baud reconfiguration in flight, full machine map,
firmware boot or audio acceptance. These remain separate integration tasks.
