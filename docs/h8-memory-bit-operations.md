# H8H register-indirect memory bit operations

Date: 2026-10-28. Baseline: merged PR #54 (`efdb6df59ba53e8497d313fb33c7b40dcd9d76c4`).
M0 increment only; no full CPU, firmware boot or audio acceptance.

Two original prefix groups and 18 indirect byte instruction groups add 40
full/partial methods (222 -> 262). The original generated bodies lose only class
qualifiers; source/generator locks and notices are retained. No new host helper.

7cP0 fetches a read-only/test extension (0x190000 internal state); 7dP0 fetches
a modifying/store extension (0x1a0000). P selects ER0..ER7. The effective byte
address uses the full ER value and the existing host applies its 24-bit bus mask.
Read extensions: register/immediate BTST, immediate BOR/BIOR/BXOR/BIXOR,
BAND/BIAND/BLD/BILD. Modify extensions: register/immediate BSET/BNOT/BCLR,
immediate BST/BIST. Low extension nibble must be zero; reserved bit 7 is rejected
where required. Read and write families cannot be interchanged. Invalid extensions
fail before any data read/write; unknown internal states remain guarded.

Flags match the register-bit forms: BTST changes only Z, carry tests change only C,
modify/store preserves CCR. No register is modified. The original read/modify/write
sequence is data read, next-instruction prefetch, then data write; read-only forms
never write. Completed accesses survive partial continuation and debt repayment
without replay. Bus failure poisons the owner; already committed writes persist.
These are pinned source semantics, not verified hardware MMIO behavior.

Validation:

- 7340032 immediate operand/bit/CCR cases with independent boolean oracle.
- 131072 register-index/address/CCR alias combinations cover every pointer and
  byte source lane; full register images, effective byte and flags compared.
- 131072 extensions (all 65536 for both prefixes), with invalid forms verified
  to have no data-bus effects. Exhaustive first-word test covers valid prefixes.
- All 31 split points around a complete read/modify/write program; exactly one
  read/write and the original read/prefetch/write ordering. Read/write failures
  poison execution and preserve committed mutations.
- 1000 whole/partitioned histories match CPU state, data and ordered bus traces.
- Original H8H decoder/handlers/execute_run: 200000 slices, 738561 instruction
  reads, 50422 byte reads, 29888 byte writes, 40255 word reads, 40274 word writes;
  zero differences. A sixth short fixture variant exercises all new groups.

The independent boolean oracle is shared with the preceding register-bit tests;
its semantics are unchanged. Original fixture services for IRQ/transfer/debug
remain disabled; this is not the full MAME device or a hardware/firmware run.
Reference sources/binaries remain local/ignored. Default tests need no archive/ROM.
No Yamaha data or DSP/EG behavior changes. Source notices alone do not establish
legal clearance or binary redistribution approval.

Next: absolute memory bit forms, stack/control flow, IRQ entry and CPU/SCI scheduling.
Firmware MIDI, offline sound, VST3 and REAPER acceptance remain open; see
[first-sound plan](FIRST_REAPER_SOUND_PLAN.md).

Negative control: both immediate memory BNOT bodies write to address+1 in an
ignored candidate copy; unchanged original fixture rejects it at seed 4, slice 7
(expected exit 1). Tracked handlers remain unchanged.

Preparation validation (Windows): 36 runnable CTest groups and 83 Python cases
PASS. One symlink-privilege test is explicitly excluded locally (WinError 1314);
full CI runs 37 groups / 84 cases. Source audit: 421 files, 201 upstream identities
and one reviewed vendor edit; notices: 229 files / five texts. Both generated
selections match locked outputs. Final-head PR CI pending.
