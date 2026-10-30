# AN1x bounded portB byte MMIO

Recorded:2026-10-30. Merged #132 as `5ac2585e9b84497764f1fe0e2bbf426bbbab84a6`;
tested head `66f480ea43b73551e934ec6528ca1dae4a5c5171`.

## Pinned facts and scope

Pinned MAME:`706dbb73281320c54d7c2f16262c1c3786f2be01`.
Rechecked `src/devices/cpu/h8/h83002.cpp`,SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
lines131/132 map FFFFD4 to `ff_r`/`ddr_w` and FFFFD6 to `port_r`/`dr_w`.
Line159 configures portB default DDR00/reserved mask00;forced input00.
Unchanged port implementation and notices: [original extraction](h8-digital-port.md),
[portA](an1x-gpioa.md). No upstream method,instruction body,dependency or Yamaha payload added.

`An1xSciBus` routes these byte addresses to a separate unchanged `H8DigitalPort` owner.
All8 bits remain usable. DDR reads FF;pin reads combine driven latch and external
inputs. Cold state is all inputs;construction emits no implicit reset callback.
Unchanged outputs suppress notification;all-output DDR suppresses input sampling.
GPIO callback ID11 identifies B and carries committed CPU bus time. Addresses alias
at24 bits;callback failures,reentry (even swallowed inner error) and missing sinks
retain existing fail-stop behavior. PortA and all earlier ports remain independent.

Only byte accesses added. GPIO words,including portB plus adjacent holes,fail before
any byte callback. Prior GPIO4/6/8/9/A unmapped-hole checks move from newly mapped
D4/D6 to actual D5/D7/D9 holes;port9's duplicated old hole is replaced by distinct holes.
IRQ/SYSCR modes,alternate pin functions,LCD/DSP/MEG,complete reset and hardware timing
remain open. This is digital-port source behavior,not physical AN1x board acceptance.

## Independent synthetic tests

`tests/an1x_gpiob_check.cpp`:

- All256 cold external patterns and262144 per-bit pin-oracle cases:
  every DDR/latch pair with four external patterns. Complete output data/direction,
  FF DDR reads and input/output callback suppression checked independently.
- GPIO ID11,notification counts and explicit bus times37/41 checked.
- Synthetic CPU DDR0F/latch5A/external15 yields1A in mapped NVRAM and CCR81.
  All241 every-cycle splits compare CPU snapshots,NVRAM,pin/output histories and timestamps.
- Independent owners and port4/6/7/8/9/A isolation,including adjacent portA output
  writes while portB stays driven low.24-bit aliases,unmapped neighbors,word
  rejection,missing sinks and suppressed-inner-error reentry covered.

CPU/generated inventory unchanged:562 methods (558 unchanged/four prior adaptations),
264 selected/0 missing/135 excluded encoding rows;not full CPU/machine acceptance.
Full candidate suite83 CTest/87 Python cases. Release build and82 locally runnable
CTest groups/86 Python cases PASS;one established Windows symlink privilege case
excluded. All five final-head CI jobs and Linux UBSan PASS. Runs41009924793/41009924918/41009924833.
No recorded automated review result at inspection;none claimed. Four documentation corrections also
update the central IR size,historical SLEEP/GPIO limits and README build count.

## Original comparison and mutation controls

Ignored native reference verifies11 verbatim cpp bodies from locked `h8_port.cpp`
(SHA256 `c767e444bebcfb849d0cf225bb8a94e8eec481d62a75c1b554d36a8eda5114ca`)
and inline `ff_r` from locked `h8_port.h`
(SHA256 `50579e13ab71992aee11fcf6372c538afa88689f72806c4dad6e693e67f3b74d`).
200000 xorshift32 operations,seed19A73,compare mapped DDR/data/pin operations,
24-bit aliases,callback counts,data,direction and port ID:zero mismatches.
The unused PCR/ODR/reset bodies are present in the fixture but not exercised here.
Explicit timestamps are independently checked by the target test;this is not
whole-machine MAME or physical-timing equivalence.

Candidate-only ignored mutants:invented portB mask80 causes225082 failed checks;
input routed to portA causes244961. There are two comparison sites per iteration,
so failed-check counts may exceed200000 operations. Positive and both controls
are separate builds;tracked source remains correct. Fixture preparation explicitly
accounts for11 cpp bodies plus inline FF read (12 original bodies in total).

Linux CI includes the new target under UBSan. Local ASan/UBSan not run in this
increment;current evidence must not be confused with prior recorded sanitizer runs.
Firmware/MIDI/PKS,audio and plugin/REAPER acceptance remain open. M0 partial;M1 unaccepted.

Merged #134 follow-up:[bounded GPIO word accesses](an1x-gpio-words.md);earlier word exclusions describe the merged byte-only increments.
