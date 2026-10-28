# H8H register-to-register transfers

Recorded: 2026-10-28. Base: merged #60, `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db`.
M0 remains partial; no IRQ, firmware or audio acceptance.

Three original instruction groups add six full/partial methods (376 -> 382):
MOV.B between all sixteen byte lanes, MOV.W between all sixteen word registers,
and MOV.L between eight extended registers. Same-source/destination aliases
read the old source and preserve all untouched lanes/upper words. Original
set_nzv helpers update N/Z, clear V and preserve every other CCR bit.
The H8H long form uses mask ff88; adjacent decimal-adjust encodings are not added.

Selected bodies are unchanged except class qualifier removal. Provenance hashes,
method lists and the locked coverage report are refreshed. Coverage becomes
184 selected encoding rows / 80 missing / 135 excluded profiles; these are not
an ISA completion percentage. The exhaustive first-word oracle now includes
576 additional encodings (256 byte, 256 word, 64 long).

Verification: 147456 width/register/alias/data/flag combinations with independent
register/flag expectations; 1000 mixed whole/partitioned histories; full reduced
decoder test; optional original decoder/handlers/execute_run comparison of
200000 slices with zero differences. The reference fixture adds MOV sequences
in a dedicated reachable case with a three-form MOV fetch guard; its rel8 loop stays representable. New native
ASan/UBSan PASS. Full local suite: 41/41 CTest and 87 Python cases PASS.
Final PR-head CI is separate evidence.

Source notices and license footprint are unchanged. No new host helper or
hardware timing assumption is added. Remaining work includes unary/small
arithmetic, memory addressing, IRQ/TRAPA/filter integration and common CPU /
peripheral ownership. No planned hold after successful checks and review.
