# H8H INC/DEC and small address arithmetic

Recorded: 2026-10-28. Follows the register MOV increment.
Sixteen groups add 32 original generated full/partial methods (382 -> 414).
INC/DEC supports byte by one and word/long by one or two. ADDS/SUBS supports
long registers by one, two or four. H8/300 base-only register forms stay excluded.
The fixed-width INC/DEC helper uses the reviewed unsigned arithmetic and restores
H/C, preserving every flag outside N/Z/V. ADDS/SUBS preserves the whole CCR.

The original host INC/DEC methods are included in the local reference build.
Independent signed-range arithmetic supplies the native overflow oracle;
byte and word inputs are exhaustive, long inputs include signed/wrap boundaries
and deterministic samples. All ADDS/SUBS destination registers and initial CCR
values are checked. 1000 partition histories compare complete snapshots/traces.
The complete first-word acceptance oracle and coverage report are refreshed:
200 selected encoding rows / 64 missing / 135 excluded profiles.

Local Release: 42/42 CTest groups and 87 Python cases PASS; new ASan/UBSan PASS;
200000 original decoder/handler/execute_run slices agree with no differences.
Source notices, locks and provenance retained. Final PR-head CI is separate.
No IRQ/firmware/audio acceptance. No planned hold after checks and review.

Windows portability follow-up: MSVC C1061 on the growing outer else-if dispatcher is fixed with ordered independent guards and immediate return after the selected handler. Internal instruction branches and generated method bodies are unchanged. This preserves first-match behavior even when a handler changes the instruction state. Final-head Windows CI must confirm compilation.
