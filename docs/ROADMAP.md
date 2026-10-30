# Development roadmap

Detailed execution plan, with milestone acceptance criteria through the first
audible firmware-driven VST3 voice in REAPER:
[First REAPER sound plan (Hungarian)](FIRST_REAPER_SOUND_PLAN.md).

Changes/fixes are recorded in the [development ledger](DEVELOPMENT_LOG.md).
The [dated repository audit](REPOSITORY_AUDIT_2026-10-28.md) preserves that snapshot;
the current status below and the development ledger supersede its old open-task counts.
Documentation reconciled against main `7ee6db8bc418f696e67f0ea17fcc8dade073e47d`
([#137](https://github.com/RobCZart82/yaman1x/pull/137), merged 2026-10-30 09:26:30 UTC).
Its tested head was `e7b5c345b2ed9dc0e38b1fa0ecc615b106602575`:
[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41031322824) and
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41031322822) passed all four jobs.
That documentation PR's automated review failed due to usage limits; no completed
review is claimed. MAME did not run for that docs-only change. Engine evidence below
remains attached to #136. The documentation closure is merged; the next engine step
may start. This reconciliation changes documentation only; its own CI is tracked on its PR.

Verified engine baseline:[#141](https://github.com/RobCZart82/yaman1x/pull/141),
`eba55a2367f9cc5e319b4f3f26049fe4da55af16`;tested head `d50532a812abd23a65a2aafaf24f28ea815c0217`.
87 CTest/87 Python;[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41085891561),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41085891536),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41085891444):all five jobs PASS.
No recorded automated review result at inspection;none claimed.
[Shared SYSCR01/09](an1x-syscr.md) connects FFFFF2 byte MMIO to CPU IRQ/TRAPA modes and
immediate priority filtering.7712 IRQ/NMI partitions,512 TRAPA CCR cases and241
pending-IRQ/SLEEP mode-switch partitions PASS;targeted ASan/UBSan PASS.
Other254 SYSCR values and F2/F3 words remain unsupported;no standby claim.
Generated CPU bodies unchanged:562 methods,264 selected/0 missing/135 excluded rows.
No active engine candidate. Next:explicit machine reset lifecycle,per-owner state,
NVRAM retention,time/input policy and repeated-reset/failed-owner acceptance tests.
Complete machine,LCD/DSP/MEG,physical timing,firmware/MIDI/PKS/audio/REAPER remain open.

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

Historical #78 increment (merged): [signed16 displacement memory](h8-displacement16-memory.md), six original groups. See [STATUS](STATUS.md) for the current baseline.
