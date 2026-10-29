# H8H absolute CCR word memory

Recorded:2026-10-29. Merged #102 as `2dab39c1598533fc0a84da9017b3f2975854ff96`;tested head `3c0f7947efc5cc170877fa99479f181d527bd195`.

## Scope and provenance

Adds four pinned MAME H8H groups: `ldc_w_abs16_ccr`, `ldc_w_abs32_ccr`,
`stc_w_ccr_abs16`, `stc_w_ccr_abs32`. Eight full/partial methods are unchanged
except class qualifiers, from commit706dbb73281320c54d7c2f16262c1c3786f2be01.
532 upstream-derived methods (528 unchanged/four prior arithmetic adaptations);
253 selected/11 missing/135 excluded encoding rows, not CPU completeness.
Source/output identities remain locked; no new dependency or Yamaha input.

Existing0140 prefix enters0x70000 continuation. Only exact6b00,6b20,6b80,6ba0
extensions are added; neighboring register/mode bits and EXR forms remain rejected.
The existing all65536-extension acceptance oracle now includes these four words.
Abs16 sign-extends to32 bits; abs32 retains all32 bits. The host word bus aligns
the address down; synthetic physical mapping masks to24 bits independently.
LDC replaces CCR with the high word byte and updates IRQ filtering, retaining
original no-IRQ completion. STC duplicates CCR into both word bytes, leaves
registers/CCR intact and uses normal completion. These are pinned source
contracts, not physical timing or firmware measurements.

## Independent evidence

`tests/h8_ccr_absolute_check.cpp` checks:

- 131072 load cases:all65536 distinct high/low word combinations per address width,
  unrelated initial CCR, complete register-image preservation.
- 131072 absolute16 address cases:all65536 encoded addresses,both directions;
  independent sign expansion and exact aligned full bus address.
- 524288 absolute32 half-address sweeps:all low halves at three high halves,
  then all high halves at fixed low half,both directions; not a2^32 cross product.
- 9216 width/direction/all256 CCR/boundary combinations. Exact memory footprint,
  word lanes, transaction type/address/count/value and unchanged neighbors.
- 241 every-cycle partitions with complete snapshots,RAM and ordered bus traces.
- 1024 IRQ/NMI/loaded-mask combinations and644 every-cycle IRQ/NMI histories.
  Pending interrupt raised on load; I filtering,NMI bypass,following NOP completion,
  exact stack PC/CCR and RTE restoration checked at both instruction widths.
- Four load/store bus faults:failed read preserves CCR,mutating write remains
  committed,CPU becomes poisoned and cannot resume.

## Original reference and negative control

Original decoder,unchanged handlers and execute_run agree over200000 slices:
681387 instruction reads,26873 byte reads,26434 byte writes,48650 word reads,
42190 word writes;zero differences. Address-bound guards require all four new
forms. The fixture overwrites low data bytes before loading,so both word lanes
are distinguishable. A candidate-only ignored source copy selecting the low byte
in all four LDC bodies mismatches the unchanged original reference at seed3/slice5
(expected exit1);tracked candidate remains correct.

## Validation and limits

Local Release68/68 CTest and87 Python PASS;targeted ASan/UBSan PASS.
Negative-control location is recorded above and in the development log/PR. Linux CI includes this target
under UBSan. All five final-head CI jobs passed: [platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/40912848068), [MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40912848027), [MAME](https://github.com/RobCZart82/yaman1x/actions/runs/40912848153).
Automated review completed on the final head without findings or unresolved threads. Remaining CCR postincrement/predecrement and
displacement forms,other missing instructions,complete AN1x machine/MMIO,
hardware timing,firmware/MIDI/audio and REAPER acceptance remain open.
