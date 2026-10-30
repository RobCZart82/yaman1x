# Bounded H83002 IRQ register MMIO

Recorded:2026-10-30. Candidate based on merged #135 (`c8cc8c0707a919d86b24d544fe818acd89227d42`),
engine baseline #134. Final-head CI/review and merge pending.

## Source facts and scope

Pinned MAME:`706dbb73281320c54d7c2f16262c1c3786f2be01`.
`src/devices/cpu/h8/h83002.cpp`,SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`,
lines140â€“143 maps ISCR FFFFF4,IER FFFFF5,ISR FFFFF6,ICR FFFFF8â€“FFFFF9.
These byte addresses now route to the existing unchanged `H8hIrqController` owned
by `An1xSciBus`. The CPU must bind that same owner via `bus.irq()`.
No new instruction/peripheral method body,dependency or Yamaha payload.

ISCR chooses level/assertion edge;IER publishes enabled pending lines;ISR write
clears zero bits then rechecks asserted levels. ICR offset0/1 maps in increasing
address order to the original byte offsets. Reads are side-effect-free. The
controller's third ICR byte remains a component capability,not an H83002 mapped register.
Original ICR write uses a noncomplemented mask and defers IRQ publication:writing
a different byte clears other bytes;repeated same-byte writes OR bits. This
unusual source behavior is preserved,not asserted to match physical silicon.
See [component contract and source notices](h8h-irq-controller.md).

Aligned big-endian words supported only atF4 (ISCR/IER) andF8 (ICR0/1),high lane
before low. All pairs preflight both addresses;odd or hole-containing words fail
before register changes. In particular ISR F6 plus unmappedF7 does not partially
clear pending interrupts.24-bit aliases normalize before dispatch. Instruction
fetch remains memory-only. Existing bus fail-stop/reentry rules remain;no full
reset API or new external-input API is introduced. IRQ pins still use the owner.
No physical pin timing or complete MAME address-space equivalence is claimed.

## SYSCR investigation and deliberately retained profile

The source maps SYSCR atFFFFF2. Original `syscr_w` stores all8 bits and immediately
calls `update_irq_filter`. Bit08 selects one-level I masking versus I/UI filtering;
original `irq_setup`/`trapa_setup` also set I or I|UI accordingly. The standalone CPU
currently hardcodes reset SYSCR0B,sets I at entry and uses I-only masking. Mapping
a freely writable SYSCR now would allow readable state that the CPU does not obey.
Therefore F2 remains unsupported (read and write fail closed);no fabricated readback
or silent ignored write is added. A later bounded CPU/host contract must update
filtering,IRQ/TRAPA entry and state/lifecycle together. Dynamic SYSCR/standby,
full board reset,timers/ADC/DMA,LCD/DSP/MEG remain separate work.

## Independent synthetic evidence

`tests/an1x_irq_mmio_check.cpp` checks:

- 65536 input-latch/IER combinations,independent lowest-enabled-vector oracle;
 disabled inputs latch ISR,and clearing an asserted level re-latches it.
- 65536 ISR-clear patterns after deassertion;zero bits clear without inventing requests.
- 65536 ISCR/IER words and65536 ICR words. Distinct byte registers/aliases and
 original noncomplemented ICR behavior are independently checked.
- 2048 line/ISCR combinations across eight external IRQs:clear,toggle,re-latch,
 acknowledgement semantics;deferred ICR publication and later IER publication.
- Odd/incomplete words and unknown SYSCR/neighbor bytes reject before mutation;
 failed-owner reuse rejected. Owners and GPIO/SCI cold state remain isolated.
- A project-generated ROM writes ISCR/IER via actual CPU word MMIO,then sleeps.
 A test-only scheduled edge at cycle100 on each IRQ line wakes it when I is clear;
 the ISR writesA5 to NVRAM and RTE restores SP. With I set it remains pending and
 asleep.3856 every-cycle partitions compare CPU snapshots,NVRAM,exceptions,
 controller publication and mapped register images. This synthetic pin schedule
 is a fixture,not a production event API or measured hardware timing.

Candidate85 CTest/87 Python;Release build and84 locally runnable CTest groups/86
Python cases PASS;one established Windows symlink privilege case excluded. Final-head CI pending. CPU inventory
unchanged:562 methods,264 selected/0 missing/135 excluded encoding rows,not full CPU acceptance.
Prior GPIO guard tests move old unmappedF4 to still-unmappedF3.

## Original component comparison and negative controls

Ignored reference verifies18 verbatim original IRQ methods and the vector table
from `h8_intc.cpp`,SHA256
`60d55cf124ed5ece134ca952c2c26fe4f71821a0f0fcac7afb9cdb9ed01d7111`.
200000 xorshift32 mixed operations,seed1A73C:byte/word register writes,IRQ pin
changes,internal requests,DMA-consumed requests,filters and acknowledgements.
All five mapped reads and published vector/level/NMI compared:zero mismatches.
Reset/NMI-edge operations are not exercised in this mapped fixture;the original
component tests remain separate evidence. This is not whole-machine equivalence.

Ignored candidate-only controls:reversed ICR offsets =>150185 mismatches;
IER writes wrongly routed to ISCR =>199554 mismatches. Original reference unchanged;
tracked candidate correct. Linux CI includes the target under UBSan;local
ASan/UBSan not run. Source/notice checks are not legal/binary clearance.
M0 partial;firmware/MIDI/PKS/audio/plugin/REAPER acceptance remains open.
