# H8H coverage inventory and CCR register/immediate family

Recorded: 2026-10-28. Base: merged audit #59,
`717b8318b7a4d0f403915793a2b5676d485c588d`.
M0 remains partial; IRQ filter publication/entry and firmware are not integrated.

## Reproducible coverage boundary

`tools/h8_opcode_coverage.py` verifies the locked `h8.lst` hash before producing
`research/h8-opcode-coverage.json`. For each of 399 instruction encoding rows it
records source line, value/mask, skip, operands, generator profile and method.
There are 181 eligible rows with selected handlers, 83 eligible rows still missing,
and 135 rows excluded by profile. This is not an opcode-word percentage or proof
of complete dispatch. Internal states/macros are excluded. A selected method means
its full/partial bodies are included; prefix/decoder/semantic tests remain separate.
Profiles follow the pinned generator's H8H enablement: common/H8H are eligible;
base-only, H8S and GT913 rows are excluded. This is source evidence, not hardware ISA verification.

```sh
python3 tools/h8_opcode_coverage.py --list work/h8-generation/h8.lst --output work/h8-coverage.json
```

The input is the locked list already produced by the generation tool; output must
be new. Missing rows include absolute/displacement transfers, CCR word memory,
INC/DEC and small arithmetic, register transfers, unary/extension, decimal adjust,
multiply/divide, TRAPA, SLEEP and EEPMOV. The structured report is the exact list.

## Implemented first missing family

Add STC CCR to any byte register, LDC any byte register to CCR, and immediate
ORC/XORC/ANDC. Existing immediate LDC remains unchanged. Five original groups add
ten unchanged generated methods, taking selected full/partial bodies from 366
to 376. The selected-file hash and provenance list are refreshed; original source
and generator identities/notices remain unchanged. EXR/MAC neighboring encodings
remain rejected. The complete reduced-decoder acceptance oracle includes the 800
newly accepted first-word encodings (32 register forms and 768 immediate forms).

CCR logic replaces/combines all eight bits, rather than using arithmetic N/Z/V
updates. STC preserves CCR and untouched registers. LDC preserves register state.
Original no-IRQ prefetch/update_irq_filter calls are retained; the fixed host's
filter hook is still a stub. This increment does not claim interrupts work.

## Verification

- 196608 immediate CCR/operand cases, 4096 STC lane/value cases and 12288 LDC
  lane/value/initial-CCR cases; independent expected bits/register preservation.
- 1000 whole/partitioned histories comparing complete snapshots and bus traces.
- EXR/MAC neighbors rejected; old exhaustive first-word oracle updated explicitly.
- New native ASan/UBSan PASS; source/notice audits PASS.
- Optional original H8H decoder/handlers/execute_run experiment: 200000 slices,
  zero differences after adding CCR sequences to a short reference fixture.
  The reference uses a dedicated reachable CCR loop with an explicit all-five-form
  fetch-coverage guard. Earlier layouts exceeded rel8 range or followed an RTE
  redirect; those fixture issues were corrected before merge without changing
  production CPU timing or jump behavior.

Normal suite: 40 CTest groups, 87 Python cases (three new coverage regressions).
No firmware/ROM/bank input is used. Final PR-head CI is separate evidence.
Next: remaining register-transfer/unary/small-arithmetic families, memory addressing,
then IRQ/TRAPA/filter integration and common CPU/peripheral ownership. Merge after
checks/review; no planned hold across later milestones.
