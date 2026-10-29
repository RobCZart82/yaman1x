# AN1x bounded ROM and RAM map

Recorded:2026-10-29. Merged CPU baseline:#114;this memory-routing increment is a candidate.

## Source facts and implementation policy

Project-owned `src/an1x_memory_map.h` routes memory using the previously recorded
[bus map](bus-map.md),rechecked against pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01:

- `src/mame/yamaha/yman1x.cpp` SHA256
  beab3e6c2eb2a2dc46ef240bd59fb1f463e1875f80e7a3610ae8ea9d22192e14:
  ROM000000–0fffff;external SRAM/NVRAM200000–23ffff.
- `src/devices/cpu/h8/h83002.cpp` SHA256
  2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92:
  advanced24-bit internal RAMfffd10–ffff0f.

The class is new project code using address facts;no upstream implementation,
new dependency or Yamaha payload is copied. Logical ROM input must be exactly
1MiB and already in CPU byte order. It is copied into private owned storage.
No byteswap,patch,file I/O,ROM authenticity or legal acquisition decision occurs.
High address bits are masked to24;RAM and instruction/data access share one map.
Aligned words are big endian;odd words throw rather than silently align,matching
the host boundary expected by the CPU's bus helper. Every word pair is checked
before mutation. ROM writes and unmapped access throw explicitly:these are
research fail-closed policies,not claims about physical open-bus/ROM-write behavior.

New instances deterministically zero SRAM/internal RAM;this is a research cold
state,not measured power-on behavior. `load_nvram` accepts an exact262144-byte
image and rejects invalid sizes before replacement. Export returns a caller-owned
copy. `clear_internal_ram` is an explicit deterministic operation preserving
NVRAM,not a physical reset model. Instances are noncopyable/nonmovable.

No MMIO routing is enabled yet:SCI,GPIO,IRQ registers,LCD,DSP/MEG and unknown
holes fail closed. There is no common machine scheduler,firmware boot or audio
claim. The next step composes existing MMIO/peripherals with timed CPU bus access.

## Independent validation

`tests/an1x_memory_map_check.cpp` verifies all1MiB logical ROM bytes and524288
instruction-word pairs. Caller mutation cannot change owned ROM. It writes and
reads every external/internal RAM byte and aligned word,checking high-address
aliases and isolated instances. Boundaries and holes—including DSP/SCI addresses—
reject access;ROM writes preserve contents;odd writes preserve NVRAM. Exact NVRAM
roundtrip,invalid-size rejection and explicit internal clear are checked.

A project-generated ROM resets the actual H8 CPU at0100,then performs byte,word
and long transfers through the common map at external/internal RAM boundaries.
241 every-cycle partitions compare CPU snapshots and persistent RAM;independent
expected registers and internal/external word values verify the resulting data.
A synthetic CPU attempt to write ROM fails without changing the image and poisons
the CPU. No original Yamaha input is used.

Local Release75/75 CTest and87 Python PASS;targeted ASan/UBSan PASS.
Linux CI includes the map test under UBSan.
The CPU's previously verified original-method reference remains unchanged;this
increment tests routing against source address facts and independent memory
expectations,not a new full-machine MAME equivalence claim. Final-head CI/review
pending. Full reset/isolation/peripheral scheduling,firmware/MIDI/PKS,DSP/audio
and plugin/REAPER acceptance remain open.
