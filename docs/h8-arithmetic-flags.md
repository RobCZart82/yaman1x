# H8H arithmetic and comparison flags

Recorded: 2026-10-28. Based on merged #49. M0 remains partial.

The limited profile now retains 78 unchanged generated full/partial methods.
This increment adds byte ADD/CMP immediate, byte ADD/SUB/CMP register forms,
and word/long ADD/SUB/CMP immediate forms. Other arithmetic, carry-input,
logical, shift and addressing forms remain unsupported and fail explicitly.

| Width/form | Supported instruction words |
|---|---|
| ADD.B #imm8, Rb | `0x8000 | (r << 8) | imm8` |
| CMP.B #imm8, Rb | `0xa000 | (r << 8) | imm8` |
| ADD/SUB/CMP.B Rs, Rd | `0x0800` / `0x1800` / `0x1c00`, plus `(s << 4) | d` |
| ADD/CMP/SUB.W #imm16 | `0x7910` / `0x7920` / `0x7930`, plus r=0..15, then immediate |
| ADD/CMP/SUB.L #imm32 | `0x7a10` / `0x7a20` / `0x7a30`, plus r=0..7, then high/low words |

There is no invented immediate SUB.B encoding; byte subtraction uses the actual
register form. Comparisons use subtraction flags while leaving all destination
register bits unchanged. Initial CCR carry is not an input to these forms;
ADDX/SUBX are not enabled by this increment.

## Flag and partial-execution contract

The six pinned do_add/do_sub helpers are adapted into a private width-generic
unsigned implementation with 64-bit intermediate storage for 8/16/32-bit values.
It preserves fixed-H8H `m_has_hc=true` behavior: H carry/borrow boundaries are
bits 4, 12 and 28 respectively. N/Z/V/C and H are replaced from the operation;
CCR bits outside those flags are retained. Incoming H/C cannot leak into an
ordinary ADD/SUB result. No H8S or alternate half-carry profile is advertised.

Generated handlers preserve original operand reads, register aliases, prefetch
ordering and partial substates. An arithmetic effect completed before a
suspended prefetch must not repeat when debt is paid or a partial handler
continues. CMP changes flags only. The existing prefix/internal-state guards,
bus charges, exception/discard rules and unsupported IRQ/transfer/debug/standby
limits are unchanged.

## Evidence

- All 65,536 byte operand pairs for each of five byte forms: 327,680 executed
  cases, with varying initial CCR bits and complete register-image checks.
- 12,288 byte register/lane/alias cases cover same-register and overlapping
  high/low-byte operands plus untouched register halves.
- 100,800 word/long boundary and deterministic random cases across every
  supported destination register. Boundaries include half-carry, zero, sign,
  unsigned carry/borrow and signed overflow transitions.
- Expected flags use an independent signed-range and modular-arithmetic oracle,
  rather than duplicating the implementation's xor overflow formula. CMP
  controls compare the complete register image before/after execution.
- A suspended ADD prefetch with odd slice/debt verifies exactly-once arithmetic.
  1,000 mixed arithmetic/condition/memory partition histories agree in full
  diagnostic state, ordered bus trace and data memory.
- Locked original H8H decoder/handlers, six original arithmetic helpers and
  original bus/prefetch/execute_run methods: 200,000 slices; 791,089 instruction
  reads, 13,555 byte reads, 13,557 byte writes, 40,593 word reads and 40,606 word
  writes; zero differences after each slice. IRQ/transfer/debug services are
  disabled; this is an isolated synthetic host, not the complete MAME device.
- Negative control: an inverted half-carry decision in an ignored candidate
  copy is detected by the unchanged original reference at seed 0, slice 5.
- Both generated selections remain byte-identical to locked outputs apart from
  removed class qualifiers and normalized line endings; retained BSD notices and
  provenance identify the added handlers and adapted arithmetic helpers.

Use the existing `check_h8_first_instructions.py --generated ...` commands for
the base/H8H variants or `--archive ... --work ...` for the original-code
experiment. Work must be new. Default tests require no archive or Yamaha data.

Next: remaining register arithmetic, carry-input and logical/shift operations,
then stack/control flow, IRQ entry and explicit CPU/SCI scheduling. Full CPU,
faithful reset/save-state, hardware timing, firmware PKS/MIDI, audio and REAPER
acceptance remain open. No upstream revision, Yamaha payload or EG behavior
change is introduced.

Candidate local validation: Windows MSVC Release build, all 31 runnable CTest groups and 83 Python cases PASS, with the one known symlink-privilege case explicitly excluded locally. Complete suite: 32 CTest groups / 84 Python cases. Source/notice audits PASS; final PR-head platform/MSan/MAME evidence remains separate.
