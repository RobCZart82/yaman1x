# H8H peripheral byte moves

Recorded:2026-10-29. Merged #110 as `3d0ce825bb341402f40dab92c0f1ec2f409b5ee7`;tested head `903844cdb2a0466a93b2a61b056191afd0b37146`.

## Scope and timing boundary

MOVFPE abs16,Rn and MOVTPE Rn,abs16 use four unchanged full/partial methods
from pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01. Only class qualifiers
removed. Original bodies explicitly retain `TODO: timing`;this increment does
not implement or validate external E-clock/peripheral handshake timing.
The shared host byte bus supplies the transaction;no new peripheral address map,
handshake state,device model,dependency or Yamaha data is introduced.

552 upstream-derived methods (548 unchanged/four earlier arithmetic adaptations),
261 selected/3 missing/135 excluded encoding rows,not complete CPU coverage.
Normal-instruction full-state masks6a40/fffffff0 and6ac0/fffffff0 permit all16 byte registers;other mode bits stay closed.
Abs16 sign-extends to full32-bit host address;byte accesses are not aligned down.
The synthetic host separately maps24 physical bits. NZV derives from transferred
byte;unrelated CCR bits and register lanes are preserved. Original load flags and
register update occur after read;store flags update before write. Host faults
poison the instance;writes mutated before fault are retained.

## Independent checks

`tests/h8_peripheral_moves_check.cpp` checks:

- 24576 direction/all16 byte registers/all256 values/three initial CCR patterns.
- 131072 direction/all65536 addresses,including odd and sign-boundary addresses.
- 2560 direction/all256 CCR/value-boundary combinations.
- Exact complete register image,other byte lane,NZV/unrelated CCR,RAM footprint,
  transaction type/count/value and sign-extended full bus address.
- A callback observes old load registers/flags versus already-updated store flags.
- 201 every-cycle partitions with complete snapshots,RAM and ordered bus histories.
- 32 direction/register faults,previous load CCR versus updated store CCR,
  retained register/write data and prohibition on resuming poisoned instances.
- Ten unsupported neighboring modes remain rejected.

## Original reference and validation

Unchanged original decoder/handlers/execute_run match200000 slices:
700465 instruction reads,19094 byte reads,32706 byte writes,39726 word reads,
39771 word writes;zero differences. Address-bound guards require both new forms.
A dedicated source fixture uses address8001 and distinguishes the byte registers.
Candidate-only ignored mutation removing address sign expansion mismatches
the unchanged original reference at seed15/slice1 (expected exit1);tracked source remains correct.

Targeted native and ASan/UBSan checks PASS. Local Release72/72 CTest,87 Python and targeted ASan/UBSan PASS. Linux CI includes the target under UBSan.
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842276), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842076), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943841959).
All five jobs passed;merged. No recorded automated review result at inspection;
no successful automated review claimed. SLEEP and EEPMOV remain missing;
full machine/MMIO,physical timing,firmware/MIDI/audio and REAPER acceptance open.

The first full regression found nine failing groups because low-word-only masks
matched unsupported internal-prefix states. Restricting both guards to normal
instruction states fixes the dispatch;all existing exhaustive prefix tests remain.
A complete rebuild after this correction passed prior prefix guards;the first-word
acceptance oracle also needed its32 newly valid MOVFPE/MOVTPE encodings registered.
Final full regression after both corrections:72/72 PASS.
