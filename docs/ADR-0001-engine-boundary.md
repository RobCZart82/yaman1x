# ADR-0001: firmware engine boundary and dependency baseline

Date: 2026-10-27. Status: architecture selected for development; M0 acceptance
is partial. No embeddable H8 implementation or plugin is claimed by this ADR.
Baseline: `d242372929083fc8626c6cea0e346d555bc7e308`; ROM-free fixes: PR #16.

## Decision

Use a host-independent C++20 engine with one explicitly owned machine per
instance. Keep the instrumented MAME executable as the research reference.
Do not load that executable or its dynamic research bridge on the audio thread.
Retain the reviewed VOP3 interpreter/JIT snapshot behind a DSP adapter.
Select direct Steinberg VST3 SDK integration for the first host adapter,
`v3.8.0_build_66`, commit `9fad9770f2ae8542ab1a548a68c1ad1ac690abe0`.
This reviewed version is a deliberate baseline, not a claim to be latest.
No JUCE or VSTGUI dependency is needed for the first engine/offline renderer.
A native minimal editor or SDK editor can be decided at M5.

Select the MAME H8/3002 implementation as the reference for a separately
reviewed extraction/port. Do not assume it is a standalone CPU library:
`h83002.cpp` depends on the MAME device model, interrupt controller, ADC, DMA,
ports, timers, watchdog and SCI. The CPU/peripheral closure and generated
instruction sources must be inventoried before importing any code. Prefer a
minimal host-services adapter over copying the whole framework. If that closure
cannot be isolated and licensed coherently, revisit this ADR before linking a
replacement. The current default build continues without an H8 or SDK download.

Alternatives: embedding full MAME would carry its framework and distribution
terms into the product; an external process does not provide bounded per-block
audio execution; writing a new H8 core first would discard the current research
reference. None is selected for the first in-process engine.

## Instance and time contract

The future `Machine` owns CPU registers/RAM, interrupt and peripheral state,
SCI0/SCI1 queues and shift registers, LCD/PKS state, two DSP models, serial links,
sample-clock remainder and all experimental profiles. No mutable process-wide
machine or MIDI state is permitted. Immutable ROM buffers may be shared only
with explicit lifetime ownership. Current two-chip bridge state is per handle;
that does not establish that the future CPU/peripheral port is instance-safe.

The host supplies timestamped MIDI at block-relative sample offsets. Convert
those to one monotonically increasing integer machine timeline with rational
remainders; schedule SCI bits at the modeled wire rate rather than placing
bytes directly in firmware RAM. Advance the CPU/peripherals to the next event
or DSP boundary, service events in a documented deterministic order, then
produce the DSP sample. Define ordering for equal-time CPU writes and DSP edges
from evidence before implementation; do not silently round timestamps or depend
on host block size. Preserve the existing 48 kHz DSP rate as a named experimental
profile until AN1x clock evidence settles it. Host-rate conversion belongs after
the machine, with filter delay accounted for in comparisons.

Proposed lifecycle: load/verify local inputs and allocate on a control thread;
reset a machine; enqueue bounded timestamped MIDI; render bounded blocks;
stop and serialize state away from the audio callback. Reject queue overflow
explicitly and enter a documented muted/reset-required state rather than drop
note-offs silently. Profiles and ROM identifiers belong in saved state; ROM
bytes and absolute local paths do not belong in public artifacts.

VOP3 may compile a stale program during sample execution. Therefore this ADR
does **not** assert real-time safety. Before M4, move/prewarm compilation off
the callback or choose a measured bounded interpreter path. No allocation,
file access, blocking IPC or unbounded diagnostics may remain in render.

## Licenses and source identities

The source archives were verified against `docs/source-lock.json` from local
inputs on 2026-10-27 without extracting Yamaha images. Their upstream commit
identities were initially unknown. They have now been recovered with explicit
evidence in [source identity recovery](source-identity-recovery.md); archive
hashes remain distinct from Git SHAs.

| Component | Identity | Scope / terms |
|---|---|---|
| VOP3 | archive SHA-256 `26dacd7280118556ed50ce6fcc03c6835866d18e17a9ad61c3bc77557809e0c9`; file lock in `third_party/provenance.json` | Included source subset, BSD-3-Clause; unchanged |
| AsmJit | `ASMJIT_LIBRARY_VERSION` 1.23.0; same archive and per-file lock | Included, zlib; unchanged |
| MAME | archive SHA-256 `acffc3d3c4d8ffab148d07de5ab0416e54841a1ac99375cfdf641eec5eca7a6e` | External research source; full distribution GPL-2.0 terms and file-specific notices |
| H8/3002 port candidate | Same MAME archive; `src/devices/cpu/h8/` | CPU/SCI source headers inspected as BSD-3-Clause; entire dependency closure not yet approved or imported |
| VST3 SDK | 3.8.0 build 66 and immutable commit above | Planned only; root and selected base/cmake/pluginterfaces/public.sdk license files checked as MIT |
| YamAN1x project code | `LICENSE` | Public distribution license remains unselected; this ADR grants no new rights |

SDK submodule revisions are recorded in `engine-dependencies.json`. The root
license alone does not license every optional bundled example/GUI component.
Review the actual linked closure and preserve notices when integrating; inspect
usage/trademark guidelines separately before naming or distributing a product.
Do not assume the MAME research executable's licensing is interchangeable with
the selected CPU files. Changing the project's public license requires the
rights holder's explicit decision before binary distribution.

Primary SDK evidence: [root license](https://github.com/steinbergmedia/vst3sdk/blob/9fad9770f2ae8542ab1a548a68c1ad1ac690abe0/LICENSE.txt),
[submodule revisions](https://github.com/steinbergmedia/vst3sdk/tree/9fad9770f2ae8542ab1a548a68c1ad1ac690abe0).
Submodule MIT notices were checked at the locked revisions in
`steinbergmedia/vst3_base`, `vst3_cmake`, `vst3_pluginterfaces` and `vst3_public_sdk`.

## Build baseline and remaining acceptance

Reproduce the ROM-free build with [BUILD_BASELINE](BUILD_BASELINE.md).
`engine-dependencies.json` records identities; it is not a dependency downloader.
CI runner labels are moving environments, not compiler version locks.

M0 remains partial until the H8 dependency closure is pinned per file and its
instance isolation and externally driven stepping pass synthetic tests; and the
eventual distribution license decision is resolved. The complete MAME research
build now has a recorded macOS arm64 toolchain and passing CI in
[the source/build evidence](source-identity-recovery.md#verified-result).
The future embedded product still needs its own reference build. This ADR establishes the boundary and SDK choice
without claiming those tasks are implemented. M1 can continue in the separate
MAME research harness while the port is prepared.

Required port acceptance: interleave two machines with different synthetic
RAM/SCI/IRQ inputs and compare each with its isolated run; reset one without
changing the other; compare complete execution under different block partitions;
test equal-time event order and MIDI queue overflow. Use generated instructions
and bytes only. Preserve the existing research capture formats and experimental
profile names. Do not change EG target hold without AN1x hardware evidence.

The VOP3 first-decode initialization finding was reproduced with MSan and has a
[reviewed reversible fix](vop3-initial-decode-fix.md). This addresses the
constructor read; it does not establish full JIT cleanliness or real-time safety.
