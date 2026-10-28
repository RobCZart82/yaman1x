# H8H stack and control flow

Date: 2026-10-28. Baseline: merged PR #56 (`a3c2eb8ba25fc40eaa5057cc388260e4fc58b9df`).
Normal fixed-profile stack/control instruction families implemented; M0 remains
partial. This is not a complete CPU, IRQ entry, firmware boot or audio acceptance.

32 original instruction groups add 64 unchanged full/partial methods (302 -> 366):

- Byte/word/long predecrement stores and postincrement loads. Word/long forms
  provide PUSH/POP through ER7; all general address-register aliases are retained.
- JMP and JSR through ER, absolute24 and absolute8 memory-indirect vector.
- BSR rel8 and rel16; all sixteen rel16 branch conditions.
- RTS and RTE advanced-mode frame restoration (no EXR stack field).

8-bit/16-bit branch decoding and ordinary instruction decoding remain behind
internal-state guards. The existing 0100 long-memory prefix now accepts both
indirect moves and pre/post moves with valid ER lane encodings. Reserved neighbors
are explicitly rejected. H8S LDM/STM and two-bit shifts are outside this H8H profile.
TRAPA/IRQ entry, interrupt acceptance, standby and bus retries remain unsupported.

The generated bodies only lose class qualifiers; source/generator locks and BSD
notices remain unchanged. Host adaptations are: s16 type, pinned no-EXR H8H
internal(cycles) charging cycles+1, exr_in_stack=false and fixed no-IRQ/no-transfer
prefetch_done_notrace. The original reference uses pinned internal and prefetch
methods, rather than the candidate's reduced helpers.

Advanced calls predecrement ER7 by four and write a four-byte return PC, high word
then low word. RTS reads the same frame and increments ER7 by four. JSR ER7 reads
the target before changing the stack pointer. RTE restores CCR plus 24-bit PC from
a supplied four-byte frame and updates the fixed no-op IRQ filter hook. This does
not demonstrate real interrupt delivery or interrupt nesting.

Predecrement source values are captured before pointer mutation; postincrement
loads update the pointer before destination write, including aliases. Original
prefetch/data access ordering and completed side effects survive suspension and
debt repayment. The existing bus aligns word/instruction reads down and applies
the 24-bit bus address contract; no invented odd-address exception is introduced.
32-bit pointers wrap with unsigned arithmetic. Host failures poison execution
while preserving already committed SP and data mutations.

Validation:

- All sixteen rel16 conditions and every even signed displacement with varying
  CCR, excluding only synthetic overlaps with the reset vector/test instructions.
- Every even signed BSR8/BSR16 displacement, excluding fixture overlaps; target,
  exact return address, four-byte frame and SP restoration checked independently.
- 20480 pre/post width/pointer/destination/seed/store-load combinations, including
  aliases, odd word addresses, zero-pointer wrap and full ER upper words.
- Every call/JMP form across 256 CCR values; ER7-target alias, full target address
  patterns and the low-page advanced indirect-vector layout.
- RTE all CCR values; 4608 additional RTS/RTE frame cases across stack wrap,
  odd/aligned pointers and target address boundaries. No EXR frame is assumed.
- 201 nested-call/explicit-long-push-pop split points and 388 additional register/
  vector/BSR/RTE splits compare full state, data and ordered bus traces.
- Mid-call first/second word-write failures preserve committed SP/data and poison
  the owner. Exhaustive first-word and long-prefix tests include the new forms.
- Original H8H decoder/handlers/flag/bus/runner methods: 200000 slices, 668426
  instruction reads, 46258 byte reads, 25800 byte writes, 67331 word reads,
  41382 word writes; zero differences. Twelve short fixture variants exercise old
  and new groups, including RTE supplied frames and all rel16 conditions.
- Negative control: RTS increments SP by two instead of four in both candidate
  bodies in an ignored copy. The unchanged reference detects seed 6, slice 5.

Reference fixture services for IRQ/transfer/debug remain disabled. Reference
sources/binaries stay local and ignored; default tests need no archive or ROM.
No Yamaha data or DSP/EG changes. Source notices alone do not establish legal
clearance or binary redistribution approval.

Remaining work: actual IRQ/TRAPA entry and IRQ filter integration, remaining CPU
instructions/addressing, common CPU/peripheral scheduling, firmware MIDI, offline
sound, VST3 and REAPER acceptance. See [first-sound plan](FIRST_REAPER_SOUND_PLAN.md).

Preparation validation (Windows): 38 runnable native CTest groups and 83 Python
cases PASS. One symlink-privilege Python case explicitly excluded locally
(WinError 1314); full CI runs 39 groups / 84 cases. Source audit: 425 files,
201 upstream identities and one reviewed vendor edit; notices: 229 files / five
texts. Both generated selections match locks. Final-head PR CI pending.
