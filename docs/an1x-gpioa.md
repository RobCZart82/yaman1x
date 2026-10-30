# AN1x bounded portA byte MMIO

Recorded:2026-10-30. Merged #130 as `fc9b483dcdd6c33385c1225303a0c0f2f0d7477c`;tested head `e6f276dcf20e48d014a71f13dd73ac74b33da029`.

## Pinned facts and scope

Original MAME commit:`706dbb73281320c54d7c2f16262c1c3786f2be01`.
Rechecked locked `src/devices/cpu/h8/h83002.cpp`,SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
advanced FFFFD1 uses `ff_r`/`ddr_w`,FFFFD3 uses `port_r`/`dr_w`.
PortA configuration:default DDR00,reserved mask00,forced-input mask00.
Unchanged register bodies/source notices are linked in [the original port
extraction](h8-digital-port.md) and [port9 evidence](an1x-gpio9.md).
No upstream method/instruction body,Yamaha payload or dependency added.

Project-owned `An1xSciBus` dispatches these byte registers to a separate unchanged
`H8DigitalPort` owner. All8 bits are usable;no bit may be forced high or removed
from output/direction. DDR reads FF;pin reads combine external and driven latch
bits. Cold state is all inputs. Unchanged output/direction suppress callbacks;
all-output DDR suppresses input sampling. Callback port identifier is10 (A),
with committed bus cycle.24-bit aliases and existing fail-stop/reentry/missing
callback guards apply. Port4/6/7/8/9 remain separate.

Only byte accesses added. Even words D0/D1 and D2/D3 now span mapped port9/portA
bytes,but remain deliberately unsupported until explicit GPIO-word semantics
are implemented/tested. They fail before any byte callback. Prior port9 hole
checks move D1/D3 to unmapped D4/D5. No implicit hardware-reset callback.
PortB,IRQ/SYSCR,alternate pins,panel/LCD,timers/ADC/DMA,physical timing and complete
board reset remain open. No firmware boot or audio claim.

## Independent evidence

`tests/an1x_gpioa_check.cpp` checks262144 independent8-bit DDR/latch/external cases,
256 cold input patterns,FF DDR,output data/direction,input/output suppression,
port/time identity,aliases,port4/6/7/8/9/machine isolation,word rejection,neighbors,
missing callbacks and suppressed-inner-error reentry.

Synthetic CPU DDR0F/latch5A/external15 =>1A in NVRAM;MOV clears N/Z/V and retains
C/I (CCR81).241 every-cycle partitions compare CPU,NVRAM and input/output
histories including timestamps. CPU/generated-handler inventory unchanged.

Ignored native fixture uses12 verbatim pinned original port bodies,configured
portA/default00/mask00.200000 xorshift32 seed19A73 mapped DDR/data/pin operations:
zero mismatches in values,callback counts and output data/direction. Present
PCR/ODR bodies not mapped/exercised. Independent tests check port IDs/timestamps;
not whole-machine MAME or physical-timing equivalence.
Ignored mutants:invented mask80 =>198248 expected failures;input routed to port9
=>252184 expected failures. Tracked source unchanged.

Full82 CTest/87 Python PASS;GPIO4/6/7/8/9/A/SCI ASan/UBSan PASS;final-head CI:all five checks PASS,
runs40997653002/40997653007/40997653087. No recorded automated review result at inspection;none claimed. Source notices are not legal/binary distribution clearance.
Firmware/MIDI/PKS/DSP/audio/plugin/REAPER acceptance remains open.
