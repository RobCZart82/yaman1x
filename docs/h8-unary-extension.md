# H8H NOT, NEG and sign/zero extension

Recorded: 2026-10-28. Follows INC/DEC/small arithmetic.
Ten original groups add twenty generated full/partial methods (414 -> 434).
NOT/NEG supports byte, word and long; EXTU/EXTS supports word and long.
No modified generated body or new arithmetic algorithm is added. A uint16 alias
is supplied for the original EXTU.L cast. Decoder masks preserve unsupported
neighbors. The exhaustive first-word oracle and provenance are refreshed.
Coverage: 210 selected encoding rows / 54 missing / 135 excluded profiles.

Independent native expectations cover every byte/word value and 65536 long
samples per operation, including signed minimum/maximum and wrap boundaries.
NOT/extensions update N/Z and clear V while preserving other flags. NEG compares
against unsigned zero-minus-value plus independent borrow/half-borrow and signed
minimum overflow rules. 1000 histories compare whole/partitioned snapshots and
bus traces. Local 43/43 CTest groups and 87 Python cases PASS; new native
ASan/UBSan PASS. Original decoder/handlers/execute_run: 200000 slices, no differences.

The dedicated reachable reference loop includes CCR, register MOV, small arithmetic
and all ten unary forms; explicit fetch masks require each family to be reached.
This corrects earlier candidate reference arrangements that followed an RTE redirect.
The narrow native expectation tests were independent of those reference fixtures.
Final PR-head CI remains separate evidence. M0 partial; IRQ/TRAPA, remaining
addressing/instructions and machine/firmware/audio integration stay open.
Merge depends on preceding feature branches; no hold beyond those dependencies
and successful checks/review is intended.
