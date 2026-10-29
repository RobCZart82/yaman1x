# H8H long displacement32 memory

Recorded: 2026-10-29. Merged #100 as `1de8971b8fdd7694a8c3051ae86611a9ff6cd3cd`; final head `ba6db1c61a3ba9d696f8affee1410e5969c0aa9d`.

## Scope and provenance

Add MOV.L @(disp32,ERn),ERm and MOV.L ERm,@(disp32,ERn) using the original
`dispatch_01007800`, `mov_l_r32d32hh_r32l` and `mov_l_r32l_r32d32hh`
full/partial bodies from pinned MAME `706dbb73281320c54d7c2f16262c1c3786f2be01`.
Six unchanged methods bring the selection to524 (520 unchanged,four earlier
reviewed unsigned arithmetic adaptations). Inventory:249 selected/15 missing/
135 excluded encoding rows; these are not CPU completeness percentages.
Generated/source identities remain locked; no new dependency or Yamaha payload.

Original0100 then7800 prefixes enter internal0x30000 continuation. The first
extension uses maskff0f, including the original pointer bit7 aliases; only third
words6b20/fff8 and6ba0/fff8 dispatch. ER8..15 data-register neighbors and every
other third word fail closed. The five-word instruction requires CPU snapshot,
private IR and original-reference fixture storage to expand from four to five.
No instruction-body adaptation is introduced.

The original unsigned base-plus-offset address wraps at32 bits. Two word accesses
transfer high then low, each aligned down independently; the second address can
wrap to zero. Full32-bit bus arguments remain separate from the synthetic host's
24-bit physical mapping. Loads update ER only after both words; stores retain
already-mutated memory if the host faults. MOV updates NZV and preserves the
other CCR bits. These are source contracts, not hardware timing measurements.

## Independent checks

`tests/h8_long_displacement32_check.cpp` covers:

- 524288 half-offset sweeps, separately sweeping all low halves at three high
  halves and all high halves at a fixed low half; not a2^32 cross product.
- 55296 direction/pointer/data-alias/address/offset/value/CCR boundary cases.
  Complete register image, exact full bus addresses, access count/order/values,
  wrapped memory bytes and unrelated CCR bits are independently checked.
- 241 every-cycle splits with complete snapshot, RAM and ordered trace equality.
- All65536 third words and the existing65536 first-extension acceptance oracle;
  all16 pointer-nibble aliases with exact paired word addresses.
- First/second load/store faults, no subsequent access after failure, committed
  first-word store and prohibition on resuming a poisoned CPU.

## Original reference and negative control

Unchanged original decoder, selected original handlers and execute_run match
200000 split slices:687107 instruction reads,27631 byte reads,23267 byte writes,
46995 word reads,40534 word writes;zero differences. Address-bound reachability
requires both new forms. A dedicated fixture uses a nonzero high offset half.
An ignored candidate-source copy omitting that high half in all four new MOV
bodies is detected at seed4/slice4 (expected exit1); original reference unchanged.

## Validation and remaining limits

Release67/67 CTest and87 Python cases PASS. Targeted independent checks and
ASan/UBSan PASS. All five final-head CI jobs passed: [platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/40906424971), [MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40906424908), [MAME](https://github.com/RobCZart82/yaman1x/actions/runs/40906424743). Linux CI also
runs the new target under UBSan. Automated review completed on the final head without findings; no unresolved threads. Remaining CCR addressing, other missing H8H instructions,
complete machine/MMIO integration, physical timing, firmware/MIDI/audio and
plugin/REAPER acceptance remain open. M0 partial;M1 unaccepted.
