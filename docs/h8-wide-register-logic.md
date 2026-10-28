# H8H word/long register logic

Date: 2026-10-28. Baseline: merged PR #51 (`ba2c91d8a156972bcf3c7b9adab98e36f7279b0c`).
This is an M0 increment, not a complete CPU or firmware/audio acceptance.

The selected original OR/XOR/AND word and long register methods and the 01f0
prefix raise the retained generated selection from 124 to 138 full/partial
methods. Generator and source locks are unchanged; qualifiers alone are removed.

Word forms use 64sd/65sd/66sd with all sixteen word lanes. Long forms use
01f0 followed by 64sd/65sd/66sd, with source/destination ER0..ER7 only.
The original prefix fetches IR[1] and dispatches through 0x110000|extension.
A separate guarded state branch accepts exactly the three ff88-masked forms;
other extensions throw and poison the runner before register mutation.
The low extension word cannot be reinterpreted as an ordinary instruction.

Logic replaces N/Z, clears V and preserves the remaining CCR bits, including
H/C and the I bit stored by LDC. IRQ acceptance is still unsupported.
Register aliases read operands before writing; unrelated lanes remain unchanged.
The original full/partial bodies preserve instruction fetch charging and debt.
No new host helper, Yamaha data, DSP/EG change or upstream revision is introduced.

Validation:

- 196608 word operations cover every first operand for OR/XOR/AND with independent
  native logic and flag expectations.
- 245760 word/long register pairs, aliases and CCR seeds cover every register
  combination and all 256 initial CCR values; the complete register image is checked.
- All 65536 01f0 extensions: exactly 192 are accepted, all others fail explicitly.
- Prefix fetch suspension, debt repayment and completed logic prefetch checks;
  1000 whole/partitioned histories agree in CPU state, memory and ordered bus trace.
- Original H8H decoder, selected handlers and execute_run comparison: 200000 slices,
  767293 instruction reads, 16534 byte reads, 16534 byte writes, 49511 word reads,
  49528 word writes; zero differences. Three short looping variants cover old
  arithmetic, carry/immediate logic and the new register logic without rel8 overflow.
- Negative control: an ignored candidate copy changes both long XOR bodies to OR.
  The unchanged original fixture detects it at seed 1, slice 8 (expected exit 1).
- Local Windows: 33 runnable native CTest groups PASS. Full suite is 34 groups /
  84 Python cases; final-head CI remains pending at preparation time.

The original fixture disables unsupported IRQ/transfer/debug services and is
not the full MAME device or a hardware/firmware run. Reference source and binaries
remain local/ignored. Source notices remain retained; source coverage alone is
not legal clearance or binary redistribution approval.

Next: shift/rotate, bit operations, stack/control flow, IRQ entry and explicit
CPU/SCI scheduling. First firmware MIDI, offline sound, VST3 and REAPER acceptance
remain open; see [the first-sound plan](FIRST_REAPER_SOUND_PLAN.md).

Preparation checks: 83 runnable Python cases PASS; one Windows symlink privilege
case explicitly excluded (WinError 1314). Source audit: 414 files, 201 upstream
identities and one reviewed vendor edit verified. Notice coverage: 229 files /
five license texts. Both generated selections match locked outputs.
