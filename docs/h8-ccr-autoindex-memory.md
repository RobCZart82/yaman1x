# H8H autoindex CCR word memory

Recorded:2026-10-29. Merged #104 as `92e52f45307dc1adad37347febd407d0d2106182`;tested head `570d7b11d5dde819ac12211671f39fd47163f81b`.

## Scope and provenance

Adds LDC.W @ERn+,CCR and STC.W CCR,@-ERn with four unchanged full/partial
methods (`ldc_w_r32ph_ccr`,`stc_w_ccr_pr32h`) from pinned MAME
706dbb73281320c54d7c2f16262c1c3786f2be01. Class qualifiers removed only.
536 upstream-derived methods (532 unchanged/four earlier arithmetic adaptations);
255 selected/9 missing/135 excluded encoding rows,not CPU completeness.
No new dependency,Yamaha input or instruction-body adaptation.

The existing0140 prefix accepts6d00/ff8f and6d80/ff8f only. All eight pointers
are permitted;other mode/register bits remain rejected by the65536-word oracle.
The original internal cycle and prefetch order remain unchanged. LDC saves the
old ER address,increments ER by two before the data read,loads CCR from the high
word byte,updates IRQ filtering and retains no-IRQ completion. STC subtracts two
from ER before its write,duplicates CCR into both bytes and uses normal completion.
ER arithmetic wraps at32 bits;word addresses align down independently from the
host's24-bit physical map. Pointer changes survive host faults;mutating writes
have no rollback. Poisoned instances cannot resume. These are original-source
contracts,not physical timing or firmware acceptance.

## Independent checks

`tests/h8_ccr_autoindex_check.cpp` checks:

- 524288 loads:all65536 word values for each of eight pointers,distinct lanes,
  unrelated initial CCR and complete final register image.
- 24576 direction/pointer/address-boundary/all256 CCR cases,including ER7,
  odd addresses and32-bit increment/decrement wrap. Exact memory footprint,
  word lanes,raw full bus address,type,count and transferred value.
- A host callback observes the complete changed register image and previous CCR
  before the read/write,independently testing pointer/data side-effect ordering.
- 201 every-cycle splits with full snapshots,RAM and ordered bus trace equality.
- 512 IRQ/NMI mask cases and322 interrupt split histories:updated filter,NMI
  bypass,incremented pointer,following NOP,stack PC/CCR and RTE restoration.
- 16 pointer/direction fault cases:preceding pointer update and previous CCR
  survive,write mutation is retained,poisoned CPU cannot resume.

## Original reference and validation

Unchanged original decoder,handlers and execute_run match200000 split slices:
683956 instruction reads,26251 byte reads,28955 byte writes,44485 word reads,
41257 word writes;zero differences. Address-bound guards require both new forms.
A dedicated fixture stores with predecrement,changes the low byte,and loads with
postincrement to restore the original pointer. An ignored candidate-only mutation
increments by four rather than two;the unchanged reference detects mismatch at
seed2/slice3 (expected exit1). Tracked code remains correct.

Local Release69/69 CTest and87 Python PASS;targeted ASan/UBSan PASS.
Negative-control result is recorded above and in the log/PR. Linux CI includes the new target under
UBSan. All five final-head CI jobs passed: [platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/40919264372), [MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40919264370), [MAME](https://github.com/RobCZart82/yaman1x/actions/runs/40919264281).
Automated review completed on the final head without findings or unresolved threads. Remaining CCR displacement
forms,other missing H8H instructions,full machine/MMIO integration,physical timing,
firmware/MIDI/audio and plugin/REAPER acceptance remain open.
