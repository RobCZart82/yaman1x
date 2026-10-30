# YamAN1x

Firmware-based Yamaha AN1x emulator research, preparing a future **VST3/AU**
plugin. Repository name: **yaman1x**.

First public research snapshot: **2026-10-26** — [v0.1.0-research](https://github.com/RobCZart82/yaman1x/releases/tag/v0.1.0-research). This is a development foundation. It does not yet produce AN1x audio,
complete a validated hardware-equivalent boot, or provide a plugin or GUI.

## Build and test — no ROM required

Requirements: CMake 3.20+, a C++20 compiler, Python 3.9+, Git and platform build
tools. The reviewed VOP3 and AsmJit source snapshot is included, with original
licenses. Configuration does not download dependencies or Yamaha data.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

The source-only build defines **85 CTest groups and 87 Python cases**. Tests generate synthetic
programs/banks in temporary directories. The two upstream tests that require
Yamaha images are not part of this suite. ROM-free GitHub Actions checks this suite on Linux x64, macOS Apple Silicon and
Windows x64; see the current status below for tested revisions.
This checks the source build, not AN1x hardware fidelity or plugin-host support.

See [engine architecture decision](docs/ADR-0001-engine-boundary.md) and
[build baseline](docs/BUILD_BASELINE.md) for the M0 decisions and remaining gates.

## Current state

Documentation reconciled against main `7ee6db8bc418f696e67f0ea17fcc8dade073e47d`
([#137](https://github.com/RobCZart82/yaman1x/pull/137), merged 2026-10-30 09:26:30 UTC).
Its tested head was `e7b5c345b2ed9dc0e38b1fa0ecc615b106602575`:
[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41031322824) and
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41031322822) passed all four jobs.
That documentation PR's automated review failed due to usage limits; no completed
review is claimed. MAME did not run for that docs-only change. Engine evidence below
remains attached to #136. The documentation closure is merged; the next engine step
may start. This reconciliation changes documentation only; its own CI is tracked on its PR.

Active candidate:[cold research machine lifecycle](docs/an1x-research-lifecycle.md).
401 restart cut points,NVRAM retain/clear and failure recovery controls PASS.
Not hardware warm reset;local88 CTest/87 Python and targeted ASan/UBSan PASS.
Final-head CI/review pending.

Verified engine baseline:[#141](https://github.com/RobCZart82/yaman1x/pull/141),
`eba55a2367f9cc5e319b4f3f26049fe4da55af16`;tested head `d50532a812abd23a65a2aafaf24f28ea815c0217`.
87 CTest/87 Python;[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41085891561),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41085891536),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41085891444):all five jobs PASS.
No recorded automated review result at inspection;none claimed.
[Shared SYSCR01/09](docs/an1x-syscr.md) connects FFFFF2 byte MMIO to CPU IRQ/TRAPA modes and
immediate priority filtering.7712 IRQ/NMI partitions,512 TRAPA CCR cases and241
pending-IRQ/SLEEP mode-switch partitions PASS;targeted ASan/UBSan PASS.
Other254 SYSCR values and F2/F3 words remain unsupported;no standby claim.
Generated CPU bodies unchanged:562 methods,264 selected/0 missing/135 excluded rows.
Cold lifecycle candidate above;physical warm reset remains open. Next:per-owner state,
NVRAM retention,time/input policy and repeated-reset/failed-owner acceptance tests.
Complete machine,LCD/DSP/MEG,physical timing,firmware/MIDI/PKS/audio/REAPER remain open.

See the [development plan through the first sound in REAPER (Hungarian)](docs/FIRST_REAPER_SOUND_PLAN.md)
for the planned firmware, audio-engine and VST3 milestones and acceptance criteria.

- Read-only AN1x 1.04 ROM identification, byte view and reset-vector inspection.
- AN1x bulk-SysEx structure/checksum validation and synthetic upload adapter.
- Two VOP3 instances, C ABI 2, register replay and independent upload-state audit.
- LCD bus model and a separate instrumented MAME research patch chain.
- Two standalone H83002 SCI channels with byte/word MMIO and DMA-qualified
  interrupt routing to the H8H controller; synthetic CPU/scheduler RX/TX and
  receive-error integration is tested, while full AN1x machine integration remains open. See [SCI MMIO/IRQ evidence](docs/h83002-sci-mmio-irq.md).
- Opt-in experimental upload-control and selected EG-level readback profiles.
- Integer sample clock; synthetic tests run 2400 samples per chip with matching
  results under different call partitions.
- Standalone H8 GPIO, internal/external IRQ helpers and a shared asynchronous
  SCI TX/RX channel, with synthetic timing, status and isolation checks. The
  bounded CPU/SCI/GPIO/IRQ integration is implemented; complete machine
  coverage, reset and firmware acceptance remain open.

The local firmware research run loads both complete DSP programs under the
experimental status profile. Its audited upload fields match Python
reconstruction and native replay. The H8 harness is register-only by default;
an explicit `assumed-48k` profile advances both DSP models before MMIO. The
complete eleven-patch research target also builds in separate macOS CI without
Yamaha data. A known upstream EG target-hold discrepancy, the
AN1x clock and table-ROM scaling remain unresolved. These are recorded in
[development status](docs/STATUS.md) and the [roadmap](docs/ROADMAP.md).

The [development ledger](docs/DEVELOPMENT_LOG.md) records merged changes and
maintenance rules. The [repository audit](docs/REPOSITORY_AUDIT_2026-10-28.md)
lists verified checks, fixes and outstanding work. Contribution requirements
are in [CONTRIBUTING.md](CONTRIBUTING.md).

## Local firmware research

No Yamaha firmware, program/table-ROM image, bank, service manual, raw capture,
compiled emulator or plugin is included. Keep local inputs and captures outside
the checkout or in ignored `roms/` and `private_runs/` folders. These inputs stay local and are not published with this repository.

```sh
python3 tools/rom_inspect.py /path/to/local-firmware.zip
python3 tools/bank_inspect.py /path/to/local-bank.syx
python3 tools/an1x_table_rom_audit.py /path/to/local-firmware.bin /path/to/VOP3-main.zip \
  --table-rom-member VOP3-main/docs/vop3_an1x/maskrom.bin
```

These tools do not change their input files. The table-ROM audit compares known
table regions with the CPU-ROM copy; it does not verify DSP address scaling or
hardware fidelity. The separate macOS MAME research build needs a compatible
local MAME source snapshot and separately obtained local firmware. See
[research instructions](research/README.md). It is not linked into this core,
and is not part of the default build.

## Source provenance and licensing

See [THIRD_PARTY.md](THIRD_PARTY.md), [licensing status](LICENSE) and the
per-file snapshot hashes in `third_party/provenance.json`. No project-wide
open-source license for the project's own code has been selected yet.
Upstream source files retain their licenses and copyright notices.

`tools/repository_audit.py` checks candidate Git files, disallowed file types,
binary/secret/path markers and every vendored source hash. It is an upload
check, not a guarantee of firmware authenticity or hardware fidelity.

Product spelling is **YamAN1x**. Existing internal `yamanix` identifiers and the
`YAMANIX` trace protocol are retained for reproducibility. Older local reports
use the previous working name YamANIx; historical context is explained in
[research history](docs/HISTORY.md).

The default branch uses an [active PR-based protection ruleset](docs/BRANCH_PROTECTION.md).

## Historical component increments

The following notes describe each increment at its original scope. Their old
coverage and pending-task statements are historical; current integration, counts
and remaining work are recorded above.

The bounded [integer-cycle dispatcher](docs/cycle-dispatcher.md) is a tested scheduling primitive; CPU/peripheral integration remains pending.

The [two-channel SCI timeline](docs/sci-cycle-timeline.md) now combines timestamped RX input with bounded UART clock dispatch. Its tie order is a software convention; CPU integration remains open.

The [H8 host-services boundary](docs/h8-host-services-boundary.md) records source-derived bus/cycle requirements and the remaining work before embedded CPU compilation.

The [H8H bus-access helpers](docs/h8h-bus-access.md) preserve source-defined word alignment, instruction/data routing and pre-access cycle charging. Full CPU execution remains open; a first generated subset is described below.

The [H8 execution budget](docs/h8-execution-budget.md) preserves inter-slice cycle debt and peripheral-boundary arithmetic; the first generated handler subset is now connected, while complete instruction coverage remains open.

The [first generated H8 instruction profile](docs/h8-first-instructions.md) compiles and executes reset, NOP, MOV.B immediate and BRA rel8 with per-instance register/prefetch/substate storage. Original H8H decoder/handler comparison agrees across 200,000 synthetic slices. Other opcodes fail explicitly; this is not a complete CPU, firmware boot or audio result.

The [H8H memory/branch extension](docs/h8-memory-branches.md) adds MOV.L immediate32, indirect byte data reads/writes and all 16 short branch conditions. The isolated original-decoder comparison agrees over 200,000 mixed slices; full CPU, IRQ/SCI integration and firmware acceptance remain open.

The [word/long memory extension](docs/h8-word-long-memory.md) adds aligned word data callbacks and original two-stage prefix decoding for long transfers. Mid-transfer continuations, aliases and wrap cases are tested against the original H8H execution path; complete CPU/firmware acceptance remains open.

The [H8H arithmetic extension](docs/h8-arithmetic-flags.md) adds selected ADD/SUB/CMP forms at 8/16/32 bits, verified against independent mathematical flags and the original H8H path. Full CPU, IRQ/SCI and firmware/audio acceptance remain open.

The [carry/register/logic extension](docs/h8-carry-register-logic.md) adds byte carry-input/sticky-Z arithmetic, wide register arithmetic, logical forms and CCR storage. Multi-byte arithmetic is tested against independent full-width results; IRQ entry and full firmware/audio acceptance remain open.

Selected CPU extension: [H8 word/long register logic](docs/h8-wide-register-logic.md).

Selected CPU extension: [H8 single-bit shifts and rotates](docs/h8-shift-rotate.md).

Selected CPU extension: [H8 register bit operations](docs/h8-register-bit-operations.md).

Selected CPU extension: [H8 indirect memory bit operations](docs/h8-memory-bit-operations.md).

Selected CPU extension: [H8 absolute memory bit operations](docs/h8-absolute-bit-operations.md).

Selected CPU extension: [H8 stack and control flow](docs/h8-stack-control-flow.md).

Merged extension (historical candidate): [coverage and CCR family](docs/h8-coverage-and-ccr.md).

Merged extension [register-to-register transfers](docs/h8-register-transfers.md) extend MOV at all three widths with lane/alias/flag checks.

Merged extension [small arithmetic](docs/h8-small-arithmetic.md) adds INC/DEC and ADDS/SUBS with explicit flag-preservation checks.

Merged extension [unary/extension family](docs/h8-unary-extension.md) adds NOT/NEG/EXTU/EXTS with independent value/flag expectations.

Merged extension [fixed-profile TRAPA entry](docs/h8-trapa-entry.md) adds synchronous trap stack/vector handling; external IRQ/NMI remains separate.

Candidate: [signed16 displacement memory moves](docs/h8-displacement16-memory.md).
