# AN1x bounded port8 MMIO

Recorded:2026-10-30. Engine baseline:#122;this increment is a candidate.

## Source facts and scope

Project-owned `An1xSciBus` routes byte DDR FFFFCD (FF read,DDR write) and pins/data
FFFFCF to a separate unchanged `H8DigitalPort`,default DDR F0,reserved mask E0,
forced-input mask00. Existing port4/port6 owners and callbacks are retained.

Pinned MAME commit:`706dbb73281320c54d7c2f16262c1c3786f2be01`. Rechecked local
locked `src/devices/cpu/h8/h83002.cpp` SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
two map entries and `H8_PORT(...PORT_8,0xf0,0xe0)` configuration. The unchanged
port methods/FF-read identities and BSD notices are linked in
[port6 evidence](an1x-gpio6.md) and [original extraction](h8-digital-port.md).
No upstream method/instruction body,Yamaha payload or new dependency added.

Bits7–5 read high and never appear in output data/direction. Lower5 bits select
driven latch or external input. At cold construction,bit4 is output low and
bits3–0 inputs;the callback is not implicitly published as a board reset.
Reserved-only DDR/latch changes suppress output notification;all lower5 outputs
suppress external reads. Timed callbacks identify port8 and use committed bus
completion cycles.24-bit aliases,existing fail-stop guards and owner isolation
apply. Missing callbacks throw rather than inventing input/discarding output.

Only byte accesses added;mixed GPIO/unmapped words reject before pin callbacks.
The old port4/port6 hole assertions now use FFFFD0 instead of newly mapped FFFFCD.
Read-only port7 at FFFFCE,other GPIO groups,IRQ/SYSCR,alternate pins,panel/LCD,
timer/ADC/DMA and full board reset remain unmapped/unaccepted. No physical pin
timing or full-machine boot claim.

## Independent evidence

`tests/an1x_gpio8_check.cpp`:262144 pin combinations (all DDR/latch values x4
external patterns),independent five-bit oracle plus reserved-high expectations.
256 extra cold external patterns prove default bit4 output low. Also checks
FF DDR reads,output/input callback suppression,exact port/time/data/direction,
port4/port6/machine isolation,word read/write boundaries,neighbors,reentry even
when inner error suppressed and missing callbacks.

Synthetic CPU DDR0F/latch5A/external15 =>FA in NVRAM;MOV sets N,clears Z/V and
retains C/I (CCR89).241 every-cycle partitions compare CPU,NVRAM and input/output
histories including timestamps. CPU/generated-handler inventory is unchanged.

Ignored native original-method fixture uses12 verbatim pinned port bodies,
configured DDR F0/mask E0,port8 identity.200000 xorshift32 seed19A73 mapped DDR/
data/pin operations:zero mismatches in values,callback counts and output data/
direction. Present PCR/ODR bodies are not mapped/exercised. Independent tests
check callback identity and timestamps;reference is not whole-machine MAME
or physical-timing equivalence.
Ignored mutations:mask E0 reduced to80 =>268791 expected failures;default DDR F0
changed to00 =>130 expected failures. Tracked code unchanged.

Full79 CTest/87 Python PASS;GPIO4/GPIO6/GPIO8/SCI ASan/UBSan PASS. Final-head CI/review
pending. Source notices alone are not legal/binary release clearance.
Firmware/MIDI/PKS/DSP/audio/plugin/REAPER acceptance remains open.
