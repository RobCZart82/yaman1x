# Third-party sources

| Component | Included material | License / notices |
|---|---|---|
| [musicastudio/VOP3](https://github.com/musicastudio/VOP3) | Five C++ source/header files; one reviewed initialization adaptation in `third_party/vop3/src` | BSD-3-Clause; `third_party/vop3/LICENSE`; copyright 2026 Wouter van Nifterick, Zhiyuan Wan, James Hansen |
| AsmJit snapshot supplied with VOP3 | Unchanged source files under `third_party/vop3/third_party/asmjit/asmjit` | zlib; `third_party/vop3/third_party/asmjit/LICENSE.md`; copyright 2008–2025 Petr Kobalicek |
| [MAME](https://github.com/mamedev/mame) | Eleven research patch files only; no executable or full source tree | Specific upstream files listed below are BSD-3-Clause; full MAME distribution has GPL-2.0 terms. License texts and original distribution notice are under `licenses/`. |

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
New `yamanix_*.h` patch files reproduce the project's own bridge and bus
interfaces. MAME research remains a separate program; this repository does
not link MAME into the plugin core. A later distribution must account for
all components actually used.

Yamaha firmware, VOP3 program/table images and patch banks are separate local
inputs, not part of these source licenses. None is vendored here. All default
tests use project-generated synthetic data. The separately licensed upstream
ROM-dependent tests remain outside this repository.
