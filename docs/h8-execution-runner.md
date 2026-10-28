# H8 execution ordering before opcode integration

Recorded: 2026-10-28. Based on merged #45. M0 remains partial.

`H8ExecutionRunner` composes the reviewed execution budget with the ordering
of pinned MAME `execute_run`: initial peripheral update before debt carry,
due-event catch-up, pending partial handler, full handlers until the peripheral
boundary, then catch-up and partial continuation. End-equal events stay deferred
as in the budget contract. Zero slices still perform the initial update.

The owner supplies `update(cycle)` returning the next absolute deadline (zero
means none), a pure `has_partial()` query, and `full(budget)` / `partial(budget)`.
Full dispatch must perform the upstream PPC/debug-hook bookkeeping when needed;
the runner does not own PC registers or implement debugging. Handlers own
instruction state and checked cycle charging. The stable counter reference may
be bound to `H8hBusAccess`. Owners must not reset, reenter, switch threads or
mutate unrelated runner state from callbacks.

A per-run callback limit counts update/full/partial calls. It protects against
nonadvancing handlers or peripheral deadlines. Exhaustion is a failure requiring
discard of both runner and owner; it is **not resumable**. Exceptions can follow
partial peripheral/CPU mutation, so they poison the runner without rollback.
Invalid slice parameters are rejected before calling the owner. This is an
offline research contract, with no real-time, save-state or standby guarantee.

## Evidence

- Native tests: exact initial-partial/deadline order, overshoot and zero/short
  slices, end-equal deadline deferral, invalid inputs, callback failure/reentry,
  nonprogress bounds and 1,000 partition histories with isolated machine state.
- Locked original-method comparison: 200,000 eventful synthetic slices,
  338,598 updates, 761,211 full and 276,336 partial callbacks; zero differences
  in ordered traces, remaining cycles, debt, pending substate and next deadline.
  The original execute_run/recompute_bcount bodies are unchanged; their host
  methods simulate two-cycle full / one-cycle partial work, with periodic
  events and debugging disabled. This validates orchestration against that
  fixture, not real instruction semantics or hardware timing.
- Two Python regressions check source identity and rejection before output or
  compiler execution. The optional reference experiment requires CMake and a
  C++20 compiler, plus the external locked source archive:

```sh
python tools/check_h8_runner_reference.py --archive /absolute/mame-master.zip --work work/runner-reference
```

Original source stays in the ignored work directory. Default tests use synthetic
inputs and require no archive or Yamaha data. Source identity and adaptation
scope are recorded in `research/h8-execution-runner-provenance.json`; retained
BSD notices are covered by the existing MAME license inventory.

Next: compile actual generated full/partial handlers against per-machine CPU
register state and this bus/budget/runner boundary, then compare synthetic reset,
flags, branches and interrupt entry against the upstream reference. SCI/MMIO
CPU ordering, complete CPU execution, M1 firmware MIDI, audio and REAPER
acceptance remain open.
