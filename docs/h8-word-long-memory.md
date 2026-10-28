# H8H word/long memory and the original 0100 prefix state

Historical #49 increment, 2026-10-28, based on #48. The current
[arithmetic extension](h8-arithmetic-flags.md) adds selected ADD/SUB/CMP forms. M0 remains partial.

The research execution profile at #49 retained 56 unchanged generated full/partial
methods. This increment adds MOV.W immediate16, indirect MOV.W and MOV.L loads/
stores, and the original dispatch_0100 prefix handler. The existing byte moves,
immediate32, reset/NOP and rel8 conditions remain covered.

| Operation | Encoding supported from the pinned decoder |
|---|---|
| MOV.W #imm16, register half | `0x7900 | r`, then immediate; r=0..15 |
| MOV.W @ERp, register half | `0x6900 | (p << 4) | r`; p=0..7, r=0..15 |
| MOV.W register half, @ERp | `0x6980 | (p << 4) | r` |
| MOV.L @ERp, ERr | `0x0100`, then `0x6900 | (p << 4) | r`; p,r=0..7 |
| MOV.L ERr, @ERp | `0x0100`, then `0x6980 | (p << 4) | r` |

Register-half encodings follow the pinned H8H decoder, including its allowed
upper-half selectors; this is source compatibility, not new hardware validation.
Other word modes, prefix extensions and opcodes still fail explicitly.

## Prefix and bus semantics

The first prefix word dispatches the original generated handler, which fetches
IR[1] and advances to internal state `0x20000 | IR[1]`. It may suspend before
that transition. The reduced decoder validates the internal state separately;
it cannot reinterpret an unsupported prefix extension as an ordinary opcode.
PPC retains the first instruction address through the extension stage, matching
the original execute_run ordering. Partial prefix/transfer states and cycle debt
are retained between contiguous slices.

Host additionally provides completed synchronous `read_word(address)` and
`write_word(address,value)`. The reviewed H8H bus helper charges two cycles before
each callback and aligns odd addresses down. The host owns big-endian lanes and
actual address mapping, including advanced-mode 24-bit wrapping. Byte callbacks
remain distinct. Research fixtures retain separate immutable instruction storage
and mutable data; they are not the AN1x address map or MMIO integration.

A long transfer performs two word accesses in order: high half at saved address,
low half at saved address+2, each separately aligned. Addresses use the original
32-bit unsigned arithmetic before host mapping. The source caches address/value
in temporary registers, so register/pointer aliases and a split between the two
words do not repeat a completed transfer or change its saved address.

Exceptions may leave the first or both writes committed. The CPU and host are
then discarded; there is no rollback or resumable failed execution. Retry/wait
states, IRQ/DMA/DTC/trace, standby, warm reset and saved-state restoration remain
unsupported. The diagnostic snapshot is still not a persistence format.

## Evidence

- Native word controls: all 65,536 values for both load and store, plus 32,768
  cases per direction across pointer/register-half aliases, flags, raw upper
  address bits and odd addresses (98,304 cases per direction in total).
- Native long controls: 16,384 cases per direction across all ER pairs, mixed/
  sign-boundary values, aliases and unsigned/24-bit address wrap.
- All 65,536 prefix extension words are checked against the reduced acceptance
  set. Dedicated odd/zero-debt continuations prove that prefix fetch and each
  long word access are performed only once. First/second-write failure controls
  exercise partial mutation and poisoned-owner behavior.
- 1,000 mixed word/long partition histories match whole runs in full diagnostic
  state, ordered bus trace and data memory. The earlier byte/condition suite and
  exhaustive first-word decoder controls continue to pass.
- Locked original H8H decoder/handlers, bus/register/flag/prefetch helpers and
  execute_run: 200,000 slices, 745,100 instruction reads, 19,304 byte reads,
  19,310 byte writes, 57,833 word reads and 57,853 word writes; zero differences
  after each slice. The fixed host disables unimplemented IRQ/transfer/debug
  services; this is not a complete MAME device or firmware run.
- Negative control: a deliberate word-byte swap in an ignored candidate copy
  is detected by the unchanged original reference at seed 0, slice 4.
- Both selected generated headers remain checked against the locked complete
  outputs, with retained BSD notices and updated selection hashes. No new
  upstream revision, Yamaha input or private trace is imported.

Use the existing `check_h8_first_instructions.py --generated ...` commands for
both output variants, or `--archive ... --work ...` for the original-code
experiment. Work directory must be new; generated reference code and executables
stay local and ignored. Default CMake/CTest checks require no archive or ROM.

Next: arithmetic/comparison flag behavior and broader instruction coverage,
then IRQ entry and explicit CPU/SCI scheduling. Complete CPU, hardware timing,
firmware PKS/MIDI, audio and REAPER acceptance remain open.

Candidate local validation: Windows MSVC Release build, all 30 runnable CTest groups PASS. All 83 runnable Python cases also PASS; the known symlink case is excluded. Complete suite: 31 groups / 84 Python cases. The known local symlink-privilege case remains explicitly excluded; final PR-head Linux/macOS/Windows, MSan and MAME evidence is separate. Source/notice audits PASS.
