# AN1x bounded port9 byte MMIO

Recorded:2026-10-30. Merged #128 as `efbf68fad922e7270fc60105a1e6947f972cee6a`;tested head `00bff586f85f7a2b6d70c7648c57c91486eeddec`.

## Pinned source and scope

Original MAME commit:`706dbb73281320c54d7c2f16262c1c3786f2be01`.
Rechecked locked `src/devices/cpu/h8/h83002.cpp`,SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
advanced FFFFD0 uses `ff_r`/`ddr_w`;FFFFD2 uses `port_r`/`dr_w`.
Port9 config:default DDR00,reserved mask C0,forced-input mask00.
Unchanged method identities/BSD notices are linked in [port6](an1x-gpio6.md)
and [original extraction](h8-digital-port.md). No upstream method/instruction
body,Yamaha payload or new dependency added.

Project-owned routing adds a separate `H8DigitalPort` owner. Upper2 bits read
high and never appear in output data/direction;lower6 bits select driven latch
or external input. DDR reads FF,not the stored DDR. Reserved-only direction/
latch changes suppress output notifications;all lower6 outputs suppress input
callbacks regardless of reserved direction bits. Cold lower6 bits are inputs.
Callbacks identify port9 and committed bus time.24-bit aliases and existing
fail-stop/reentry/missing-callback guards apply. Port4/6/7/8 remain independent.

Only byte registers added. GPIO words still reject before any byte callback,
including D0 with unmapped D1 and D2 with unmapped D3. Prior port4/6/8 hole guards
move D0 to unmapped D4. No implicit board reset callback is emitted. PortA/B,
IRQ/SYSCR,alternate pins,panel/LCD,timers/ADC/DMA,physical timing and complete
hardware reset remain open;no firmware boot or audio claim.

## Independent evidence

`tests/an1x_gpio9_check.cpp` checks262144 per-bit DDR/latch/external combinations,
256 cold external patterns,FF DDR,output data/direction,input/output suppression,
port/time identity,24-bit aliases,per-port/machine isolation,word reads/writes,
neighbors,missing callbacks and reentry even with suppressed inner error.

Synthetic CPU DDR0F/latch5A/external15 =>DA in NVRAM;MOV sets N,clears Z/V and
retains C/I (CCR89).241 every-cycle partitions compare CPU,NVRAM and input/output
histories/timestamps. No CPU/generated-handler inventory change.

Ignored native fixture uses12 verbatim pinned original port bodies,configured
port9/default00/mask C0.200000 xorshift32 seed19A73 mapped DDR/data/pin operations:
zero mismatches in values,callback counts and output data/direction. Present
PCR/ODR bodies are not mapped/exercised. Independent tests check callback port/
timestamps;reference is not whole-machine MAME or physical-timing equivalence.
Ignored mutations:mask C0 reduced to80 =>199031 expected failures;default DDR00
changed to10 =>130 expected failures. Tracked code unchanged.

Full81 CTest/87 Python PASS;GPIO4/6/7/8/9/SCI ASan/UBSan PASS;final-head CI:all five checks PASS,
runs40993876811/40993876814/40993877026. No recorded automated review result at inspection;none claimed. Source notices alone are not legal/binary distribution clearance.
Firmware/MIDI/PKS/DSP/audio/plugin/REAPER acceptance remains open.

Candidate follow-up:[portA byte MMIO](an1x-gpioa.md);earlier omissions describe #128.
