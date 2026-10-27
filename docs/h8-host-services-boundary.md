# H8 host-services boundary before CPU compilation

Recorded: 2026-10-27. Base: merged PR #41. M0 remains partial.
This increment enumerates the framework boundary required by the CPU port.
It does not compile or execute an embedded H8 CPU.

## Reproducible evidence

`tools/h8_host_services.py` verifies the locked MAME ZIP, traverses the existing
30-file H83002 candidate inventory, verifies and runs the pinned instruction
generator twice, then scans those sources and four verified generated outputs.
`research/h8-host-services.json` contains hashes, token counts and source line
references across 34 files. Only identifiers/locations are recorded; no firmware
or new upstream implementation is imported. Two independent full runs produced
byte-identical reports on macOS. Generated output identities match the existing
four-output lock with LF normalization; source bytes remain unnormalized.

Run from the repository root with a new work directory and new output path:

```sh
python3 tools/h8_host_services.py --archive /absolute/path/to/mame-master.zip --work work/h8-host-services --output work/h8-host-services-report.json
```

The reviewed vocabulary covers ten service groups. The scanner masks comments,
ordinary/raw strings and character literals while preserving line numbers.
It does not preprocess, resolve symbols, distinguish definitions from calls,
interpret inactive branches or discover arbitrary unknown services. Token counts
measure references, not dependencies or effort. Framework transitive dependencies,
compiler/linker closure and distribution licensing remain unproven.

## Required host contracts

Line references below are to the exact locked upstream revision, not local files.
The machine owns all mutable state; no shared CPU/RAM/peripheral globals.

| Boundary | Pinned evidence | Required implementation/acceptance |
|---|---|---|
| Construction and lifecycle | `h8.h` CPU base/NVRAM inheritance; `h83002.cpp` device ownership; `h8.cpp` start/reset | Own registers, RAM, callbacks and peripherals per instance; separate power-on initialization from reset; interleave two machines without leakage. |
| Fetch/data bus | `h8.cpp` 490–532, `h8.h` memory-access members | Distinguish instruction cache fetch from data reads; preserve big-endian byte lanes and side effects/DMA qualification. Word helpers align down before bus access. Test odd addresses at CPU boundary while SCI MMIO retains its aligned-only contract. |
| Cycle accounting | `h8.cpp` 330–374; generated full/partial handlers | Preserve remaining instruction cycles, event boundary and instruction substate. Carry negative budget debt into the next execution slice. Test horizons inside bus/instruction sequences and partition invariance against the upstream reference. |
| Absolute time | `h8.h` now-as-cycles and system-clock helpers; `h8.cpp` reset baseline | Relate externally supplied integer cycles to CPU totals without host wall time. Audit clock-domain conversion and overflow; reset/standby treatment needs separate proof. |
| IRQ and DMA/DTC | `h8.cpp` trigger/set IRQ methods; `h83002.cpp` 191–221 | Preserve qualification before pending latch, filter publication and interrupt taking. Existing IRQ adapters cover selection, not CPU entry/stacking or transfer execution. Unsupported transfers must fail explicitly. |
| Peripheral callbacks | `h8_cpu_base.h`, `h8.h`, `h83002.cpp` map | Route port/ADC/SCI callbacks to machine-owned models. GPIO/IRQ/SCI pieces exist; ADC, timers, watchdog, refresh and actual DMA remain separate integration work. No invented PKS replies. |
| Standby | `h8.h` suspension/time accessors and device notifications | Define frozen clock state, wake/reset semantics and peripheral deadline shifts before enabling standby. The SCI timeline does not implement standby. |
| State and NVRAM | `h8.cpp` start/reset/NVRAM paths | Inventory persisted state separately from debugger state; do not call partially reconstructed state a valid save-state. Reset must not blindly erase every field. |
| Diagnostics/debugger | `h8.cpp` execution hooks, side-effect suppression and disassembly | Host tracing must be optional and preserve debug-read side effects. Keep disassembly/UI services outside the first embedded execution target where possible, with explicit replacements. |

## First CPU build sequence

1. Review a minimal base/H8H CPU subset with generated outputs and retained
   notices; identify exactly which peripheral/device declarations it requires.
   The 30-file inventory is an over-approximation, not the chosen compile set.
2. Implement a narrow host bus/cycle boundary preserving the word-alignment and
   budget-debt rules above. Do not replace upstream partial handlers with an
   invented instruction-level timing model.
3. Compile the generated handlers against that boundary. Start with synthetic
   reset vectors, register moves, branches and RAM accesses; prove instruction
   state, flags, fetch/data transactions and cycle positions against MAME.
4. Integrate IRQ entry and SCI deadlines with explicit CPU/MMIO ordering; repeat
   whole/partitioned/interleaved tests. Only then attempt local firmware boot.

Current software timeline tie order is still a convention. These observations
are derived from source, not physical AN1x validation. M1/PKS/MIDI and audio
acceptance are unaffected; the original ROMs and existing MAME patches are unchanged.

## Verification scope

Eight Python regressions cover masking/line identity, token boundaries/counts,
determinism/source changes, invalid encoding, explicit unknown-service limits,
archive rejection before generator execution and recorded report/lock coherence.
Synthetic tests need no local MAME archive. Real regeneration uses the user's
external locked ZIP read-only, never downloads or modifies a Yamaha ROM.

Local validation / helyi ellenőrzés: macOS arm64 Release, 25/25 CTest groups and 68 Python cases PASS; source/notice audits PASS. Final PR-head platform CI pending.
