# H8H displacement32 CCR word memory

Recorded:2026-10-29. Merged #108 as `3c7005a3a26da4149668f6ab9f4695e1672bdc6e`;tested head `ffef22c9d5794ec78028a0f2d07cb9659ff9d5bb`.

## Scope and source

Adds LDC.W @(disp32,ERn),CCR and STC.W CCR,@(disp32,ERn),with two distinct
original prefix continuations. Eight unchanged full/partial methods from pinned
MAME706dbb73281320c54d7c2f16262c1c3786f2be01;class qualifiers removed only.
548 upstream-derived methods (544 unchanged/four earlier arithmetic adaptations),
259 selected/5 missing/135 excluded encoding rows,not complete CPU coverage.
No new dependency,Yamaha data or instruction-body adaptation.

0140 then7800/ff8f enters internal0x80000;only exact6b20 loads.
0140 then7880/ff8f enters0x90000;only exact6ba0 stores. Eight pointer registers,
no extra register bits or cross-direction third words accepted. Existing five-word
IR storage is sufficient. Base-plus-offset wraps32 bits;word address alignment
and host24-bit physical map remain separate. LDC selects the high word byte,
updates IRQ filtering and retains original no-IRQ completion;STC duplicates CCR
into both bytes and uses normal completion. ER is unchanged. These are pinned
source contracts,not physical timing or firmware evidence.

## Independent checks

`tests/h8_ccr_displacement32_check.cpp` verifies:

- 524288 word loads:all65536 values per pointer,distinct byte lanes,unrelated
  initial CCR and complete register-image preservation.
- 524288 half-offset sweeps:all low halves at three high halves,then all high
  halves at fixed low half,both directions;not a2^32 cross product.
- 147456 pointer/base/offset/CCR boundary cases;independent widened unsigned sum,
  exact aligned full bus address,type/count/value and RAM footprint.
- 261 every-cycle full/partial histories,including five-word IR snapshots.
- All65536 third words in both directions (131072 cases),cross-direction and
  neighboring-register rejection. Existing65536 first-extension oracle updated
  with valid downstream words for the newly supported prefix states.
- 512 IRQ/NMI masks and322 interrupt split histories;filter updates,NMI bypass,
  following NOP,exact stack PC/CCR,unchanged pointer and RTE restoration.
- 16 bus-fault cases;previous CCR/pointer preserved,mutating stores retained,
  poisoned owner cannot resume.

## Original reference and negative control

Unchanged original decoder,handlers and execute_run match200000 split slices:
704020 instruction reads,15533 byte reads,29144 byte writes,41509 word reads,
41556 word writes;zero differences. Address-bound guards require both new forms.
Candidate-only ignored source copy omitting the upper offset half in all four
LDC/STC bodies mismatches at seed0/slice1 (expected exit1);tracked code unchanged.

The initial existing first-extension oracle supplied zero as the third word,
so it incorrectly rejected16 newly valid prefix entries. Supplying their required
third word fixes the fixture;the separate exhaustive third-word tests retain
invalid-extension rejection. No engine workaround was introduced.

Local Release71/71 CTest and87 Python PASS;targeted ASan/UBSan PASS. Linux CI includes the new target under UBSan. [Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469453), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469399), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469419).
All five jobs passed;merged. No recorded automated review result at inspection;
no successful automated review claimed.
Remaining SLEEP,MOVFPE/MOVTPE,EEPMOV and complete machine/MMIO integration,
physical timing,firmware/MIDI/audio and plugin/REAPER acceptance remain open.
