# Development roadmap

Detailed execution plan, with milestone acceptance criteria through the first
audible firmware-driven VST3 voice in REAPER:
[First REAPER sound plan (Hungarian)](FIRST_REAPER_SOUND_PLAN.md).

Changes/fixes are recorded in the [development ledger](DEVELOPMENT_LOG.md).
The [repository audit](REPOSITORY_AUDIT_2026-10-27.md) identifies current gaps.
Active candidate: [AN1x bounded ROM/RAM map](an1x-memory-map.md),75 CTest/87 Python.
CPU handlers/inventory unchanged. MMIO holes fail closed;synthetic CPU reset/transfers only.
Final-head CI/review pending;merged evidence below remains #114.

Verified engine baseline: [#114](https://github.com/RobCZart82/yaman1x/pull/114),
`3172c55e9a1cea766e5326bfd56a98f68d6420f3`;tested head `532c5937cfc2b0b3f142e935cebba8dfa4912000`.
74 CTest groups/87 Python cases;562 upstream-derived methods (558 unchanged,
four prior arithmetic adaptations);264 selected/0 missing/135 excluded encoding rows.
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229492), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229514), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229555):all five jobs passed.
No recorded automated review result at inspection;no successful automated review claimed.
The prior CPU series is closed;the map candidate is above. [Fixed-profile SLEEP/wake evidence](h8-sleep-wake.md).
All eligible rows select handlers;this is not full CPU/machine acceptance.
Next:AN1x address map/MMIO and common CPU/peripheral scheduler,reset/isolation,
then local firmware/MIDI/PKS evidence. Dynamic SYSCR,standby,DMA/DTC,trace/retry,
physical timing,firmware/audio and REAPER acceptance remain open.
Original peripheral timing TODO remains;no E-clock/handshake validation.

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
