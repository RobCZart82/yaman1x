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

Active engine candidate:[shared bounded SYSCR01/09](an1x-syscr.md).
CPU/host integration,7712 IRQ/NMI partitions,512 TRAPA CCR cases and241 mode-switch partitions PASS.
Local87 CTest/87 Python and targeted ASan/UBSan PASS;final CI/review pending;merged historical evidence below remains #136/#139.

Merged preparation #139:[standalone SYSCR mode contract](h83002-syscr-modes.md).
65536 independent mode/CCR cases and pinned original-method comparison PASS.
No CPU/MMIO binding yet;integrated engine evidence below remains #136.
Tested head `4d5ca9ba548cd4b3c3547ba191c2660daeebfce4`,merge `b84d3b20bbef1f51a14af64da1a57df9f5cf2a88`.
All five CI checks PASS (platform/UBSan41072881939,MSan41072881943,MAME41072882034).
No recorded automated review result at inspection;none claimed. Shared-SYSCR candidate above supersedes the preparation-only scope.

Verified engine baseline: [#136](https://github.com/RobCZart82/yaman1x/pull/136),
`7aaa8d0178e06b8161c8508cda1cebdb20e2c072`;tested head `6d58a3df9307b4513b61ea01d9eed3d70bd6e87a`.
85 CTest groups/87 Python cases;CPU inventory unchanged:562 upstream-derived methods
(558 unchanged,four prior adaptations);264 selected/0 missing/135 excluded encoding rows.
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141599),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141600),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141620):all five jobs passed.
Automated code review completed with no findings on the tested head;no security-review success claimed.
No integrated engine change in this preparation candidate. [Mapped IRQ registers](an1x-irq-mmio.md) connect
ISCR/IER/ISR/ICR0/1 and bounded F4/F8 words to the CPU's existing IRQ owner.
CPU-configured edge interrupts wake SLEEP and execute the ISR;I masks them.
Original ICR mask/publication retained. SYSCR F2 stays unsupported while the CPU
uses fixed0B;dynamic filter/entry state must change together in a later increment.
Local84 runnable CTest/86 Python PASS;one Windows symlink privilege case excluded.
Next:CPU/host SYSCR contract and mode tests,then complete machine reset.
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

Historical #78 increment (merged): [signed16 displacement memory](h8-displacement16-memory.md), six original groups. See [STATUS](STATUS.md) for the current baseline.
