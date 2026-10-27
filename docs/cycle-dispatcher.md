# Bounded integer-cycle event dispatcher

Recorded: 2026-10-27. Base: main #38, `47950097b564f7ca48a1a2fd27a125066c3fc7a9`.
This is a project-specific M0 scheduling primitive, independent of pending SCI
adapter PR #39. It is not yet a CPU/peripheral scheduler or a hardware timing model.

`src/cycle_dispatcher.h` advances a monotonic unsigned 64-bit cycle position
through an owner-defined event stream. The owner provides `next_event()` as an
optional cycle and `service(cycle)` for one event. No event uses `nullopt`;
cycle zero and UINT64_MAX are valid event times. No cycle arithmetic is needed.

## Boundary and continuation contract

`advance_to(horizon, budget, owner)` includes events exactly at the horizon.
Each service call consumes one unit of budget. With no due event, the dispatcher
advances directly to the horizon, including with zero budget. If a due event
remains when budget is exhausted, it returns `complete=false` and stays at the
last serviced cycle (or its previous position if nothing was serviced).
The caller can resume with the same horizon and a fresh budget. A recurring
same-cycle event is permitted but bounded by this budget; it never hangs one call.

An owner may insert events during service, including at the current cycle.
It must expose the earliest pending event and must not silently execute multiple
unbounded events in one service callback. Ordering of tied sources, external
line changes, CPU instructions, interrupt taking and MMIO remains the owner's
explicit responsibility. This dispatcher does not choose a hardware tie rule.

A backwards caller horizon is rejected without poisoning the instance. A stale
owner deadline, or an exception from either owner callback, marks the dispatcher
failed. Further advancement is rejected: discard both owner and dispatcher,
because callbacks can already have changed peripheral state. Callback reentry
is prohibited and rejected. Callback code must propagate contract errors.
There is no reset or save-state API; construct fresh matching owners instead.
Copies are disabled. Owners must not be shared across machine instances.

The dispatcher itself allocates no memory. This is not a real-time guarantee:
owner callbacks may allocate, throw, or run too long. The outer host must enforce
its own work budget and handle incomplete results before advancing CPU time.
Actual CPU instructions and SCI wiring are not integrated in this increment.
A future adapter must translate SCI's zero-as-no-event sentinel to `nullopt`.

## Verification

The native regression checks 1000 deterministic 80-event streams for identical
event logs and final times under whole, partitioned and budget-limited advances.
It also covers inclusive deadlines, same-cycle ordering retention, zero budget,
idle jumps, invalid horizons, stale deadlines, callback exceptions, reentry,
no-progress streams, instance isolation and the maximum cycle value.
All events are synthetic and no Yamaha files are used. Local macOS arm64 Release:
23/23 CTest groups and 60 Python cases PASS. The native dispatcher regression
also passes AddressSanitizer and UndefinedBehaviorSanitizer. Final PR-head
platform CI is separate evidence.

Next: define and test an owner for two SCI channels and timestamped external
line events, explicitly record its tie convention, then integrate CPU stepping
only after the H8 framework/generated-source dependencies are closed. M0 remains
partial, and firmware, MIDI, audio and REAPER acceptance remain open.

## Reconciliation with merged SCI adapter

PR #39 subsequently merged into main. PR #40 now includes that baseline and retains both separate native test groups. The previous 23-group validation is historical; the combined suite has 24 CTest groups and 60 Python cases. Synchronization resolves build-registration and ledger conflicts without changing either component implementation. Full CPU/peripheral ownership remains pending.

Combined-tree validation: local macOS arm64 Release 24/24 CTest groups and 60 Python cases PASS. Final synchronized PR-head CI remains separate evidence.
