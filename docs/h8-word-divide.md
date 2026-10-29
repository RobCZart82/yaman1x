# H8H unsigned word divide

Candidate based on merged PR87, engine baseline PR86. DIVXU.W5300/ff08 divides
a32-bit destination ER by a16-bit source register half. Both operands precede
write; low16 quotient bits occupy the low word, remainder the high word. A zero
divisor preserves the whole destination. Fixed11 internal cycles plus existing
host charge are preserved. Reserved ER neighbors fail-stop.

## Exact type adaptation and flag boundary

Pinned full/partial bodies use int q/r locals. The candidate explicitly uses u32
for both so division results and packing remain unsigned32, including quotients
above INT_MAX and remainders above32767. This is a documented type clarification,
not a claim of observed UB under the required C++20 build. No arithmetic/flag
algorithm is changed. Exact before/after blocks and original-body hash are in CPU
provenance. Selection requires the locked block twice and rejects expression drift.
The original reference bodies remain unmodified.

Original N follows divisor bit7, not bit15; Z follows zero divisor. Other CCR bits
are unchanged. Preserve this source behavior pending hardware evidence; it is not
independently verified AN1x behavior. No signed division,DSP,EG or Yamaha inputs.

## Independent evidence

Unsigned64 oracle derives remainder by subtraction:524288 divisor sweeps (all
65536 divisors against eight32-bit boundary numerators),30720 register/alias/CCR
cases,20000 random pairs,241 split states and ordered instruction/event histories.
Whole register images, zero retention, quotient truncation, high remainders and
no data accesses checked. Explicit0x0080/0x0100/0x8000/0x8080 cases distinguish
bit7 from bit15. Existing exhaustive first-word/signed-prefix guards remain;
reserved ER neighbors are separately rejected. Local arithmetic PASS.

Original decoder/handlers/execute_run:200000 slices,688814 instruction reads,
37085 byte reads,24311 byte writes,53425 word reads,40515 word writes;zero differences.
Address-bound guards require high-bit,zero and1-divisor paths plus all prior families.
The original word-divide fixture bounds quotients below INT_MAX and remainders
below128; full32/high-remainder correctness belongs to the independent oracle.
A deliberate quotient+1 mutation in both ignored candidate bodies is detected at
seed9/slice26. No tracked mutation remains.

Candidate:61 CTest/87 Python;492 upstream-derived methods (488 unchanged,4 adapted:
2 prior MULXU.W widenings and2 new DIVXU.W local-type changes). Inventory237 selected,
27 missing,135 excluded encoding rows, not CPU percentage. Local Release build,60 runnable CTest groups and
86 Python PASS; one known Windows symlink case excluded. Remote platform/UBSan/MSan/MAME pending. Linux UBSan now includes word divide,
retaining previous arithmetic tests and original unsigned-multiply overflow control.
Generated selections match locked originals plus explicit adaptations; source/
notice checks PASS. Physical timing,firmware/MIDI/audio/plugin acceptance remain open.

See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).

Closure: merged [PR88](https://github.com/RobCZart82/yaman1x/pull/88), engine `3c5f7dddfdf54532670d9cdddd0ade0aab7c92bd`, final head `fc0db2778270a5ffdc612ab46ccd7be0b0d9b4fe`. All five jobs PASS, including Linux UBSan word/byte divide and prior arithmetic controls; [run links](STATUS.md). Preparation-time candidate/pending notes above are historical.
Final source inventory:475 files,201 locked upstream identities,one reviewed manifest edit; four generated-body adaptations separately recorded in CPU provenance. Source/notice checks PASS.
