# CPU SCI unread-RDR overrun and pending IRQ ordering

Candidate after #72 parity integration; no production engine/ROM changes.
Two valid external frames begin at cycles160 and600 while the CPU holds CCR.I.
A 256-iteration synthetic INC/BNE loop delays LDC0 until both frames complete.
At cycle1000 tests require ORER plus RDRF, no exception yet and first-byte RDR
preserved. After unmasking, the CPU error ISR captures SSR at RAM0x400 and the
preserved RDR at0x401, clears ORER/RDRF using the read latch, and executes RTE.

The existing controller retains the first RXI request while ORER raises ERI.
Expected order is ERI52/56, then the previously latched RXI53/57. The dedicated
RXI handler only returns. This is explicit locked-model latch behavior, not a
physical AN1x interrupt-order validation or a claim that clearing SSR cancels
an already-latched request. Both entries must restore the same stack pointer.

Both receive channels and all256 first-byte values use second=first XOR0xff:
512 cases. Every split0..2400 on both channels compares CPU snapshots, RAM,
bus trace, TX wire and exception sequence:4802 split histories. Inputs, vector
contents, delay program and handlers are all constructed before CPU execution.
No mid-run instruction patch is used. A local negative control omitting the
second frame must fail the overrun assertions.

Scope: fixed8-bit no-parity BRR=0, two frames and one overrun. No general FIFO,
DMA, repeated-overrun storm, third-frame recovery, combined error priority,
full AN1x map, firmware boot or audio acceptance. Existing FER/PER tests remain
separate evidence. Parent merge and own final-head CI/review are required.
