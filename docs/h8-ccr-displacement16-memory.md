# H8H signed16 displacement CCR word memory

Recorded:2026-10-29. Merged #106 as `aee769ef224e2686ccd4a98bbd9d3851f8cc6399`;tested head `e3af0b4453d19194da8d32b24e6d409d4ab8809f`.

## Scope and provenance

Adds LDC.W @(disp16,ERn),CCR and STC.W CCR,@(disp16,ERn) using four unchanged
full/partial methods (`ldc_w_r32d16h_ccr`,`stc_w_ccr_r32d16h`) from pinned
MAME706dbb73281320c54d7c2f16262c1c3786f2be01. Class qualifiers removed only.
540 upstream-derived methods (536 unchanged/four prior arithmetic adaptations);
257 selected/7 missing/135 excluded encoding rows,not complete CPU coverage.
No new dependency,Yamaha input or generated-body adaptation.

Existing0140 continuation accepts6f00/ff8f and6f80/ff8f;all eight pointers enabled,
other mode/register bits rejected by the existing65536-extension oracle. Signed16
offset is added to the full32-bit ER,wrapping at32 bits. The word bus aligns down;
24-bit physical host mapping remains separate. ER is unchanged. LDC replaces
CCR from the high byte,updates IRQ filtering and retains original no-IRQ completion;
STC duplicates CCR into both word bytes and uses normal completion.
These are original-source contracts,not physical timing or firmware measurements.

## Independent checks

`tests/h8_ccr_displacement16_check.cpp` checks:

- 524288 word loads:all65536 word values for each pointer,distinct lanes,
  unrelated initial CCR and complete register-image preservation.
- 131072 displacement cases:all65536 encodings,both directions;expected signed
  offset calculated independently by subtraction rather than signed16 casting.
- 147456 direction/pointer/base-boundary/offset-boundary/all256 CCR cases.
  Exact RAM footprint,byte lanes,transaction type/count/value and aligned full
  bus address;negative offsets and32-bit overflow/underflow included.
- 221 every-cycle splits with snapshots,RAM and ordered bus histories.
- 512 IRQ/NMI masks and322 every-cycle interrupt histories;filter updates,NMI
  bypass,unchanged pointer,following NOP,exact stack PC/CCR and RTE restoration.
- 16 pointer/direction bus faults;previous CCR/pointer preserved,mutating stores
  remain committed,poisoned CPU cannot resume.

## Original reference and validation

Unchanged original decoder,handlers and execute_run match200000 slices:
692668 instruction reads,20993 byte reads,29422 byte writes,42522 word reads,
42566 word writes;zero differences. Address-bound guards require both new forms.
The dedicated fixture uses offsetffff and distinguishable high/low data bytes.
An ignored candidate-only copy replaces signed offset expansion with unsigned;
the unchanged original reference detects mismatch seed1/slice2 (expected exit1).
Tracked code remains correct.

Local Release70/70 CTest and87 Python PASS;targeted ASan/UBSan PASS.
Negative-control result is recorded above and in the log/PR. Linux CI includes this target under
UBSan. [Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577461), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577203), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577221).
All five jobs passed;merged. No recorded automated review result at inspection;
no successful automated review claimed. Remaining CCR displacement32,
other missing H8H instructions,full AN1x machine/MMIO integration,physical timing,
firmware/MIDI/audio and plugin/REAPER acceptance remain open.
