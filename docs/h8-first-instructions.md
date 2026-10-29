# First generated H8 instructions on the standalone bus/runner

Initial increment: #47 based on #46, 2026-10-28. Current memory/branch extension
is based on merged #47; see [the extension contract](h8-memory-branches.md). M0 remains partial.

`H8FirstInstructions<Host>` is an intentionally limited advanced-H8H execution
profile. The initial eight methods have expanded to 512 upstream-derived full/partial methods:
reset, NOP, immediate byte/word/long moves, all rel8 conditions, prefix dispatch
indirect byte/word/long loads/stores, selected ADD/SUB/CMP and carry-input forms,
logic, LDC immediate CCR and single-bit shifts/rotates. The selection now includes base-H8 and H8H operations. 508 bodies are unchanged except class qualifiers. Two MULXU.W bodies have reviewed unsigned widening; two DIVXU.W bodies
use explicitly unsigned quotient/remainder locals. See [multiply](h8-word-multiply.md)
and [divide](h8-word-divide.md) contracts;508 bodies remain unchanged.

The profile owns its 16 register halves, CCR/EXR, PC/PPC/NPC, IR/PIR, temporary
registers, instruction/substate and cycle debt per instance. IR has four words
for immediate32 and long absolute decoding; the diagnostic snapshot is not a persistence format. `H8hBusAccess`
charges before completed instruction and byte/word data accesses; `H8ExecutionRunner` preserves
peripheral-boundary and partial-handler ordering. The reduced decoder rejects
all other opcodes explicitly and poisons execution through the runner.

## Host and lifecycle contract

- Host must outlive the CPU and provide `read_instruction_word(address)`,
  `read_byte(address)`, `write_byte(address,value)`, word equivalents and `update(cycle)` returning the next absolute peripheral deadline (zero=none).
  The bus owns address-space mapping, including 24-bit mapping/wrapping when
  appropriate. CPU word reads align down exactly as in the reviewed bus helper.
- Reads and writes must complete synchronously. Wait/retry behavior is disabled; no host
  API advertises it. Program storage must remain immutable during execution.
- IRQ input/entry, DMA, DTC, trace/debug hooks and standby remain unimplemented.
  LDC stores all CCR bits but the IRQ-filter hook is still a no-op; no interrupt
  acceptance is claimed. Prefetch completion therefore uses the original no-request/no-transfer/
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
python tools/check_h8_first_instructions.py --generated /absolute/path/to/verified/h8h.hxx --h8h
python tools/check_h8_first_instructions.py --archive /absolute/mame-master.zip --work work/first-reference
```

Each --generated command checks its complete generated output against its lock before
comparing the selection. The archive command validates the original archive, regenerates
all four locked outputs twice, verifies selected bodies, and compiles a local
reference using the original generated **H8H decoder**, original selected
handlers, execute_run, recompute_bcount, instruction-read and flag/register/
prefetch helpers. Unsupported reference handlers throw. Debug/IRQ/transfer
services are disabled. The fixture uses synthetic memory and deadlines; it
is not the complete MAME device or an AN1x firmware run. Optional `--cmake`
selects CMake; a configured C++20 compiler is required. Work directory must be
new; original implementation bodies and binaries stay in ignored local work.

## Historical #47 evidence and remaining work

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

The byte-memory and rel8-condition part of the former next step is now covered
by the extension contract. Word/long operations, arithmetic/compare flags, IRQ
entry and explicit CPU/SCI ordering remain open. Full opcode coverage, timers/ADC/transfers,
faithful reset/save-state, firmware PKS/MIDI, hardware timing, audio and REAPER
acceptance remain open. M0 and M1 acceptance criteria are unchanged.

Historical #47 local validation: Windows MSVC Release build, all 28 locally runnable CTest groups PASS; all 81 runnable Python cases PASS with the one established symlink-privilege test explicitly excluded. Complete candidate suite: 29 CTest groups / 82 Python cases. Source audit and notice coverage PASS; final PR-head platform/MSan/MAME results remain separate evidence.

Current word/long and prefix semantics, new host word callbacks and independent controls: [word/long contract](h8-word-long-memory.md).

Current arithmetic forms, fixed-profile half-carry boundaries and independent flag controls: [arithmetic contract](h8-arithmetic-flags.md).

Current carry-input/sticky-Z, register and logical forms plus the limited CCR/IRQ contract: [carry/register/logic](h8-carry-register-logic.md).

Word/long register OR/XOR/AND now use the original 01f0 long prefix and guarded 0x110000 extension state. See [wide register logic](h8-wide-register-logic.md). Shift/rotate, bit, stack, IRQ entry and CPU/SCI integration remain open.

The [single-bit shift/rotate extension](h8-shift-rotate.md) adds eight operations at 8/16/32 bits using unchanged selected handlers and a width-generic unsigned adaptation of the pinned helpers. Two-bit shifts, bit operations, stack/IRQ and CPU/SCI remain open.

The [register-bit extension](h8-register-bit-operations.md) adds original byte-register bit operations and carry boolean/load/store forms. H8S two-bit shifts are outside the H8H profile. Memory bit operations, stack/control flow and IRQ/SCI remain open.

The [indirect memory bit extension](h8-memory-bit-operations.md) adds original 7c/7d prefixes and register-indirect byte bit forms with guarded internal states. Absolute forms, stack/control flow and IRQ/SCI remain open.

The [absolute memory bit extension](h8-absolute-bit-operations.md) adds original 7e/7f prefixes and all abs8 byte bit forms with guarded internal states. Stack/control flow and IRQ/SCI remain open.

The [stack/control extension](h8-stack-control-flow.md) adds the normal H8H stack and control instruction families and synthetic RTE frame restore. internal(cycles) charges cycles+1 for the fixed no-EXR H8H profile. IRQ/TRAPA entry, full CPU coverage and CPU/SCI integration remain open.

Signed16 displacement candidate: six original groups; see [contract](h8-displacement16-memory.md). Fixed-profile IRQ/TRAPA and synthetic CPU/SCI integration are implemented; full machine/firmware/audio acceptance remains open.

Merged unsigned byte multiply: [scope, checks and word promotion risk](h8-byte-multiply.md).

Merged signed multiply: [scope,original helpers and evidence](h8-signed-multiply.md).

Merged unsigned byte divide: [scope and evidence](h8-byte-divide.md).

Merged unsigned word divide: [source flags and full-range evidence](h8-word-divide.md).

Merged DAA/DAS: [decimal oracle and evidence limits](h8-decimal-adjust.md).

Merged signed byte divide: [scope and remaining word extreme case](h8-signed-byte-divide.md).

Merged signed word divide: [32-bit-int profile and extreme flags](h8-signed-word-divide.md).

Displacement32 byte/word candidate: [address wrap,alias and prefix controls](h8-displacement32-memory.md).
