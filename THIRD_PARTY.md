# Third-party sources

| Component | Included material | License / notices |
|---|---|---|
| [musicastudio/VOP3](https://github.com/musicastudio/VOP3) | Five C++ source/header files; one reviewed initialization adaptation in `third_party/vop3/src` | BSD-3-Clause; `third_party/vop3/LICENSE`; copyright 2026 Wouter van Nifterick, Zhiyuan Wan, James Hansen |
| AsmJit snapshot supplied with VOP3 | Unchanged source files under `third_party/vop3/third_party/asmjit/asmjit` | zlib; `third_party/vop3/third_party/asmjit/LICENSE.md`; copyright 2008–2025 Petr Kobalicek |
| [MAME](https://github.com/mamedev/mame) | Eleven research patch files; no executable or full source tree | Specific upstream files listed below are BSD-3-Clause; full MAME distribution has GPL-2.0 terms. License texts and original distribution notice are under `licenses/`. |
| MAME H8 adaptations | Eleven GPIO/IRQ/SCI/bus/budget headers, including two compatibility facades and the H83002 SCI MMIO/IRQ adapter under `src/`; source identities in the peripheral provenance records | BSD-3-Clause; Olivier Galibert; `licenses/MAME-BSD-3-Clause.txt` and retained file headers |

The VOP3 input archive has SHA-256
`26dacd7280118556ed50ce6fcc03c6835866d18e17a9ad61c3bc77557809e0c9`.
Its upstream commit is `a3c8182811c398efd41daadf78d6fabcf50af56a`, recovered
from the ZIP comment and verified against its complete Git tree. The archive
hash remains distinct from the commit ID; see [identity evidence](docs/source-identity-recovery.md). `third_party/provenance.json` records every included upstream file's
hash and size. The repository auditor checks them without network access.
The source-only build recipe in `cmake/vop3/` is an explicitly adapted version
of the upstream recipe: examples and upstream tests that need Yamaha images
are omitted. AsmJit and four VOP3 files are unchanged. One VOP3 header has an explicit
[reversible initialization fix](docs/vop3-initial-decode-fix.md), recorded in
the provenance manifest with its original upstream hash and exact patch.

MAME patch context comes from:

- `src/mame/yamaha/yman1x.cpp`: AJR, BSD-3-Clause.
- `src/osd/mac/main.mm`: R. Belmont, BSD-3-Clause.
- `src/osd/mac/video.cpp`: Olivier Galibert and R. Belmont, BSD-3-Clause.
- `src/devices/video/lc7985.cpp`: Olivier Galibert, BSD-3-Clause.

The original MAME archive identity is in `docs/source-lock.json`.
`src/h8_digital_port.h` is a standalone adaptation of the BSD-3-Clause H8
digital-port register logic by Olivier Galibert. Its source identities,
omissions and validation are in [the peripheral record](docs/h8-digital-port.md)
and `research/h8-digital-port-provenance.json`; the original terms remain in
`licenses/MAME-BSD-3-Clause.txt`.
`src/h8h_internal_irqs.h` also adapts a bounded subset of Olivier Galibert's
BSD-3-Clause H8H interrupt-controller selection logic. Source hashes and
omissions are in `research/h8-internal-irqs-provenance.json` and
[the internal IRQ record](docs/h8-internal-irqs.md).
`src/h8h_irq_controller.h` extends the same adaptation with H8H register/pin
semantics and original publication points; see [the controller record](docs/h8h-irq-controller.md).
`src/h8_sci_cycle_clock.h` adapts the internal integer-cycle scheduling subset
of the same author's BSD-3-Clause SCI source; source hashes and limits are in
`research/h8-sci-clock-provenance.json` and [the clock record](docs/h8-sci-cycle-clock.md).
New `yamanix_*.h` patch files reproduce the project's own bridge and bus
interfaces. MAME research remains a separate program; this repository does
not link MAME into the plugin core. A later distribution must account for
all components actually used.

Yamaha firmware, VOP3 program/table images and patch banks are separate local
inputs, not part of these source licenses. None is vendored here. All default
tests use project-generated synthetic data. The separately licensed upstream
ROM-dependent tests remain outside this repository.

The standalone `src/h8_sci_status.h` adapts the pinned MAME H8 SCI SSR latch and qualified DMA flag rules (BSD-3-Clause, Olivier Galibert). Source identity and scope: `research/h8-sci-status-provenance.json`; ownership/integration requirements: `docs/h8-sci-status.md`.

## Reviewed source/notice coverage

`third_party/license-coverage.json` maps the currently reviewed files to their
component, origin, license texts and required notices. Run
`python tools/license_coverage.py`; the default CTest suite runs the same check.
The existing repository audit separately verifies vendored content hashes and
the reversible VOP3 edit. Both checks are required; neither is legal clearance.

The coverage check detects missing registered files, missing required notices,
changed/missing reviewed license texts, unregistered vendored code and research
patches, and unregistered explicitly marked adaptations under `src/`/`cmake/`.
It cannot discover unattributed copied code or decide license compatibility.
Update this inventory and review the actual license whenever importing code;
do not merely add a path to make a failing check green. The generic MAME BSD
text intentionally remains byte-identical to upstream; actual copyright holders
are retained in source headers and the attribution above.

Before binary distribution, verify notices in the actual shipped package and
review every linked dependency. Full MAME research distribution is a separate
review from selected BSD excerpts. The VST3 SDK remains planned, not vendored;
its actual linked closure needs review when integrated. The project-specific
license remains unselected in `LICENSE`; this change does not relicense any code.

`src/h8_sci_async_tx.h` composes the reviewed status/cycle adapters with the BSD-3-Clause asynchronous transmit state machine by Olivier Galibert. Source identity and bounded scope: `research/h8-sci-tx-provenance.json`; contract and evidence: `docs/h8-sci-async-tx.md`.

`src/h8_sci_async_rx.h` adapts the BSD-3-Clause asynchronous receive/sampling/error rules by Olivier Galibert and composes the reviewed SSR/cycle helpers. Source identity and scope: `research/h8-sci-rx-provenance.json`; evidence and integration limits: `docs/h8-sci-async-rx.md`.

`src/h8_sci_async_channel.h` is the shared asynchronous SCI implementation (BSD-3-Clause, Olivier Galibert), preserving one clock/status/register owner per channel. The earlier TX/RX headers now forward to it. Source identity: `research/h8-sci-channel-provenance.json`; integration evidence and limits: `docs/h8-sci-async-channel.md`.

`src/h83002_sci_mmio.h` composes two reviewed SCI channels with the pinned H83002
register map and interrupt vectors, preserving DMA-before-IRQ qualification
(BSD-3-Clause, Olivier Galibert). Identities and scope:
`research/h8-sci-mmio-provenance.json`; evidence and owner contract:
[H83002 SCI MMIO/IRQ](docs/h83002-sci-mmio-irq.md).

The H8H bus-access helpers retain BSD-3-Clause/Olivier Galibert notices. Their exact `h8.cpp` identity is in `research/h8h-bus-provenance.json`; the [component record](docs/h8h-bus-access.md) describes the isolated adaptation and optional original-method comparison.

The H8 execution-budget arithmetic retains the same MAME BSD notices; exact `h8.cpp` source identity and scope are in `research/h8-execution-budget-provenance.json`.

The H8 execution-order runner retains the same MAME BSD notices; scope and source identity: `research/h8-execution-runner-provenance.json`. Original reference method bodies remain outside tracked sources.

The first-instruction research subset retains Olivier Galibert BSD-3-Clause notices for 536 selected upstream-derived H8/H8H full/partial handlers and adapted host helpers. `research/h8-first-instructions-provenance.json` records source/generated identities and adaptations; `tools/check_h8_first_instructions.py` checks the selection against the locked full generated output. No additional Yamaha data or new upstream revision is imported.

Merged signed16 displacement: six original groups; see [contract](docs/h8-displacement16-memory.md). Fixed-profile IRQ/TRAPA and synthetic CPU/SCI integration are implemented; full machine/firmware/audio acceptance remains open.

Merged unsigned byte multiply: [scope, checks and word promotion risk](docs/h8-byte-multiply.md).

Merged MULXU.W: two selected methods have a reviewed unsigned-widening adaptation,
not a claim of unchanged bodies; [exact scope/provenance](docs/h8-word-multiply.md).

Merged signed multiply: [scope,original helpers and evidence](docs/h8-signed-multiply.md).

Merged unsigned byte divide: unchanged original handlers; [scope and source-derived flags](docs/h8-byte-divide.md).

Merged unsigned word divide: two additional generated-body local-type adaptations
(int q/r to u32), alongside the two MULXU.W widenings; [scope and provenance](docs/h8-word-divide.md).

Merged DAA/DAS: four unchanged original handlers; [scope and evidence](docs/h8-decimal-adjust.md).

Merged signed byte divide: four unchanged prefix/instruction methods; [scope](docs/h8-signed-byte-divide.md).

Merged signed word divide: two unchanged handlers; [extreme quotient/source limits](docs/h8-signed-word-divide.md).

Merged displacement32 byte/word: ten unchanged prefix/instruction methods; [scope](docs/h8-displacement32-memory.md).
