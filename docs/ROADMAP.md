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
3. **Timed H8–VOP3 execution.** Connect sample stepping to periodic execution
   and MMIO, define upload/run and same-time access ordering, and audit evolving
   readback values against the firmware's checks. Keep experimental profiles
   explicit and opt-in.
4. **MIDI and one dry reference voice.** Provide the required MIDI/peripheral
   behavior, establish a documented first signal path and compare it with a
   reference instrument. Full PKS-ROM emulation, chip routing and MEG effects
   remain separate tasks.
5. **VST3/AU host layer.** Wrap the validated engine, check real-time behavior,
   sample-rate conversion and state serialization, then begin product UI work.

Repository work: keep the source-only audit, clean-clone build, dependency
provenance and license notices current. Add cross-platform CI before declaring
support beyond the validated local platform.
