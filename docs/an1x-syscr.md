# Bounded shared CPU/host SYSCR

Recorded:2026-10-30. Merged #141 as `eba55a2367f9cc5e319b4f3f26049fe4da55af16`;tested head `d50532a812abd23a65a2aafaf24f28ea815c0217`.
All five final-head CI checks PASS:41085891561/41085891536/41085891444.
No recorded automated review result;none claimed.
 FFFFF2 byte read/write now uses the bus-owned SYSCR09 cold
value. Only09 and01 are accepted. Other254 values throw before byte mutation,
poisoning the host. F2/F3 words stay unsupported. This is deliberately narrower
than original MAME's unrestricted byte store;no standby/extra-mode acceptance.

Source facts and independent original-method comparisons are in
[h83002-syscr-modes.md](h83002-syscr-modes.md),pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h83002.cpp SHA2562b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92.
The three mode methods matched all65536 pairs in #139;this increment reuses their
project-owned equations. Generated instruction bodies remain byte-identical to
regenerated locked outputs (both H8/H8H selection checks PASS).

## Shared policy and lifecycle

CPU reads optional host.syscr_value() for IRQ/TRAPA entry,otherwise retains the
legacy fixed0B profile. Optional sync_cpu_ccr(ccr) publishes I/UI changes into the
host's IRQ owner wherever original handlers call update_irq_filter;entry hooks
also synchronize after setting I/UI. Host stores only the relevant CPU CCR mirror
for immediate SYSCR-write filtering;there is no second CPU SYSCR byte. CPU CCR
arithmetic alters NZVHC,not I/UI;LDC/RTE/filter paths retain their original hooks.

A byte write republishes the IRQ filter immediately from current host SYSCR and
last synchronized CPU CCR. This permits priority1 pending IRQ to wake SLEEP after
09->01 with I=1/UI=0. Host cold mirror is80;CPU reset updates it via existing hooks.
Callers must use the same IRQ owner passed to the CPU,one CPU per bus,and may not
mutate host/controller during callbacks or concurrently. Generic hosts without
SYSCR hooks retain fixed0B behavior and existing reference controls.

Cold reconstruction remains the reset policy;no in-place hardware reset API is
added. The mirror is not a CPU snapshot/restore facility. External mode writes
are only supported between execution or from the owning scheduler,not reentrant
callbacks. NMI still bypasses priority filtering through the existing controller.

## Independent integration evidence

`tests/an1x_syscr_check.cpp` configures SYSCR through actual CPU MMIO and tests
2 modes x4 I/UI masks x2 priorities xIRQ/NMI x241 split positions =7712 partitions.
Checks independently predict eligibility,entry CCR,IRQ/NMI vector,restored CCR/SP,
NVRAM,controller publication and full/partial CPU equivalence.512 TRAPA cases
capture entry CCR before MOV changes flags and verify saved pre-entry CCR/SP.
The initial test used RTE after TRAPA and expected a single trap;original return
address semantics caused repeat entry. The fixture now stops inside the ISR;
IRQ cases independently test RTE. No CPU instruction body was changed to satisfy it.

241 additional partitions change host09->01 while a priority1 IRQ is pending and
CPU sleeps,checking immediate wake/current-mode entry. Direct tests check immediate
masking/unmasking/readback,24-bit alias,owner isolation,all254 unsupported bytes,
failed-owner state and incomplete-word preservation. Prior GPIO/IRQ hole guards
move F2 to still-unmapped F0. Existing ICR deferred-publication semantics remain.
The production host still lacks a general external-pin event queue;the scheduler
used for edges/mode changes is synthetic test infrastructure,not hardware timing.

CPU helper adaptation/provenance updated;562 generated methods/264 selected rows
unchanged. Full87 CTest/87 Python and targeted ASan/UBSan PASS. Ignoring host mode
in CPU entry (fixed0B mutant) causes2056 expected failures;tracked code unchanged. No full-machine original-reference comparison claimed.
No firmware/MIDI/PKS/DSP/audio/plugin/REAPER acceptance. Full reset remains next.
