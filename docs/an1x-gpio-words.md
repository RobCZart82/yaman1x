# Bounded GPIO word accesses

Recorded:2026-10-30. Candidate based on #133 (`12e923edb2fbccb791fa5d8d9383c493c7c4c1cc`),
merged engine #132. Final-head CI/review and merge pending.

## Source facts and project policy

Pinned MAME `706dbb73281320c54d7c2f16262c1c3786f2be01`,
`src/devices/cpu/h8/h83002.cpp` SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`
records adjacent byte handlers:CE port7 input/CF port8 pins;D0 port9 DDR/D1 portA DDR;
D2 port9 pins/D3 portA pins. Other current GPIO word pairs contain unmapped bytes.
Existing port register bodies and source identities remain unchanged.

This project-owned bounded composition accepts aligned,big-endian words only
when both GPIO bytes are mapped. Reads atFFFFCE,FFFFD0,FFFFD2;writes atFFFFD0,FFFFD2.
Read-only CE causes the whole CE/CF write to fail before callbacks. Odd words and
pairs containing holes fail before effects.24-bit aliases are normalized first.
No open-bus or unmapped-write behavior is invented. Byte accesses remain unchanged.
Instruction fetch remains ROM/RAM-only. No new Yamaha input,dependency or copied method.

High-address-significance lane executes first (base),then low lane (base+1),at
the same committed CPU bus timestamp. This is an explicit deterministic software
contract consistent with existing big-endian SCI composition;no physical GPIO
arbitration/timing or complete MAME memory-system equivalence is claimed.
The CPU bus adapter charges one word access (existing two-cycle model),not two
new byte charges. Board wait states and actual pin timing remain unverified.

Preflight rejects static unsupported pairs before any GPIO callback. A callback
failure may leave earlier lane effects committed;there is no rollback. Reentry
poisons the owner,even if the callback catches the inner exception. The code
checks that poison between lanes,preventing a second callback after first-lane
reentry. After any failure discard the bus and its CPU. Callback sinks must not
mutate retained peripheral references or execute concurrently.

## Independent tests

`tests/an1x_gpio_word_check.cpp`:

- 65536 paired DDR writes/FF reads and65536 data/pin cases with independently
 reconstructed port9 maskC0 and portA full8-bit values;24-bit aliases exercised.
- 65536 port7/8 read words using all high/low input combinations;port8 defaultF0,
 maskE0 reflected in an independent expected value (bit4 driven low).
- Exact high-before-low input/output callback identity and timestamps;unchanged
 word output suppresses notifications. DDR reads need no input sink.
- Odd/incomplete/read-only writes fail before callbacks. Missing sinks,first/second
 lane callback faults,normal and swallowed-inner-error reentry,failed reuse covered.
 First-lane fault prevents second-lane callbacks;second-lane fault retains prior effects.
- Synthetic CPU configures paired DDR0F0F,writes5AA5,reads external15/62 =>DA65,
 storesDA65 in NVRAM,R2=DA65,CCR89.241 every-cycle partitions compare complete CPU
 snapshots,NVRAM and ordered pin callback histories/times.

Prior byte-port guard tests retain only unsupported word cases;newly valid word
paths have dedicated positive tests. Candidate suite84 CTest/87 Python;Release build,83 locally runnable CTest groups
and86 Python cases PASS;one established Windows symlink privilege case excluded.
The final target rebuild also passes the missing-sink cases. Final-head CI pending. No CPU/generated-handler body changed:562 methods,264 selected rows.

## Original component comparison and controls

Ignored fixture verifies11 verbatim cpp bodies plus inline FF read against the
locked `h8_port.cpp`/`h8_port.h` hashes:
`c767e444bebcfb849d0cf225bb8a94e8eec481d62a75c1b554d36a8eda5114ca`,
`50579e13ab71992aee11fcf6372c538afa88689f72806c4dad6e693e67f3b74d`.
Two original port instances (port9 maskC0,portA mask00) execute explicit high/low
method calls.200000 xorshift32 mixed operations,seed19A73:zero mismatches in
word values,callback counts,data,direction and alias handling. This checks original
port-equation composition,not MAME address-space dispatch or physical pin order.
Unused PCR/ODR/reset bodies are present but not exercised.

Candidate-only ignored swapped-read-byte mutant produces262198 failed checks;
low-lane-write-from-high-byte mutant produces455493. Multiple value/state sites per
iteration mean failed checks can exceed the operation count. Positive and mutants
built separately;tracked source remains correct.

Linux CI includes the target under UBSan. Local ASan/UBSan not run. Source/notice
checks do not constitute legal or binary-distribution clearance. Remaining holes,
IRQ/SYSCR,full reset,LCD/DSP/MEG,firmware/MIDI/PKS,audio and REAPER remain open.
M0 partial;M1 unaccepted.
