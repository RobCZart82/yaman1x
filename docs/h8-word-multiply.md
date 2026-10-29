# H8H unsigned word multiply

Candidate based on merged PR81, engine baseline PR80. MULXU.W5200/ff08 selects
16 source-register halves and8 destination ER registers; low destination word
and source word are read before writing the32-bit result. CCR is unchanged.
Fixed H8H m_has_mac=false selects19 original internal cycles plus existing host
charge. No data transfers, MAC profile, DSP or EG changes are introduced.

## Reviewed generated-body adaptation

Pinned MAME commit706dbb73281320c54d7c2f16262c1c3786f2be01 generates
`u16(r32_r(m_IR[0])) * r16_r(m_IR[0] >> 4)`. Both operands promote to signed
32-bit int;65535*65535 exceeds INT_MAX. Each selected full/partial body explicitly
widens the left operand to u32 before multiplication. The maximal product
4294836225 fits u32. No other body semantics change. Provenance records exact
before/after expressions and the original method hash. Selection requires the
locked original expression exactly twice; it rejects a changed source expression.
480 prior bodies are unchanged;2 new bodies contain this reviewed adaptation.
The full482 upstream-derived methods must not be described as all unchanged.

## Independent validation

Unsigned64 oracle:327680 operand sweeps (every16-bit value against five factors),
9600 register/source/destination-alias/CCR boundaries,20000 deterministic random
pairs,241 split execution snapshots and ordered traces. No data access; complete
register images and all CCR bits checked. Local Release PASS.

Unmodified original decoder/handlers/execute_run:200000 slices,695786 instruction
reads,37638 byte reads,24864 byte writes,55084 word reads,42173 word writes;
zero differences. All earlier reachability guards remain; new MULXU.W must be reached.
This reference fixture restricts both word operands to0..32767 before each loop,
so it does not rely on overflow in the original. High-product correctness belongs
to the independent oracle, not the original-reference comparison.
A deliberate product+1 mutation in both ignored candidate bodies is detected at
seed9/slice9. No tracked mutation is retained.

Local57 runnable CTest/86 Python PASS; one known Windows symlink privilege case
excluded. Candidate total58 CTest/87 Python. A strengthened existing Python test
checks adaptation/original separation and rejects drift in the original expression.
Linux CI builds the word test under UBSan with recovery disabled, runs the full
arithmetic oracle, and requires a separate process reproducing the original
u16*u16 promotion to fail with signed integer overflow. An unsanitized binary is
rejected by that control. Actual UBSan/platform/MSan/MAME results are pending;
local Windows did not run UBSan. Source/notice inventories pass separately.

Synthetic/source-reference evidence does not establish physical timing,
firmware/MIDI/audio or plugin acceptance. M0 remains partial. See
[plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).

Closure: merged [PR82](https://github.com/RobCZart82/yaman1x/pull/82), engine `80b237ee7a9ad6b286a13c9253d076aa42bb0214`, final head `4f2cffc88c843cc9d431f5c0a6ec20ee2869d85b`. All five jobs PASS, including actual Linux UBSan oracle and original-overflow negative control; [run links](STATUS.md). Preparation-time candidate/pending notes above are historical. This resolves the recorded C++ promotion risk for the fixed H8H word-multiply subset.
Final source inventory:469 files,201 locked upstream identities,one reviewed manifest edit; the two generated-body widening adaptations are recorded separately in CPU provenance. Source and notice checks PASS.
