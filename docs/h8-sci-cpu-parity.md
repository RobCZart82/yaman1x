# CPU SCI parity status and interrupt selection

Integration candidate after #71. No production CPU/UART changes.
The shared synthetic fixture accepts optional SMR and an explicit wire parity
bit, with unchanged no-parity defaults. CPU writes SMR0x20/0x30 before enabling
SCR and sending data. An independent popcount-based input oracle sets either
the valid parity bit or its complement. For each mode/channel/byte, valid input
must reach RAM through RXI53/57. Invalid input must set PER alone among receive
flags, reach RAM as captured SSR through ERI52/56, clear the error, and RTE.

2048 byte/mode cases cover both channels, both parity-select values, both good
and bad input and all256 bytes. All8808 split points compare snapshots, bus,
RAM, TX wire timestamps/levels and exceptions. Transmitted fixed0x96 is separately
checked for start/eight bits/parity/stop/final idle at32-cycle spacing.

Polarity boundary: the locked source initializes parity to1 for SMR0x20 and0
for SMR0x30, then XORs data/parity bits. The fixture explicitly follows that
source convention. It does not label the values as hardware-validated even/odd
modes or silently correct a possible source/manual discrepancy. External
hardware/manual validation is still required. This is source-profile parity
acceptance, not a physical AN1x electrical contract.

Fixed8-bit BRR=0 profile only; no7-bit, two-stop, combined FER/PER, overrun,
in-flight mode changes or post-PER recovery proof. Prior FER recovery evidence
is separate. Firmware boot/audio remain open. Own final-head CI/review required.
