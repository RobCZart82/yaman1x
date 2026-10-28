# Synchronous H83002 TRAPA entry — fixed reset SYSCR

Recorded: 2026-10-28. Follows unary/extension family.
Add original TRAPA imm2 full/partial handlers (434 -> 436) and four encodings.
Coverage: 211 selected rows / 53 missing / 135 excluded profiles.
The current profile fixes SYSCR to reset value 0x0b: trap setup sets I and uses
vectors 8..11. Other SYSCR modes and external IRQ/NMI entry remain unsupported.
Source identity for h83002.cpp is added to the existing provenance; the original
trap_setup is compiled in the reference fixture. No generated body is changed.

The original stack write order, pre-mask CCR, 24-bit vector fetch, timing and
partial continuation are preserved. Snapshot includes the taken vector.
Optional Host.exception_hook(vector) is an observer at the original debugger
notification point; it must not re-enter or reconfigure the CPU. Observer/bus
failure is fail-stop with already committed stack/vector state retained.
No interrupt-controller pending bit is acknowledged by synchronous TRAPA.

Native evidence: all four vectors x 256 CCR values, every split point through
80 cycles, first/second stack write failures and observer exception. Full local
suite 44/44 CTest groups, 87 Python cases; new ASan/UBSan PASS. Original
instruction/decoder/execute_run reference: 200000 slices, zero differences,
with a fetch guard requiring all four TRAPA forms in a dedicated reachable case.

Important source-parity boundary: in the pinned model the stacked return address
is m_NPC (the trap instruction address in this fixture, 0x108). The test deliberately
records that source behavior; it does not establish physical AN1x trap return
semantics. Do not silently change it without separate hardware/manual evidence.
RTE frame restore is independently tested by the earlier stack/control family.
Final PR-head CI remains separate; parent merges required. No firmware/audio or
external IRQ acceptance. Next: controller/filter publication and external IRQ
entry, then shared CPU/peripheral scheduling. No hold beyond dependency/checks.
