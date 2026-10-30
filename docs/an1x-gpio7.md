# AN1x read-only port7 byte MMIO

Recorded:2026-10-30. Merged #126 as `a8b0ca4c87478aa188b902ef997b7c88c284bbf8`;tested head `32dcba307e72f2acfcffd494c890afbc2894c57f`.

## Source facts and lifecycle boundary

Pinned MAME commit:`706dbb73281320c54d7c2f16262c1c3786f2be01`.
Rechecked locked `src/devices/cpu/h8/h83002.cpp`,SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
advanced FFFFCE is `.r(m_port7,port_r)`;configuration default DDR00,mask00.
There is no mapped port7 DDR/data-write/PCR register here.

Project-owned `An1xSciBus` routing adds a separate,unchanged `H8DigitalPort`
owner. At its fixed zero DDR/mask,every pin comes from caller input.
Source methods/notices remain those of [the original extraction](h8-digital-port.md).
No upstream body,instruction,Yamaha payload or new dependency is added.

Each byte read samples `gpio_read(7,committed_cycle)` once.24-bit aliases and
existing fail-stop/missing-callback/reentry guards apply. No initial input,
output or PKS reply is invented. Port4/6/8 state and callbacks remain separate.
NVRAM persistence excludes peripheral registers and is not full board reset.

Read-only writes are explicitly rejected before callbacks under the bounded
host fail-closed policy;this does not claim MAME's unmapped-write/open-bus
behavior. GPIO word accesses,including adjacent port7+port8 at FFFFCE/FFFFCF,
remain unsupported and reject before either byte callback. The prior port8
write-neighbor guard still correctly rejects FFFFCE;it is now read-only rather
than wholly unmapped. Other ports,IRQ/SYSCR,alternate pins,panel/LCD,timers/ADC/DMA,
physical timing and complete hardware reset remain open.

## Independent validation

`tests/an1x_gpio7_check.cpp` checks all256 direct bytes,port7 callback identity,
exact timestamps and24-bit aliases. Synthetic CPU ROM reads each of256 input
values into NVRAM. Independent MOV N/Z/V/C/I expectations include zero and both
sign ranges.161 every-cycle partitions compare CPU,NVRAM and input histories.
Tests also cover per-port/per-machine input identity,port8 drive isolation,
input-only behavior,read-only writes,word rejection,missing callbacks and
reentry even when the sink suppresses the inner failure.

Ignored native original-method fixture contains12 verbatim pinned port bodies,
configured port7/default00/mask00. It exercises device_start and port_r with
200000 xorshift32 seed19A73 reads:zero mismatches in values and callback counts.
Other present register bodies are not mapped/exercised here. This is register
comparison,not full-machine MAME or physical timing equivalence.
Routing port7 reads to port8 in an ignored mutant produces769 expected failures;
tracked source is unchanged. CPU/generated-handler inventory is unchanged.

Full80 CTest/87 Python PASS;GPIO4/6/7/8/SCI ASan/UBSan PASS;final-head CI:all five checks PASS,
runs40990014063/40990014152/40990013951. No recorded automated review result at inspection;none claimed. Source-notice checks are not legal/binary release clearance.
Firmware/MIDI/PKS/DSP/audio/plugin/REAPER acceptance remains open.

Merged #128 follow-up:[port9 masked MMIO](an1x-gpio9.md);earlier other-port omissions describe #126.
