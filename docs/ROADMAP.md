# Development roadmap

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

Repository work: keep the source-only audit, clean-clone build, dependency
provenance and license notices current. Add cross-platform CI before declaring
support beyond the validated local platform.
