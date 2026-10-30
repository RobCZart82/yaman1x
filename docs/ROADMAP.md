# Development roadmap

Detailed execution plan, with milestone acceptance criteria through the first
audible firmware-driven VST3 voice in REAPER:
[First REAPER sound plan (Hungarian)](FIRST_REAPER_SOUND_PLAN.md).

Changes/fixes are recorded in the [development ledger](DEVELOPMENT_LOG.md).
The [repository audit](REPOSITORY_AUDIT_2026-10-27.md) identifies current gaps.
Active candidate:[read-only GPIO7](an1x-gpio7.md),80 CTest/87 Python locally PASS.
GPIO4/GPIO6/GPIO7/GPIO8/SCI ASan/UBSan PASS;final-head CI/review pending;merged evidence below remains #124.

Verified engine baseline: [#124](https://github.com/RobCZart82/yaman1x/pull/124),
`73a5b5cac78617216901f9db19736902ccd1d0a1`;tested head `33a7a85fc92252d949a7f3867e2d80bbe5b165a8`.
79 CTest groups/87 Python cases;CPU handlers unchanged:562 upstream-derived methods
(558 unchanged,four prior adaptations);264 selected/0 missing/135 excluded encoding rows.
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127930), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127955), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127919):all five jobs passed.
[Automated code review](https://github.com/RobCZart82/yaman1x/pull/124#issuecomment-6512683713) completed on the exact tested head with no findings.
No security review result recorded.
The port8 series is closed;port7 candidate above. [Bounded masked port8](an1x-gpio8.md) adds DDR/pins byte MMIO
with default F0/mask E0 and separate owner alongside port4/port6. Pin truth tables,
cold bit4 output,callback identity/time/suppression,isolation and CPU partitions pass.
Next:read-only port7 at FFFFCE,with independent input/ownership/CPU tests;then
remaining GPIO groups. IRQ-control/SYSCR modes and full machine reset remain open.
Full machine,LCD/DSP/MEG,physical timing,firmware/MIDI/PKS/audio and REAPER acceptance
remain open. Full handler selection is not full CPU/machine acceptance.

Next M0 work, in order:

1. Retain full eligible H8H handler selection (264 rows),fixed-profile SLEEP/wake and original reference controls;
   retain the reviewed [word-multiply widening and UBSan control](h8-word-multiply.md);
   retain fixed-profile IRQ/TRAPA limitations and pinned reference comparisons.
2. Connect CPU, address map/MMIO and existing peripherals to a common scheduler;
   verify event ordering, isolation, reset and partition-independent execution.
3. Close whole-machine dependency/distribution requirements. Source notice checks
   alone are not legal or binary release clearance.

M0 remains partial. M1 protocol research can proceed independently in the pinned
MAME harness. M1 firmware MIDI acceptance and M2–M6 audio/plugin work remain open.
The research items below are evidence requirements, not a second implementation order.

These tasks follow the first private source snapshot. No GUI work is planned
before the engine has a validated signal path.

1. **EG target/hold behavior.** The source/description discrepancy is
   reproduced synthetically. Determine the hardware-relevant arming/holding
   rule from an AN1x reference capture before changing the upstream model or an
   isolated adapter; keep the current characterization explicitly non-hardware
   validated.
2. **AN1x sample clock and table-ROM access.** A 128 KiB VOP3 mask-ROM image
   has been identified in an external research archive and the shipped-voice
   table regions match the CPU-ROM copy. The DMS image identity, VOP3 address
   index scale and actual sample rate remain unverified. Trace the clock path
   from the service schematic and primary chip evidence; preserve original
   input files.
3. **Timed H8–VOP3 execution.** An opt-in research patch now connects sample
   stepping to MAME time and orders due samples before DSP MMIO; a local MAME
   build and 1/5-second firmware runs advanced both chips. Its assumed 48 kHz
   rate is unverified for AN1x, and this is not a complete boot. Audit evolving
   firmware readbacks and establish the hardware clock before treating timing
   as device behavior. Keep experimental profiles explicit and opt-in.
4. **MIDI and one dry reference voice.** A separate MAME patch now connects
   MIDI IN through H8 RXD0 / SCI0; a local MIDI-file smoke run shows line
   transitions. Verify that firmware consumes note events and establish a
   documented dry output path, then compare with a reference instrument. Full
   PKS-ROM emulation, chip routing and MEG effects remain separate tasks.
   Direct SCI snapshots and keyboard-port traffic are now observable; both
   passive and DSP-experiment runs leave SCI0 disabled at sampled instants
   while SCI1 has 30 TDR writes and no RDR reads. Trace the PKS setup and MIDI
   enable control flow before implementing any assumed keyboard replies.
5. **VST3/AU host layer.** Wrap the validated engine, check real-time behavior,
   sample-rate conversion and state serialization, then begin product UI work.

Repository work: keep source/notice audits, dependency provenance, current
status and the ledger in step with each PR. Cross-platform ROM-free CI already
exists; retain it alongside MSan and the separately filtered MAME build. Plugin,
hardware and host support require their own evidence.

Current candidate: [signed16 displacement memory](h8-displacement16-memory.md), six original groups.
