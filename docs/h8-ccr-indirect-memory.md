# H8H CCR indirect word memory

Recorded:2026-10-29. Baseline:#96; candidate not yet merged.

## Scope and source identity

Pinned MAME `706dbb73281320c54d7c2f16262c1c3786f2be01`:
original `dispatch_0140`, `ldc_w_r32ih_ccr`, `stc_w_ccr_r32ih` full/partial
methods, with class qualifiers removed only. Six additional methods bring the
selection to518 (514 unchanged/four earlier reviewed MULXU.W/DIVXU.W adaptations).
The source/generated hashes remain locked; no new dependency or Yamaha data.
Two rows bring the inventory to247 selected/17 missing/135 excluded; encoding
rows are not CPU completeness or a decoder coverage percentage.

The raw0140 first word enters original0x70000 continuation. Only6900/ff8f
(load) and6980/ff8f (store) are enabled; all other extension words fail closed.
LDC reads the high byte of an aligned word, replaces the complete CCR, updates
its IRQ filter and retains original no-IRQ prefetch completion. STC writes CCR
to both word bytes and leaves registers/CCR intact. Full32-bit bus addresses
and host24-bit physical mapping remain separate; odd word addresses align down.
No new semantic helper or generated-body adaptation.

## Synthetic independent checks

`tests/h8_ccr_memory_check.cpp` checks:

- 524288 load cases:all65536 word values for each of eight pointer registers;
  high and low byte differ across the sweep, unrelated CCR seed and full register image checked.
- 24576 load/store cases:all256 CCR values, eight pointers and six address boundaries
  (0,1,00ffffff,01000000,80000000,ffffffff). Word lanes, RAM size, exact full
  bus address/type/count, all registers and complete CCR are checked.
- All655360140 extension words, rejecting unsupported forms including other
  CCR address modes and EXR variants. Existing65536 first-word guard updated.
- 201 every-cycle full/partial splits with complete snapshots, RAM and ordered bus trace.
- 512 IRQ/NMI mask cases:pending interrupt raised on the data read; LDC updates
  filtering, NMI bypasses I, following NOP completes before interrupt entry,
  stack PC0114/CCR and RTE restoration checked.322 IRQ/NMI every-cycle splits.
- Read/write faults poison the CPU and prohibit reuse; writes have no rollback.

These are synthetic source contracts, not physical AN1x timing measurements.

## Original reference and negative control

The locked original decoder, unchanged instruction bodies and `execute_run`
match the candidate over200000 split slices:678480 instruction reads,32869 byte
reads,25984 byte writes,45506 word reads,39039 word writes;zero differences.
An address-bound reachability guard requires both new extensions at010a/0112.

The initial reference program stored identical bytes before its load, so a
low-byte mutation was invisible. The strengthened fixture overwrites the low
byte witha5 before loading. An ignored source copy changes only both candidate
LDC bodies from high-byte selection to low-byte selection. The unchanged original
reference detects mismatch seed5/slice7 (expected exit1). Tracked code remains correct.

## Validation and limits

New target PASS;86 runnable Python cases PASS,one known Windows symlink privilege
case excluded. All65 locally runnable CTest groups PASS (full suite66; Python wrapper handled
separately). Linux/macOS/Windows,MSan,MAME CI pending.
CI includes the new target under Linux UBSan; local UBSan not run.
Source/notice audit passed (201 locked upstream files,one reviewed local edit,
229 covered files,five license texts); this is not legal clearance.
Automated code-review quota was exhausted in the previous increment; no successful
new automated review is claimed without current evidence.

Remaining CCR addressing modes, long displacement32, sleep/peripheral/EEPROM
instructions, complete machine/MMIO integration, hardware timing, firmware/MIDI,
audio and plugin/REAPER acceptance remain open. M0 partial;M1 unaccepted.
