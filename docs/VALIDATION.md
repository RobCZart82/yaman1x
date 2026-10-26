# Initial source-snapshot validation

Date: 2026-10-26. Platform actually tested: macOS, Apple Silicon, AppleClang 21.

The source snapshot was configured, built and tested in two separate build
folders. The second source tree was made with `git clone --no-local` from the
initial local commit `e422cc12a79eec5d1cb0705a4927a7931896f790`, without using
the earlier research workspace's dependency directories or Yamaha data.

Both runs passed all **nine CTest groups**:

1. Integer sample clock.
2. Synthetic DSP serial loopback / interpreter–JIT comparison.
3. LCD bus.
4. Experimental upload-control contract.
5. Two-chip synthetic VOP3 timing/readback.
6. Shared-library register bridge.
7. Native upload replay.
8. Python unit tests for ROM/SysEx/trace/upload/readback/repository checks.
9. Repository source-boundary/provenance audit.

The audit verifies 201 unchanged upstream source/license/debugger-description
files against the included manifest. The original seven MAME patches and the
project's core source files match the local research versions. Vendored source
and patch bytes are preserved with explicit Git attributes.

No Yamaha data is needed to reproduce these default checks. Full upstream
tests requiring Yamaha images remain outside this source snapshot. This
validation does not establish complete boot, AN1x audio, hardware fidelity or
support for untested platforms. Known EG behavior and other open engine work
are recorded in STATUS.md and ROADMAP.md.
