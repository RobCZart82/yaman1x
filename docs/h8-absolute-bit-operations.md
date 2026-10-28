# H8H absolute-byte memory bit operations

Date: 2026-10-28. Baseline: merged PR #55 (`e53b6fe7e2906b472e898c57d59ccfe1b791fa96`).
M0 increment only; no full CPU, firmware boot or audio acceptance.

Original 7e/7f prefix groups and all 18 absolute-byte bit instruction groups
add 40 full/partial methods (262 -> 302). Bodies only lose class qualifiers;
source/generator locks and BSD notices are retained. No new host helper.

7eAA selects read/test extensions (0x1b0000 internal state); 7fAA selects
modify/store extensions (0x1c0000). AA is an unsigned byte. The original
address expression 0xffffff00|IR[0] is retained; the reviewed 24-bit host
maps it to 0xffff00|AA. This is the upper 256-byte page, not the zero page.
The synthetic host has no real H83002 MMIO map; hardware effects are unverified.

The extension families, reserved bits and CCR semantics match the preceding
[indirect forms](h8-memory-bit-operations.md). Invalid extensions fail before
any data access. Read-only forms do not write; modify forms retain original
data-read, next-instruction-prefetch, data-write order. Partial continuation
and cycle debt do not replay completed accesses. No CPU register is modified.

Validation:

- 7340032 operand/bit/CCR cases with the independent shared boolean oracle.
- 262144 short-address/source-lane cases cover all 256 short addresses, all
  sixteen index-byte lanes and register preservation for four register-index forms.
- All 131072 extension encodings across both prefixes; invalid forms have no
  data accesses. Existing exhaustive first-word decoding covers both full prefixes.
- 31 execution split points with exactly-once read/write, bus failure controls,
  and 1000 whole/partitioned histories matching state, data and ordered trace.
- Original H8H decoder/handlers/execute_run: 200000 slices, 713963 instruction
  reads, 76009 byte reads, 40236 byte writes, 34589 word reads, 34603 word writes;
  zero differences. Seventh short fixture variant exercises all new groups.

Reference fixture IRQ/transfer/debug services remain disabled; this is not the
full MAME CPU or a firmware/hardware run. Sources/binaries remain ignored and
local; default tests need no archive or ROM. No Yamaha data or DSP/EG changes.
Source notices alone do not establish legal or binary redistribution clearance.

Next: stack/control flow, IRQ entry and CPU/SCI scheduling. Firmware MIDI,
first offline sound, VST3 and REAPER acceptance remain open; see the
[first-sound plan](FIRST_REAPER_SOUND_PLAN.md).

Negative control: replacing the upper-page address with a zero-page address in
both immediate absolute BNOT bodies is detected at seed 5, slice 10 (exit 1).
Tracked handlers remain unchanged.

Preparation validation (Windows): 37 runnable CTest groups and 83 Python cases
PASS; one symlink-privilege case excluded locally (WinError 1314). Full CI runs
38 groups / 84 cases. Source audit: 423 files, 201 upstream identities, one
reviewed vendor edit; notices: 229 files / five texts. Both generated selections
match locked outputs. Final-head PR CI pending.
