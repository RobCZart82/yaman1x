# H8H register byte bit operations

Date: 2026-10-28. Baseline: merged PR #53 (`806ddf86a6373731f7b138c985a55ed41c583d6a`).
M0 increment only; no full CPU, firmware boot or audio acceptance.

18 original instruction groups add 36 full/partial methods (186 -> 222):
register-index and immediate BSET/BNOT/BCLR/BTST, immediate BST/BIST,
BOR/BIOR/BXOR/BIXOR/BAND/BIAND/BLD/BILD. Bodies retain their notices and
are unchanged apart from class qualifiers. No host helper is added.
Source and generator locks are unchanged.

60sd..63sd take the bit index from the source byte's low three bits.
70bd..73bd use an immediate index 0..7; bit 7 in these encodings is reserved.
67bd/67(8+b)d store C/inverted C; 74bd..77bd combine/load a selected bit
into C, with bit 7 selecting inversion. All sixteen byte lanes are supported.

Modify/store operations preserve the complete CCR. BTST changes only Z
(the inverse of the tested bit); carry boolean/load forms change only C.
Source/destination aliases read the old index before writing. Untouched
register lanes and upper ER words remain unchanged. Original prefetch,
full/partial and cycle-debt behavior is retained. Unsupported internal states
remain guarded before ordinary instruction decoding.

Profile correction: two-bit shift/rotate forms are marked s20 in the locked
h8.lst (for example 1040/fff0 SHLL.B two). The locked generator maps s20 to
h8s2000_device, while this project targets H8H/H83002. The generated H8H
decoder does not accept these forms. They stay explicitly unsupported, rather
than being treated as missing H83002 instructions. Earlier next-step notes
mentioning them are corrected by this evidence; the upstream lock is unchanged.

Validation:

- 7340032 immediate cases: 14 operations, all 256 operands, eight bit positions,
  all 256 initial CCR states; independent boolean/bit-array oracle.
- 524288 register-index cases cover all 256 source bytes, all operand bytes,
  four operations and both incoming C states, including ignored upper index bits.
- 262144 register source/destination/CCR alias combinations compare full registers.
- Exactly-once bit toggle and prefetch checks through suspension and debt repayment.
- 1000 mixed whole/partitioned histories agree in state, memory and ordered trace.
- The existing exhaustive 65536 first-word test now includes these forms and
  still rejects reserved immediate encodings and H8S shift neighbors.
- Original H8H decoder/handlers/execute_run: 200000 slices, 771922 instruction
  reads, 15953 byte reads, 15956 byte writes, 47774 word reads, 47795 word writes;
  zero differences. Five short reference variants retain earlier instruction groups.
- Negative control: changing immediate BNOT toggle to bit-set in an ignored
  candidate copy is detected at seed 3, slice 51 (expected exit 1).

The fixture disables unsupported IRQ/transfer/debug services. It is not the
full MAME CPU or a firmware/hardware run. Research sources/binaries stay local
and ignored; default checks need no archive or ROM. Source notice checks do
not establish legal clearance or binary redistribution approval.

Next: memory bit forms, stack/control flow, IRQ entry and CPU/SCI scheduling.
Firmware MIDI, first offline sound, VST3 and REAPER acceptance remain open;
see [first-sound plan](FIRST_REAPER_SOUND_PLAN.md).

Preparation validation (Windows): 35 runnable native CTest groups and 83 Python
cases PASS. One symlink-privilege Python case excluded locally (WinError 1314);
full CI runs 36 groups / 84 cases. Source audit: 418 files, 201 upstream
identities, one reviewed vendor edit; notices: 229 files / five license texts.
Both selected generated-header checks PASS. Final PR CI pending.
