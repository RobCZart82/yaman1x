# H8H IRQ/NMI entry — fixed reset SYSCR

Recorded: 2026-10-28. Candidate after synchronous TRAPA (#64).
Original state_irq full/partial bodies bring the selected total to 438 methods.
Instruction inventory remains 211 selected / 53 missing / 135 excluded rows:
interrupt entry is an internal state, not another instruction encoding.

An optional H8hIrqController pointer binds the existing controller to the CPU.
It and the host must outlive the CPU; callbacks must not re-enter it. Unbound
instances retain the prior no-external-interrupt profile. Fixed SYSCR=0x0b uses
CCR.I filter 2/0, NMI bypass and no EXR stack. Normal prefetch samples the
controller; noirq prefetch deliberately suppresses selection. Original stack,
vector fetch, mask, acknowledgement and prefetch ordering are retained.
Exception observers run at the original notification point. Stack access failure
is fail-stop and leaves the request pending. No mutable SYSCR, DMA/DTC/trace,
standby or bus retries are implemented.

Independent native checks cover 512 IRQ-mask/NMI-return cases, 322 full/partial
split histories, an external edge pin with ISR acknowledgement, two stack-fault
positions and CPU/controller isolation. NMI arrives before the fixture LDC and
stacks CCR=0x80/PC=0x106; normal IRQ becomes eligible after LDC and stacks the
loaded CCR/PC=0x10a. These are deliberately distinct event-boundary expectations.
The original-method reference adds independent IRQ publication, original
take_interrupt and original state_irq to its existing decoder/execute_run path.
It compares CPU state, memory/bus trace, exception notifications and final IRQ
selection across 200000 slices, including dedicated reachable IRQ/NMI programs.

Final PR-head CI and predecessor merges remain separate acceptance gates.
No firmware boot, physical AN1x equivalence or audio acceptance is claimed.
Next step: bind the CPU bus and event scheduler to the existing SCI/MMIO model
and test an interrupt-driven byte exchange. Remaining instruction rows are still
explicitly unsupported rather than silently treated as NOPs.
