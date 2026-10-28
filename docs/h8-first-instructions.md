# First generated H8 instructions on the standalone bus/runner

Recorded: 2026-10-28. Based on merged #46. M0 remains partial.

`H8FirstInstructions<Host>` is an intentionally limited advanced-H8H execution
profile. It compiles eight real generated full/partial methods: reset, NOP,
MOV.B immediate-to-register and BRA rel8. The methods are inherited base-H8
operations in the original H8H decoder. Their bodies are unchanged; only class
qualifiers are removed so they can be included inside the standalone template.

The profile owns its 16 register halves, CCR/EXR, PC/PPC/NPC, IR/PIR, temporary
registers, instruction/substate and cycle debt per instance. `H8hBusAccess`
charges before completed instruction reads; `H8ExecutionRunner` preserves
peripheral-boundary and partial-handler ordering. The reduced decoder rejects
all other opcodes explicitly and poisons execution through the runner.

## Host and lifecycle contract

- Host must outlive the CPU and provide `read_instruction_word(address)` and
  `update(cycle)` returning the next absolute peripheral deadline (zero=none).
  The bus owns address-space mapping, including 24-bit mapping/wrapping when
  appropriate. CPU word reads align down exactly as in the reviewed bus helper.
- Reads must complete synchronously. Wait/retry behavior is disabled; no host
  API advertises it. Program storage must remain immutable during execution.
- IRQ, DMA, DTC, trace/debug hooks and standby are disabled and have no enabling
  API. Prefetch completion therefore uses the original no-request/no-transfer/
  no-IRQ/no-trace branch. Reset IRQ-filter publication is a no-op in this profile.
- Construction uses deterministic research cold-state defaults and optionally
  an initial CCR for synthetic experiments. These are not verified AN1x
  power-on values. It starts in the real reset-vector handler; there is no warm
  reset or load-state API. The EXR value is tracked as the original reset code
  assigns it, without claiming H8S/EXR instruction support.
- Snapshot is read-only diagnostic state, not a save-state format. No callback
  reentry, concurrent mutation or cross-machine register sharing is allowed.
  Callback/unsupported-opcode failures require discarding CPU and host state.
  Invalid slice parameters are rejected by the existing runner contract.
- This is an offline research subset; no real-time or complete CPU guarantee.

## Source verification and reproducibility

`research/h8-first-instructions-provenance.json` records input file identities,
locked generated-output hash, selected methods and retained BSD attribution.
The selected header hash is checked in default Python tests. The source lock
continues to identify the upstream generator and instruction list; only the
reviewed selection is committed, with no Yamaha inputs.

```sh
python tools/check_h8_first_instructions.py --generated /absolute/path/to/verified/h8.hxx
python tools/check_h8_first_instructions.py --archive /absolute/mame-master.zip --work work/first-reference
```

The first command checks the complete generated output against its lock before
comparing the selection. The second validates the original archive, regenerates
all four locked outputs twice, verifies selected bodies, and compiles a local
reference using the original generated **H8H decoder**, original selected
handlers, execute_run, recompute_bcount, instruction-read and flag/register/
prefetch helpers. Unsupported reference handlers throw. Debug/IRQ/transfer
services are disabled. The fixture uses synthetic memory and deadlines; it
is not the complete MAME device or an AN1x firmware run. Optional `--cmake`
selects CMake; a configured C++20 compiler is required. Work directory must be
new; original implementation bodies and binaries stay in ignored local work.

## Evidence and remaining work

- Native tests: reset-vector/read/prefetch sequence; 4,096 register/immediate
  combinations; 65,536 initial-CCR/value cases; all 256 signed BRA offsets
  including odd targets; byte-lane preservation, zero/odd slices and debt;
  1,000 whole/partitioned histories; instance isolation and failure handling.
- Original-method/decoder experiment: 100 synthetic programs, 2,000 slices each,
  899,400 instruction reads; zero differences in full diagnostic state, ordered
  read address/data trace and next peripheral deadline after every slice.
- Negative control: a separate ignored candidate copy with a deliberately wrong
  register-byte shift fails against the unchanged original reference at seed 0,
  slice 0. The tracked implementation was not changed for that experiment.
- Six Python regressions cover complete nested partial-body extraction,
  duplicate/missing methods, generated/selected corruption, LF/CRLF behavior,
  recorded source identities and bad-archive rejection before output/execution.

Next: expand generated instruction coverage to data-memory operations and
conditional branches with independent flag/bus controls, then implement IRQ
entry and explicit CPU/SCI ordering. Full opcode coverage, timers/ADC/transfers,
faithful reset/save-state, firmware PKS/MIDI, hardware timing, audio and REAPER
acceptance remain open. M0 and M1 acceptance criteria are unchanged.

Candidate local validation: Windows MSVC Release build, all 28 locally runnable CTest groups PASS; all 81 runnable Python cases PASS with the one established symlink-privilege test explicitly excluded. Complete candidate suite: 29 CTest groups / 82 Python cases. Source audit and notice coverage PASS; final PR-head platform/MSan/MAME results remain separate evidence.
