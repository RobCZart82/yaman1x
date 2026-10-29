# H8H EEPMOV byte block copies

Recorded:2026-10-29. Merged #112 as `b14cd16e8d4d149cdb622371ac29928904bbc295`;tested head `41c5c34be49913dea5dc025f589cfde068a27736`.

## Scope and source

EEPMOV.B and EEPMOV.W use eight unchanged pinned MAME full/partial methods:
two base prefix continuations plus two advanced-H8H copy bodies. Pinned commit
706dbb73281320c54d7c2f16262c1c3786f2be01;class qualifiers removed only.
Select H8H EEPMOV.B,not the base-H8 implementation with16-bit address registers.
560 upstream-derived methods (556 unchanged/four prior arithmetic adaptations),
263 selected/1 missing/135 excluded encoding rows,not complete CPU acceptance.
No dependency,Yamaha data or generated-body adaptation introduced.

Exact first words7b5c/7bd4 enter distinct internal0x170000/0x180000 states;
only598f is accepted as the second word. Both variants copy individual bytes
from ER5 to ER6. B counts R4L only,preserving R4H;W counts all R4. ER4's upper
half remains untouched. Pointers increment and count decrements only after a
successful read/write pair,with32-bit wrap. CCR remains unchanged. Overlapping
ranges propagate bytes sequentially,not as a buffered memmove. Zero count causes
no data access. Original prefetch occurs after the block;full/partial read and
write suspension points remain unchanged.

Host failures poison the CPU. Read failure has no current-byte commit. A write
that mutates before throwing retains its mutation,while current count/pointers
still represent the previous completed pair. No rollback or retry is promised.
The host maps24 physical bits separately from full32-bit bus arguments.
These are source contracts,not EEPROM programming timing or AN1x hardware proof.

## Independent evidence

`tests/h8_eepmov_check.cpp` checks:

- 768 byte-count cases:all256 count values at three CCR patterns.
- Ten word-count boundaries including0,255,256,32767,32768,65534 and65535.
  W is still a byte copy;maximum count is exercised completely.
- 336 width/count/source-boundary/overlap combinations,including identical
  ranges,forward/backward overlap,24-bit aliases and32-bit pointer wrap.
- Independent sequential memory reconstruction,exact alternating read/write
  type/value/full-address history,complete register image and CCR preservation.
- 362 every-cycle partitions with complete snapshots,RAM and ordered traces;
  suspension between read and write does not repeat either completed access.
- All65536 second words per width (131072 guards),plus updated first-word oracle.
- 12 width/read-or-write/first-second-third-byte faults. Exact committed count,
  pointers,previous copies,faulting write mutation,no extra access and no-resume.

## Original reference and negative control

Original decoder,unchanged handlers and execute_run match200000 slices:
689873 instruction reads,24390 byte reads,38002 byte writes,39726 word reads,
39771 word writes;zero differences. Address-bound reachability guards require
both new copy forms. The fixture copies across32-bit pointer wrap.
An ignored candidate-only mutation advancing ER5 by two instead of one mismatches
at seed15/slice4 (expected exit1);tracked code unchanged.

Local Release73/73 CTest and87 Python PASS;targeted ASan/UBSan PASS.
Linux CI includes this target under UBSan. [Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371785), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371726), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371748).
All five jobs passed;merged. No recorded automated review result at inspection;
no successful automated review claimed.
SLEEP remains unimplemented. Mid-block interrupt injection is not separately
hardware validated;original completion/IRQ policy is retained. Full machine/MMIO,
physical timing,firmware/MIDI/audio and plugin/REAPER acceptance remain open.
