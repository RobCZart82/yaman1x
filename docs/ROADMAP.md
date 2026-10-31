# Development roadmap

Detailed execution plan, with milestone acceptance criteria through the first
audible firmware-driven VST3 voice in REAPER:
[First REAPER sound plan (Hungarian)](FIRST_REAPER_SOUND_PLAN.md).

Changes/fixes are recorded in the [development ledger](DEVELOPMENT_LOG.md).
The [dated repository audit](REPOSITORY_AUDIT_2026-10-28.md) preserves that snapshot;
the current status below and the development ledger supersede its old open-task counts.
Merged research evidence:[#145](https://github.com/RobCZart82/yaman1x/pull/145),
`e470f406c2d65eccda70e0c6381db1f826b2810b`;five CI jobs PASS on head
`90f8aea04fdddce17de60fe706b690b3b5f62c3b`. No recorded automated review result.
[Pinned method contract](h83002-warm-reset-contract.md):five files/six methods;91 Python tests.
Current candidate adds independent cold CPU reset execution tests:256 CCR seeds,
three absolute time origins and seven cuts;89 CTest/91 Python PASS locally.
Targeted ASan/UBSan PASS;final CI/review pending;no warm reset API added.
#146 documentation merged at41a1c1cea0b777653ede9e470ec5e6e7797b2ff2;all four CI jobs PASS.
Engine implementation baseline remains #143.

Verified engine baseline:[#143](https://github.com/RobCZart82/yaman1x/pull/143),
`301eec75c72028b8f377ee4e099fce0b1df4de5e`;tested head `d2da27c15c8ebf8c083f35bb4a2cef18eba4d8c4`.
88 CTest/87 Python;[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41127313949),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41127314006),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41127313987):all five jobs PASS.
Actual macOS runner15;historical required check name macos-14 retained for ruleset compatibility.
No recorded automated review result at inspection;none claimed.
[Cold research lifecycle](an1x-research-lifecycle.md) owns stable CPU/bus epochs,immutable ROM and
explicit NVRAM preserve/clear.401 cut-point restarts match fresh CPU/TX histories;
full256KiB NVRAM retains across8 epochs;invalid replacement preserves old epoch.
Failed/reentrant recovery and epoch-relative copied RX schedules tested;targeted ASan/UBSan PASS.
Not hardware warm reset. Research candidate above verifies method identities only. Next:verify warm-reset
retention/publication matrix and reset ordering/time/input semantics before adding
an in-place reset API;test active SCI,IRQ pins,GPIO latches,RAM and pending events.
Generated CPU bodies unchanged:562 methods,264 selected/0 missing/135 excluded rows.
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
