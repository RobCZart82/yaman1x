# Development roadmap

Detailed execution plan, with milestone acceptance criteria through the first
audible firmware-driven VST3 voice in REAPER:
[First REAPER sound plan (Hungarian)](FIRST_REAPER_SOUND_PLAN.md).

Changes/fixes are recorded in the [development ledger](DEVELOPMENT_LOG.md).
The [repository audit](REPOSITORY_AUDIT_2026-10-27.md) identifies current gaps.
Verified engine baseline: [#108](https://github.com/RobCZart82/yaman1x/pull/108),
`3c7005a3a26da4149668f6ab9f4695e1672bdc6e`;tested head `ffef22c9d5794ec78028a0f2d07cb9659ff9d5bb`.
71 CTest groups/87 Python cases;548 upstream-derived methods (544 unchanged,
four prior arithmetic adaptations);259 selected/5 missing/135 excluded encoding rows.
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469453), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469399), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469419):all five jobs passed.
No recorded automated review result at inspection;no successful automated review claimed.
No active engine candidate. [Displacement32 CCR evidence](h8-ccr-displacement32-memory.md).
Remaining:SLEEP,MOVFPE/MOVTPE,EEPMOV. Full CPU,machine/MMIO,physical timing,
firmware/MIDI/audio and REAPER acceptance remain open.

Next M0 work, in order:

1. Close remaining H8H opcodes/addressing (5 encoding rows remain after displacement32 CCR; next: MOVFPE/MOVTPE);
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
