# H8H carry-input, register arithmetic and logic

Recorded: 2026-10-28. Based on merged #50. M0 remains partial.

The limited profile now retains 124 unchanged generated full/partial methods.
This increment adds word/long register ADD/SUB/CMP, byte ADDX/SUBX immediate and
register forms, byte register/immediate AND/OR/XOR, word/long immediate logic,
and LDC immediate CCR. Remaining addressing forms, wide register logic,
shift/rotate, bit, stack/control-flow and IRQ/SCI integration remain open.

| Operation | Added encoding |
|---|---|
| LDC #imm8, CCR | `0x0700 | imm8` |
| ADDX/SUBX.B #imm8, Rb | `0x9000` / `0xb000`, plus `(r << 8) | imm8` |
| ADDX/SUBX.B Rs, Rd | `0x0e00` / `0x1e00`, plus `(s << 4) | d` |
| ADD/SUB/CMP.W Rs, Rd | `0x0900` / `0x1900` / `0x1d00`, plus register selectors |
| ADD/SUB/CMP.L ERs, ERd | `0x0a80` / `0x1a80` / `0x1f80`, plus s,d=0..7 |
| OR/XOR/AND.B immediate | `0xc000` / `0xd000` / `0xe000`, plus byte register and immediate |
| OR/XOR/AND.B register | `0x1400` / `0x1500` / `0x1600`, plus source/destination |
| OR/XOR/AND.W immediate | `0x7940` / `0x7950` / `0x7960`, plus r=0..15 and immediate |
| OR/XOR/AND.L immediate | `0x7a40` / `0x7a50` / `0x7a60`, plus r=0..7 and high/low words |

## Carry and CCR semantics

ADDX/SUBX consume the incoming C bit. Zero is sticky: a nonzero result clears Z;
a zero result preserves incoming Z rather than asserting it. This permits a
multi-byte operation to retain Z only when every result byte is zero and Z was
initially set. H/N/V/C are recomputed with the pinned fixed-H8H half-carry rule;
unrelated CCR bits remain unchanged. Source operands are captured before an
aliased destination is written. Ordinary arithmetic remains independent of
incoming C; comparisons never write the destination.

Logical operations replace N/Z/V and preserve H/C and unrelated bits. LDC stores
the full CCR, including I, enabling exact incoming-flag controls in real test
programs. It executes the original filter-publication hook, which is still a
no-op in this fixed profile: there is no IRQ input/entry implementation. Storing
I must not be mistaken for implemented interrupt acceptance. The original
prefetch_done_noirq path is retained; future IRQ integration must implement its
distinction from ordinary prefetch completion.

Generated operand, prefetch and partial-handler ordering remains unchanged.
No completed carry operation may repeat on a debt-only or partial continuation.
The immutable-code/synthetic-data host, address mapping and callback/discard
contracts from earlier increments continue to apply. No warm-reset/save-state,
retry, DMA/DTC, debugger/trace, standby or real-time guarantee is added.

## ROM-free controls

- 1,048,576 carry cases cover all byte operand pairs, both incoming C and both
  incoming Z states for four immediate/register ADDX/SUBX forms.
- 30,720 word/long register/lane/alias arithmetic cases verify full register
  images and comparison no-write behavior.
- 393,216 byte logic pairs, 12,288 byte logic alias cases and 36,864 word/long
  immediate logic cases check results, NZV and preserved H/C, including I=0/1.
- 20,000 four-byte ADDX/SUBX programs agree with an independent 32-bit signed/
  modular oracle on result, carry/borrow, overflow, half-carry and sticky zero.
  The oracle is shared with earlier arithmetic tests; reset-I and carry/sticky-Z
  modes are explicit so old cold-state controls remain unchanged.
- A suspended carry prefetch and 1,000 mixed partition histories prove state,
  memory and ordered-bus equality, including exactly-once arithmetic.
- Locked original H8H decoder, selected handlers and original flag/bus/runner
  methods: 200,000 slices, 782,229 instruction reads, 14,665 byte reads, 14,666
  byte writes, 43,910 word reads and 43,930 word writes; zero differences.
  Two short program variants repeatedly exercise old and new forms; a range
  guard rejects a reference loop too long for rel8. Dedicated zero-result
  ADDX/SUBX cases exercise preserved clear/set Z, with carry inputs also varied.
- Negative control: forcing Z on a zero carry result in an ignored candidate
  copy is detected by the unchanged original reference at seed 1, slice 53.
- Both selected headers match locked generated outputs and retain BSD notices.
  The extended-byte helper adapts the pinned do_addx8/do_subx8 implementations;
  no upstream revision or Yamaha data is introduced.

The original-code fixture disables unimplemented interrupt/transfer/debug
services and is not the complete MAME device or a firmware/hardware run.
Use existing `check_h8_first_instructions.py --generated ...` checks or the
`--archive ... --work ...` experiment; generated reference bodies/executables
stay local and ignored. Default CMake checks need no archive or ROM.

Next: remaining wide register logic, shift/rotate and bit operations, then
stack/control flow, IRQ entry and explicit CPU/SCI scheduling. Full CPU,
faithful reset/save-state, hardware timing, firmware PKS/MIDI, audio and REAPER
acceptance remain open.

Local validation (Windows, 2026-10-28): 32 runnable CTest groups and 83 Python
cases passed. The single symlink privilege test was excluded locally because
Windows denies symlink creation (WinError 1314); CI runs the full 33-group /
84-case suite. Repository audit: 412 files, 201 upstream identities and one
reviewed local edit verified. Source notices: 229 files, five license texts.
These source checks do not establish legal clearance or binary redistribution.
Both selected generated-header checks passed. Final PR CI remains pending.
