# First standalone H8 peripheral: digital port

Date: 2026-10-27. Branch `feat/h8-source-inventory`.
Status: standalone Windows test and Linux/macOS/Windows CI PASS; integration NOT RUN.

[PR #24](https://github.com/RobCZart82/yaman1x/pull/24) merged after all checks
passed at `896bd7882309bb03868c5c3816f4f9b7b068b858`: [three-platform suite](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477),
[constructor MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462)
and [complete macOS MAME build](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442).
This validates the standalone GPIO test on three platforms; the research MAME
build does not integrate the new GPIO component into the firmware machine.

`src/h8_digital_port.h` adapts the register/pin logic of MAME's H8 digital
port into an instance-owned, header-only C++ component. It has no MAME headers,
device scheduler, heap allocation, global machine or CPU dependency. The owning
machine supplies input/output callbacks at each operation.

Source identity and authorship are recorded in
`research/h8-digital-port-provenance.json`. The adapted code retains the upstream
BSD-3-Clause and Olivier Galibert notices; terms are in
`licenses/MAME-BSD-3-Clause.txt`. No project-wide license decision is implied.
Upstream source was read from the already locked original archive, not copied
from an unreviewed inbox replacement.

## Preserved behavior and explicit omissions

- DDR selects driven versus input/high-impedance pins. Forced inputs override
  readback direction; reserved masked pins read high.
- Data-register reads return the latch plus the upstream reserved-bit mask;
  pin reads combine external levels and driven outputs.
- Writes notify the owner only when the data/direction output pair changes.
  The first reset publishes initial levels; repeated unchanged reset does not.
- Like upstream `device_reset`, reset preserves DDR/data/PCR/ODR. Construct
  a fresh instance for the upstream device-start initialization behavior.
- PCR/ODR are stored/read using upstream masks. Pull-up electrical effects,
  open-drain drive behavior and alternate IRQ/timer pin functions are not added;
  the upstream model does not implement those effects in these register methods.
- MAME device identifiers, address maps, save-state registration, NVRAM stream
  I/O and logging are omitted. Persistence is therefore not implemented here.

This is not a replacement H8 CPU or SCI and is not wired into the research
MAME executable. It does not alter any firmware observation, LCD model, PKS
reply, VOP3 or EG behavior. No timing equivalence or hardware fidelity is claimed.

## Validation

Local MSVC 19.44 Release compilation and `yamanix_h8_digital_port` passed.
The independent per-bit truth-table test covers all 256 DDR values and all
256 latch values, four reserved masks and three forced-input masks (786432
combinations), each at four external pin patterns. It additionally checks
output levels/direction, data readback, suppression of unnecessary reads/writes,
reset register retention, control-register masks and isolated instances.

An additional local MSVC Release differential fixture used the verbatim bodies
of eleven register/reset/update methods from the hash-locked upstream
`h8_port.cpp`, with a minimal callback host instead of MAME device ownership.
It compared 600000 deterministic mixed operations (xorshift32 seed `0x19a73`,
four masks, three forced-input settings, 50000 operations per configuration)
against the adapted model, including read/output callback counts, register and
pin values. Result: **zero mismatches**. The temporary fixture remains outside
Git; it is a register-method differential, not a full MAME device test.
No AN1x hardware trace was executed. The standard test is part of CTest
for [PR #24](https://github.com/RobCZart82/yaman1x/pull/24) Linux/macOS/Windows
runs. Their final checks record platform validation; this development branch
must not be merged before they pass.

## Framework boundary finding

The pinned `h8_cpu_base.h` already defines the CPU/peripheral contract: port
read/write, IRQ/DMA signaling, SCI TX/clock, clock/cycle queries, standby,
ADC and transfer-state requests. It nevertheless inherits `cpu_device`.
SCI also directly uses device timers/attotime, state registration and
side-effect suppression, and distinguishes `total_cycles()` from
`now_as_cycles()`. A fake universal time callback would lose those distinctions.

The GPIO extraction deliberately starts below that timing boundary. Next:
map each framework service used by SCI/interrupt/timer code to explicit owning
machine services, then port one timed peripheral with synthetic scheduling
controls. The CPU's generated instruction handlers still require real memory,
cycle and interruption semantics; compiling empty framework stubs would not
establish an executable H8 port.
