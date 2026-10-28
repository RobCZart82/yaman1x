# H8H single-bit shifts and rotates

Date: 2026-10-28. Baseline: merged PR #52 (`88e88bd8551f78bc312230d459ea3c92d7b66f01`).
M0 increment only; no complete CPU, firmware boot or audio acceptance.

SHLL, SHAL, SHLR, SHAR, ROTXL, ROTL, ROTXR and ROTR now execute at 8/16/32 bits.
The 24 original generated instruction groups add 48 full/partial methods,
raising the selected count from 138 to 186. Both generation/source locks stay
unchanged; selected bodies only lose class qualifiers and retain BSD notices.

Encodings: 10/11/12/13 high bytes select left/right shift or left/right rotate;
low selectors 00/10/30 choose byte/word/long and bit 7 selects arithmetic shift
or plain rotate. Byte/word allow all sixteen lanes; long accepts ER0..ER7.
The reduced decoder retains the internal-state guard before ordinary opcodes.
Two-bit, H8S and other unsupported forms still fail explicitly.

A width-generic unsigned helper adapts the pinned 24 do_sh*/do_rot* helpers.
N/Z/C reflect the result and outgoing bit; V is clear except SHAL signed
range overflow. H and unrelated CCR bits are preserved; ROTX consumes old C.
Unsigned 64-bit intermediate arithmetic avoids signed left shifts and truncates
at the selected width. No change to IRQ acceptance, DSP/EG or Yamaha data.

Validation:

- 524288 byte cases: every operand and all 256 initial CCR values, eight operations.
- 1048576 word cases: every operand, both incoming C values, eight operations.
- 65536 deterministic 32-bit operand/carry cases and 24576 long boundary/CCR cases.
- 81920 register-lane/CCR cases compare the complete register image.
- Independent bit-array movement oracle; SHAL overflow checked with signed range.
- Suspended prefetch and debt repayment do not repeat the shift or extension fetch;
  the legitimate next-instruction fetch is distinguished from a repeated old fetch.
- 1000 whole/partitioned histories match state, memory and ordered bus traces;
  the existing exhaustive first-word decoder test includes new forms and rejects neighbors.
- Pinned original decoder, handlers, flag helpers and execute_run: 200000 slices,
  773235 instruction reads, 15788 byte reads, 15791 byte writes, 47286 word reads,
  47300 word writes; zero differences. Four short variants retain earlier coverage.
- Negative control: ignoring carry input in ROTXR in an ignored candidate copy
  is detected at seed 2, slice 8 (expected exit 1). The initial mixed fixture
  did not detect this mutation; explicit C=0/C=1 ROTXR cases were added before
  recording the final evidence. The unmodified strengthened fixture still agrees.

The original-code fixture disables unsupported interrupt/transfer/debug services;
it is not the complete MAME CPU or a hardware/firmware run. Original research
sources/binaries remain ignored and local. Default tests need no archive or ROM.
Source notice coverage does not establish legal or binary redistribution clearance.

Next: two-bit forms and bit operations, stack/control flow, IRQ entry and common
CPU/SCI scheduling. Firmware MIDI, offline sound, VST3 and REAPER acceptance remain
open; see [first-sound plan](FIRST_REAPER_SOUND_PLAN.md).

Preparation validation (Windows): 34 runnable native CTest groups and 83 Python
cases PASS. One symlink-privilege Python case is explicitly excluded locally
(WinError 1314); full CI runs 35 groups / 84 cases. Source audit: 416 files,
201 upstream identities and one reviewed vendor edit; notices: 229 files / five
license texts. Both selected generated-header checks PASS. Final PR CI pending.

Profile correction (next increment): the pinned two-bit forms carry the s20
(H8S/2000) marker and are absent in the H8H decoder. They are not a remaining
H83002 task; see [register-bit extension](h8-register-bit-operations.md).
