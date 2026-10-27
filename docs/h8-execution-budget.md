# H8 execution-slice budget and peripheral boundary arithmetic

Recorded: 2026-10-27. Base: merged #43,
`f8b1f4fbd4a422eab33d633e2384f1c82c11623d`. M0 remains partial.

`H8ExecutionBudget` isolates carry/deadline arithmetic from the pinned MAME
`execute_run` and `recompute_bcount` methods. Provenance and retained BSD notices
are recorded in `research/h8-execution-budget-provenance.json`. This is not an
instruction runner, decoder or complete CPU/peripheral scheduling integration.

## Owner protocol

1. `begin_slice(start, cycles)` opens a nonnegative signed-int budget. After the
   first slice, starts must equal the previous end, including zero-length slices.
2. The owner performs its initial peripheral update and calls
   `recompute_deadline(event)` before `apply_carry()`, matching upstream ordering.
3. `apply_carry()` pays the previous slice's overshoot. If it consumes the entire
   slice, the remaining debt survives; the counter is zero and no instruction
   work is permitted. Initial peripheral catch-up may still be needed.
4. During execution, `full_step_allowed()` means remaining > boundary;
   `event_due()` means boundary != 0 and remaining <= boundary. `due_cycle()`
   returns the original event timestamp for the owner's peripheral update.
   The owner must recompute the deadline after servicing it; otherwise a due
   predicate stays due. Partial-handler orchestration is still the owner's work.
5. `finish_slice()` requires no positive budget. Negative remaining cycles become
   next-slice debt, then the visible counter becomes zero. Existing unspent debt
   is preserved when carry already exhausted a slice.

The stable `int& counter()` can be held by the reviewed H8H bus helper. Only the
execution owner may change it, only in its execution phase, using checked cycle
charges; callbacks and other threads must not modify it. Bus exceptions require
following the bus owner's discard contract. This arithmetic object has no bus,
callbacks, exception rollback or instruction/peripheral state to reset.
Copies are disabled; construct independent budget/bus owners per machine.

## Deadline rules and limits

With fixed slice end E, upstream total_cycles()+remaining equals E at this
boundary. The component therefore stores E rather than a MAME device clock.
Event zero means no event. An event at or after E has no boundary in this slice;
an earlier event has boundary E-event. Already-due events are retained for
explicit owner catch-up; this is not a monotonic external-event dispatcher.
No absolute execution timestamp or relation to the SCI timeline is invented.

Invalid phase calls, noncontiguous starts, negative slice budgets, end overflow,
unrepresentable event distances and INT_MIN debt conversion fail explicitly.
Deadline-distance checks finish before updating the previous boundary. Guards
extend the original arithmetic's representable domain checks; they do not add
hardware behavior. After corrupted counter/debt state, discard the CPU owner.
No save-state, standby or real-time guarantee is included.

## Synthetic evidence

- Native tests: 1,000 partition histories through the H8H two-cycle bus helper,
  debt across zero/short slices, event boundaries/end equality, catch-up,
  phase errors, integer limits and two-machine isolation.
- Optional isolated original-method experiment: 200,000 synthetic no-event
  slices agree on action count, remaining budget and debt; another 200,000
  deadlines agree with original `recompute_bcount`, with zero differences.
  The original methods are extracted only after ZIP/source hash verification.
- The reference fixture uses synthetic two-cycle actions and disabled debugging;
  it does not execute H8 opcodes, full/partial instruction semantics, IRQ entry
  or firmware. Native deadline tests do not establish the complete event loop.

```sh
python3 tools/check_h8_budget_reference.py --archive /absolute/path/to/mame-master.zip --work work/h8-budget-reference
```

The optional reference tool needs a GCC/Clang-style C++20 compiler (`--compiler`,
default `c++`) and a new work directory. Original method bodies and executable
stay local. Default CMake tests are ROM-free and do not need that archive.

Next: connect actual generated full/partial handlers to a per-instance CPU state
and this budget/bus boundary, then verify reset vectors, flags, branches and
interrupt entry against the MAME reference. CPU/MMIO ordering and hardware
clock/tie validation remain open. Firmware MIDI, audio and REAPER acceptance
are unchanged.

Local validation / helyi ellenőrzés: macOS arm64 Release, 27/27 CTest groups and 74 Python cases PASS; new native ASan/UBSan and source/notice audits PASS. Final PR-head CI pending.
