# Development roadmap

These tasks follow the first private source snapshot. No GUI work is planned
before the engine has a validated signal path.

1. **EG target/hold behavior.** Reproduce the source/description discrepancy,
   determine the hardware-relevant arming/holding rule, and add a regression
   check before changing the upstream model or a clearly isolated adapter.
2. **AN1x sample clock and table-ROM scale.** Establish the actual clock and
   table representation from primary hardware/firmware evidence. Record
   assumptions separately from measured facts; preserve original input files.
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
