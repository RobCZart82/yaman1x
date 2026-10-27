# Initial source-snapshot validation

This page preserves historical source-snapshot results. The current full
review and fresh macOS validation are recorded in the
[2026-10-27 repository audit](REPOSITORY_AUDIT_2026-10-27.md).
Historical test counts below must not be used as the current suite count.

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
project's core source files match the local research versions. An additional
opt-in timed VOP3 patch also compiled in the local MAME research target and
completed 1- and 5-second firmware runs. It remains outside the CMake CI and
uses the unverified 48 kHz model assumption. Vendored source and patch bytes
are preserved with explicit Git attributes.

No Yamaha data is needed to reproduce these default checks. Full upstream
tests requiring Yamaha images remain outside this source snapshot. This
validation does not establish complete boot, AN1x audio, hardware fidelity or
support for untested platforms. Known EG behavior and other open engine work
are recorded in STATUS.md and ROADMAP.md.

## ROM-free cross-platform CI

The first GitHub Actions matrix passed on 2026-10-26 for PR #9:

- `ubuntu-24.04` (Linux x64)
- `macos-14` (macOS Apple Silicon)
- `windows-2025` (Windows x64)

Each job configured and built the CMake project, then passed all nine CTest
groups. These checks use no Yamaha ROMs or patch banks. The workflow's exact
job names are required by the active default-branch ruleset. This validates
these source-build environments only; it does not establish AN1x hardware
fidelity, audio output or VST3/AU host compatibility.

The read-only table-ROM audit also compares an external 128 KiB table image
with the table copy embedded in a local AN1x 1.04 firmware image. It verifies
the shipped-voice exponential, duplicated powers-of-two, decay, sine and
clipper regions byte for byte. This is a content comparison, not proof of the
external image's provenance, the DMS ROM's identity, table-index scaling or
hardware behavior. Neither input is included in the repository.
