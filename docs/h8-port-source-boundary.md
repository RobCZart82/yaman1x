# H8/3002 port: initial source boundary

Date: 2026-10-27. This is an M0 preparation result, not an embedded CPU port.

`h8-source-inventory.json` records 30 source/header files reached from
`h83002.cpp` and `h83002.h` in the hash-locked original MAME archive. Every
record includes its SHA-256, size, leading notices and quoted includes. No
upstream source or firmware payload is copied into this repository.

Reproduce without extracting the archive:

```sh
python3 tools/h8_source_inventory.py --archive /absolute/path/to/mame-master.zip
```

Compare the JSON output with the checked-in inventory. `--output` writes a new
file exclusively, refusing to overwrite an existing inventory. The archive
must match `research/mame-source-lock.json`; newer inbox snapshots are rejected.

## What this establishes

The H83002 declaration brings in the H8H/H8 base, ADC, DMA, ports, interrupt
controller, refresh, timer16, SCI and watchdog. The base's matching implementation
also references DTC and disassembly components. Pairing headers with matching
implementation files intentionally over-approximates the candidate source set.
This is useful for review, not proof that every component is needed by AN1x.

External quoted includes remain visible as boundaries: `emu.h`, `logmacro.h`
and generated `cpu/h8/h8.hxx`, `h8h.hxx`, `h8d.hxx`, `h8hd.hxx`.
The upstream `scripts/src/cpu.lua` rules generate these four files from
`src/devices/cpu/h8/h8.lst` using `h8make.py` with arguments respectively
`s o`, `s h`, `d o`, `d h`, followed by the output path. The generator and
instruction list are build inputs outside the include traversal; they have
not been imported or executed by this inventory tool.

## Required next implementation work

1. Capture the generator/list/build-rule hashes and reproduce generated outputs
   in an isolated build directory. Review their notices and actual dependencies.
2. Enumerate framework services used by the 30-file candidate set: device
   construction/ownership, address spaces, delegates, timers/attotime, scheduler,
   interrupts, state registration and debugger/disassembler interfaces.
3. Define the minimal host-services boundary and implement it incrementally.
   Preserve per-machine ownership and external time stepping; do not emulate
   firmware peripherals by invented replies.
4. Compile the extracted/ported subset with generated synthetic instructions;
   prove isolated/interleaved equivalence, reset isolation, IRQ/SCI scheduling
   and partition-independent stepping before calling the H8 port complete.

`complete_compiler_closure=false` and `license_clearance=false` are deliberate.
The regex does not evaluate preprocessor conditions, macros or angle-bracket
includes; it does not traverse the MAME framework or prove link dependencies.
Leading file notices are recorded as evidence, not a legal clearance of the
eventual distribution. Keep the separate research MAME executable as reference.

Local validation: synthetic tests cover cyclic includes, cpp/header pairing,
missing roots, external/generated boundaries, deterministic ordering, changed
source hashes and escaping include rejection. The real locked archive produced
the 30-file inventory. Compiler execution of an embedded H8 port: NOT RUN.
The Mac firmware experiment remains separately documented in `MAC_M1_HANDOFF.md`.
