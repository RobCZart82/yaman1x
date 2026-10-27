# Standalone H8H bus-access helpers

Recorded: 2026-10-27. Base: merged #42,
`e3f71a89f6d967034a3c05926a7a86c595ab9a49`.
M0 component work: the CPU instruction decoder/runner is not yet integrated.

`src/h8h_bus_access.h` adapts five pinned MAME `h8.cpp` helpers for the H8H case
(`m_has_exr=false`). Original BSD-3-Clause/Olivier Galibert notices are retained;
`research/h8h-bus-provenance.json` records source identity and scope. This is not
a full CPU, address-space implementation, instruction cache or firmware test.

## Preserved bus/cycle contract

Each helper charges two units to the CPU-owned signed `int` remaining-cycle
counter **before** invoking the host bus, including when the charge crosses zero.
A negative budget is observable and intentional; this layer does not replenish
it, run instructions or apply inter-slice debt. The future execution runner must
preserve the upstream full/partial-handler and carry semantics separately.

| Helper | Host operation | Address |
|---|---|---|
| `read16i` | `read_instruction_word` | low bit cleared |
| `read8` | `read_byte` | unchanged |
| `write8` | `write_byte` | unchanged |
| `read16` | `read_word` | low bit cleared |
| `write16` | `write_word` | low bit cleared |

Instruction and data paths stay distinct. The host bus implements big-endian
word storage/lanes, address-width mapping, side effects and CPU/DMA context.
No 24-bit truncation is added here: the pinned helpers pass addresses to the
configured address space. The synthetic bus uses a small test-only backing
store while retaining full input addresses in its trace. It is not an AN1x map.
SCI MMIO remains aligned-only; a future CPU bus composes these layers after
CPU word alignment, rather than weakening the peripheral API.

The bus and counter must outlive the adapter and belong to that CPU instance.
Copies are disabled. Callbacks must not re-enter the adapter or modify its
counter; contract exceptions must propagate. Counter underflow is rejected
before a bus callback or counter mutation. A callback exception leaves the
already charged cycles and possible bus side effects in place, poisons the
adapter, and prevents subsequent access. Discard the CPU/bus owner after such
failure. No rollback, reset, save-state or real-time callback guarantee is made.
H8S/EXR behavior and wait-state models are outside this adapter's scope.

## Verification

The ROM-free native regression checks exact callback ordering and pre-access
cycle values, instruction/data separation, word alignment versus byte addresses,
all 65,536 word values with known big-endian byte lanes, signed-counter limits,
bus faults, callback reentry and two-machine isolation. It passes ASan/UBSan.

A separate optional reference check verifies the whole locked source ZIP and
exact `h8.cpp` identity, extracts only the five unmodified method bodies into a
new local work directory, and compiles them against the same synthetic bus.
`m_has_exr` is fixed false. The candidate and original bodies agree on return
values, counter values, ordered bus calls and final memory after 200,000 mixed
operations. The work directory holds the temporary header/executable; neither
is committed. This comparison validates only these helpers, not the whole CPU.

```sh
python3 tools/check_h8_bus_reference.py --archive /absolute/path/to/mame-master.zip --work work/h8-bus-reference
```

This optional command needs a C++20 compiler accepting GCC/Clang-style flags
(default `c++`, overridable with `--compiler`); the normal CMake target also
supports the project's Windows compiler. Four Python regressions check extraction,
source-lock coherence and archive rejection before compiler execution. CI uses
synthetic inputs; it does not need the user's archive or Yamaha files.

## Next gate

Integrate the execution budget and full/partial instruction state with generated
handlers; provide a real per-instance bus and test synthetic reset vectors,
register/flag operations and branches against the reference. Then connect IRQ
entry and SCI deadlines with explicit CPU/MMIO ordering. The current two-cycle
source model and timeline tie convention remain unvalidated against AN1x hardware.
Firmware MIDI acceptance, audio and REAPER remain open.

Local validation / helyi ellenőrzés: macOS arm64 Release, 26/26 CTest groups and 72 Python cases PASS; new native ASan/UBSan PASS. Final PR-head platform CI remains separate evidence.
