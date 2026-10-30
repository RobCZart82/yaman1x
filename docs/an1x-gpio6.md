# AN1x bounded port6 MMIO

Recorded:2026-10-30. Merged #122 as `804c8a5100ac281945f643c4c9f8f969f263da14`;tested head `32b36079e0a37f420193b410320f7705d728e29e`.

## Pinned facts and composition

Project-owned `An1xSciBus` routing now dispatches existing port4 and new port6
accesses to separate,unchanged `H8DigitalPort` owners. See [port4](an1x-gpio4.md)
and [original port extraction/provenance](h8-digital-port.md). No upstream method
body,instruction,ROM payload or new dependency is added.

Original MAME commit:`706dbb73281320c54d7c2f16262c1c3786f2be01`. Rechecked directly
in the locked archive:

- `src/devices/cpu/h8/h83002.cpp` SHA256
  `2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`:
  advanced byte DDR FFFFC9 uses `ff_r`/`ddr_w`;pins/data FFFFCB uses `port_r`/`dr_w`.
  H8_PORT6 configuration is default DDR80,reserved mask80,forced-input mask00.
- `src/devices/cpu/h8/h8_port.h` SHA256
  `50579e13ab71992aee11fcf6372c538afa88689f72806c4dad6e693e67f3b74d`:
  inline `ff_r` returns FF regardless DDR.
- `src/devices/cpu/h8/h8_port.cpp` SHA256
  `c767e444bebcfb849d0cf225bb8a94e8eec481d62a75c1b554d36a8eda5114ca`:
  existing reserved-bit/pin/output equations and change suppression.

Read pins return bit7 high and lower7 bits from driven latch or external input.
Output data and direction exclude bit7. Changing only reserved DDR/latch bit7
must not create output callbacks. All lower7 outputs suppress external reads
independently of reserved DDR bit7. DDR reads FF,not stored direction.

Callbacks identify port6 and committed bus cycle. Existing absent-callback and
reentry guards remain fail-stop.24-bit aliases normalize before routing. Port4
PCR and port4 pins retain independent state. The previous port4 hole test now
checks FFFFCD instead of newly implemented FFFFC9. Other GPIO,IRQ/SYSCR,timer/ADC,
DMA,panel/LCD/alternate pins remain outside this map. Mixed GPIO/unknown words
reject before byte callbacks. Cold construction does not emit an invented board
reset callback. Full hardware reset and physical timing are still open.

## Evidence

`tests/an1x_gpio6_check.cpp` checks262144 DDR/latch/external combinations against
an independent per-bit7-pin oracle plus reserved-high expectation,output data/
direction,FF reads,input suppression and reserved-bit-only write suppression.
It checks exact port IDs/timestamps,port4/port6/machine isolation,mapped neighbors,
word reads/writes,missing callbacks and suppressed-inner-error reentry.

Synthetic CPU ROM writes DDR0F/latch5A,reads externalA5 and stores AA in NVRAM.
Expected MOV flags are N1,Z0,V0,C/I retained (CCR89).241 every-cycle partitions
compare CPU state,NVRAM and input/output histories including timestamps.
No CPU/generated-handler coverage change is claimed.

Ignored differential fixture uses the same12 verbatim original register/lifecycle
bodies as the port4 reference,configured with default DDR80/mask80. Mapped DDR/
data/pin operations and output updates are exercised (PCR/ODR bodies present
but not mapped here).200000 xorshift32 seed19A73 operations:zero mismatches in
read values,callback counts and output data/direction. No full-machine MAME or
physical-timing equivalence claim. Independent tests separately check port identity.
Removing reserved mask80 in an ignored mutant causes197637 expected failures;
tracked code is unchanged.

Full78 CTest/87 Python PASS;GPIO4/GPIO6/SCI integration ASan/UBSan PASS.
Final-head CI:all five checks PASS,runs40982044271/40982044353/40982044283.
[Automated code review](https://github.com/RobCZart82/yaman1x/pull/122#issuecomment-6512135987) completed with no findings on this head.
No security review result recorded. Source notices remain those of the
unchanged port extraction;source checks alone are not legal release clearance.
Firmware/MIDI/PKS/audio/plugin/REAPER acceptance remains open.
