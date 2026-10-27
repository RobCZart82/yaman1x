# H8H IRQ register and pin adapter

Implementation: [PR #27](https://github.com/RobCZart82/yaman1x/pull/27).
Date: 2026-10-27. Local MSVC Release tests PASS; CPU/SCI integration NOT RUN.

`H8hIrqController` wraps the internal selector with the pinned MAME H8H
external IRQ and register behavior: eight pins/vectors 12..19, IER enable,
ISR latched state, ISCR level/assertion-edge selection, three ICR bytes and
NMI edge selection. Source hashes and BSD-3-Clause authorship are retained
in `research/h8-internal-irqs-provenance.json` and the header. Original
terms: `licenses/MAME-BSD-3-Clause.txt`.

The adapter uses an instance-owned pull interface. `current()` is the last
selection that the upstream controller would publish to its CPU; it is not
an unconditional recomputation from current registers. The future owner must
transfer that value to CPU state at the appropriate machine scheduling point.
This does not implement CPU interrupt entry, vector fetch, stack handling,
DMA, device scheduling or memory-map wiring.

## Preserved behavior

- A disabled IRQ can latch ISR and become selectable when IER is enabled.
  H8H level requests remain latched after deassertion until acknowledged or
  explicitly cleared. Clearing ISR while the level remains asserted re-latches it.
- Assertion-edge mode latches only on a new logical assertion. ISR clearing
  while the input stays asserted does not manufacture another edge.
- External and internal requests share the upstream priority rule and
  lower-vector tie break. Qualified NMI uses vector 7 and priority two.
- `nmi_input(asserted, machine_time_after_zero)` updates the remembered input
  even at time zero, but latches only the selected transition after time zero.
  The owner supplies this time predicate; the class does not invent a clock.
- ICR writes and some mode/reset operations do not immediately republish an
  IRQ in the pinned upstream. This adapter preserves those publication points.
  Reset clears register/pending state, keeps physical input levels and CPU
  filters, and retains the last publication. The CPU reset sequence must
  reapply filters before relying on that publication.
- Unsupported IRQ lines, vectors and ICR offsets are rejected before mutation.

## Unverified upstream ICR behavior

The pinned H8H `icr_w` uses `(old & byte_mask) | shifted_data`, with no
complement on the mask. It retains/ORs the addressed byte and clears the
other bytes. It also defers publication. The adaptation intentionally preserves
this expression; tests make it explicit. This is not hardware validation of
the rule. Do not silently replace it with a conventional byte assignment:
first check the H83002 register map, firmware usage and primary hardware
evidence, then propose a separately justified change if appropriate.

## Validation

Native tests cover all eight pins in level and edge modes; disabled latching,
IER enabling, ISR clear/re-latch, acknowledge, held-input suppression,
internal/external arbitration, NMI time-zero and edge cases, instance isolation,
ICR masks/deferred publication, reset publication and invalid arguments.

A separate local differential fixture compiles the verbatim pinned upstream
register, input, reset, priority and update methods with a minimal device/CPU
host. Deterministic xorshift32 seed `0x1a73c` drove 300000 mixed register,
filter, pin, NMI, DMA, acknowledge and reset operations. It compared all
register bytes and the CPU-facing vector/level/NMI publication after every
operation: **zero mismatches**, MSVC Release. The original method fixture stays
in ignored local work output and is not a full MAME-machine or hardware test.

The standard CTest suite exercises the adapted class. Final platform and full
research-build evidence belongs to the implementation PR; firmware execution
with the new adapter remains NOT RUN because it is not wired into that machine.

## New inbox note assessed

The 2026-10-27 incoming `AN1x_DUMP-hiánypótlás.txt` suggests PKS HLE and a
dummy missing ROM. It is a research suggestion, not protocol evidence.
The current LCD bus and SysEx tools do not supply the SCI1 PKS handshake or
inject virtual controls into firmware RAM. An empty file named as the missing
ROM would not establish that protocol or validate boot. No assumed reply,
dummy-ROM success path or firmware patch is introduced from that note.
PKS HLE still requires evidence for response bytes, state and timing.
