# AN1x bounded GPIO4 MMIO composition

Recorded:2026-10-30. Merged #120 as `41f8b6db0d55743495b4953c0ba746dfc4114446`;tested head `a96bfc40bc518f1315aa3c828c0da1548abbd6aa`.

## Pinned facts and ownership

The new project-owned routing in `An1xSciBus` reuses the unchanged,instance-owned
`H8DigitalPort` from [the digital-port extraction](h8-digital-port.md). Existing
BSD-3-Clause/Olivier Galibert notices and provenance remain applicable. No new
upstream method body or Yamaha payload is copied into tracked files.

Original MAME commit:`706dbb73281320c54d7c2f16262c1c3786f2be01`.
Rechecked directly in the locked local archive:

- `src/devices/cpu/h8/h83002.cpp` SHA256
  `2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
  port4 default DDR00,mask00;advanced-address byte registers below.
- `src/devices/cpu/h8/h8_port.cpp` SHA256
  `c767e444bebcfb849d0cf225bb8a94e8eec481d62a75c1b554d36a8eda5114ca`:
  retained port equations/callback suppression/PCR storage.

| Address | Read | Write |
|---|---|---|
| `0xffffc5` | `ff_r`:always FF,not DDR readback | DDR |
| `0xffffc7` | pin levels,driven latch bits plus external input bits | data latch |
| `0xffffda` | PCR | PCR |

Addresses normalize to24 bits like the existing map. Only these byte accesses
are added. Mixed mapped/unmapped word accesses and odd words fail before GPIO
side effects;full H83002 mixed-width/open-bus behavior is not claimed.
Instruction fetch remains memory-only. Other ports/IRQ/SYSCR remain fail-closed.

Sink supplies `gpio_read(4,cycle)` and `gpio_write(4,cycle,data,direction)` when
access needs it. Missing callbacks throw,never inventing pin levels or silently
discarding outputs. Reads use external input only for input-direction bits.
Output callbacks include high-impedance data bits and explicit direction,and
occur only when the pair changes. PCR storage does not model electrical pullups.
Callbacks receive the committed CPU bus completion time;the existing fail-stop
reentry guard also covers GPIO,even if a sink suppresses the inner exception.

Cold construction emits no implicit GPIO output;first DDR/data write publishes
it. This is a bounded research lifecycle,not a full board device-reset sequence.
Each machine owns its port;NVRAM images exclude peripheral registers. Port4
alternate functions,panel/LCD wiring,other ports,IRQ control,SYSCR priority modes,
timers/ADC/DMA and physical pin timing remain open. SYSCR changes cannot safely
be advertised by merely storing a byte:the current CPU IRQ/TRAPA profile is fixed.
The pinned original SYSCR resets09 and changes IRQ filtering through bit3;future
implementation must propagate this to IRQ/TRAPA/standby semantics with tests.

## Validation and limits

`tests/an1x_gpio4_check.cpp` checks262144 pin cases (256 DDR x256 latch x4 external
patterns) with an independent per-bit oracle,including external callback suppression
when all pins are outputs. It checks all256 PCR values,FF DDR reads,unchanged output
suppression,explicit port/timestamps,24-bit aliases,isolated owners,unmapped holes,
other ports/IRQ/SYSCR rejection,word rejection,reentry and absent callbacks.

Synthetic CPU ROM writes DDR F0/data A5,reads external pins3C and stores AC in NVRAM.
MOV of negative AC sets N,clears Z/V and retains C/I:final CCR89. The initial test
incorrectly expected N clear;the oracle was corrected without changing CPU code.
241 every-cycle partitions compare CPU state,NVRAM,input sampling and output
histories/timestamps. No instruction body or eligible-row inventory changes.

An ignored native differential fixture includes12 verbatim register/lifecycle
method bodies from pinned `h8_port.cpp`,with minimal callback host and no MAME
framework. Mapped operations exercise DDR/data/PCR/pin methods,start and output
update;ODR bodies are present but not part of this map.200000 deterministic mixed
operations,xorshift32 seed19A73:zero mismatches in values,input/output counts and
output data/direction. This is register-method evidence,not full MAME-machine
or hardware timing equivalence. The independent map oracle checks DDR FF reads.

Ignored mutation controls:DDR readback instead ofFF =>65281 expected failures;
raw data-latch reads instead of pins =>497267 expected failures. Tracked code
remains unchanged. Full77 CTest/87 Python PASS;GPIO4+SCI integration ASan/UBSan PASS.
Linux CI includes the new target under UBSan. Final tested-head CI:all five checks PASS,runs40977946004/40977945988/40977945935.
No recorded automated review result at inspection;none claimed.
No firmware MIDI acceptance,AN1x audio or REAPER plugin claim.

Candidate follow-up:[port6 masked byte MMIO](an1x-gpio6.md). Earlier map omissions above describe #120.
