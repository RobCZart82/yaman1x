# YamAN1x fejlesztési napló

## Standalone upper ADC scan restart — candidate, 2026-11-07

The slow scan model can restart on AN4 after ADST has been cleared.
The new rising edge replaces the pending sample and deadline with the
original first-conversion delay of 266 cycles. AN4–AN7 commit to ADDRA–ADDRD,
preserving previous register values until each new conversion completes.
Subsequent deadlines remain 256 cycles apart. The model retains or clears
ADF according to the written bit. The reference is locked H8_ADC_3337
at MAME 706dbb73281320c54d7c2f16262c1c3786f2be01.

Independent tests check upper scan callbacks, register indices, control
bits and four exact deadlines across 1,035 time partitions, followed by
a restart into the lower bank. The existing lower scan and bus tests
also pass. Next, connect the 0x37 request to the bus callback and repeat
the local firmware diagnostics. Hardware, boot and audio acceptance remain open.

## #240 merged — 2026-11-07

Merge 52d963fe5cd72af911793311eb493ae3e48ab5cd. All four checks passed on
ac315e667a60c39ab1544639035f2ec35d6633e0. Automatic code review completed on
that head with no findings (issue comment 6706807740). No open review threads.
Squash merge used the verified head. All four applicable main checks passed before further work.

## Upper ADC scan request after ADST clear — merged in #240, 2026-11-07

Four read-only 1.04 probes on main c013f56 passed ADCSR=0x13 and reached
ADCSR=0x37 at PC 0x1B38. The synthetic zero-input runs stopped at cycle
127,793,965; the 1023-input runs stopped at 127,794,154. Each pair used
4,096- and 1,000,000-cycle slices and matched its cycle and PC. The
requested cap was 200 million cycles with 20 million callbacks.
The local ROM hash was unchanged and both VOP3 sample counters stayed zero.

The locked H8_ADC_3337 mode_update selects start channel 4 and end channel
7 for 0x37, with repeat and slow conversion timing. Its adcsr_w starts a
new conversion when ADST rises after the preceding 0x13 write. The current
bounded bus accepts only the lower scan and rejects this request. Next,
extend the explicit scan model to AN4–AN7 and verify the new start edge,
result-register mapping and deadlines against the original source.
Physical input calibration, firmware boot, MIDI and audio remain open.

## #239 merged — 2026-11-07

Merge c013f563a459d6681b8394e31672bce3e172cd15. All five checks passed on
a9d1dbb738998fd9b036638511e69f4049582618. Codex automatic code review
completed on that head with no findings, recorded in issue comment 6705892779.
No open review threads. Squash merge used the verified head.
All five main checks passed before the next firmware diagnostic.

## Clear ADST during the slow scan — merged in #239, 2026-11-07

Four traced read-only probes identified ADCSR value 0x13 at cycle
127,793,034, PC 0x1AAC. Each JSON matched its earlier uninstrumented
extended-run baseline byte for byte. The ROM hash was unchanged.

In locked MAME 706dbb73281320c54d7c2f16262c1c3786f2be01,
h8_adc_device::adcsr_w updates ADST and ADF but does not cancel an active
REPEAT mode on this write. timeout continues scheduling channel samples.
The bounded model now retains that deadline after 0x13, exposes ADST clear
and applies the original ADF write-zero behavior. A new start edge while
the scan remains active is a separate unsupported profile.

The bus test checks register readback, unchanged deadline, the following
sample callback and result lane over 1,035 time partitions. Both focused
ADC tests passed. Next, rerun the extended firmware diagnostic. Physical
ADC behavior, firmware boot, MIDI reception and audio remain open.

## #238 merged — 2026-11-07

Merge dbadc1fc5b5c22abc09708e09cfd7de3dd6d047a. All four applicable checks passed on
f1f17694a56d291e0a838716547a63063a5c1e39. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## Extended firmware ADC boundary — merged in #238, 2026-11-07

A temporary diagnostic checkout raised only the local requested cycle cap
to 200 million. The callback cap remained 20 million. Four read-only 1.04
swap16 probes used synthetic ADC values 0 and 1023 and slices of 4,096
and 1,000,000 cycles. All stopped at cycle 127,793,034, PC 0x1AAC, on a
byte write to ADCSR at 0xFFFFE8, with unsupported active ADC scan control.
The ROM hash was unchanged and VOP3 sample counters remained zero.

The earlier 100-million-cycle snapshots remain endpoint observations.
A first attempt to snapshot a failed bus hid the execution result behind
an observer error. Guarding the temporary observer on bus failure exposed
the boundary above. No failed-bus register snapshot is claimed. Next,
trace the requested ADCSR value and verify its original H8_ADC_3337
semantics at pinned MAME commit 706dbb73281320c54d7c2f16262c1c3786f2be01.
Firmware boot, MIDI readiness and audio remain open.

## Serial readiness at the cycle limit — merged in #237, 2026-11-07

On main 3e0dbeb, four read-only 1.04 probes repeated the synthetic low/high
ADC experiments with 4,096- and 1,000,000-cycle slices. All reached the
100-million-cycle limit. Side-effect-suppressed SCI snapshots at the end
matched across all four runs. SCI0 and SCI1 had SMR=00, BRR=0F, SCR=00
and SSR=84. SCR=00 leaves both transmit and receive disabled in the pinned
H8 SCI reference. This is an endpoint observation.

The same snapshots read TSTR=FF, channel 3 TCR=85 and ADCSR=B3. The
ADC scan is active with ADF latched. The local ROM hash was unchanged.
Both VOP3 sample counters remained zero. Next, trace control flow after
ADC setup toward serial initialization and compare readiness at later
bounded milestones. Firmware boot, MIDI reception and audio remain open.

## #236 merged — 2026-11-06

Merge 3e0dbeb8f7c502666c972cf53d8eae8fa1dbc4da. All four applicable checks passed on
c5c27fdfa2d4493c24d1ca47c9173b898c169344. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## Firmware reaches the cycle limit with the ADC scan — merged in #236, 2026-11-06

Four read-only 1.04 swap16 diagnostics on main a0ba565 passed the 0x33
ADC start. The temporary idle-panel probe supplied either 0 or 1023 to
every ADC channel, with 4,096- and 1,000,000-cycle slices for each value.
All reached the requested 100-million-cycle limit without an access fault.
For each value, both slice sizes ended at the same PC and peripheral cycle.
The low profile ended at PC 0x3C736, cycle 100,000,001. The high profile
ended at PC 0x3C50E, cycle 100,000,002. These are synthetic inputs.

The trace showed AN0 sampled at 95,707,121, AN1 at 95,707,387 and the
remaining samples every 256 cycles in AN0–AN3 order, consistent with the
locked H8_ADC_3337 reference. The ROM hash was unchanged. Both VOP3 sample
counts stayed at zero. Next, identify the two end-of-run firmware paths
and which initialization or MIDI readiness milestones they have reached.
Firmware boot, physical ADC behavior and audio remain unverified.

## #235 merged — 2026-11-06

Merge a0ba5650b0d84afe58964365fe3ad81ff46f7355. All five checks passed on
6cf7259a2a43fd895cbb1b5ca82652ef3890e713. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## Slow ADC scan on the AN1x bus — merged in #235, 2026-11-06

An explicit scan callback supplies each AN0–AN3 sample at the machine
deadline. The 0x33 profile exposes aligned ADDR words and byte lanes,
preserves the prior AN3 result until its first scan commit, and updates
ADF at each completed sweep. Reset cancels pending scan events.

The bus test covers 1,035 time partitions with independent sample values,
lane expectations and exact deadlines. The pinned H8_ADC_3337 model below
defines the timing. A fresh firmware probe with explicit scan inputs is
the next step. Board calibration, boot and audio acceptance remain open.

## #234 merged — 2026-11-06

Merge ea172cf428ce4b35c699279aa657f0cac18e4359. All five checks passed on
dd9a48420734ba69be1ff7b7727cbb356e400617. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## Standalone slow AN0–AN3 scan — merged in #234, 2026-11-06

The bounded 0x33 scan samples AN0 at start, commits after 266 cycles
and then commits each successive channel every 256 cycles. The caller
supplies a 10-bit sample for the next channel at each deadline. ADDR
words retain their last committed values. AN3 completion sets ADF and
wraps to AN0 while ADST stays set. ADF can be cleared independently.
This subset uses the locked H8_ADC_3337 reference below.

The independent test checks two sweeps across 2,059 time partitions,
including result packing, ADF timing, reset and invalid event rejection.
Next, integrate the scan into the bus and timeline with explicit sample
inputs. Firmware boot, physical ADC behavior and audio remain open.

## #233 merged — 2026-11-06

Merge c11f45a36299ce90d7f1ee9ad0983b6f9979bae9. All four applicable checks passed on
05657b2f7f4c94ebf22f1bb0d5be029d481f44d4. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## Firmware A/D scan request — merged in #233, 2026-11-06

All four read-only firmware probes wrote ADCSR 0x33 at 0xFFFFE8,
cycle 95,707,121, PC 0x1B84. Each traced diagnostic JSON was identical
to its earlier baseline. The probes covered synthetic AN3 values 0 and
1023 and 4,096- and 1,000,000-cycle slices. The local ROM hash was unchanged.

The locked original H8_ADC_3337 at MAME commit
`706dbb73281320c54d7c2f16262c1c3786f2be01` interprets 0x33 as a
software-started, repeating slow scan of AN0 through AN3 with ADIE clear.
It samples AN0 on start and commits after 266 cycles. Subsequent channels
commit every 256 cycles and the following channel is sampled at each commit.
The first ADF assertion is 1,034 cycles after start, when AN3 commits.
ADST remains set and the scan wraps to AN0. Each later sweep takes 1,024
cycles. These are original-model timings. Next, test this scan in isolation
with explicit synthetic values, then consider bus integration. Board analog
behavior, firmware boot, audio and REAPER acceptance remain open.

## #232 merged — 2026-11-06

Merge d6a0751da9ddab9a39b77455587669f1a0679e7f. All five checks passed on
c86b1d34beb6786e34bfa4e08f3f9a476cf3567a. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## Aligned ADDRD word read — merged in #232, 2026-11-06

The explicit AN3 profile now composes the aligned word at 0xFFFFE6 from
the two existing result bytes, in big-endian order. The locked original
H83002 map and ADC source define the byte lanes and 10-bit packing.
Tests check the value before and after conversion, the 24-bit alias,
alignment, nearby addresses, read-only behavior and idle-profile guards.

Four local read-only 1.04 probes with synthetic samples 0 and 1023 and
4,096- and 1,000,000-cycle slices passed the word read. All stopped at
cycle 95,707,121, PC 0x1B84, on a byte write to 0xFFFFE8 with unsupported
ADC control mode. The ROM hash was unchanged and both VOP3 sample counters
remained zero. Next, trace the new control value. Physical AN3 input,
firmware boot, audio and REAPER acceptance remain open.

## #231 merged — 2026-11-06

Merge c94aa251152a5ac854ccaae478508404e9db812d. All four applicable checks passed on
e8cb4618f16fc63e5e3d2b3eb9d01d6988f89a06. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## AN3 board input and ADDRD word boundary — merged in #231, 2026-11-06

Yamaha DM/DMS drawing 8811454 connects IC1 pin 83 (P73/AN3) to H8A15
through R22 (10 kΩ), with C20 (0.01 µF) from the input to DGA. This
identifies an RC-filtered address line. Its voltage at the actual sample
instant depends on bus history and remains unmeasured.

A temporary probe in a separate checkout supplied explicit 10-bit values
0 and 1023. For each value, the read-only 1.04 swap16 diagnostic ran with
4,096- and 1,000,000-cycle slices. All four runs sampled at cycle
95,706,795 and reached cycle 95,707,083, PC 0x1B6E. Each then stopped on
a 16-bit ADDRD read at 0xFFFFE6. The two sample values probe continuation
only. Both VOP3 sample counters remained zero, and the ROM hash was unchanged.

The original H83002 map at locked commit
`706dbb73281320c54d7c2f16262c1c3786f2be01` maps ADDR as eight bytes.
Its original `addr8_r` packs the two ADDRD bytes as result bits 15–6.
Next, test an aligned ADDRD word composed from those two lanes and rerun
the bounded diagnostic. Board voltage and timing, firmware boot, audio
and REAPER acceptance remain open.

## #230 merged — 2026-11-06

Merge 7740d0154b3c6ebe68ef601fdcb1509efb1abdb8. All five checks passed on
73717eb68d39390ff7ee6b5bb8eb58302c7826d0. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## AN3 conversion on the machine timeline — merged in #230, 2026-11-06

A sink can supply a 10-bit AN3 sample through the optional
`adc_an3_sample(cycle)` callback. The bus samples on the ADCSR start
write at FFFFE8, schedules the pinned 266-cycle completion and exposes the
result at FFFFE6–FFFFE7. CPU deadline queries see that event. SCI and ITU
run before ADC on ties under project policy. Default sinks keep the idle
ADC profile. Other result channels, ADCR, word accesses and ADC IRQ stay
outside this profile.

Synthetic tests verify the callback cycle, result lanes, aliases, flag
clearing, reset, strict addresses and continuation over 268 bus and 321
CPU partitions. The original H8_ADC_3337 reference remains pinned below.
Next, trace the board AN3 input and choose an explicit diagnostic source
before running the local firmware again. Physical calibration, firmware
boot, audio and REAPER acceptance remain open.

## #229 merged — 2026-11-06

Merge 15082c08dda331d572d83bc5a7707fa60da537a1. All five checks passed on
6a9bee24725b00dfbdad8825917dad818ffb7853. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## Standalone slow AN3 conversion — merged in #229, 2026-11-06

The standalone model accepts the observed single AN3 start with an explicit
10-bit sample. It commits the latched sample after 266 cycles, sets ADF,
clears ADST and exposes both ADDRD byte lanes. A repeated start while busy
does not sample again. ADF uses the original write-zero behavior. Reset
clears the result and cancels a pending conversion. IRQ enable, fast
conversion, scan and abort during a conversion are outside this profile.

The ROM-free test checks all 1,024 result values and 268 time partitions
against independently calculated byte values, status and deadlines. The
reference is the locked H8_ADC_3337 source documented below. The next step
is MMIO and machine-timeline integration with an explicit analog input.
Board analog values, firmware boot, audio and REAPER acceptance remain open.

## #228 merged — 2026-11-06

Merge 393b132bc4035cda031fbbeda8ab682e0c7ed065. All four applicable checks passed on
e2f61de1ea8248e655f9f4e4b1162ebe36f67be5. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## Firmware A/D start boundary — merged in #228, 2026-11-06

A temporary write trace in a separate checkout recorded ADCSR value 0x23
at 0xFFFFE8, cycle 95,706,795, PC 0x1B64. Both 4,096- and 1,000,000-cycle
slices produced the same write value, and each diagnostic JSON matched its
uninstrumented baseline byte for byte. The local 1.04 ROM was opened
read-only and its hash was unchanged.

The locked MAME commit `706dbb73281320c54d7c2f16262c1c3786f2be01`
maps ADDR bytes at FFFFE0–FFFFE7, ADCSR at FFFFE8 and ADCR at FFFFE9,
using H8_ADC_3337 with interrupt vector 60. The ADC source SHA-256 is
`68577b708cac2233a31c1dfa5a4e83162a0db295d6d92805002f29ad4334be19`.

In that source, 0x23 starts a software-triggered single conversion on
AN3 with the slow clock and the interrupt disabled. The conversion
samples at start, then after 266 CPU cycles commits the result to ADDRD,
sets ADF and clears ADST. The register exposes the 10-bit result in bits
15–6. ADCSR preserves ADF only when the old flag and the written bit are
both set. These are original-model semantics. Board analog values and
physical timing remain open. The next step is a bounded single-conversion
ADC test using explicit synthetic input and independently checked result
bytes, status and deadline. Firmware boot, audio and REAPER remain open.

## #227 merged — 2026-11-06

Merge a3d365c4737ddbb43771d69b5c2e9f3395ecef1a. All five checks passed on
2b5a6c81deae56a6911f48efdefe6a2e30b9cec9. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## Idle panel byte diagnostic — merged in #227, 2026-11-06

Yamaha DM drawing 8811454 shows IC6's second decoder enabled by H8CS3,
with RD and A19 on its select inputs. At 0x680000 in H8 area 3, A19 is
high and a read asserts RD low. The decoder selects PNLCS from 2Y2.
PNLCS enables IC19, which buffers the pulled-up S1–S8 inputs onto D8–D15.
The even byte selects that upper lane. The pinned original MAME driver
at `706dbb73281320c54d7c2f16262c1c3786f2be01` only maps ROM and NVRAM,
so it supplies no panel read value.

The explicit idle panel experiment returns FF for the observed byte
read at 0x680000, representing all switches released. It preserves the
previous profiles' strict boundary and leaves writes, words and nearby
addresses unmapped. Synthetic CPU tests check the value, 24-bit alias,
access width, adjacent addresses and continuation across three slice sizes.

With the 1.04 swap16 ROM opened read-only, both 4,096- and 1,000,000-cycle
slices passed the panel read and stopped at cycle 95,706,795, PC 0x1B64,
on a byte write to 0xFFFFE8 (unsupported ADC conversion or IRQ enable).
Both VOP3 sample-pass counters stayed at zero. The ROM hash was unchanged.
Next, characterize the ADC request. Panel scanning, hardware timing,
firmware boot, audio and REAPER acceptance remain open.

## #226 merged — 2026-11-06

Merge a6a294fa43266335e324fc502e73725cf92bac23. All five checks passed on
bfc73f303c9a2bbb0c82ad7b05ea39eaf5dd9b49. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All five main checks passed before further development.

## ITU input B without PKSCLK transitions — merged in #226, 2026-11-06

The AN1x research bus accepts the observed zero-based channel 3 input B
selection with automatic clear disabled. TSTR retains its start bit while
the channel has no internal deadline or clock ticks. The count and latched
flags remain accessible. Switching back to an internal clock resumes from
the TCR write cycle. The locked original MAME dispatch harness verifies
65,536 external-clock cases without input transitions. The bus test adds
65 time partitions covering the address alias, mixed channel progress,
flag retention, restart, restored internal deadline and reset.

The read-only 1.04 swap16 firmware diagnostic used the LCD/MEG/VOP3 level
experiment and 4,096- and 1,000,000-cycle slices. Both passed the earlier
FFFF60 write and stopped at cycle 92,381,680, PC 0xEA98, on an unmapped
byte read at 0x680000. Both recorded VOP3 register writes of 11,557 and 516
and zero VOP3 sample passes. The ROM hash was unchanged. The next step is
to identify the 0x680000 read from board and source evidence. PKSCLK
hardware timing, firmware boot, audio and REAPER acceptance remain open.

## #225 merged — 2026-11-06

Merge 944df7d3b66f9cdef24fa5b9336ab9e2aeaefdda. All four applicable checks passed on
fac7f85da6f625d42915c89ead1e4bcf412a080b. GitHub reported no conflicts or review
threads and required zero approving reviews. No automated review result or
quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## PKSCLK board path — merged in #225, 2026-11-06

The Yamaha AN1x Service Manual DM/DMS drawing 8811454 routes PKSCLK
from H8 IC1 PA1/TCLKB and IC21 SK (pin 11) through L29 to DM CN6 pin 5.
KBS drawing 8811457 continues from KBS CN1 pin 5 to the HD63B05V0F PKS
IC1 CK/D5 pin 19. CL1 is a separate 8 MHz resonator on XTAL pins 24 and
25. These connections establish the signal path. Serial clock direction
and edge timing require separate evidence.

The locked original MAME driver at commit
`706dbb73281320c54d7c2f16262c1c3786f2be01` disables the PKS CPU and marks
its firmware undumped. Its 8 MHz setting describes the PKS CPU oscillator.
The next step is an explicitly bounded no-edge profile for channel 3 input
B, followed by the same two local boot slices. That diagnostic can show
what the H8 firmware does without PKS clock transitions. Physical clock
timing, PKS protocol, firmware boot and audio acceptance remain open.

## #224 merged — 2026-11-06

Merge 38b3cbfbdcc0280997ef41bc01a2ff868f641a1f. All four applicable checks passed
on 911852d385af52ed19b4dc1efaacdba915b6b035. GitHub reported no conflicts or
review threads and required zero approving reviews. No automated review result
or quota error was recorded. The verified SHA was used for the squash merge.
All four applicable main checks passed before further development.

## ITU external clock B reference — merged in #224, 2026-11-06

The [Hitachi HD6413002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual?r=1052021)
Figure 1-3 identifies FP-100A pin 96 as TCLKB/TEND1/TP1/PA1. The Yamaha AN1x
Service Manual identifies IC1 pin 96 as PA1. The DM/DMS circuit drawing
8811454 shows that pin on PKSCLK, with a 10 kΩ pull-up to +5D through R11.
The drawing identifies the signal path. Its transition timing remains open.

The dispatch harness ran 65,536 external-clock cases against unchanged
counter and scheduler methods from the locked original MAME source. With
internal time advancing and no external input, the reference cancels the old
internal deadline, preserves the count, flags and clock anchor, and emits no
interrupt. This is evidence for a bounded no-edge profile.

## #223 merged — 2026-11-06

Merge 2a96101e1adedb09bea49a61662f9273d444666d. All four applicable checks passed
on 60c6082c27f175cfc000381f3ba1a893173c5483. No automated review result or quota
error was recorded. Manual review checked the documentation and ledger against
both local diagnostic outputs. GitHub reported no conflict or open review
threads and the ruleset required zero approving reviews. The verified SHA was
used for the squash merge. All four applicable main checks passed before external-clock work.

## Local firmware diagnostic after ITU IRQ integration — 2026-11-06

The #222 merge (32c5eee) passed all five main checks. Its local 1.04 probe used
the swap16 view and the LCD/MEG/VOP3 level experiment with a 100-million-cycle
limit and 20-million-callback limit. Both 4,096-cycle and 1,000,000-cycle slices
stopped at peripheral cycle 11,925,102, PC 0xB60, on a byte write to FFFF60.
The error is now Unsupported active ITU clock or clear mode. A temporary local
diagnostic reproduced both baseline JSON results byte for byte and identified
zero-based channel 3, clock selector 5 and clear selector 0. The locked H8H
TCR method maps that combination to external input B without automatic clear.
The local firmware hash was unchanged after the probes.

Both runs report no DSP sample passes and leave firmware boot and audio
acceptance open. Next, verify the pinned reference behavior and board evidence
for channel 3 external clock B before extending the bounded profile. The
reference counter and deadline methods skip internal scheduling for external
clock types. That behavior does not establish the physical input waveform.


## #222 merged — 2026-11-06

Merge 32c5eee199c65411217274e7d675b0e3d62ce3ee. All five checks passed on
c332b4a4ef038d99508f311da5650ef52b6f4534. No automated review result or quota
error was recorded. Manual inspection covered timeline order, the DMA gate and
event budget. GitHub reported no conflict or open review threads. The ruleset
required zero approving reviews. Squash merge used the verified SHA.
All five main checks passed before the local firmware diagnostic.

## Active ITU interrupt delivery — candidate, 2026-11-06

Enabled ITU events now enter the shared bus timeline and CPU deadline query.
At a tie the existing SCI input and SCI events run first, followed by one ITU
timestamp in channel and compare/overflow order. This ordering is project
policy. The shared event limit counts each ITU timestamp once. Each vector
offers the existing DMA callback its absolute cycle before publication to the
H8 interrupt controller. A consumed event still latches the timer status.
Timer reset clears the timer state and the existing controller reset clears
pending interrupts.

Tests verify five vectors at exact cycles, polling flags, DMA consumption, event
budget exhaustion, and a synthetic CPU waking from SLEEP to execute the timer
handler across 221 split points. The pinned MAME dispatch harness remains the
counter and vector reference. This bounded internal-clock profile still needs a
fresh local firmware diagnostic and hardware acceptance.

Baseline #221 (aec0396) is merged and all five main checks passed.
Local validation passed 134 CTest groups, the rebuilt TIER/IRQ target,
targeted ASan/UBSan, the locked MAME dispatch cases and both audits.
Remote CI and automated review are pending.


## #221 merged — 2026-11-06

Merge aec03964267ff94eb17dbd78a6646168436444f0. All five checks passed on
8cad9cfccef94cdf9b027585b0e58116a8659bf7. No automated review result or quota
error was recorded. Manual inspection covered the start, stop, polling guard
and reset. GitHub reported no conflict or open review threads. The ruleset
required zero approving reviews. Squash merge used the verified SHA.
All five main checks passed before further development.

## Active ITU polling profile — candidate, 2026-11-06

Byte accesses at FFFF60 expose TSTR with an E0 reset value and full written-byte
readback. The low five bits start and stop individual scheduler channels at the
current bus cycle. Internal clock and free/A/B-clear modes are supported with
interrupt enables clear. Starting with IRQ enabled, enabling IRQ while running,
and applying unsupported active clock/clear modes fail closed. Bus time advances
active counters and polling flags. Supported reset stops the timers and clears
their counters and flags.

Tests cover all 256 start masks, exact start/stop and wrap boundaries, compare
and overflow flags, active reconfiguration, reset, and 129 split CPU runs with
a synthetic TSTR write. The pinned source contract verifies original TSTR and
channel-enable methods. This is a bounded polling profile. CPU IRQ delivery
and the local firmware diagnostic at FFFF60 remain subsequent steps.

Baseline #220 (a6a8643) is merged and all five main checks passed.
Local validation passed all 134 CTest groups, the rebuilt TSNC/TSTR target,
targeted ASan/UBSan, the pinned source contract and both audits.
Remote CI and automated review are pending.


## #220 merged — 2026-11-06

Merge a6a86432ff4328416ca95fadb822a822b2c7fad4. All five checks passed on
823bad70db52313dcb9c41e94170f6b709274af5. No automated review result or quota
error was recorded. Inspection of counter addresses, byte lanes, reset and
tests found no actionable issue. GitHub reported no conflict and no open
review threads. Zero approving reviews were required. The squash merge used the
verified SHA. All five main checks passed before further development.

## Stopped ITU counter MMIO — candidate, 2026-11-05

The bus now maps all five stopped TCNT words and their big-endian byte lanes to
the scheduler's counter state. Writes settle the scheduler at the current bus
cycle. Stopped counters retain their values as time advances and clear to zero
on supported peripheral reset. Odd word accesses reject before mutation.

The bus tests check all 327,680 TCNT word values across the five addresses,
both byte lanes, address aliases, channel isolation, reset and continuation
across 129 CPU slice splits. The existing TGR checks remain in place. The pinned
MAME contract verifies the original TCNT methods and the counter reference
checks 131,072 original update_counter comparisons. This step is bounded to
stopped channels. Timer start and CPU interrupt delivery are subsequent steps.

Baseline #219 (bf58ebc) is merged and all five main checks passed.
Local checks passed: 134 CTest groups, a separate rerun of the final expanded
TGR/TCNT test, targeted ASan/UBSan, the pinned MAME references and both audits.
Remote CI and automated review are pending.


## #219 merged — 2026-11-05

Merge bf58ebc1ed5ca0c3220d2170f11ecc91d4a29411. All five checks passed on
25a4059b892132c7672fdac405b8df7ea8724767. No automated review result or quota
error was recorded. Inspection of register mapping, byte lanes and reset found
no actionable issue. Review threads were empty and GitHub reported no conflict.
The ruleset required zero approving reviews. Squash merge used the verified SHA.
All five main checks passed before further development.

## Stopped ITU compare and interrupt registers — candidate, 2026-11-05

The stopped ITU scheduler now owns the values behind the ten TGR words and five
TIER/TSR pairs. Bus accesses retain their established address and width rules,
including big-endian byte writes to TGR, normalized reserved bits, and reset
values. Writes settle the stopped scheduler at the current bus cycle. TSTR,
active counter MMIO, and CPU interrupt delivery remain later integration steps.

Existing tests cover all 655,360 TGR word values and both byte lanes, all
TIER/TSR byte values at all five channels, reset, address aliases, invalid word
accesses, and split CPU execution. The pinned MAME source contract covers the
original TGR and TIER/TSR methods. This is a bounded source-compatible profile.
Baseline #218 (4e9c91f) is merged with all five main checks green.
Local validation passed 134/134 CTest groups (62.34s), targeted ASan/UBSan,
the pinned MAME references, and both repository audits. Remote CI and review
are pending.


## #218 merged — 2026-11-05

Merge 4e9c91f84352a429a2a68e8f67187d9f3226f3f9. All five checks passed on
277906615f7ece1373d0928b61aaac09ee5ab7f5. No automated review result or quota
error was recorded. Inspection of the bus mapping, reset and stopped-state
tests found no actionable issue. No open review threads or conflicts. The
ruleset required zero approving reviews. Squash merge used the verified SHA.
All five main checks passed before further development.

## Stopped ITU TCR bus integration — candidate, 2026-11-05

Writes to the five byte-mapped TCR registers configure the corresponding stopped
scheduler channel at the current bus cycle when the locked source profile is
supported. All register encodings retain the existing bit-7-normalized readback.
Unsupported encodings expose no runnable profile. Cold and supported peripheral
reset reapply the internal free-running profile at the current epoch. TSTR,
active counter MMIO, and CPU interrupt delivery remain separate acceptance gates.

The bus test checks all 256 values at each of the five TCR addresses, including
24-bit aliases, channel isolation, reset, and the absence of events while stopped.
The existing CPU continuation checks and locked original TCR differential remain
in place. This is a bounded source-compatible profile. Hardware acceptance and
the firmware rerun at FFFF60 remain open.

Baseline #217 (d06b064) is merged with all five main checks green.
Local validation: 134/134 CTest groups (63.48s), targeted ASan/UBSan, all 256
original TCR encodings, repository audit and notice audit passed.
Candidate CI and automated review are pending.


## #217 merged — 2026-11-05

Merge d06b064eda188296e7f87bc99aa4d415b5890394. All five checks passed on
d7bac7b43f49e0e4ef08c57a6835c1ceda1b3515. No automated review result or quota
error was recorded. Manual inspection of the decoder and original reference
harness found no actionable issue. No open review threads or conflict. The
ruleset required zero approving reviews. Squash merge used the checked SHA.
All five main checks passed before further development.

TCR candidate validation134/134 CTest PASS(57.30s),103 Python PASS,strict
ASan/UBSan target PASS,256 original-method cases PASS. Audit673 files,
notice229 files/5 texts,diff-check PASS. CI sanitizer selection53 groups.

## ITU TCR source-profile decoder — candidate,2026-11-05

Pure decode_itu_control maps96 supported internal-clock/no-external-clear TCR
values to divider,phase and free/A/B clear mode. All256 encodings are checked.
The locked H8H source compares raw count_type0..3 against enum DIV_2=6,
therefore phase stays zero even with edge-selection bits. This source behavior
is explicit and awaits hardware validation. Other encodings return empty.
Original unchanged tcr_update differential passes all256 encodings under
ASan/UBSan. Its exact method hash is pinned in the checker,whole-file identity
and archive lock verified. Counter integration probes cover all96 accepted
profiles. Bus/MMIO writes and CPU IRQ routing remain next acceptance gates.
Candidate134 CTest/103 Python. Own remote CI/review pending.
Merged baseline #216,189df0e,all five main checks PASS.


## #216 merge reconciled — 2026-11-05

GitHub confirms merge189df0efc99baf128631e4472a77fec349bd9226 at18:19:25Z.
Exact PR head a52cecc71b18e87b0e40b150eb884e8cb9abbe1d has five successful
checks and completed automated review without findings,comment6665151486.
Review threads empty. Fresh main pulled. Three main checks successful,Ubuntu
and research checks running. Next development waits for completion.

## Selectable-mode validation — 2026-11-05

133/133 Release CTest PASS(54.51s),103 Python PASS,targeted strict ASan/UBSan
PASS. Original reference2048 mixed streams,10240 single-channel and131072
A/B auto-clear values/flags PASS under ASan/UBSan. Audit670 files,notice229
files/5 texts,diff-check PASS. Linux sanitizer selection52 groups.
Own remote CI and automatic review pending.


## Selectable per-channel ITU clear mode — candidate,2026-11-05

The shared scheduler now accepts independent free-running,A-clear and B-clear
channel modes through guarded settled-time writes. Default A-clear and the
existing0/1 source API are preserved. Mode switches settle the old configuration
and retain count,flags,enables and phase. Snapshot clear_index2 means free run.
Independent65536 free-running cases,2304 transitions,65 switch partitions and
an exact mixed-mode vector/time fixture PASS under targeted ASan/UBSan.
Original locked methods pass2048 mixed-channel streams,10240 single-channel
and131072 existing auto-clear value/flag cases. [Contract](h8-itu-clear-mode.md).
The candidate defines133 CTest/103 Python cases. Own CI/review pending.
Merged baseline:#215,2dfbc0eb73c2f20aa961684cff6e2b1c06bf2131,all five main
checks successful. Next supported TCR/MMIO decoding and CPU IRQ routing.


## #215 merged — 2026-11-05

Merge2dfbc0eb73c2f20aa961684cff6e2b1c06bf2131,exact head
6929aac9c7a577302b02b0d3a69d32ca01c99a82. All five CI checks successful.
Automatic review result absent and quota error unconfirmed. Primary agent
inspected scheduler/tests/reference and diff without actionable findings.
Live rules require zero approving reviews;review threads empty,mergeable clean.
Merged using expected SHA and normal squash endpoint. Fresh main fetched.
Main CI pending before any next development.


## Scheduler publication recovered — 2026-11-05

A subsequent normal push succeeded. Candidate branch now available remotely.
The two earlier commit_refs_rules rejections remain historical observations.
Remote CI and automatic review await PR creation.

## Scheduler publication failure — 2026-11-05

Two normal pushes of the candidate branch were rejected by GitHub with
fatal error in commit_refs_rules. No open PR exists. Local tested commit
c35abb6 remains available. Publication waits for remote recovery. The exact
cause is unconfirmed. Default branch ruleset remains active.


## Scheduler candidate validation — 2026-11-05

Release build and132/132 CTest PASS(92.31s),103 Python PASS,targeted strict
ASan/UBSan PASS. Original-method differential2048 five-channel streams and10240
single-channel cases PASS. Source audit668 files,notice229 files/5 texts and
diff-check PASS. CI sanitizer selection51 groups. Remote CI/review pending.


## Shared five-channel ITU scheduler — candidate,2026-11-05

H8ItuChannelScheduler merges five bounded channel owners by absolute deadline.
A shared edge consumes one budget unit with at most15 vector callbacks.
Equal timestamps use channel order then A/B/overflow as explicit project policy.
Callback time and scheduler epoch agree. Budget/callback/reentry failures stop
all future dispatch. Reconfiguration occurs at settled time.
33 split points,simultaneous vectors,stop/reconfigure and auto-clear recurrence
are checked independently. Locked original MAME methods pass2048 five-channel
stream comparisons plus10240 existing value/flag/vector/time comparisons under
ASan/UBSan. [Contract](h8-itu-channel-scheduler.md).
CPU IRQ delivery,mixed-mode MMIO and fresh firmware diagnostics remain next.
Remote CI and automatic review pending. Merged baseline is7b792ca (#214),with
all four applicable main checks successful. The candidate defines132 CTest
and103 Python cases. The previous reset-fix evidence below remains historical.


## Javítás lezárva — #213, 2026-11-05

Beolvadt: [#213](https://github.com/RobCZart82/yaman1x/pull/213),
merge `d60e6eb9907d0a9c89938e1677d68e54b319da74`,
tesztelt head `a421a261a6647e759dac7d5132567d82f2de32fc`.
Időpont: 2026-11-05 10:09:33 Europe/Budapest (09:09:33 UTC).
[Linux/macOS/Windows, UBSan és ASan/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686134),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686447),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686368): mind az öt feladat sikeres.
Automatikus review észrevétel nélkül lezárult a `95b7edb0feaa97be23d1105fe59b6f259596e640`
forráscommiton; utána csak három dokumentum PR-hivatkozása változott. A végső headre
külön review-t nem állítunk. Nyitott review-szál nem volt a beolvasztáskor.

A teljes külső LCD/MEG/VOP3 cold reset alapreferencián keresztül is működik,
és az alapmutatós felszabadítás teljes. Reprodukció: javítás előtt megtartott
MEG/VOP3 állapot miatti kudarc; javítás után két alapnézetes reset és két
alapmutatós destruktorpróba PASS. Helyi Release/130 futtatható CTest PASS;
Python 102/103 PASS, egy WinError1314 symlink-jogosultsági akadály. Teljes
CI-készlet: 131 CTest/103 Python. A sanitizer-bizonyíték a fenti Linux CI.

Nincs aktív motorjelölt. Ez a lezáró egyeztetés dokumentációs változás;
saját ellenőrzései a kapcsolódó PR-en követhetők, a fenti CI a #213-hoz tartozik.
Következő: aktív ITU gépütemező/MMIO- és CPU IRQ-bekötés, majd friss helyi
firmware-diagnosztika. M0 részleges; boot/MIDI/hang/plugin/REAPER elfogadás nyitott.

## Forrásaudit és cold-reset javítás — 2026-11-05

Ellenőrzött motoralap: #212, `5915ccb54f07f7649192ea3dbfa4277c56f0ce01`.
A tesztelt head `28f81562685616cb89bdf96f269c62bcc9a375de`; a platform/sanitizer,
MSan és MAME futások sikeresek (41922483350 / 41922483298 / 41922483383).
Automatikus review-eredmény az ellenőrzéskor nem volt rögzítve.
131 CTest/103 Python eset. A korábbi #211 „jelölt” megjegyzéseit történeti
adatként őrizzük; #211 és #212 beolvadt. A közös SYSCR (#141) és a korlátozott
reset (#168–169) már elkészült, a teljes hardver-reset és boot nincs elfogadva.

Reprodukált hiba: az An1xVop3ResearchSink LCD- vagy MEG-alapreferenciáján hívott
cold_reset csak az alapréteget nullázta; a MEG/VOP3 állapot és számlálók megmaradtak.
A cold_reset most virtuális, a leszármazottak override-ot használnak, az alap
virtuális destruktort kapott. Így az alapmutatón birtokolt teljes adapter is
felszabadul. Két alapnézetes resetpróba ellenőrzi a teljes állapotot, az új
nullaidős korszakot és a számlálókat; két alapmutatós tulajdonosi próba a teljes
objektum megsemmisítését. A teszt javítás előtt elbukott, utána PASS.
Ez a közös külső adapterfelület javítása; a jelenlegi konkrét típusos CLI-út
ugyanezt a cold resetet eddig is közvetlenül hívta.

Aktív javítás: [#213](https://github.com/RobCZart82/yaman1x/pull/213), fix/research-sink-cold-reset. Release build és 130/130 helyben
futtatható CTest PASS (53,85 s). Python: 102/103 PASS, egy ismert symlink-esetet
a Windows-jogosultság akadályozott (WinError1314). Forrásaudit: 665 fájl/201
upstream azonosság PASS; licencértesítés: 229 fájl/5 szöveg PASS, jogi/bináris
engedélyezést nem jelent. Belső linkek/UTF-8/napló-ősök PASS. Helyi sanitizer
nem futott. Saját távoli CI/review függőben; korábbi CI nem ennek az eredménye.
Következő motorlépés: aktív ITU ütemező/MMIO és CPU IRQ-bekötés; utána friss helyi
firmware-próba. PKS/MIDI, DSP/hang/plugin/REAPER elfogadása nyitott. Yamaha-adatot
nem használunk; EG target-hold módosítás továbbra is hardveres bizonyítékhoz kötött.

## Történeti fejlesztési és előkészítési bejegyzések

Az alábbi korábbi „jelölt”, „függőben” és „következő” állítások a saját dátumukra
vonatkoznak; az aktuális feladatsort a fenti összefoglaló és az M0-szakasz tartalmazza.

## Dispatch candidate validation — 2026-11-04

131/131 CTest PASS(54.12s),103 Python PASS,strict targeted ASan/UBSan PASS;
10240 unchanged original dispatch comparisons PASS under ASan/UBSan.
Audit665 files,notice229 files/5 texts,diff-check PASS;CI sanitizer50 groups.
No ROM accessed. Own remote CI/review pending;not merged.


## Bounded ITU dispatch — candidate,2026-11-04

H8ItuBoundedDispatch owns one supported channel and emits exact-time A/B/overflow
vectors for channels0–4. Timestamp budget bounds work;simultaneous events use one
budget unit and at most three callbacks. Reentry,callback failure or exhausted
budget permanently stops the owner. Mutations occur only at its settled time.
Independent vector/time fixtures,129 split points,reconfiguration and fail-stop
controls;10240 original MAME dispatch/vector/time/count/flag comparisons PASS
under ASan/UBSan. Three unchanged methods extracted from the locked archive only
into a private temporary harness. CPU interrupt-controller delivery,MMIO start,
firmware boot and audio acceptance remain open. Next selected bus/CPU integration.
Own remote CI/review pending;candidate is separate from merged baseline.

Merged baseline:#211,f00587194b70ba3a7489da9a486f9ac6cba65ce2,130 CTest/103 Python;
all five PR and main checks PASS. Automatic review completed without findings
on3c2c51af1d89da2df5c98d5310383d826e212214;see development ledger evidence.
Older candidate sections below are historical records,not current open PRs.


## #211 merged — 2026-11-04

Mergef00587194b70ba3a7489da9a486f9ac6cba65ce2;tested head
3c2c51af1d89da2df5c98d5310383d826e212214. Five checks passed
(runs41908190943/41908191069/41908191372). Automatic code review completed
without findings on that head at2026-11-04T20:37:38.555561Z,
https://github.com/RobCZart82/yaman1x/pull/211#issuecomment-6641489795.
No unresolved threads/conflict. Fresh main pulled;all five main checks PASS.
User explicitly reactivated the20-minute sequential heartbeat after prior
capacity recovery. Next bounded exact-time dispatch before CPU/MMIO integration.

## Pure ITU next-event deadline — candidate #211,2026-11-04

Validation:130/130 CTest PASS (58.89s),103 Python PASS;extended final
deadline test rebuilt and targeted Release/strict ASan/UBSan PASS,including
invalid snapshot checks. Strict compiler PASS;original unchanged recalc_event
131072 deadline comparisons PASS under ASan/UBSan. Audit661 files,notice229
files/5 texts,diff-check PASS;CI sanitizer49 groups. No ROM accessed.
Own remote CI/review pending.

New next_itu_event computes the next enabled A/B/overflow event from supported
free-running/auto-clear snapshots without advancing or mutating state. Equal
edge distances combine event bits. Disabled/no reachable enabled event returns
empty;already latched status does not suppress recurring events. Absolute
phase/divider boundaries are used;unrepresentable future time throws without
mutation. Unsupported snapshot clock/cycle values reject. No IRQ dispatch,
CPU scheduler or MMIO integration added.
2048 independent tick searches plus pure/simultaneous/recurring/time-overflow
controls.131072 deadline comparisons with unchanged locked recalc_event PASS
under ASan/UBSan,explicit current time and no CPU callback delivery. Original
source only in private temporary harness. Next bounded dispatch and precise
vector/time/reconfiguration tests before bus start. No ROM/boot/audio acceptance.
Own remote CI/review pending.


## Internal-clock ITU auto-clear events — candidate #210,2026-11-04

Validation:129/129 CTest PASS (53.47s),103 Python PASS;targeted strict
ASan/UBSan and -Wall -Wextra -Werror PASS. Locked unchanged update_counter
131072 auto-clear value/flag comparisons PASS under ASan/UBSan.
Audit657 files,notice229 files/5 texts,diff-check PASS;CI sanitizer48 groups.
No ROM accessed. Own remote CI/review pending.

New H8ItuAutoClear composes the existing counter/status for A/B-selected auto-
clear. Cycle is selected TGR+1 in32 bits;comparison target is wrapped16-bit
TGR+1. Tick distances capture A/B matches and overflow with IRQ disabled,
including starts beyond clear boundary and TGRFFFF. Mutations settle old time;
invalid configuration/index/time preserves state. No external clear/capture,
deadline,IRQ delivery or MMIO start. Existing free-running component unchanged.
1048576 independent initial/cycle/target tick cases,4128 A/B-source/phase/splits,
polling/reconfigure/FFFF/uint64 tests.131072 comparisons with locked unchanged
update_counter pass for values/flags,IER0;original source only in temporary
private harness. Initial fixture assumed immediate clear after lowering threshold
below current count;fixed to check continued count until full wrap,then explicit
counter rewrite for the immediate-clear test. No implementation workaround.
Next bounded deadlines/IRQ dispatch and selected MMIO integration;no firmware
boot or audio acceptance. Original MAME reference unchanged. Own CI/review pending.


## #209 merged — 2026-11-04

Merge8ffbef03db22f7bd573ce1e20c485fa1230fb2b0;tested head
96a198bd5e26313a436f8ec10dcdb44da004193f. Five checks passed
(runs41840927476/41840927537/41840927394). Automatic code review completed
without findings on that head at2026-11-04T13:46:14.035182Z,
https://github.com/RobCZart82/yaman1x/pull/209#issuecomment-6632244771.
No unresolved threads/conflict. Fresh main pulled;all five main checks PASS.
Next internal-clock auto-clear event boundaries with independent/original
checks;bounded deadlines/IRQ/MMIO remain separate.

## Free-running ITU counter events — candidate #209,2026-11-04

Validation:128/128 CTest PASS (51.34s),103 Python PASS;targeted counter/event
strict ASan/UBSan and -Wall -Wextra -Werror PASS. Original unchanged counter
method131072 free-running value/flag comparisons PASS under ASan/UBSan.
Audit653 files,notice229 files/5 texts,diff-check PASS;CI sanitizer47 groups.
No ROM accessed. Own remote CI/review pending.

New H8ItuFreeRunning composes the monotonic counter and H8H latch for the
restricted cycle65536 internal-clock profile. Absolute tick-distance crossings
set compareA/B and overflow flags even when interrupts are disabled. TGRFFFF
compares at wrapped target0. Polling reads settle elapsed time;count/compare/
enable/status changes settle old state first. Invalid configuration/index/time
rejects without mutation. No IRQ callbacks,deadlines,auto-clear,capture or MMIO.
Independent per-tick oracle covers262144 initial/target cases;2064 divider/phase/
split cases,disabled polling/clear/reenable/stop and uint64 limits tested.
A locked unchanged original update_counter harness compares131072 free-running
value/flag cases with two TGRs and IER0. No original code saved in repository.
Counter exposes pure elapsed_ticks to share overflow-safe time conversion.
Next:auto-clear event semantics/boundaries if needed,then bounded deadlines and
IRQ dispatch before bus start. Firmware still stopsFFFF60;no boot/audio proof.
Own CI/review pending.


## #208 merged — 2026-11-04

Merge96df5e102387f1490253e821f127a44049743dee;tested head
6b638f8a711c0c6fa90f6b254f08d2521c111de6. Five checks passed
(runs41829048600/41829048580/41829048565). Automatic code review completed
without findings on that head at2026-11-04T12:26:09.848108Z,
https://github.com/RobCZart82/yaman1x/pull/208#issuecomment-6630531673.
No unresolved threads/conflict. Fresh main pulled;all five main checks PASS.
Next counter-to-status compare/overflow event detection,boundary/polling tests
and original reference;IRQ/deadline/MMIO integration remains separate.

## Isolated H8H ITU status latch — candidate #208,2026-11-04

Validation:127/127 CTest PASS (53.18s),103 Python PASS;targeted strict
ASan/UBSan and -Wall -Wextra -Werror PASS. Original3 unchanged methods
compared over524288 injected event/enable/clear states under ASan/UBSan.
Audit649 files,notice229 files/5 texts,diff-check PASS;CI sanitizer46 groups.
No ROM accessed. Own remote CI/review pending.

New H8ItuStatus explicitly latches compareA/compareB/overflow events independent
of enables. Read status/enables normalize reserved bitsF8;status write zero
clears and one preserves. pending() reports only flags&enables eligibility,
not interrupt-controller pending state or callback delivery. Reset clears low
flags/enables and exposes normalizedF8:component reset policy,not whole original
raw-register/hardware reset equivalence. Invalid event bits reject unchanged.
524288 independent event/enable/clear cases plus cumulative disabled-latch,
reenable/reset/invalid controls PASS. Another524288 comparisons use three
unchanged original H8H methods in a temporary ASan/UBSan harness. Original ISR
events are supplied explicitly;this does not prove counter compare detection,
polling-driven event generation,deadline or IRQ delivery. Next:counter-to-status
event detection with independent boundary/polling tests,then deadlines/dispatch
before MMIO start. No ROM,firmware or audio acceptance. Own CI/review pending.


## #207 merged — 2026-11-04

Merge04bb10ae8e31c1b681196eee923bbf12b4f7e388;tested head
3315d2899e5f2d9ff11ca5046ab0e3e3f817c51c. Five checks passed
(runs41818785929/41818785669/41818785632). Automatic code review completed
without findings on that head at2026-11-04T11:08:47.838803Z,
https://github.com/RobCZart82/yaman1x/pull/207#issuecomment-6629084169.
No unresolved threads/conflict. Fresh main pulled;all five main checks PASS.
Next isolated compare/overflow flags and H8H status-clear/enable behavior
with disabled-IRQ polling tests;no MMIO start until deadlines/integration ready.

## Isolated internal-clock ITU counter — candidate #207,2026-11-04

Validation:126/126 CTest PASS (53.47s),103 Python PASS;targeted strict
ASan/UBSan and -Wall -Wextra -Werror PASS. Original unchanged method131072
value comparisons PASS under ASan/UBSan. Audit645 files,notice229 files/
5 texts,diff-check PASS;CI sanitizer45 groups. No ROM accessed.
Own remote CI/review pending.

New H8ItuCounter owns monotonic absolute time,count,enable,divider/phase and
explicit cycle1..65536. It settles old time before enable/count/config changes;
invalid profile/time rejects without mutation. Disabled time does not accumulate
later catch-up. Internal shifts0..3,zero/half phase,wrap and starts beyond new
clear boundary supported. No flags,IRQ,deadlines,MMIO or firmware start enabled.
Independent per-tick oracle covers2097152 initial-count/profile cases;258 splits,
stop/restart/reconfigure,invalid retention and uint64 time extremes tested.
Tick quotient avoids now+phase overflow;counter arithmetic avoids count+delta
overflow. These large-time extensions are independently checked,not claimed
original-method comparisons at overflowing source arithmetic.
131072 value comparisons with the unchanged locked update_counter PASS under
ASan/UBSan in a temporary harness. TGR count0/IER0 deliberately excludes flag/
IRQ behavior;cycle and clock configured directly. Original source never copied
into repository. Next:compare/overflow flags and disabled-IRQ polling tests,
then bounded deadlines/dispatch before bus integration. No ROM/audio acceptance.
Own CI/review pending.


## #206 merged — 2026-11-04

Merge84930a1b98b88bb28300dd6f40f0ac09dc8360b6;tested head
5722afdd6b8d159501aef8f47146d4898f249be0. Five checks passed
(runs41811877899/41811878274/41811877793). Automatic code review completed
without findings on that head at2026-11-04T10:07:10.889187Z,
https://github.com/RobCZart82/yaman1x/pull/206#issuecomment-6628054317.
No unresolved threads/conflict. Merge call timed out,but live PR confirmed
closed/merged with the above SHA;no duplicate merge requested. Fresh main
pulled;main checks pending. Next isolated internal-clock ITU counter before
flags/IRQ/deadline/MMIO integration.

## Active ITU source contract — candidate #206,2026-11-04

Validation:locked archive identity check PASS (3 files/19 methods);103 Python
tests PASS,including3 new extraction/negative tests and optimized-mode targeted
PASS. Audit641 files,notice229 files/5 texts,diff-check PASS. Initial checker
used a nonexistent lock key upstream_commit;corrected to the existing commit
key before manifest generation and final checks. No motor/runtime change;
CTest runtime suite not rerun. No ROM accessed. Own CI/review pending.

Three locked original files/19 exact method identities checked through the
private verified archive snapshot. TSTR settles counters before enable changes;
compare flags may change with IRQ disabled;absolute divider/phase time,clear/
wrap boundaries,TGRFFFF u16 comparison and reset-after-children order require
independent tests. No active timer behavior enabled. Next:isolated monotonic
internal-clock counter,then flags/IRQ/deadlines,before MMIO/firmware acceptance.
[Contract and gates](an1x-active-itu-contract.md). Own CI/review pending.


## #205 merged — 2026-11-04

Merge2191a82ce0a7d98b9b25e1a82c7f7f9c4a43d3a1;tested head
ea23632073847f523cc0efe313ac1b834ddc7aad. Five checks passed
(runs41801773189/41801773168/41801773266). Automatic code review completed
without findings on that head at2026-11-04T08:46:35.513939Z,
https://github.com/RobCZart82/yaman1x/pull/205#issuecomment-6626566493.
No unresolved threads/conflict. Fresh main pulled;all five main checks PASS.
Next:pin active ITU TSTR/counter/flag/IRQ/time behavior from original source
before enabling the firmware stopFFFF60. No boot or audio acceptance.

## Firmware level-read experiment — candidate #205,2026-11-04

Validation:125/125 CTest PASS (52.68s),100 Python PASS. Extended sink/CLI
Release and strict ASan/UBSan PASS;strict compiler PASS. Audit637 files,
notice229 files/5 texts,diff-check PASS. Read-only real-ROM probes at both
slices agree on actual stop,counters and clockless profile;hash unchanged.
Existing MAME reference and native bridge unchanged. Own CI/review pending.

New explicit lcd-meg-vop3-level-experiment-16mhz profile configures the native
sink and host adapter for experimental_an1x_level;MEG zero stubs and upload
contract remain explicit. Older register-only profile still rejects22. No sample
advancement. Sink cold reconstruction preserves selected policy while clearing
state;subset reset retains levels. Synthetic CLI checks three slices,selected
read counters,old-profile rejection and invalid index;native sink verifies
-256=>C000,subset retention and policy-preserving cold reset.

Read-only unchanged1.04 ROM,swap16,100M cycles/20M callbacks,4096/1000000 slices:
identical stop at cycle11925102,PC2912(0xB60),data_writeFFFF60 byte,
Unmapped AN1x byte write. Native writes[11557,516],reads[448,0],samples[0,0];
MEG controls5/status_reads1,GPIO6316,TX/clock0. Completed-slice cycles
11923456/11000000,callbacks2000165/1839738 exclude the failed slice as designed.
ROM SHA4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe
unchanged before/after. Both-chip callbacks do not prove full upload acceptance;
448 read callbacks do not alone prove scan ordering or the specialC000 branch.
No complete boot,MIDI or audio acceptance. The next blocker is the ITU TSTR
start path atFFFF60:active timer counter/flag/IRQ/time behavior needs original
reference and independent tests before enabling it. Do not treat the old
stopped-ITU storage profile as an active timer implementation.
Pinned original MAME/bridge unchanged. Own CI/review pending.


## #204 merged — 2026-11-04

Mergef0e159f5c1c76956f078e8c32e24a8e058c6b669;tested head
3ca993d9153495146c0d613eed855e06e4923a23. Five checks passed
(runs41792234321/41792234331/41792234326). Automatic code review completed
without findings on that head at2026-11-04T07:28:03.224613Z,
https://github.com/RobCZart82/yaman1x/pull/204#issuecomment-6625187784.
No unresolved threads/conflict. Fresh main pulled;main CI pending.
Next:separately named sink/probe level-read experiment with zero samples.

## Explicit experimental native level readback — candidate #204,2026-11-04

Validation:clean Release full build,125/125 CTest PASS (97.59s),100 Python
PASS. New level and extended native-failure groups strict ASan/UBSan PASS;
-Wall -Wextra -Werror PASS. CI sanitizer44 groups,audit636 files,notice229
files/5 texts,diff-check PASS. Independent131072 signed-input/chip cases,
32 valid channel/chip values and496 invalid indices. No ROM accessed.
Own CI/review pending.

A shared Vop3LevelReadPolicy keeps default reject and separately names
experimental_an1x_level. Native owner chooses ABI profile1 once at construction;
invalid policy or configuration failure rejects/destructs safely. Host adapter
allows22 only with explicit policy and backend capability confirmation. Reg21
remains separate. No unknown read fallback,no clock advance or firmware profile
change. Default adapter rejects22 even on an experimental backend.
Independent integer oracle checks131072 signed-input/chip cases,all16 channels
on both chips,and496 invalid selected indices. Bad index poisons the adapter,
backend read counters remain unchanged. Strict configuration/mask/enum controls
and native configuration-failure destruction are tested. Existing timing tests
cover fractional evolution and target/aim discrepancy;physical conversion is
still experimental. Next:separately named bounded firmware experiment without
sample advancement,then distinct clock/upload/timing work. Original bridge and
pinned MAME reference unchanged;no ROM accessed or boot/audio acceptance.
Own remote CI/review pending.


## #203 merged — 2026-11-04

Merge0a8af1d63d58de5bbe9e0fd29055063118402081;tested head
0dfe222721a27e7804211e1bf41a15426b7a014b. Four required checks passed
(runs41782923319/41782923279);research build not triggered for docs-only.
No automatic review result recorded;no unresolved threads/conflict.
Fresh main pulled;main checks pending. Next:explicit native/host experimental
reg22 policy and independent signed-level/index tests;default stays strict.

## Reg22 readback contract — candidate #203,2026-11-04

Validation:clean temporary Release build of existing bridge/adapter/native
checks;five selected CTest groups PASS (including normal/optimized VOP3 timing),
three EG-consumer audit unit tests PASS. Audit633 files,notice229 files/5 texts,
diff-check PASS. Previous /tmp build had been cleaned;new build used rather
than claiming prior binaries ran. Runtime unchanged;full suite not rerun for
this documentation-only change. No ROM accessed. Own remote CI/review pending.

Earlier firmware-consumer evidence is reconciled with the current strict native
stop. SELECT0..15,signed level and64 scaling are supported by prior analysis;
physical rounding/saturation/EG timing remain unverified. Historical installed-
DMS trace and current headless sink are distinct profiles. Snapshot PC54678
and historical bus-next-PC54676 are different observation fields,not proof of
instruction mismatch. Existing ABI2 profile1 is a labelled model experiment;
strict adapter22 remains rejected. Next:explicit opt-in read policy with native
profile selection and independent index/value tests,then separate firmware
experiment;sample advancement remains another gated step. No runtime change.
[Contract and evidence](vop3-readback-contract.md). Own CI/review pending.


## #202 merged — 2026-11-03

Merge127435b0d5bcb53934fdff6ee0582b924175493d;tested head
bde252f5f4e10663a306175caecf755644d39af6. Five checks passed
(runs41719234355/41719234464/41719234362). No automatic review result
recorded;no unresolved review threads/conflict. Fresh main pulled.
Main CI pending. Next investigate440044/reg22 selected-index/readback/time
evidence;do not silently return zero or call the experimental model hardware proof.

## Native VOP3 firmware-probe profile — candidate #202,2026-11-03

Validation:124/124 CTest PASS;100 Python PASS. Final CLI targeted Release and
strict ASan/UBSan PASS after metrics-unavailable reporting refinement;strict
-Wall -Wextra -Werror PASS. Both real-ROM probes repeated on final binary
with identical stop/counters and unchanged hash. Audit632 files,notice229 files/
5 texts,diff-check PASS. No upstream/MAME code changed;no ROM data uploaded.

New explicit lcd-meg-vop3-register-experiment-16mhz profile combines LCD16MHz,
original MEG zero-read stubs and experimental firmware upload control with the
native register-only dual VOP3 sink. Older profiles remain strict at VOP3.
JSON names the profile and reports per-chip writes/reads/sample_passes plus
audio_accepted=false;metrics availability is explicit (null counters if a native
handle failed),preserving the original execution-stop diagnosis;firmware_boot_accepted remains false. No DSP sample is
advanced;the model48kHz assumption is not silently applied to the CPU clock.
Synthetic CLI tests cover both chips,upload command,three slices,old-profile
rejection and unsupported read/byte faults,with unchanged input hashes.

Read-only local1.04 ROM (SHA2564f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe)
with swap16,100M cycles/20M callback cap and4096/1000000 slices yields identical
physical stop:cycle11260853,PC345720(0x54678),data_read440044 word,
error Unimplemented VOP3 register read. Native writes[10236,0],reads[0,0],
sample_passes[0,0],MEG controls5/status_reads1,GPIO6316,TX/clock0.
Completed-slice cycles11259904/11000000 and callbacks1886465/1839738 differ by
slice boundary as designed;they exclude work committed in the failed slice.
ROM SHA unchanged before/after. Counts show accepted core callbacks,not all
register semantics implemented or hardware-equivalent DSP program acceptance.
The next blocker is reg22 readback evidence/selected-index/timing semantics;
no zero fallback or undocumented ready response added. Existing bridge has an
experimental EG-readback path,but this profile deliberately keeps it disabled.
Pinned original MAME source lock unchanged;this is project integration,not new
original-method code. No boot,MIDI or audio acceptance. Own CI/review pending.


## #201 merged — 2026-11-03

Merge91f0e7cf05efd054aee5e454940b77d0a17beb61;tested head
dfd6ba01a51dce0e335b0fb2f66fa331c82045a9. All five PR checks passed
(runs41706757086/41706757211/41706757071). No automatic review result
recorded;no unresolved review threads or conflict. Fresh main pulled.
Main checks pending;next bounded firmware profile must explicitly label
MEG zero-read stubs and experimental upload control,not hardware acceptance.

## VOP3 CPU word-bus integration — candidate #201,2026-11-03

Validation:124/124 full CTest PASS;100 Python PASS. After the new test
fixture was extended with wait/reentry cases,it was rebuilt and passed again
in Release and strict ASan/UBSan;strict compiler PASS. Initial fixture edit
placed a loop after a const token and failed compilation;fixed before these
results,no stale-binary pass counted. Audit631 files,notice229 files/5 texts,
diff-check PASS. CI sanitizer selects43 groups. No ROM accessed.

Explicit optional sink hooks route only full-word data access in canonical
440000..4400FF and480000..4800FF windows after CPU24-bit normalization.
Byte/instruction accesses and sinks without opt-in stay strict. Odd addresses
and unsupported reads reject through the existing adapter. Pure wait query
uses external area2 only for selected VOP3 words;32 ASTCR/WCER/WCR combinations
are checked independently. No permissive MMIO fallback added.
An1xVop3ResearchSink composes LCD/MEG with privately owned native dual VOP3,
sharing the monotonic sink epoch and fail-stop policy. Upload status defaults
to reject;the pre-existing firmware-contract experiment requires explicit choice.
Subset bus reset retains external chips;explicit stopped-owner cold_reset
reconstructs both chips and the LCD/MEG epoch. No samples are advanced.
1024 independent coefficient/address/value cases,aliases,counters,read/width/
opt-in rejection,swallowed reentry,reset/rewind and129 synthetic CPU run cuts
check the integration. A MOV.W executed by CPU writes a verified BE coefficient.
No firmware CLI profile change,ROM access,boot/MIDI/audio acceptance or physical
clock/status proof. Existing pinned original MAME source lock is unchanged;
this step composes existing project adapters,not a new upstream-method port.
Next:explicit bounded firmware-probe profile with separate assumption labels.
Own remote CI/review pending.


## #200 merged — 2026-11-03

Merge97e4cf6cf504aec3116510284d2042548bbfa91b;exact tested head
bcdaec33f8d8704b55e68db994044049c99e4bfb. All five PR checks successful,
no unresolved review threads or conflict. No automatic review result recorded;
no successful-review claim. Fresh main pulled. Main CI pending;next step is
explicit CPU word-bus routing to native VOP3 sink.

## Native VOP3 register backend — historical candidate #200,2026-11-03

Validation: full122/122 CTest PASS (63.57s),then added fault group123
individually PASS;100 Python PASS. Both new groups strict ASan/UBSan PASS;
-Wall -Wextra -Werror compilation PASS. Audit628 files,notice229 files/5
texts,diff-check PASS;CI sanitizer selects42 groups. Native/source bridge
unchanged. No ROM opened or modified. Own remote CI/review pending.

A noncopyable/nonmovable RAII owner connects the existing ABI2 bridge to the
standalone full-word host adapter. Both chips belong to one private handle;
no raw handle escapes. Internal bridge error -3 destroys the handle and later
calls fail closed. Invalid arguments/masks preserve native state. Construction
checks ABI2 and allocation. Backend must outlive its referencing host adapter.
1024 independent coefficient/address/value cases check fixed BE snapshot offsets,
exact write/read counters,strict read rejection,two-chip and owner isolation;
32 reconstruction epochs check fresh state and unrelated-owner preservation.
A separate fault-injected ABI fixture verifies ABI/allocation rejection,four
internal-error paths,exact destruction and no callbacks after failure. It does
not simulate a real DSP fault. No samples are advanced,so sample counters remain zero. No clock,CPU bus,
firmware boot,MIDI,table ROM or audio acceptance is added. Control status remains
only the explicitly selected firmware-contract experiment from #199.
The pinned original MAME reference in research/mame-source-lock.json is unchanged;
this integration reuses the existing project bridge,not a new original-MAME
method implementation or physical YSS236 proof. Next: explicit CPU word-bus/sink
routing and bounded firmware diagnostics. Local/remote validation recorded in
DEVELOPMENT_LOG; own PR CI and automatic review pending.


Frissítve: 2026-11-03. Időzóna: Europe/Budapest.
Ellenőrzött motoralap: `301eec75c72028b8f377ee4e099fce0b1df4de5e` (#143).

## VOP3 hostadapter lezárása — 2026-11-03

A #199 beolvadt `6f761b3027f2de27c7321d39c83f6e6e0c8c2e49` commitként.
Pontos head:`51366589a6828b06c952ff354b7b3fadc62952df`;mind5 CI sikeres.
Futások:41681455942,41681455958,41681455950. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T17:10:43.532192Z-kor:
https://github.com/RobCZart82/yaman1x/pull/199#issuecomment-6610502619
Nyitott review-szál vagy konfliktus nincs. Következő:natív VOP3-híd és adapter
összekötése,lifecycle/metrics/fail-stop tesztekkel,CPU-buszbekötés előtt.
Új lépés előtt main CI ellenőrzése szükséges;boot/effekt/hang nem elfogadott.

## VOP3 teljes szavas hostadapter — jelölt,2026-11-03

Önálló template adapter a két kanonikus440000/480000,egyenként100 bájtos
ablakot chip/regiszter/full-word backendhívásra képezi. Páratlan cím,részmaszk,
tükör és ablakon kívüli cím előzetesen elutasított. Reg1 feltöltésvezérlés csak
explicit experimental_firmware_contract módban,a korábbi UploadControlContract
újrahasználatával:chipenként külön bit15 státusz,csak0/1 command. A write a
backendhez is eljut;egyéb input csak a modellben implementált EG_STATUS21.
Nem gyártunk alap nullás választ22-re vagy más ismeretlen olvasásra.
Backendhiba vagy elnyelt újrabelépési kísérlet mérgező,nem folytatható.
65024 route/value eset,chip controls,bounds/mask/read/fail-stop/reentry strict
ASan/UBSan PASS. Tesztbackend szintetikus,nem firmware/élő VOP3 integráció.
Szerződés:[vop3-host-word-adapter.md](vop3-host-word-adapter.md).
Helyi121/121 CTest (51,28s),100 Python,strict célzott ASan/UBSan PASS.
Audit624 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer40 csoport.
Saját CI/review függőben. Következő:natív híd és kutatási
busz explicit bekötése;MEG fizikai státusz és valódi hang továbbra nyitott.

## MEG forrás-stub kísérlet lezárása — 2026-11-03

A #198 beolvadt `94bf3fe1e8c708b4a2fa31fd7d4081f080d439db` commitként.
Pontos head:`7dc0b587d8ae45ef036f26de5f4ecc451486c2fe`;mind5 CI sikeres.
Futások:41664675511,41664675355,41664675306. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T15:35:16.405619Z-kor:
https://github.com/RobCZart82/yaman1x/pull/198#issuecomment-6607930821
Nyitott review-szál vagy konfliktus nincs. Következő:VOP3-host kapcsolat
kutatása az explicit forrás-stub profilban;MEG fizikai státusz továbbra ismeretlen.
Új lépés előtt main CI ellenőrzése szükséges;boot/effekt/hang nem elfogadott.

## Explicit eredeti MEG-stub státuszkísérlet — jelölt,2026-11-03

MEG fizikai státusz/kézfogás szerződés továbbra ismeretlen. Rögzített eredeti
MAME s2_r/s10_r/s11_r mind00-t adó log-stub,nem ready-bit modell.
Ezért csak új explicit lcd-meg-source-stubs-16mhz profil kapcsolja a három
kanonikus olvasást00-ra,számlált megfigyeléssel. Alap/lcd/write-observe profil
szigorú marad;minden más read/word továbbra hibázik. Nincs fizikai vagy audio-
elfogadás.768 policy/read,lyuk/alias/word/reset/isolation és129 CPU-felosztás
strict ASan/UBSan PASS;768 eredeti zero-read összevetés PASS.

Ideiglenes,repo-n kívüli kontrollban00 válasszal VOP3-ig jut,FF állandó
válasszal100M ciklusig várakozó útvonal marad;ez nem mért hardveres válasz.
Új explicit profil:4096/1000000 szeletnél egy stubolvasás/öt vezérlőmegfigyelés
után azonos440002 word write stop,perifériaciklus10832302,PC353780,
6316 GPIO,0 TX/clock él. ROM változatlan. Szerződés:
[meg-source-status-experiment.md](meg-source-status-experiment.md).
Helyi120/120 CTest (62,25s),100 Python,strict ASan/UBSan és eredeti referencia
PASS. Audit621 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer39 csoport.
Saját CI/review függőben. Következő:VOP3-host kapcsolat
kutatása;MEG fizikai státuszhoz külön elsődleges specifikáció/mérés szükséges.

## MEG megfigyelő integráció lezárása — 2026-11-03

A #197 beolvadt `d7f08c1eee2ebf306cd35aeaa9b26434b0913e41` commitként.
Pontos head:`a45dc7c595c091bae5111feb648f479ce5ec1267`;mind5 CI sikeres.
Futások:41650965433,41650965655,41650965487. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T14:11:35.408230Z-kor:
https://github.com/RobCZart82/yaman1x/pull/197#issuecomment-6606002728
Nyitott review-szál vagy konfliktus nincs. Következő:MEG offset2 státusz/
kézfogás szerződés bizonyítéka;eredeti zero-read stub nem fizikai ready-igazolás.
Új lépés előtt main CI ellenőrzése szükséges;boot/effekt/hang még nem elfogadott.

## MEG megfigyelő buszprofil — jelölt,2026-11-03

MEG külső byte write csak explicit sink opt-in mellett a400000..40003F
szűk ablakban továbbítható;a hostadapter továbbra ellenőrzi az odd lane/offsetet.
Read/word/instruction továbbra strict,nincs generikus MMIO-fallback.
A tiszta enable-query mellett area2 ASTCR/WCR/WCER időzítés használatos,
24-bit alias költséggel együtt. An1xMegResearchSink saját MEG-tárolót birtokol,
közös LCD/MEG idő- és fail-stop epochot használ;subset reset megtartja,külön
cold_reset újraépíti. Probe csak új explicit lcd-meg-observe-16mhz módban
kapcsolja be;a régi alap/lcd profil nem változik. meg_observed_controls számláló
csak log-only megfigyelés,nem parancshatás vagy működő effekt.

256 bank/control,alias/wait/reset/strict/poison és129 CPU-felosztás strict
ASan/UBSan PASS. CLI explicit/profilnegatív és olvasási hibakontroll PASS.
Változatlan helyi ROM4096/1000000 szelettel:négy megfigyelt vezérlőírás után
azonos stop400005 byte read,PC0xDA0C,perifériaciklus10814942,6316 GPIO,
0 TX/clock él. Nincs fake MEG-ready és boot/hangelfogadás.
Szerződés:[an1x-meg-research.md](an1x-meg-research.md). Helyi119/119 CTest (71,81s),100 Python,strict célzott ASan/UBSan PASS.
Audit618 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer38 csoport.
Saját CI/review függőben. Következő a MEG státusz-szerződés bizonyítékának vizsgálata.

## MEG hostadapter lezárása — 2026-11-03

A #196 beolvadt `7e93410925450aa1715a65fca9babba395bd9128` commitként.
Pontos head:`651cad9f8d72210479484d2477b3ef94cd552a18`;mind5 CI sikeres.
Futások:41638949671,41638949433,41638949383. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:explicit megfigyelő MEG-sink/buszbekötés,ismeretlen olvasások és
státuszok szigorú elutasításával. Előbb main CI ellenőrzése szükséges.

## MEG hostcím-adapter — jelölt,2026-11-03

A helyi szervizrajz51.PDF-oldalán IC10 MEG A0..A4 a CPU A1..A5 hálózataira
csatlakozik,D0..D7 az alsó adatbájtra;LWR/RD/MEGCS külön jelzett. Kanonikus
kutatási ablak:400001+2*offset,offset0..18.400007 így3-as vezérlőoffset.
A teljes chip-select engedély és címtükrözés továbbra nincs igazolva.
Önálló MegHostWriteAdapter nem köti be a CPU-buszt. Alapból a nem implementált
vezérlők hibáznak;külön explicit observe_original_stubs módban hat eredeti
logging-only írás csak számlált megfigyelés,nincs hatás/status/ready/audio.
Minden nem támogatott bájtsáv/lyuk/tükör/offset elutasított,tároló állapotát nem
módosítja.1536 vezérlőérték,65536 tárolópár,ablak/lane/reset kontroll strict
ASan/UBSan PASS. Eredeti21 metódussal720896 tároló+1536 logging-only összevetés
PASS. Szerződés:[meg-host-write-adapter.md](meg-host-write-adapter.md).
Helyi118/118 CTest (51,59s),100 Python,strict ASan/UBSan és referencia PASS.
Audit615 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer37 csoport.
Saját CI/review függőben.
Következő:explicit kutatási sink-bekötés,szigorú ismeretlen olvasásokkal.

## MEG regisztertárolás lezárása — 2026-11-03

A #195 beolvadt `1c5d35fcaa0110cbae0dab6f280f0020b24105e7` commitként.
Pontos head:`d2f9dfec15e3fabc102a21164453bb1441ff6a3c`;mind5 aktuális CI sikeres.
Futások:41624961321,41624961195,41624961011. A macOS125879878868 feladat
hosted runner kapacitáshiány miatt nem kapott gépet;csak ezt kértük újra a
workflow befejezése után. Végső macOS125896759096 sikeres;a GitHub új próbában
új Windows/Ubuntu check-identitást is közölt,mindkettő sikeres. Futó munkát nem
indítottunk újra. Automatikus review-eredmény nem rögzített;nyitott review-szál
és konfliktus nincs,merge-state clean. Következő:MEG hostcím/bájtsáv/vezérlő-
szerződés bizonyíték alapján;400007 még tiltott. Előbb main CI ellenőrzése.

## MEG önálló regisztertárolás — jelölt,2026-11-03

A400000 körüli effektinterfész a korábbi rajzi/firmware-kutatás jelöltje;
a rögzített eredeti AN1x driver nem térképezi fel,ezért400007 buszelérését
most nem engedjük át. Önálló MegRegisterStorage:8-bit selector,tíz256-bájtos
bank,két256-szavas constant/offset bank független high/low lane-nel.
Nincs státuszolvasás,audio vagy vezérlőparancs-szimuláció;a logging-only
MAME-stubok nem bizonyítanak elfogadható választ. Nem támogatott offset hibázik.
Cold zero projektpolitika (eredeti start/reset üres);reset megtartja a tárolót.
720896 független bank/lane eset,65536 szóérték,isolation/strict/reset strict
ASan/UBSan PASS.15 eredeti metódussal720896 összevetés,azonos explicit kezdő-
állapotból;nem hardveres reset-/busz-/hangelfogadás. Szerződés:
[meg-register-storage.md](meg-register-storage.md). Helyi117/117 CTest (50,57s),100 Python,strict ASan/UBSan és eredeti
referencia PASS. Audit612 fájl,notice229 fájl/5 szöveg,diff-check PASS;
CI sanitizer36 csoport. Saját CI/review függőben. Következő:hostcím/vezérlőoffset bekötés bizonyítékának tisztázása.

## Leállított TIER-tárolás lezárása — 2026-11-03

A #194 beolvadt `1e67c1b379aebe0bec94dbac39e9b199adca9d3f` commitként.
Pontos head:`2681fdc5494aa6f8d2c792be406306ffcc40d16b`;mind5 CI sikeres.
Futások:41614096928,41614096943,41614096814. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:400007 periféria eredeti címtér- és működési szerződése.
Új fejlesztés előtt main CI ellenőrzése szükséges;boot/hang még nem elfogadott.

## Leállított ITU TIER-tárolás — jelölt,2026-11-03

Öt TIER önállóan tárolja a három engedélyező bitet,reserved read-oneF8,
resetF8. Minden1280 csatorna/raw kombináció elfogadott és visszaolvasható.
Ez konfigurációtárolás,nem aktív ITU IRQ-szolgáltatás:TSTR továbbra tiltott,
TSR üres,nincs counter/capture/event ami flaget vagy IRQ-t kelthetne.
Az eredeti tier_update m_tier|F8,IER A/B/V megfeleltetés és üres ISR alapján;
a számlálóindítás vagy külső esemény támogatása előtt aktív flag/IRQ kell.
A régi idle-teszt tiltáselvárását tároláselvárás váltja,de disabled/reset/status/
word kontrolljai megmaradnak. Új1280 érték,owner/reset/time/start-negatív és129
CPU-felosztás strict ASan/UBSan PASS.1280 eredeti TIER/üres-TSR összevetés PASS.

Változatlan helyi ROM,100000000 ciklus/20000000 callback,lcd-16mhz,
4096/1000000 szelet:következő stop400007 byte write,PC0xD9F2,
perifériaciklus9555129,6316 GPIO,0 TX/clock él. Nem boot vagy hangelfogadás.
Szerződés:[an1x-itu-tier-stopped.md](an1x-itu-tier-stopped.md). Helyi116/116 CTest (85,95s),100 Python,strict célzott ASan/UBSan és
eredeti referencia PASS. Audit608 fájl,notice229 fájl/5 szöveg,diff-check PASS;
CI sanitizer35 csoport. Saját CI/review függőben. Következő a400007 periféria eredeti címtér-szerződése.

## Firmware-diagnosztika lezárása — 2026-11-03

A #193 beolvadt `9a56f7ab5acab3d33f64e206cefdd9debe9bcbc8` commitként.
Pontos head:`d0864c1642eb1ec2e1c38f4afc98c763e5e82bac`;mind5 CI sikeres.
Futások:41603324777,41603324735,41603324731. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T08:27:56.524295Z-kor:
https://github.com/RobCZart82/yaman1x/pull/193#issuecomment-6598885247
Nyitott review-szál vagy konfliktus nincs. Következő:TIER4 IRQ-engedély és
szükséges ITU flag/aktív állapot szerződésének vizsgálata rögzített eredetiből.
Új lépés előtt main CI ellenőrzése szükséges;boot/hang továbbra nem elfogadott.

## Firmware-próba:korlát és valódi ITU-stop — jelölt,2026-11-03

A probe callback-korlátja továbbra explicit és véges,de legfeljebb20000000
lehet;a ciklus/szelet limitek változatlanok. Jelentés új mezői:stop_reason
(cycle_limit/callback_limit/execution_error),callback_limit_requested.
A callback-limit továbbra mérgező fail-stop,nem folytatható költségkeret.
A korábbi1000000 korlátnál mért megállás nem eszközhiba volt.

100000000 kért ciklus/20000000 callback,swap16,lcd-16mhz és4096/1000000
szelet mellett azonos valódi stop:FFFF94 TIER4 byte write,PC0x40A62,
perifériaciklus8798299,5269 GPIO,0 TX/clock él. Hiba:Unsupported ITU interrupt
enable;firmware_boot_accepted=false. ROM hash előtte/utána változatlan.
Következő a leállított TIER4 engedély és tényleges ITU flag/IRQ szükséglet
vizsgálata rögzített eredeti forrásból,nem az elutasítás egyszerű kikapcsolása.
Szerződés:[an1x-firmware-event-diagnostics.md](an1x-firmware-event-diagnostics.md).
Saját CI/review függőben. Helyi115/115 CTest (50,22s),100 Python PASS;
audit605 fájl,notice229 fájl/5 szöveg,diff-check PASS. CPU/periféria változatlan.

## P9 kutatási profil lezárása — 2026-11-03

A #192 beolvadt `8d53be572aab80db6bda2f9629366002b252bc2d` commitként.
Pontos head:`23dd23fba4d221b7e413e2377282745a458de94f`;mind5 CI sikeres.
Futások:41594348384,41594348397,41594348298. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T07:11:33.760494Z-kor:
https://github.com/RobCZart82/yaman1x/pull/192#issuecomment-6597624180
Nyitott review-szál vagy konfliktus nincs. Következő:korlátos firmware/esemény-
diagnosztika az LCD/P9 után;callback-limit nem eszközhiba és nem bootelfogadás.
Új lépés előtt main CI ellenőrzése szükséges.

## P9 kutatási bemeneti profil — jelölt,2026-11-03

A korábbi LCD MAME-patch kifejezetten read_port9=FF szintet ad,a rögzített
eredeti h8_device::port_default_r szinténFF-et ad nem bekötött portokra.
Csak az explicit LCD research sink P9 bemenetén vezetjük be ezt a headless
kutatási profilt. Nem fizikai huzalozásbizonyítás,nem általános GPIO-fallback.
A H8 portmodell DDR/kimenet/reserved bit keverése megmarad,más ismeretlen
bemenet továbbra mérgező hibát okoz. Alapértelmezett no-LCD probe nem változik.
Független bites oracle minden65536 DDR/data kombinációra,alias/idő/reset és
más portok fail-stop kontrolljai. Szerződés:[an1x-p9-input.md](an1x-p9-input.md).
Helyi115/115 CTest (52,63s),100 Python és strict ASan/UBSan PASS;
az optimalizált fixture újrafordítva,célzott CTest ismét PASS. CI/review függőben.
Audit604 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer34 csoport.
500000 ciklus1/64/257 szelettel hiba nélkül,azonos PC/state/periféria/22 GPIO;
hosszabb próbák callback-limitnél álltak meg,nem eszközhibánál. ROM változatlan.
Nem boot/hangelfogadás;következő további korlátos firmware- és eseménydiagnosztika.

## LCD-sink integráció lezárása — 2026-11-03

A #191 beolvadt `0952306f918f57582f928aea8f44fff5123d0749` commitként.
Pontos head:`5ffe1075fb8860d76cd6fa9b16470328e4622681`;mind5 CI sikeres.
Futások:41585964104,41585964059,41585963977. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T05:49:09.070883Z-kor:
https://github.com/RobCZart82/yaman1x/pull/191#issuecomment-6596475450
Nyitott review-szál vagy konfliktus nincs. Következő:P9 bemenetek bekötési
szerződése bizonyíték alapján;előbb main CI ellenőrzése.

## LCD-sink gépintegráció — jelölt,2026-11-03

Saját LCD-sink kapcsolja össze a portadaptert és a headless vezérlőt;explicit
ciklus/µs skálával. Nem-LCD bemenet továbbra hibázik,egyéb kimenet nem gyárt
eszközválaszt. GPIO-idővisszalépés vagy eszközhiba mérgezi a sink-epochot;
explicit cold_reset rekonstruálja. Gépi részreset megtartja a külső LCD-t.
129 CPU-felosztás,busy/read-latch/command/DDR/reset/fail-stop célzott ASan/UBSan
PASS. Az első fixture tévesen R/W-t és E-t egyszerre ejtette,ami valódi írásélt
hozott létre és busy-t indított;helyes read-falling,majd RW-váltás sorrendre javítva.

A probe opcionális lcd-16mhz profilja16 ciklus/µs-t használ;eredeti yman1x.cpp
H83002(config,16_MHz_XTAL) alapján explicit kutatási profil,nem hardveridő-mérés.
Alapértelmezett no-LCD működés megmarad. ROM-free CLI-tesztek mindkét profilt és
ismeretlen profil elutasítását ellenőrzik. Valós helyi változatlan1.04 ROMmal
szelet1/64/257 mellett azonos új stop:FFFFD2 P9 byte read,PC0x3F66A,
perifériaciklus482170,10 GPIO,0 TX/clock él. KorábbiFFFFD6 olvasáson túljutott.
Nem teljes boot vagy hang. Következő:P9 bemenetek bekötési szerződése bizonyíték
alapján,majd új korlátos próba. Szerződés:[an1x-lcd-machine.md](an1x-lcd-machine.md).
CI/review függőben.
Helyi114/114 CTest (50,76s),100 Python,célzott strict ASan/UBSan PASS.
Forrásaudit602 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer33 csoport.

## LC7985 állapot/memória lezárása — 2026-11-03

A #190 beolvadt `0186721943c8243203ca58391317a69cc0dc0253` commitként.
Pontos head:`62279dac4602d6c3a5ede47a999e770da491eab4`;mind5 CI sikeres.
Futások:41578172689,41578172934,41578172711. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:explicit órajelű LCD-sink bekötés,GPIO/reset/fail-stop tesztek,
majd változatlan helyi firmware megfigyelése;előbb main CI ellenőrzése.

## LC7985 állapot és memória — jelölt,2026-11-03

Önálló headless vezérlő80 bájt DDRAM-mal és64 bájt CGRAM-mal,cím/mód/entry/
display/shift állapottal. A #189 busy-órát használja;utasítás busy alatt
figyelmen kívül marad,de eredeti dr_r/dr_w szerint az adatút nem busy-gated.
Egy/két soros címátmenetek,entry-shift,statuscím és reset kezelve. Nincs render,
font-ROM vagy firmware-probe-bekötés. Érvénytelen DDRAM-adatindex elutasított,
nem az eredeti potenciális tömbtúlindexelés reprodukciója.
114688 független memóriaeset,cím/status/ready/reset/GPIO-latch célzott ASan/UBSan
PASS.50000 művelet/állapot összevetés a hash-ellenőrzött eredeti MAME kilenc
változatlan metódusával PASS,biztonságos címprofilban. Ideiglenes harness,nincs
repo-ba másolt eredeti implementáció vagy Yamaha-adat. Ez nem hardverelfogadás.
Szerződés:[lc7985-controller.md](lc7985-controller.md). CI/review függőben.
Helyi113/113 CTest (50,56s),100 Python,strict ASan/UBSan és eredeti referencia
PASS;forrásaudit599 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer32 csoport.

## LC7985 busy-óra lezárása — 2026-11-03

A #189 beolvadt `d95364222f124ba6cec967de50f7b3427a0ff086` commitként.
Pontos head:`8cb6d09029aa55d9d33ef38246352e45088a9614`;mind5 CI sikeres.
Futások:41570074733,41570074872,41570074684. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:LC7985 cím/mód/memória állapotkezelés,előbb main CI ellenőrzése.
A busy-óra önálló komponens,firmware-probe-ba még nincs bekötve.

## LC7985 busy-óra — jelölt,2026-11-03

Önálló időkomponens:reset10000µs,beküldött utasítás busy alatt teljesen
figyelmen kívül hagyandó;00 nem indít busy-t,01/02/03=16400µs,minden04..FF
utasítás40µs a rögzített eredeti MAME ir_w prioritása alapján. Adathozzáférés
nem használja ezt az API-t;az eredeti dr_r/dr_w nem indít busy-t.
Explicit egész ciklus/µs skála,határpillanatban kész,monoton idő,overflow
előzetes elutasítás állapotváltozás nélkül. Nincs LCD-memória vagy teljes
vezérlő/probe-integráció;ez még nem oldja fel a Port B boot-megállást.
768 parancs/skála eset,10001 időfelosztás,reset/határ/overflow strict ASan/UBSan
PASS. Rögzített eredeti LC7985 teljes fájlhash ellenőrizve,aktív differenciál
nem készült. Szerződés:[lc7985-busy-clock.md](lc7985-busy-clock.md).
Saját CI/review függőben;következő a vezérlő állapot/memória szerződése.
Helyi112/112 CTest (50,86s),100 Python,strict ASan/UBSan PASS.
Forrásaudit595 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer31 csoport.

## LCD GPIO adapter lezárása — 2026-11-03

A #188 beolvadt `93e4180be0948501845ccc7303b7b53008fa271d` commitként.
Pontos head:`e9eb1cefae3078ab56c9b5572f8607a4619b00e4`;mind5 CI sikeres.
Futások:41562056261,41562056282,41562056303. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:LC7985 vezérlőállapot/busy ciklusszerződés a rögzített eredetiből,
majd kis tesztelt implementáció;előbb main CI ellenőrzése.

## LCD GPIO–vezérlő adapter — jelölt,2026-11-03

A meglévő LcdBus bekötést ciklusjelölt vezérlőhívásokhoz kapcsoló önálló
adapter készült. PA/PB/P9 kimeneti szinteket fogad,status/data readet E-felfutás,
command/data writeot E-lefutás indít;azonos szint nem ismétel tranzakciót.
Olvasott érték latchelt;nem LCD-bemenet hibázik,más portkimenet kezeletlenként
jelzett. A busy flag,memória és időkonverzió a későbbi vezérlő feladata.
512 független bájt/RS eset,él/latchelt adat/idő/routing ellenőrzés strict
ASan/UBSan alatt PASS. A fake vezérlő csak tesztfixture,nem LCD-emuláció.
Nincs firmware-probe bekötés vagy új bootelfogadás. Következő:LC7985 állapot/
busy időzítés szerződés a rögzített eredeti forrásból,majd integráció.
Szerződés:[an1x-lcd-gpio.md](an1x-lcd-gpio.md). CI/review függőben.
Helyi111/111 CTest (78,59s),100 Python,ASan/UBSan PASS;sanitizer CI30 csoport.
Forrásaudit592 fájl,notice229 fájl/5 szöveg és diff-check PASS.

## Port B diagnosztika lezárása — 2026-11-03

A #187 beolvadt `cddf9afa7cad0e23c45e66223b4cf87f31d27c1d` commitként.
Pontos head:`6ac84539108e9c567a8d79f715bbae21bb9a287c`;mind4 CI sikeres.
Futások:41555012549,41555012530. Kutatási build útvonalszűrő miatt nem indult
(kizárólag dokumentáció). Automatikus kódreview ugyanazon headen találat nélkül
kész2026-11-03T00:45:49.831659Z-kor:
https://github.com/RobCZart82/yaman1x/pull/187#issuecomment-6592328238
Nyitott review-szál vagy konfliktus nincs. Következő:Port B külső protokoll
bizonyítékának vizsgálata;új fejlesztés előtt a main CI ellenőrzése szükséges.

## Hosszabb boot-diagnosztika — 2026-11-03

A #186 utáni main mind5 CI-ellenőrzése sikeres. Változatlan1.04 ROM,privát
swap16,10000000 kért ciklus,1000000 callback-limit mellett a próba482160.
perifériaciklusnál megáll:byte data_readFFFFD6 (Port B),PC0x3F666,
instruction_state1864224/substate0. Szeletméret1/64/257 esetén azonos fault,
PC és perifériaidő;teljes visszatért szeletciklus rendre482158/482112/482132,
callback739440/116288/104976.9 GPIO,0 TX/clock él. A hibás szelet munkája nem
szerepel a completed_slice számlálóban,ezért ezek eltérése önmagában nem hiba.
Ez diagnosztikai részprofil,nem firmware-boot-,hardver- vagy hangelfogadás.

Rögzített eredeti MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h83002.cpp132. sor:FFFFD6 portb port_r/dr_w;159. sor utáni konfiguráció
PORT_B,resetDDR00,mask00. Ellenőrzött privát archívumpillanatképből olvasva.
A chipcímtér nem bizonyítja az AN1x külső huzalozását vagy bemeneti értékét.
Következő:Port B külső eszköz/protokoll szerződés vizsgálata meglévő kutatási
forrásokból,majd explicit bemeneti modell és független tesztek. A hibajelzést
nem oldjuk fel bizonyítatlan fix bemenettel. ROM SHA előtte/utána azonos:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

## ITU üres státuszprofil lezárása — 2026-11-03

A #186 beolvadt `d869fa7de2246e1e4001ede06c2288488acb067e` commitként.
Pontos head:`0573354bb2be692d45ce8c3eda12645a25faad4c`;mind5 CI sikeres.
Futások:41542980274,41542980458,41542980204. Automatikus kódreview találat
nélkül kész ugyanazon headen2026-11-02T23:09:25.034545Z-kor:
https://github.com/RobCZart82/yaman1x/pull/186#issuecomment-6590790696
Nyitott review-szál és konfliktus nincs. Friss mainből következő kutatás:
ellenőrizhető boot-checkpoint és hosszabb korlátos diagnosztika;100000 ciklus
önmagában továbbra nem boot- vagy hangelfogadás. A main mind5 CI-ellenőrzése is sikeres.

## ITU megszakítások letiltott állapota — jelölt,2026-11-03

Öt TIER csak letiltott IRQ-profilt fogad:32 raw elfogadott/224 tiltott érték
csatornánként. Öt TSR üres jelzőállapotot ad;mind256 írás utánF8 marad,
szoftver nem állíthat flaget. TSTR tiltott,nincs aktív counter/event/IRQ.
Gyártói10.2.12–13 és rögzített eredeti tier_update/isr_update/isr_to_sr alapján.
2560 értékeset,alias/reset/no-effect/word és129 CPU-vágás ellenőrizve.
Helyi validáció:110/110 CTest (60,27s),100 Python,strict ASan/UBSan PASS.
A változatlan helyi ROM swap16 próbája100000 kért ciklust teljesít hiba nélkül:
25339 callback,perifériaciklus100002,PC634348,9 GPIO,0 TX/clock él.
Ez korlátos futási megfigyelés,nem teljes firmware-boot vagy hangelfogadás.
Szerződés:[an1x-itu-irq-idle.md](an1x-itu-irq-idle.md). Saját CI/review függőben.
Forrásaudit589 fájl,notice229 fájl/5 szöveg,diff-check PASS;ROM SHA változatlan.

## ITU I/O-profil lezárása — 2026-11-02

A #185 beolvadt `74bd4e0990c02d3b084d40c62e98e71ee343a7c1` commitként.
Ellenőrzött head:`adaa6e3d0e7140cdbb149a02a270719a4443f882`;mind5 CI sikeres.
Futások:41534258437,41534258773,41534258263. Automatikus kódreview ugyanazon
headen2026-11-02T22:08:31.050236Z-kor találat nélkül kész;bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/185#issuecomment-6589650975
Nyitott review-szál és változtatáskérés nincs. Következő:FFFF66 TIER0 és a
leállított ITU statusflag/IRQ-engedély határok vizsgálata friss mainből.

## ITU compare-mód kimenet nélkül — jelölt,2026-11-02

Öt TIOR csak no-output compare profilt fogad:4 raw/252 tiltott csatornánként,
readback/reset88. Nincs jelalak/capture/külső él/compare-event/IRQ,csatornák állnak.
Gyártói10.2.11 alapján;eredeti MAME base TIOR read00/write-ignored stub,ebből
nem állítunk aktív I/O referenciát.1280 csatornaérték,alias/reset/no-effect/word
és129 CPU-vágás. Korábbi negatív kontroll a támogatott65 helyett69-et ellenőriz.
Firmware88 írása teljesül,következő stopFFFF66 TIER0,PC0xA7E,ciklus315;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-io.md](an1x-itu-io.md).
Helyi validáció:109/109 CTest (60,44s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit587 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan28 csoport;saját CI/review és merge függőben.

## ITU-csatornavezérlők lezárása — 2026-11-02

A #184 beolvadt `3e005e001022c5ac11272eb754cee6a0cda75bfd` commitként.
Ellenőrzött head:`41eb7461e9e530abe25b5f36118119e9f45fb719`;mind5 CI sikeres.
Futások:41525537583,41525537762,41525537584. Automatikus kódreview ugyanazon
headen2026-11-02T21:09:22.713478Z-kor találat nélkül kész;bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/184#issuecomment-6588496395
Nyitott review-szál és változtatáskérés nincs. Következő:FFFF65 TIOR0
compare/no-output és capture korlátok vizsgálata friss mainből.

## Leállított ITU-csatornavezérlők — jelölt,2026-11-02

Öt TCR tárolja az óra/él/counter-clear konfigurációt,reserved bit7 read-one,
reset80. TSTR továbbra tiltott,nincs aktív számláló/külső él/clear/IRQ/esemény.
Gyártói10.2.10 és rögzített eredeti tcr_w/tcr_update alapján,aktív counter-
differenciál nélkül.1280 érték,alias/owner/reset/no-event/word és129 CPU-vágás.
Firmware83 írása teljesül,következő stopFFFF65 TIOR0,PC0xA7A,ciklus307;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-control.md](an1x-itu-control.md).
Helyi validáció:108/108 CTest (95,67s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit585 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan27 csoport;saját CI/review és merge függőben.

## ITU általános regiszterek lezárása — 2026-11-02

A #183 beolvadt `89267c4ef11dd5d08f286433baff32624cce52ba` commitként.
Ellenőrzött head:`9603b0067c2e08e0f89e20e32d166f83667151d0`;mind5 CI sikeres.
Futások:41516087446,41516087367,41516087375. Review-szál és változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:FFFF64 TCR0
óra/él/counter-clear konfigurációja leállított ITU-n,friss mainből.

## Leállított ITU általános regiszterek — jelölt,2026-11-02

Tíz GRA/GRB tároló szó/bájt hozzáféréssel,FFFF cold/reset. Önálló big-endian
bájtsávok,24-bit alias,páratlan szó előzetes elutasítás. Nincs aktív ITU-start,
compare/capture/flag/buffer/IRQ;counter és vezérlők továbbra tiltottak.
Rögzített eredeti tgr_r/w és gyártói10.2.8/10.3.1 alapján,aktív timer-differenciál
nélkül.655360 szóérték,bájtsávok,regiszterizoláció,reset/RAM/idő/no-event és129
CPU-vágás. Firmware következő stopFFFF64 TCR0,PC0xA76,perifériaciklus299;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-general.md](an1x-itu-general.md).
Helyi validáció:107/107 CTest (65,44s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit583 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan26 csoport;saját CI/review és merge függőben.

## ITU-kimenettiltás lezárás — 2026-11-02

A #182 beolvadt `bdf0ff6d14f1e2eea7f4486f927177a056b22d93` commitként.
Ellenőrzött head:`f4be4b9d717524c0290dfe2ecf271f8934ae1c3b`;mind5 CI sikeres.
Futások:41503635803,41503636111,41503635895. A research-macOS várakozó
feladatát nem indítottuk újra;végül sikeresen lefutott. Review-szál és
változtatáskérés nincs;automatikus review-eredmény nem volt rögzítve.
Következő:FFFF6A GRA0 és a leállított ITU általános regiszterek word/byte/reset
szerződése friss mainből,aktív counter vagy compare-esemény feltételezése nélkül.

## ITU-kimenetek letiltása — jelölt,2026-11-02

Gyártói10.2.5 alapjánTOER hideg/resetFF,de a firmwareC0 írása minden kimeneti
engedélyt töröl.4 raw elfogadott/252 engedélyező elutasított. Nincs aktív pinfunkció,
outputcallback/PWM/counter/IRQ;TOCR és páros word tiltott. Eredeti MAME-kezelő
read00/write-ignored stub,ettől a dokumentált kézikönyvi profil eltér.
256 érték,hideg/tiltott/reset különbség,alias/RAM/idő/word és129 CPU-vágás.
Firmware továbbjut,következő stopFFFF6A GRA0 szóírás,PC0xA6A,perifériaciklus277;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-output.md](an1x-itu-output.md).
Helyi validáció:106/106 CTest (59,64s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit581 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer25
csoport;saját CI/review és merge függőben.

## ITU normálmód lezárás — 2026-11-02

A #181 beolvadt `45ffd942a8d95f223622533684fc42df30681bc6` commitként.
Ellenőrzött head:`1b75363ddc73461c807ddd910a40893f05ac8130`;mind5 CI sikeres.
Futások:41494316497,41494316508,41494316498. Review-szál és változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:FFFF90 ITU
kimenetengedélyező szerződése és fizikai kimeneti korlátok vizsgálata friss mainből.

## ITU normál mód és puffer nélküli funkció — jelölt,2026-11-02

Gyártói10.2.3–10.2.4 alapjánTMDR csak normál mód,FDIR konfiguráció tárolt;
4 raw elfogadott/252 tiltott. TFCR normál/puffer nélküli profil,szintén4/252.
Eredeti rögzített MAME-kezelők itt read00/write-ignored stubok,nem aktív referencia.
Nincs PWM/fázisszámlálás/buffertranszfer/counter/IRQ;FDIR még nem hajt overflow-t.
512 regiszterérték,alias/reset/RAM/idő/word és129 CPU-vágás. FirmwareA0 írása
most teljesül,következő stopFFFF90 output-enable,PC0xA62,perifériaciklus263;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-mode.md](an1x-itu-mode.md).
Végleges helyi validáció:105/105 CTest (51,37s),100 Python,strict célzott
ASan/UBSan PASS. Forrásaudit579 fájl,notice229 fájl/5 szöveg,diff-check PASS.
CI sanitizer24 csoport;saját CI/review és merge függőben.

## ITU-szinkronprofil lezárás — 2026-11-02

A #180 beolvadt `7c3b7a05dc2bf19fe7e6da3c010196c633d67907` commitként.
Ellenőrzött head:`24244db847b0fac75afd9e0e4fb3bbb8e5df0b02`;mind5 CI sikeres.
Futások:41484649901,41484650551,41484649978. Review-szál és változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:FFFF62 TMDR
módvezérlő gyártói szerződésének és eredeti stubkorlátjának vizsgálata friss mainből.

## Megtartott ITU-szinkronprofil — jelölt,2026-11-02

Gyártói10.2.2 alapjánFFFF61 TSNC (MAME neveTSYR),readbackE0. Csak független
csatornák,8 raw elfogadott/248 szinkronengedély elutasított. Az eredeti rögzített
MAME-kezelő itt stub:read00/write ignored;ettől a gyártói profil tudatosan eltér.
Nincs aktív ITU-szinkronizálás/counter/IRQ vagy eredeti runtime-differenciál állítva.
256 érték,alias/reset/RAM/idő/word és129 CPU-vágás. Változatlan helyi firmwareE0
írása teljesül,következő stopFFFF62 TMDR,PC0xA5A,perifériaciklus247;
192 teljes szeletciklus/55 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-sync.md](an1x-itu-sync.md).
Helyi validáció:104/104 CTest (58,03s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit577 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer23
csoport;saját CI/review és merge függőben.

## Nyugalmi ADC lezárás — 2026-11-02

A #179 beolvadt `48ed518157ae3850b2cae70e89aa8a4ae8a4f5e1` commitként.
Ellenőrzött head:`c7ea69da70b221046536b03a608099040ff03475`;mind5 CI sikeres.
Futások:41474846838,41474846627,41474846596. A research-macOS igazolt
hosted-arm64 kapacitáshiba utáni újrapróbálása sikeres. Review-szál és
változtatáskérés nincs;automatikus review-eredmény nem volt rögzítve.
A #178 merge-commit main-futásainak két macOS kapacitáshibája is rendezett:
41471009802 és41471009805 végső feladatai sikeresek,MSan korábban sikeres.
Következő:FFFF61 ITU TSYR inicializálás szerződésének vizsgálata friss mainből.

## Nyugalmi ADC-konfiguráció — jelölt,2026-11-02

Friss main,#178 után. ADCSR FFFFE8 csakADST0/ADIE0:csatorna/scan/óra tárolt,
ADF szoftveresen nem állítható.64 elfogadott/192 elutasított raw érték.
Nincs analóg minta/konverzió/eredmény/trigger/IRQ;ADCR és páros word tiltott.
Gyártói14.2.2 és rögzített eredeti ADC-kezelők alapján,aktív ADC-differenciál nélkül.
256 érték,alias/reset/RAM/idő/no-effect/word és129 CPU-vágás. Firmware03 írása
most teljesül,következő stopFFFF61 ITU TSYR,PC0xA56,perifériaciklus239;
192 teljes szeletciklus/55 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-adc-idle.md](an1x-adc-idle.md).
Helyi validáció:103/103 CTest (58,11s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit575 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer22
csoport;saját CI/review és merge függőben.

## Aktív buszóra lezárás — 2026-11-02

A #178 beolvadt `51226f9524ca0c1cc39d487b8c31b7232c8ccddf` commitként.
Ellenőrzött head:`63e76d1f7cd3adc5fd0c5066311b5279e318f29e`;mind5 CI sikeres.
Futások:41466453406,41466453320,41466453532. Nyitott review-szál és
változtatáskérés nincs;automatikus review-eredmény nem volt rögzítve.
Következő:FFFFE8 ADC-megállás szerződésének vizsgálata és a még hiányzó
refresh IRQ/eseménybekötés logikus sorrendjének meghatározása friss mainből.

## Aktív refresh buszóra — jelölt,2026-11-02

A #177 beolvadt `3f0baf6696bedd423650dc2aeb094bddfd6f5aaf` commitként;
ellenőrzött head934936237d7d8fcb9cfc24d8dc2b317afe12052d,mind5 CI sikeres.
Review-szál/változtatáskérés nincs,automatikus review-eredmény nincs rögzítve.
Az önálló timer most a busz abszolút idejével halad,hét osztóval;CMIE továbbra
elutasított. Nincs IRQ-deadline vagy fizikai refresh/DRAM. Reset megtartja az időt.
Független hét ciklusorákulum,reset/CMF/debt és257 CPU-felosztás;eredeti524288
összehasonlítás továbbra PASS. Változatlan helyi ROM192 teljes szeletciklus/55
callback utánFFFFE8 ADC-írásnál áll meg,PC0xA44,perifériaciklus204,9 GPIO-jelzés.
Nem teljes boot vagy hang. Szerződés:[an1x-refresh-clock.md](an1x-refresh-clock.md).
Helyi validáció:102/102 CTest (63,59s),100 Python,célzott strict ASan/UBSan
és524288 eredeti referencia-összehasonlítás PASS. Forrásaudit573 fájl,notice229
fájl/5 szöveg,diff-check PASS. CI sanitizer21 csoport;saját CI/review függőben.

## Önálló aktív refresh-timer — jelölt,2026-11-02

Friss main,#176 után. Külön komponens,nincs még gépi/IRQ bekötés. Hét osztó,
abszolút fázis,compare/CMF,CMIE kéréslekérdezés,reset és korlátos deadline.
Független ciklusorákulum és16416 felosztás;524288 összehasonlítás változatlan,
privát snapshotból fordított eredeti MAME-kezelőkkel strict ASan/UBSan alatt.
Eltérő túlcsordulás/compare-fölötti számláló és CMF read-gate korlát dokumentált.
FFFFAD gépi megállás változatlan. Következő:eseményidővonal/IRQ/CPU bekötés.
Szerződés:[h8-refresh-timer.md](h8-refresh-timer.md).
Helyi validáció:101/101 CTest (50,40s),100 Python,célzott és eredeti referencia
ASan/UBSan PASS. Forrásaudit571 fájl,notice229 fájl/5 szöveg,diff-check PASS.
CI sanitizer20 csoport;saját CI/review és merge függőben.

## Leállított timer lezárás — 2026-11-02

A #176 beolvadt `ba29551ec8c3d4ab4da6bb7ec38d48f4381b84bf` commitként,
2026-11-02T13:53:22Z. Ellenőrzött head:`00157c36bdc90f19f7c3ac6a797c3fad630bdbf5`.
Mind az öt végső CI sikeres;futások41450001106,41450001129,41450001118.
Két korábbi macOS feladat hosted-arm64 kapacitáshiány miatt szakadt meg;
célzott újrapróbálás után sikeresek. Review-szál/változtatáskérés nincs;
automatikus review-eredmény nem volt rögzítve. Következő az aktív órajel,
compare és IRQ függetlenül ellenőrzött szerződése friss mainből.

## Leállított refresh-timer alap — jelölt,2026-11-02

Friss main,#175 után. RTMCSR csakCKS0/CMIE0 (16 raw érték),CMF szoftveresen
nem állítható;RTCNT/RTCOR bájtos tárolás és támogatott reset00/FF. Teljes256
vezérlés,65536 számláló/compare pár,alias/word/reset és129 CPU-vágás.
Eredeti rögzített MAME forrás és gyártói7.2.2–7.2.4 alapján;aktív timer
referencia-differenciál még nincs. Futó óra/IRQ továbbra tiltott;a helyi ROM0F
írásaFFFFAD címen ezért változatlanul megáll. Ez előkészítés,nem boot-előrelépés.
Szerződés:[an1x-refresh-stopped.md](an1x-refresh-stopped.md).
Validáció:teljes futás99 meglévő teszt PASS;az új teszt kivételtípus-javítás
utáni célzott CTest is PASS (0,35s),így mind100 csoport ellenőrzött.
100 Python,strict célzott ASan/UBSan,forrásaudit567 fájl,notice229 fájl/5 szöveg
és diff-check PASS. CI sanitizer19 csoport;saját CI/review függőben.

## RFSHCR lezárás — 2026-11-02

A #175 beolvadt `fa6af2f88821960bddcb1bad3858ffde4c13c44c` commitként.
Ellenőrzött head:`fb41b93953de9badfd0bb2bdc1417047a75f2b5e`;mind az öt CI sikeres.
Futások:41441860362,41441860316,41441860278. Nyitott review-szál/változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:RTMCSR/timer
szerződés vizsgálata friss mainből,engedélyezett időzítés feltalálása nélkül.

## Megtartott RFSHCR profil — jelölt,2026-11-02

A beolvadt #174 után friss mainből:gyártói7.2.1 alapján FFFFAC=RFSHCR.
Csak00/02 írás fogadott,readback02;254 más érték elutasított. Timer/DRAM/PSRAM,
refresh-pin/mux/ciklus nincs szimulálva. Eredeti MAME map nem kezeli RFSHCR-t;
FFAD–FFAF timer címek továbbra tiltottak. 256 érték,alias/reset/RAM/idő/word
és129 CPU-vágás ellenőrzés. Részletek:[an1x-refresh-profile.md](an1x-refresh-profile.md).
Változatlan helyi ROM következő megállásaFFFFAD,PC0xA1C,perifériaciklus124;
64 teljes szeletciklus/19 callback,nem teljes boot vagy hang.
Helyi validáció:99/99 CTest (56,78s),100 Python és célzott strict ASan/UBSan PASS.
Forrásaudit565 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer18 csoport.
Saját CI/review és beolvasztás függőben.

## BRCR lezárás — 2026-11-02

A #174 beolvadt `e0bdde0048214930dc1a656b083b94af96147fc5` commitként.
Ellenőrzött head: `80097f42a2380b584e511d31286d4f3ea303f988`;mind az öt CI sikeres.
CI-futások:41435081433,41435081517,41435081672. Review-szál és változtatáskérés nincs;
automatikus review-eredmény nem volt rögzítve,nem tekintjük sikeres automatikus review-nak.
Következő lépés a 0xFFFFAC regiszter forrásalapú azonosítása friss mainből.

## Megtartott BRCR profil — korábbi jelölt, 2026-11-02

A #173 beolvadt `876050a1dd299dd44b9b95742d5b8067ecbc9404` commitként;
öt végső CI sikeres az e6fe544f12d00e858bce536800147c18b81954ed headen.
A macOS feladatok igazolt hosted-arm64 kapacitáshiány után újrapróbálva sikerültek;
kódhiba vagy szabálymegkerülés nem kellett. Automatikus review ugyanazon headen
2026-11-02T09:32:38Z-kor találat nélkül kész;bizonyíték a PR botösszegzésében.
Gyártói6.2.5 alapján BRCR0xFFFFF3 változatlanFE profil:16 tartalékbit-normalizált
írás elfogadott,240 buszátadást/cím-pinfunkciót módosító érték elutasított.
Nincs BREQ/BACK/arbitráció vagy fizikai pinmux-szimuláció;a logikai24-bit map marad.
256 érték,alias/reset/memória/idő,word előzetes elutasítás és129 CPU-vágás tesztelt.
A változatlan helyi ROM már64 teljes szeletciklust/19 callbacket teljesít,majd
0xFFFFAC bájtírásnál áll meg,PC0xA18,perifériaciklus116,öt GPIO-outputjelzéssel.
Ez nem teljes boot vagy hardveridő. Szerződés:[an1x-bus-release.md](an1x-bus-release.md).
Következő0xFFFFAC forrás/hardver-szerződésének azonosítása és korlátos kezelése.
Helyi validáció:98/98 CTest (50,51s),100 Python,célzott strict ASan/UBSan PASS.
Forrásaudit563 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer17 csoport.
Az IRQ-hole teszt korábbi BRCR-ismeretlencím elvárását frissítettük;word-őrök megmaradtak.
Saját CI/review és beolvasztás függőben.

## Programozott wait-state részprofil — jelölt, 2026-11-02

A #172 beolvadt `5aadb6279ea98f608550453b06953a7676e5372f` commitként;
öt CI sikeres aff9b87f42f56c806a3df3e3eb0e050df0a1d15b7 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Gyártói6.2.3/6.2.4 alapján WCR/WCER bájtoskezelés:programozott0–3 várakozó
állapot vagy controller-wait nélkül;pin-wait1/auto írás elutasítva. WCER-disabled
pin-wait0 ág explicit WAIT-inaktív kutatási feltétel,nem mért AN1x állapot.
WCR felső bitek read-one,WCER minden bit tárolt;csak engedélyezett külső3-state
ROM/SRAM kap3+count költséget. Kétállapotú/belső hozzáférés nem nyúlik meg.
Buszköltség-őr2..6-ra bővül;hideg/reset profilASTCR00/WCRF4/WCERFF,nem fizikai
alapállapot. Páros word és ismeretlen MMIO továbbra fail-closed.
256 WCR/256 WCER érték,független területmaszk,idő/counter/alulcsordulás,
reset/alias/word és257 CPU-vágás. Eredetnyilvántartás frissítve.
A változatlan helyi ROM már BRCR0xFFFFF3 írásnál áll meg,PC0x9FC,
perifériaciklus60. Teljes boot/hang/plugin továbbra sincs elfogadva.
Szerződés:[an1x-wait-profile.md](an1x-wait-profile.md). Következő korlátos BRCR profil.
Release build és mind97 CTest sikeres (81,60s);100 Python és strict célzott
ASan/UBSan sikeres,UINT64_MAX/overflow időhatárral is. CI sanitizer-szűrő16
csoportot választ. Forrásaudit561 fájl,notice229 fájl/5 szöveg,diff-check sikeres.
CI/review és beolvasztás függőben.

## ASTCR kiválasztott hozzáférési idő — jelölt, 2026-11-02

A #171 beolvadt `ecee85e7f8daf1039fac431e420072e8fe9e6491` commitként;
öt CI sikeres ac71f992a2c03049e6da13413765afa167c43f458 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Gyártói kézikönyv6.2.2:ASTCR0xFFFFED területenként2/3 állapotot választ.
A firmware0F-et ír,ezért a puszta latch nem lett volna elég. A buszsegéd új,
őrözött,pure access_cycles lekérdezése2/3 ciklust számol;default host továbbra2.
ROM/SRAM az ASTCR alsó bitjeit követi,belső területek régi2 ciklusos kutatási
költsége marad. ASTCR minden256 értéke tárolt. Reset00 kifejezetten no-wait kutatási
profil,NEM a kézikönyv fizikai FF resetje. WAIT/WCR/WCER késleltetés még nincs.
Független idő/counter/címteszt mind5 buszműveletre,költséghatár/reentry/alulcsordulás,
256 regiszterérték és193 CPU-vágás;adaptációs eredetnyilvántartás frissítve.
A változatlan helyi ROM most WCER0xFFFFEF bájtírásnál áll meg,PC0x9F4,
perifériaciklus44. Ez kutatási modell,nem hardveridő vagy bootelfogadás.
Részletek:[an1x-access-state.md](an1x-access-state.md). Következő WCER/wait-state profil.
Release build és mind96 CTest sikeres (72,86s);100 Python és strict célzott
ASan/UBSan sikeres. CI sanitizer-szűrő pontosan15 csoportot választ.
Forrásaudit559 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben.

## ABWCR leképezett-területi profil — jelölt, 2026-11-02

A #170 beolvadt `a9c1a2d4c3b6924463269c8985f38f62e1aa8611` commitként;
öt CI sikeres a19483ca548d73031933c523598568c9afda00fbd headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
A gyártói H8/3002 kézikönyv6.2.1 alapján0xFFFFEC az ABWCR. Projektprofil:
ROM/SRAM terület0/1 marad16-bit,ezért alsó2 bit nem állítható;felső6 bit
visszaolvasható,de az érintett külső területek továbbra sincsenek leképezve.
64 érték támogatott,192 elutasított. Reset00 explicit kutatási mód,nem igazolt
AN1x mode-pin állapot. Word/szomszédos ASTCR-hozzáférés továbbra is fail-closed.
Az első00-only policy a firmware08 írását elutasította;csak a területazonosítás
után bővült a megengedett profil. Forrás:[an1x-bus-width.md](an1x-bus-width.md).
Független256 érték,alias/reset/word/szomszédteszt,129 CPU-folytatási vágás.
A helyi változatlan ROM swap16 próbája már ASTCR0xFFFFED írásnál áll meg,
PC0x9F0,perifériaciklus36 (korábbanABWCR30). Teljes boot/hang/plugin nincs.
Release build és mind95 CTest sikeres (62,02s);100 Python és strict célzott
ASan/UBSan sikeres. CI sanitizer-szűrő pontosan14 csoportot választ.
Forrásaudit557 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben.
Következő:ASTCR és támogatott buszidőzítési profil célzott vizsgálata.

## Korlátos firmware-boot próba — jelölt, 2026-11-02

A #169 beolvadt `4cdc916e3bdd6e31eda23adfb184955b753b4ac1` commitként;
öt CI sikeres aza95352d262a4149ace591b908913ae5f76025938 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új in-process CLI explicit raw/swap16 memórianézettel,szigorú ciklus/szelet/callback
korlátokkal és JSON stopjelentéssel. A ROM csak olvasott,nem másoljuk repository-ba.
Hibás buszhozzáférés cím/típus/szélesség metaadata látható;ismeretlen GPIO-input
fail-closed. Nincs kitalált PKS/MIDI/LCD/DSP válasz vagy bootelfogadás.
Szintetikus CLI-teszt ellenőrzi a progresszt,ROM/MMIO/GPIO hibát,dekóder/callback
megállást,argumentumokat,bájtsorrendet,méretet és a változatlan bemeneti SHA-t.
Az első dekóderkontroll FFFF-je érvényes MOV volt;javítva invalid EEPMOV-kiterjesztésre.
Helyi1.04 ROM explicit swap16 nézetben reset0x9D8,első stop0xFFFFEC bájtírás,
PC0x9EC,perifériaciklus30. Eredeti SHA előtte/utána azonos. A kezdeti nyersnézet
bájtsorrendhibás kísérlet volt,nem firmware-bizonyíték;oddreset-őr bekerült.
Részletes szerződés:[an1x-boot-probe.md](an1x-boot-probe.md). Következő0xFFFFEC
forrás/hardver-szerződésének vizsgálata,majd célzott MMIO-fejlesztés.
Release build és mind94 CTest sikeres (51,24s);100 Python és strict célzott
ASan/UBSan CLI-teszt sikeres. CI sanitizer-szűrő pontosan13 csoportot választ.
Forrásaudit555 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben;teljes boot/hang/plugin továbbra sincs elfogadva.

## Gépszintű támogatott reset — jelölt, 2026-11-02

A #168 beolvadt `90e484efa59d66d7da0dae0ade34022afb11aefe` commitként;
öt CI sikeres a777d562505fe0966f8353d063238b6caf3e62f15 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új reset_supported_state gép-API:csak élő,álló gépen,CPU-adósság nélkül és nem
előrefutó perifériaidővel. Érvénytelen határ módosítás nélkül visszautasítva.
A jelen időpontra eső input előbb feldolgozódik,utána CPU-resetkérés,majd a
#168 szerinti buszperiféria-reset. Siker után külön reset-origin/számláló frissül;
abszolút idő,hideg generáció,RAM/GPIO és jövőbeli RX megmarad. Hideg újraépítés
külön nullás időpéldányt és nullás támogatottreset-számlálót ad.
Elkapott gépi reentry azonnal megjelöli a buszt is hibásnak,hogy ne legyenek
utána további kimenetek. Részleges resethiba fail-stop;hideg újraépítés helyreállít.
Teszt81 megállási pont:41 elfogadott,40 adósságos elutasítás;regiszter/RAM/idő,
literális SCI-kimenetsorrend,ismétlés,jövőbeli és pont időhatáron eső RX,reentry.
Ez a támogatott részhalmaz integrációja,nem fizikai RESET-assert/felengedés vagy
minden AN1x-eszköz elfogadása. Eredeti referencia MAME706dbb73281320c54d7c2f16262c1c3786f2be01
és rögzített reset/child-order források;új teljesgép upstream runtime trace nincs.
CI ASan/UBSan szűrő pontosan12 csoportot választ. Release build és mind93 CTest
sikeres (59,08s);100 Python és strict célzott ASan/UBSan sikeres. Elutasítás után
a ciklusadósság visszafizetése és az újabb reset is tesztelt. Rögzített reset és
konfigurációs források egyeznek;forrásaudit552 fájl,notice229 fájl/5 szöveg és
diff-check sikeres. CI/review és beolvasztás függőben.
Következő:korlátos firmware-boot diagnosztika és a hiányzó gépi MMIO/MIDI út felmérése.

## Támogatott buszperifériák resetje — jelölt, 2026-11-02

A #167 beolvadt `e0b8c9037913a054a8f37e5ccba19a0a571409ac` commitként;
öt CI sikeres a27ef4adae734e2beae33fb1611306c56242ccd10 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új busz API:reset_supported_peripherals álló tulajdonoson,SYSCR09/IRQ,majd
port4/6/7/8/9/A/B,SCI0/SCI1 sorrenddel. Az IRQ-szűrőt a megtartott CPU CCR-ből
újrapublikálja;fizikai szintek megmaradnak. RAM/GPIO-regiszter/inputütemezés és
abszolút idő változatlan;régi SCI-shifteresemények törlődnek. Az újra publikált
SCI órajel/TX-szintek a jelenlegi buszidőn jelennek meg. CPU-resetet nem indít.
A részhalmaz sorrendje projektbeli integráció,nem teljes MAME-eszközbejárás:
DMA/ADC/timer/watchdog/refresh és fizikai RESET-vonal nincs lefedve.
GPIO/SCI callbackben elnyelt reentry azonnal megállítja a további kimeneteket.
Független teszt3 abszolút kezdőidővel:szó szerinti kimenetsorrend,időbélyegek,
regiszterprofil,RAM/GPIO-megőrzés,tartott IRQ újralatch,függő RX,idempotens
GPIO-újrapublikálás és két reentry-határ. Az első elvárt nyom két GPIO-maszkját
javítottuk (port6:7F,port8:10 irány);motorkód miatt nem kellett módosítani.
A CI ASan/UBSan szűrő pontosan11 csoportot választ. Release build és mind92
CTest sikeres (72,06s);100 Python és strict célzott ASan/UBSan sikeres.
Rögzített reset/konfigurációforrások egyeznek;forrásaudit551 fájl,notice-lefedettség
229 fájl/5 szöveg és diff-check sikeres. CI/review és beolvasztás függőben.
Következő a CPU/busz reset összehangolása.

## Halasztott CPU-reset — jelölt, 2026-11-02

A #166 beolvadt `3bf3ed4dd4ff422b79918d8bc3889b812ec75e1f` commitként;
öt CI sikeres a 2b8528c0c2d7b57c1baa7925ceab9701b920b614 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új request_reset_after_debt CPU-átmenet csak álló,élő runneren:régi részutasítás
eldobása,reset-dispatch kijelölése,kért/felvett IRQ-könyvelés törlése.
Általános regiszterek,CCR és a már lezajlott buszhatások/ciklusadósság megmaradnak.
A következő futás előbb visszafizeti az adósságot,utána kezd reset-vektort olvasni.
Ez projektbeli halasztott CPU-szabály,nem fizikai RESET-vonal vagy teljesgép-reset.
A hostnak kell a perifériák/bemenetek resetjét külön rendeznie;gépszintű API nincs.
Független szó szerinti időnyom:3 abszolút kezdőidő,21 kérési pont,7 folytatási
vágás (441 eset),nemnulla regisztermegőrzés,ismételt kérés és reentry fail-stop.
Eredeti referencia változatlan:MAME706dbb73281320c54d7c2f16262c1c3786f2be01;
research/h83002-reset-contract.json és generált resetkezelők. Nem új teljes MAME
warm-reset runtime összevetés;az adaptációk eredetnyilvántartása frissítve.
Release build és mind91 CTest sikeres (85,62s);100 Python sikeres.
Strict célzott ASan/UBSan sikeres,a rögzített eredeti resetforrások egyeznek.
Forrásaudit550 fájl,notice-lefedettség229 fájl/5 szöveg,diff-check sikeres.
CI/review és beolvasztás még függőben.

## Gépi időtartomány-integráció — jelölt, 2026-11-02

A #165 beolvadt `09af5e8f9850d356f72c3557aa5c4bceb5ed74fe` commitként
(2026-11-02T02:42:54Z). Öt CI sikeres a bedad28721aa1d838a799c759ee9da3cfb13220e
headen;automatikus review-eredmény nincs rögzítve,nyitott review-szál nem volt.
A gép korábbi cursor_ számlálóját ResetTimeDomain abszolút ideje váltja fel.
Sikeres szelet végén frissül;hibás tartomány nem változtatja az időt,a részleges
végrehajtási hiba továbbra is fail-stop. Hideg újraépítés új nullás időpéldányt
kap,meglévő gépgenerációval. CPU-adósság és input-határidő nem kerül átírásra;
reset_origin nincs meghívva,nincs új meleg-reset API vagy hardveres elfogadás.
401 vágási ponton összevetjük az egyben/szétvágott CPU-állapotot,RX-fogyasztást
és TX-idősort;nullás/negatív szeletek és hideg újraindítás is ellenőrzött.
CI ASan/UBSan készlet bővül az idősegéddel és a teljes gépéletciklussal.
Helyi Release build és mind91 CTest sikeres (73,52s);100 Python sikeres.
Célzott strict ASan/UBSan gépteszt sikeres;CI-szűrő pontosan10 csoportot választ.
Forrásaudit550 fájl,notice-lefedettség229 fájl/5 szöveg és diff-check sikeres.
Távoli CI és végső review még függőben.
Következő:korlátos reset-owner szerződés a fennmaradó input/scheduler feltételekkel,
majd firmware-boot/MIDI és DSP-hangút;GUI és ROM-módosítás nélkül.

## Reset-időtartomány segéd — jelölt, 2026-11-02

A #164 beolvadt e620483c7d95e07aeb2bce69431fdb2a87c1fd03;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Új önálló ResetTimeDomain:
abszolút idő monoton,reset-origin/generáció külön,átváltási túlcsordulás elutasítva.
Három kezdőidő,300 reset,visszalépés és64 bites határesetek független tesztje.
Célzott strict ASan/UBSan sikeres. Még nincs bekötve a gépbe;CPU-adósságot,
inputsort vagy perifériát nem módosít,nem teljesgép-reset. A kezdeti fordítási
hiba hiányzó initializer_list fejléc volt,javítva. Következő a gép időkezelési
integrációja definiált nyugalmi határon.

## Reset időtartományok — jelölt, 2026-11-02

A #163 beolvadt d513d4fdf8ff7c9ba0eedc064fab9247a9355f64;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Négy eredeti időkezelési módszer
azonossága rögzítve:local_time,total_cycles,abort_timeslice,synchronize.
A resetelt CPU-ciklusszám nem azonos az abszolút perifériaidővel;szinkronizáció
zero-delay timerrel történik,nem közvetlen callbackkel. Következő:korlátos
időtartomány-szabály független tesztje,majd resetintegráció. Motorkód változatlan.
100 Python sikeres;teljes runtime készletet kutatási változás miatt nem futtattunk újra.

## Sorba állított RESET-vonal — jelölt, 2026-11-02

A #162 beolvadt36b71314dfae075b046b5b0bde34b92f0acb3c9b;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az eredeti empty_event_queue
callback azonossága is rögzítve. RESET ASSERT felfüggeszt,felengedés resetel,
majd folytat. A perifériakimenetek assertion alatti kezelésére upstream FIXME van.
A queue-index a callback végén törlődik,nem a device_input::reset-ben.
Új callback-azonosság teszt:megváltozott/hiányzó/dupla definíció.99 Python sikeres.
Motorkód változatlan;következő a scheduler-szinkronizáció és helyi idő vizsgálata.

## Framework owner-feloldás — jelölt, 2026-11-02

A #161 beolvadt a5b73ac6da7276f5afa4bbb1794b40a319a6a41f;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az eredeti mconfig.cpp teljes
fájlazonossága rögzítve. Forrásvizsgálat:resolve_owner kettőspontonként meglévő
szülőt keres,add_device a szülő listájának végére illeszt,majd konfigurál.
Ebből a nested DMA/timer-bejárás következik,nem22 közvetlen CPU-gyerek.
Ez forrásból levont következtetés,nem teljes runtime resettrace;SCI-base címkék,
konfigurációs overloadok és scheduler/bemeneti queue-idő továbbra is vizsgálandó.
Motorkód változatlan;98 Python és lockolt forrásazonosság sikeres.

## H83002 konstruktorcímkék — jelölt, 2026-11-02

A #160 beolvadt2b95cfe97bd6a292593871bbb51d0e407ab32988;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. A leltár most a konstruktor
finder-címkéit is ellenőrzi:DMA dma:0/1,időzítők timer16:0…4.
A SCI címkéit nem következteti ki a származtatott forrásból. Új független
teszt vizsgálja a beágyazott címkét és a hiányzó finder elutasítását.
Motorkód változatlan;a tényleges framework-owner/resetbejárás továbbra is nyitott.

## H83002 perifériakonfiguráció — jelölt, 2026-11-01

A #159 beolvadt0b61abcf2b03c28979f173a3cce30ec92f7bbd59;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az új leltár22 konfigurációs
bejegyzés sorrendjét és eredeti forrásazonosságát ellenőrzi. Különbség:
lexikai létrehozás nem egyenlő a tényleges szülő/gyerek resetbejárással.
Két új regressziós teszt:sorrendcsere/duplikáció és hiányzó módszer.
Motorkód nem változott;teljesgép-reset továbbra sincs. Következő:owner-feloldás,
eszközlista-beillesztés és idő/bemeneti eseménykezelés.

## Execution input reset pontosítása — jelölt, 2026-11-01

A #158 beolvadt a25f29c913c53e664d1019b6ce2d80dab694ee1e,
2026-11-01T21:42:20Z;négy CI sikeres,nem volt rögzített automatikus review-eredmény.
A negyedik rögzített framework-módszer a device_input::reset:csak az aktuális
és tárolt IRQ-vektort állítja alaphelyzetbe. Bemeneti szintet vagy queue-indexet
nem ír ebben a módszerben. A tágabb queue-törlési komment nem viselkedési bizonyíték.
A teljes reset-sorrend/idővonal továbbra is nyitott;motorkód nem változott.
95 Python sikeres;valódi lockolt archívum négy módszere egyezik.
Következő:gyerekek konkrét sorrendje és ütemezett bemenet/idő kezelés.

## Auditjavítás 3 — dokumentáció egységesítése, jelölt, 2026-11-01

A #157 beolvadt15217f7fe1ffa8e6894730b24357feac311c1c0d,
2026-11-01T19:52:16Z;pontos head49f64aa142409fef04fbf7393caeaada053971b0.
README/státusz/roadmap aktuális összefoglalója90 CTest/95 Python leltárra,
#154–157 beolvadt változásokra és a valóban nyitott gépintegrációs kapukra frissült.
A régi jelölt/pending bejegyzések történeti állapotként elkülönítve maradnak.
A teljes helyi90/95 bizonyíték #156-hoz,a nyolc sanitizer-csoport #157-hez tartozik.
Ez csak dokumentáció;runtime tesztet nem futtatunk újra. Jelölt,nem beolvadt.
Következő:execution-input reset,gyereksorrend és scheduler-idő kutatása/kis tesztelt
integrációja. Gépszintű reset,firmware/MIDI/PKS és hang továbbra sincs elfogadva.

## Historical development snapshots

Earlier candidate descriptions below record their preparation-time state;
merged status is tracked in development-ledger.json and the current summary.

## Auditjavítás 2 — sanitizer CI-lefedettség, jelölt, 2026-11-01

A #156 beolvadt a4f91cefc4ccfa7669e6dbc7eee3459d1137b43e,
2026-11-01T17:53:56Z;öt CI sikeres a39c421da743feecb3e5aea808f4cd012c9c026b9 headen.
Az új Ubuntu CI-lépés külön ASan+UBSan buildből futtatja a CPU-reset,SCI,
IRQ,GPIO,runner,busz,dispatcher és újrabelépési regresszió nyolc tesztjét.
A tesztszűrő nulla találatnál is hibát jelez (--no-tests=error).
Helyi ASan/UBSan:8/8 sikeres;a CI-regex pontosan8 regisztrált tesztet választ.
Motorkód változatlan;CI és napló módosul. A harmadik,dokumentációs egységesítési
csomag csak ennek beolvadása után következik. Jelölt,nem beolvadt.

## Hibakezelés és forrásazonosság — jelölt, 2026-11-01

A #155 beolvadt e581c7b36dad130efc3254fcd25b18c7f02ea5bb,
2026-11-01T17:32:32Z;négy CI sikeres a f9e2ae067ab0bed312be895aebc59daf30224511 headen.
Az első auditjavító csomag a dispatcher,CPU-runner és buszsegéd elnyelt
újrabelépési kivételét is végleges hibává teszi. A külső hívás sem adhat sikert,
és a hibás owner újra nem használható. A resetarchívumot hash-ellenőrzött
privát ideiglenes másolatból olvassuk;a forrásnév későbbi cseréje nem hat rá.
Új regressziós C++/Python kontrollok;95 Python és célzott ASan/UBSan sikeres.
Teljes build és90/90 CTest sikeres;95 Python sikeres;célzott ASan/UBSan sikeres.
A regressziós teszt a korábbi kódot elutasítja. Jelölt,nem beolvadt. A CI-bővítés és általános
állapotdokumentáció külön következő PR lesz,csak e csomag beolvadása után.

## #155 Windows SVG-hash javítás — 2026-11-01

A Windows CI 88 tesztet teljesített,de a repository-audit az SVG hashét elutasította.
A text=auto sortöréskonverzió módosította a bájtpontosan rögzített logót.
A .gitattributes most kizárja ezt az egy SVG-t a szövegkonverzióból;az eredeti
hash és az audit szigorúsága változatlan. A motor nem módosult.

## Arany vektorlogó a README elején — jelölt, 2026-11-01

A projektgazda kérésére az arany,valódi vektoros logó került a legfelső sorba.
A H1-blokk helyett kép és közvetlen sortörés következik,üres bekezdés nélkül;
a projektleírás közvetlenül alatta kezdődik. Az SVG átlátszó,beágyazott PNG nélkül.
Az asset ellenőrzőösszege és eredetleírása frissült. Motorkód változatlan.

## README logó és teljes aktualizálás — jelölt, 2026-11-01

A projektgazda átlátszó SVG-logója váltja a felső szöveges címet,520px szélességgel,
YamAN1x alternatív szöveggel és H1 szerkezettel. Pontos fájlhash-kivétel védi
az assetet;a forrás/engedély leírás frissült. A README tesztszámai89 CTest/94 Python,
a beolvadt és jelölt állapot elkülönül,a korábbi audit történeti dátuma egyértelmű.
A teljes README áttekintve;motor/ROM/GUI nem változott. A #154 része,még nem beolvadt.

## Keretrendszer reset-sorrend — jelölt, 2026-11-01

A #153 beolvadt1a2468855cf35ee5641d586bd1b6777a5e75f71f,
2026-10-31T19:07:13Z;aktuális head8f314a5c93471300b7ac6a478416c4d0514cd005.
Négy CI sikeres (41225516208,41225516296);sikeres automatikus review-t nem állítunk.
Az új archívumellenőrző két eredeti framework-fájl/három módszer azonosságát
rögzíti. Sorrend:interface pre-reset,szülő reset,gyerekek resetje,szülő utómunka,
interface post-reset. Az execution-interface külön törli a ciklusszámlálót és
reseteli a bemeneti objektumokat;ezért a komponensbemenet-megőrzési tesztek
nem bizonyítják a teljes framework viselkedését. Következő:bemeneti objektumok,
gyerek-létrehozási sorrend és scheduler-idő. Új melegreset-API nincs.
94 Python sikeres;teljes runtime készletet kutatási változás miatt nem futtattunk újra.
Az automatika továbbra is leállítva;ez a felhasználó által kért folytatás.

## README hardverfotó — jelölt, 2026-10-31

A projektgazda saját AN1x-fotója a README projektleírása alá került.
Angol képaláírás külön jelzi,hogy eredeti fizikai hardver,nem plugin vagy GUI.
Az eredeti képfájl változatlan;az upload-ellenőrző csak ezt az egy pontos
SHA256-azonosságú képet engedi át. Más bináris fájlok tiltása megmarad.
Külön képeredet/engedély leírás készült;új képlicencet nem feltételezünk.

## Aktuális összefoglaló az első REAPER-hangig — 2026-10-31

A #152 beolvadt `8b2930234de7b0b512f8ecdb9f04414782f14a53`,
2026-10-31T17:02:44Z (Budapest18:02:44). A pontos head
`cfca0c785364cb44f8d478735a2b8d82b84d16e7` négy CI-feladata sikeres:
41210850333 és41210850351. Az automatikus review kvótakorlát miatt nem készült el;
ez nem kódhibát bizonyító review,és nem sikeres review. Bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/152#issuecomment-6544784804
A beolvadást élő GitHub-adat igazolja;az asszisztens korábban visszatartotta.
A húszperces automatika leállítva marad;ez a dokumentációfrissítés nem indítja újra.

Hátralévő fő lépések,függőségi sorrendben:

1. **Teljes gépintegráció:** CPU,busz,szükséges perifériák és DSP közös idővonala;
   indulási/reset-sorrend és megőrzött állapotok igazolása. A komponens-resettesztek
   beolvadtak,de gépszintű melegreset még nincs. Csak a firmware által ténylegesen
   igényelt hiányzó CPU/periféria-funkciókat kell célzottan pótolni.
2. **Firmware-indulás és MIDI:** a helyi eredeti firmware stabil indulása,
   PKS-kommunikáció szükséges viselkedésének tisztázása,SCI0 valódi byte-vétel,
   note-on/note-off firmware-szintű feldolgozás. A PKS ROM hiánya nincs megoldva;
   szükséges protokollhelyettesítés vagy további hardveres adat még vizsgálandó.
3. **Egy voice DSP-jének elfogadása:** táblarom-címzés,órajel,EG és visszaolvasások,
   CPU–DSP sorrend és chipközi jelút. A programfeltöltés önmagában nem hangbizonyíték.
4. **Első offline hang:** dokumentált száraz kimenet és PCM-skálázás;WAV note-on,
   tartás,note-off és lecsengés eseményekkel. Nemnulla,véges és reprodukálható jel.
   MEG csak akkor halasztható,ha a kiválasztott száraz hangút valóban működik nélküle.
5. **Valós idejű beágyazott motor:** időbélyeges MIDI,audioblokkok,mintavételi
   illesztés,előre foglalt memória,határidőmérés és beragadt hangok kezelése.
6. **Minimális VST3:** MIDI/audio adapter,példányonkénti állapot,helyi ROM-betöltés,
   csomagolás és validálás. GUI-fejlesztés továbbra sincs engedélyezve;az első próbához
   hosztparaméter/konfiguráció használható. AU és kidolgozott felület későbbi lépés.
7. **REAPER-elfogadás:** plugin felismerése,MIDI-hang indítása/elengedése,lehallgatás,
   WAV-mentés,offline összehasonlítás és projekt újranyitása rögzített környezetben.

Jelenleg nincs igazolt AN1x-hang,VST3 vagy AU. A sok sikeres CPU/perifériateszt
nem jelenti a firmware/DSP/hangút elfogadását. Az első hang legnagyobb nyitott
kockázata az indulási/MIDI–PKS út és az egy voice-hoz szükséges DSP-viselkedés.
A teljes hardver minden funkciójának emulálása nem előfeltétele az első hangnak.

## Korábbi előkészítési bejegyzések — történeti állapot

Az alábbi jelölt/függőben megjelölések a készítéskori állapotot őrzik.
A jelenlegi állapotot a fenti összefoglaló és a strukturált beolvadási napló adja.

## SCI reset utáni vétel — jelölt, 2026-10-31

A #151 beolvadt e4e79d4e0244a05784b7da705e9496f601446e99,
2026-10-31T14:14:36Z;négy CI sikeres,aktuális head review észrevétel nélkül
lezárult (issuecomment-6544283995).
Az új teszt mind256 bájtot fogadja reset után,1000 abszolút kezdőidővel.
Előtte73 ciklusnál megszakít egy régi vételt. Az alacsonyan tartott RX újraengedélyezése
nem képez hamis startélt;magasra váltás után az új keret pontos bájtot,SSR-t,
egyetlen vételi IRQ-t és hibamentes üres eseménysort eredményez. DMA-nyugtázás
csak az új vételi készjelzést törli. Ez komponensvizsgálat,nem gépszintű reset.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód változatlan;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## SCI reset utáni adás — jelölt, 2026-10-31

A #150 beolvadt1ef73ef6fe3f198c2a82d9e962a282860218dd79,
2026-10-31T13:34:33Z;négy CI sikeres,aktuális head review észrevétel nélkül
lezárult (issuecomment-6543843939).
Az új teszt mind256 bájtot újraküldi reset után,1000 abszolút kezdőidővel.
Előtte73 ciklusnál megszakít egy másik keretet. Független,bitenként és időponttal
megadott elvárás ellenőrzi a keretet,az egyszeri kész/befejezés IRQ-t és az
üres eseménysort. Ez komponenshelyreállítás,nem gépszintű melegreset.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód változatlan;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## IRQ-reset bemenetmegőrzés — jelölt, 2026-10-31

A #149 beolvadt1cb2f4a229fac079d5b318f5187ed2a340161838,
2026-10-31T12:54:35Z;négy CI sikeres,az aktuális head automatikus review-ja
észrevétel nélkül lezárult (issuecomment-6543431784).
Az új teszt65536 tartott IRQ/engedélyezési maszkot ellenőriz:reset után
újralatchelt szintek,törölt regiszterek,owner-frissítésig megtartott publikáció,
legalacsonyabb engedélyezett vektor,megtartott NMI-bemenet és új él elfogadása.
Ez komponensszerződés,nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt végső CI/review függőben.

## Aktív SCI reset teszt — jelölt, 2026-10-31

A #148 beolvadt1ec8ff3b657816e736930fe8029427409777f555,
2026-10-31T11:09:52Z;négy CI sikeres,automatikus review-eredmény nem volt rögzítve.
Az új komponensvizsgálat401 ciklusponton szakítja meg az egyidejű TX/RX-et:
clock magas majd TX magas,regiszteralapértékek,üres határidő,megőrzött alacsony
RX és régi befejezési/IRQ események hiánya. Ismételt reset mindkét magas
kimenetet újra publikálja. Ez nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest és ASan/UBSan,91 Python,eredeti resetazonosság sikeres.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## GPIO-reset megőrzési teszt — jelölt, 2026-10-31

A #147 beolvadt:b39643f330314f21e16a946c0e7e00e5a5eb41d0,
2026-10-31T09:59:11Z;négy CI sikeres,pontos head618667b31cd2ca7e6c80c063face8d844ed19aeb.
Automatikus review-eredmény nem volt rögzítve.
Az új teszt196608 maszk/irány/adat kombináción három ismételt resetet
ellenőriz. Független elvárás a DDR/DR/PCR/ODR megőrzése,a változatlan
kimenetértesítés elnyomása és a lábolvasás bitenként számolt eredménye.
Ez komponensvizsgálat,nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest,ASan/UBSan és91 Python sikeres;eredeti resetazonosság egyezik.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt végső CI/review még függőben.

## CPU-reset végrehajtási teszt — jelölt, 2026-10-31

A #146 beolvadt `41a1c1cea0b777653ede9e470ec5e6e7797b2ff2`,
2026-10-31T08:59:10Z;négy CI sikeres,review-eredmény nem volt rögzítve.
Az új független CPU-indulási teszt256 CCR×3 kezdőidő×7 szeletelés=5376
folytatási esetet ellenőriz:vektorolvasások,prefetch,filterközlés sorrendje,
megőrzött bitek,EXR,részállapot és időadósság. A filternek nincs időargumentuma;
a teszt az előző buszművelet idejét és a sorrendet rögzíti.
Strict fordítás és célzott ASan/UBSan sikeres;az eredeti generált MAME H8H
összehasonlítás200000 szeleten nulla eltérést mutat. Teljes91 Python sikeres.
Ez hideg CPU-indulás,nem új melegreset-API vagy teljesgép-reset bizonyíték.
Teljes build és89 CTest sikeres;CCR-törlési negatív kontroll6858 hibát jelez.
Végső CI/review függőben;a jelölt még nincs beolvadva.

## #145 lezárása és CPU-reset határvizsgálat — 2026-10-31

A #145 beolvadt: `e470f406c2d65eccda70e0c6381db1f826b2810b`,
2026-10-30T22:25:47Z;ellenőrzött head `90f8aea04fdddce17de60fe706b690b3b5f62c3b`.
Öt CI-feladat sikeres (41134650020,41134651634,41134649966).
Automatikus review-eredmény nem volt rögzítve;nem állítunk sikeres review-t.
A [resetdokumentum](h83002-warm-reset-contract.md) új helyi kódvizsgálata külön
rögzíti a CCR megőrzését,vektorolvasási/prefetch sorrendet,részállapotokat és
az időadósság/folytonos idővonal nyitott döntéseit. Ez dokumentációs jelölt,
nem végrehajtott upstream reset-differenciál és nem új melegreset-API.
Motorkód nem változott;runtime tesztet e dokumentáció miatt nem futtattunk újra.
Következő:a felsorolt resetmegfigyelések független végrehajtási tesztjei,
majd eszközsorrend és nem nulla időpont megtartása.

## Aktív kutatási lépés — rögzített melegreset-szerződés, 2026-10-30

Öt eredeti forrásfájl/hat device_reset módszer SHA256-azonossága reprodukálható
archívumellenőrzővel rögzítve. GPIO csak kimenetet frissít,SCI magas clock/TX-et
publikál,IRQ törlés után szintbemenetet újraértékel;nem általános nullázás.
[Mátrix és nyitott implementációs kapuk](h83002-warm-reset-contract.md).
Ez forráskutatás,nem melegreset-API vagy teljesgép/hardveregyezés.
Négy új független Python kontroll;teljes91 Python PASS. Motorkód nem változott;
a korábbi88 CTest/87 Python bizonyíték #143-hoz tartozik,nem újrafuttatott eredmény.
A #144 beolvadt9ddf040ec03ffe298dfdfcc259330c99ff1e570a,2026-10-30T21:52:30Z;
négy CI sikeres,automatikus review-eredmény nem volt rögzítve.
Következő:generált CPU-reset/runner és eszköz-reset-sorrend/időszabály ellenőrzése,
majd aktív SCI/IRQ/GPIO megőrzési és eseményközlési tesztek;teljes reset még nyitott.

## Aktuális lezárás — #143 hideg kutatási életciklus, 2026-10-30

Beolvadt `301eec75c72028b8f377ee4e099fce0b1df4de5e`,tesztelt head `d2da27c15c8ebf8c083f35bb4a2cef18eba4d8c4`.
Időpont:2026-10-30 22:27:12 Europe/Budapest (21:27:12 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127313949),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127314006),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127313987):mind az öt feladat sikeres.
Tényleges macOS15,a required check történeti macos-14 neve kompatibilitási okból marad.
Nincs rögzített automatikus review-eredmény;nincs sikeres review-állítás.
88 CTest/87 Python,célzott ASan/UBSan és szigorú fordítói ellenőrzés PASS.
401 újraindítási pont friss CPU/TX-példánnyal egyezik;teljes256KiB NVRAM8 epochon
megmarad,explicit clear nulláz. Érvénytelen bemeneti csere megtartja a régi epochot.
Hibás/reentráns futás csak rekonstrukcióval áll helyre;ROM/owner/RX-sor izoláció PASS.
NVRAM-visszatöltést kihagyó próbahiba410 elvárt kudarc.
[Hatókör](an1x-research-lifecycle.md):hideg kutatási rekonstrukció,nem hardveres meleg reset.
533 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
Nincs aktív motorjelölt. Következő:a rögzített forrás melegreset-megőrzési/kimenetközlési
mátrixa,globális reset-sorrend és idő/bemeneti szabályok;csak utána in-place API.
Aktív SCI,IRQ-pin,GPIO-latch,RAM és függő esemény független tesztje szükséges.
Teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER még nyitott.
Új motorlépés a lezáró dokumentáció beolvadása után indulhat.

## CI-javítás — macOS14 kivezetése, 2026-10-30

A #142 macOS-feladata és célzott újrafuttatása a GitHub tervezett brownoutja miatt
megszakadt,nem fordítási/teszthiba miatt. A futtató2026-11-02 dátummal megszűnik.
Mindkét aktív workflow macos-15-re vált. A kötelező check történeti neve
`build-and-test (macos-14)` marad a ruleset25910947 kompatibilitása miatt;
a tényleges runner macos-15,a teljes tesztlépések változatlanok. Nincs szabálymegkerülés.
A MAME-kutató build Xcode16.2 rögzítése marad:a macOS15 hivatalos képe tartalmazza.
Új CI/review az új headen szükséges;korábbi zöldek nem igazolják az új workflowt.
Forrás:https://github.com/actions/runner-images/issues/13518
Kép:https://github.com/actions/runner-images/blob/main/images/macos/macos-15-Readme.md

## Történeti lezárás — #141 közös SYSCR, 2026-10-30

A #141 beolvadt `eba55a2367f9cc5e319b4f3f26049fe4da55af16`,tesztelt head `d50532a812abd23a65a2aafaf24f28ea815c0217`.
Időpont:2026-10-30 17:23:15 Europe/Budapest (16:23:15 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891561),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891536),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891444):mind az öt feladat sikeres.
Automatikus review-eredmény nem volt rögzítve;nincs sikeres review-állítás.
87 CTest/87 Python és célzott ASan/UBSan PASS;7712 IRQ/NMI-partíció,512 TRAPA CCR-eset,
241 SLEEP közbeni módváltás PASS. Fix módra visszaállított próbahiba2056 elvárt kudarc.
[Hatókör](an1x-syscr.md):csak01/09,közös owner,azonnali filter,helyes IRQ/TRAPA-belépés.
Generált H8/H8H-törzsek újragenerált lockkal egyeznek;562 módszer/264 kiválasztott sor változatlan.
A korábbi15424-es köztes darabszámot7712-re javítottuk;az integrációs teszt ezt igazolja.
Nincs aktív motorjelölt. Következő:explicit reset-életciklus,owner-állapotok,NVRAM-megőrzés,
idő/bemeneti sor kezelése,ismételt reset és hibás owner kezelése független tesztekkel.
Teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER továbbra is nyitott.
Új motorlépés a lezáró dokumentáció beolvadása után indulhat.

## Lezárt előkészítő lépés — #139 SYSCR módlogika, 2026-10-30

Beolvadt `b84d3b20bbef1f51a14af64da1a57df9f5cf2a88`,tesztelt head
`4d5ca9ba548cd4b3c3547ba191c2660daeebfce4`,2026-10-30 16:12:00 Europe/Budapest
(15:12:00 UTC). Mind az öt CI sikeres:
[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41072881939),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41072881943),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41072882034).
Nincs nyitott review-szál vagy rögzített automatikus review-eredmény;utóbbit nem állítjuk.

86 CTest/87 Python és célzott ASan/UBSan PASS.65536 mód/CCR eset és három eredeti
MAME-módszer összevetése:0 eltérés;hibás módszűrés8192 elvárt kudarc.
[Hatókör és következő integrációs kapuk](h83002-syscr-modes.md).
Ez önálló előkészítés:FFFFF2 továbbra is unmapped,a CPU rögzített profilja változatlan.
Az integrált gépalap #136 marad;nincs új firmware-/hangelfogadás.
Következő:közös SYSCR-owner,azonnali IRQ-filter és IRQ/TRAPA-belépés módváltás,
NMI/függőIRQ/SLEEP/RTE és partíciótesztek;utána teljes gépreset.
Nincs aktív motorjelölt;a lezáró dokumentáció beolvadása után indulhat új lépés.

## Dokumentáció-egyeztetés — 2026-10-30, #137 után

Ellenőrzött main: `7ee6db8bc418f696e67f0ea17fcc8dade073e47d`.
A [#137 dokumentációs lezárás](https://github.com/RobCZart82/yaman1x/pull/137)
2026-10-30 10:26:30 Europe/Budapest időpontban beolvadt (09:26:30 UTC).
Tesztelt head: `e7b5c345b2ed9dc0e38b1fa0ecc615b106602575`.
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41031322824),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41031322822): mind a négy feladat sikeres.
MAME ezen a dokumentációs változáson nem futott; a motor MAME-bizonyítéka a #136-hoz tartozik.
A #137 automatikus kódellenőrzése kvótalimit miatt nem fejeződött be; ez nem
sikeres review, és nem konkrét kódhiba. A #136 sikeres review-ja külön bizonyíték.

A motoralap továbbra is #136; nincs új motorfunkció vagy új firmware-/hangelfogadás.
A lezáró dokumentáció már beolvadt, a következő motorlépés megkezdhető:
közös CPU/host SYSCR-állapot, szűrés és IRQ/TRAPA-belépés módtesztjei, majd teljes gépreset.
Az alábbi korábbi lezárások és jelöltbejegyzések dátumhoz kötött történeti adatok;
régi „függőben” vagy „következő” megjegyzésük nem az aktuális feladatsor.

A mostani dokumentációjavítás rögzíti a #137 lezárását a strukturált naplóban,
frissíti az M0 elavult 31 hiányzó kódolássoros feladatát (jelenleg 264 kiválasztott,
0 hiányzó sor), és pontosítja a README korábbi komponensleírásainak hatókörét.
Helyi ellenőrzés: UTF-8 és belső linkek, JSON/egyedi PR-azonosítók/merge-ősök,
85 CTest/87 Python leltár, forrásaudit és licencértesítés-lefedettség PASS.
A futásidejű teszteket ehhez a kizárólag dokumentációs javításhoz nem futtattuk újra.
A javítás saját [#138 PR-ellenőrzései](https://github.com/RobCZart82/yaman1x/pull/138) ott követhetők; az itt hivatkozott korábbi CI
nem ennek az új dokumentációs változatnak az eredménye.

## Legutóbbi motorlezárás — #136, 2026-10-30

A [#136 PR](https://github.com/RobCZart82/yaman1x/pull/136) beolvadt:
merge `7aaa8d0178e06b8161c8508cda1cebdb20e2c072`,tesztelt head `6d58a3df9307b4513b61ea01d9eed3d70bd6e87a`.
Időpont:2026-10-30 10:04:38 Europe/Budapest (2026-10-30T09:04:38Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141599),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141600),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141620):mind az öt feladat sikeres.
Review:`completed_no_findings`;CI és review külön bizonyíték.

Beolvadt:**85 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
IRQ-MMIO:ISCR F4,IER F5,ISR F6,ICR0/1 F8/F9;szóalak F4/F8,24 bites címalias.
Odd/hiányos szó előzetesen elutasítva. ICR nem komplementált maszk és késleltetett
publikáció megőrizve (nem igazolt fizikai helyesség).65536 engedélyezés/szint,
65536 törlés,131072 szó,2048 él/szint és3856 CPU-partíció PASS. CPU-ból beállított
IRQ SLEEP-et ébreszt,ISR A5-öt ír NVRAM-ba,RTE SP-t helyreállít;I maszkol.
Eredeti18 IRQ-művelet/table200000 lépésben nulla eltérés. Fordított ICR-offset150185,
téves IER=>ISCR199554 hibával felismerve. Helyi84 futtatható CTest/86 Python PASS;
ismert Windows-symlink eset kizárva. Linux CI UBSan PASS;helyi ASan/UBSan nem futott.
525 fájl forrásauditja/229 fájl licencnyilvántartása PASS (nem jogi engedély).
[Pontos források,hatókör és SYSCR-feltárás](an1x-irq-mmio.md).

A SYSCR bit08 a CPU szűrését és IRQ/TRAPA-belépését együtt érinti. Fix0B profil
marad,F2 olvasás/írás elutasítva,nincs elhallgatott vagy kitalált működés.
Nincs aktív motorjelölt. Következő:külön CPU/host SYSCR-szerződés és módtesztek,
majd teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER elfogadás nyitott.
A lezáró dokumentáció #137-ben beolvadt; ez a sorrendi feltétel teljesült.

## Történeti lezárás — #134, 2026-10-30

A [#134 PR](https://github.com/RobCZart82/yaman1x/pull/134) beolvadt:
merge `7c46a56495a166ed9e6abbe4bea11915e08bbedd`,tesztelt head `963fc91ca7512a80e5830dc8a784c8c186c9e5e9`.
Időpont:2026-10-30 08:46:00 Europe/Budapest (2026-10-30T07:46:00Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299804),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299832),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299487):mind az öt feladat sikeres.
Review:`no_recorded_result_at_inspection`;CI és review külön bizonyíték.

Beolvadt:**84 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
GPIO-szóolvasás:FFFFCE,D0,D2;írás:D0,D2. Előzetes ellenőrzés az odd/hiányos/readonly
írás ellen. Big-endian,felső bájt előbb,azonos buszidőbélyeg:explicit szoftverszabály,
nem fizikai időzítési vagy teljes MAME-címtér-egyezési állítás. A CPU egy szóhozzáférést
számol. Első bájt reentry-je után a második nem fut;korábbi mellékhatás nem vonódik vissza.
196608 független eset,241 CPU-partíció PASS;DA65 NVRAM/R2,CCR89 ellenőrizve.
Eredeti portműveletek soros kompozíciója200000 lépésben nulla eltérés. Felcserélt
olvasási bájt262198,hibás alsóbájtírás455493 hibás ellenőrzéssel felismerve.
Helyi83 futtatható CTest/86 Python PASS;ismert Windows-symlink eset kizárva.
Linux CI UBSan PASS;helyi ASan/UBSan nem futott.523 fájl forrásauditja/229 fájl
licencnyilvántartása PASS (nem jogi engedély). [Pontos szerződés](an1x-gpio-words.md).
Nincs aktív motorjelölt. Következő:IRQ/SYSCR szűkített bekötésének feltárása,majd
teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER elfogadás nyitott. Új motorlépés
csak e lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #132, 2026-10-30

A [#132 PR](https://github.com/RobCZart82/yaman1x/pull/132) beolvadt:
merge `5ac2585e9b84497764f1fe0e2bbf426bbbab84a6`,tesztelt head `66f480ea43b73551e934ec6528ca1dae4a5c5171`.
Időpont:2026-10-30 06:59:33 Europe/Budapest (2026-10-30T05:59:33Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924793),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924918),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924833):mind az öt feladat sikeres.
Review állapot:`no_recorded_result_at_inspection`;CI- és review-bizonyíték külön kezelve.

Beolvadt:**83 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
PortB FFFFD4/FFFFD6,alap00/maszk00,külön állapot,callback ID11.262144 bit-orákulumos
eset,256 hideg bemenet,241 CPU-partíció PASS.0F/5A/15 =>1A NVRAM,CCR81.
Eredeti portműveletek200000 lépésben nulla eltérés. A referencia11 cpp-törzset
és inline FF-olvasást ellenőriz (összesen12 eredeti törzs). Téves maszk225082,
téves bemeneti port244961 hibás ellenőrzéssel felismerve;iterációnként két
ellenőrzési hely miatt a szám meghaladhatja a műveletszámot.
Helyi82 futtatható CTest/86 Python PASS;ismert Windows-symlink eset kizárva.
Linux CI UBSan PASS;helyi ASan/UBSan ebben a lépésben nem futott.
521 fájl forrásauditja és229 fájl licencnyilvántartása PASS (nem jogi engedély).

Auditjavítások:IR öt szóra frissítve,régi SLEEP/GPIO-korlátok történeti hatóköre
jelölve,README build-tesztszám66-ról83-ra javítva. A README eltérése az előző
áttekintésből kimaradt;most javítva. A ROADMAP régi #78-jelöltfelirata is történeti jelölést kapott. [Pontos források és korlátok](an1x-gpiob.md).
Nincs aktív motorjelölt. Következő:GPIO-szóhozzáférések pontos meghatározása és
tesztjei,majd IRQ/SYSCR/teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #130, 2026-10-30

A [#130 PR](https://github.com/RobCZart82/yaman1x/pull/130) beolvadt:
merge `fc9b483dcdd6c33385c1225303a0c0f2f0d7477c`,tesztelt head `e6f276dcf20e48d014a71f13dd73ac74b33da029`.
Időpont:2026-10-30 04:45:46 Europe/Budapest (03:45:46 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653002), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653007), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653087):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**82 CTest/87 Python**. PortA FFFFD1/FFFFD3,alap00/maszk00,callback ID10;
262144 nyolcbites eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>1A NVRAM,CCR81. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:téves maszk198248,rossz port252184 elvárt kudarc.
GPIO4/6/7/8/9/A/SCI ASan/UBSan PASS;519 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpioa.md).
Nincs aktív motorjelölt. Következő:portB DDR/adat,alap00/maszk00;utána a GPIO
szóhozzáféréseinek pontos meghatározása és tesztelése.
IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #128, 2026-10-30

A [#128 PR](https://github.com/RobCZart82/yaman1x/pull/128) beolvadt:
merge `efbf68fad922e7270fc60105a1e6947f972cee6a`,tesztelt head `00bff586f85f7a2b6d70c7648c57c91486eeddec`.
Időpont:2026-10-30 04:05:45 Europe/Budapest (03:05:45 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993876811), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993876814), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993877026):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**81 CTest/87 Python**. Port9 FFFFD0/FFFFD2,alap00/maszk C0;
262144 bit-orákulumos eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>DA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:hiányos maszk199031,hibás alap-DDR130 elvárt kudarc.
GPIO4/6/7/8/9/SCI ASan/UBSan PASS;517 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio9.md).
Nincs aktív motorjelölt. Következő:portA DDR/adat,alap00/maszk00;utána portB,
majd a GPIO-szóhozzáférés pontos meghatározása és tesztelése.
IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #126, 2026-10-30

A [#126 PR](https://github.com/RobCZart82/yaman1x/pull/126) beolvadt:
merge `a8b0ca4c87478aa188b902ef997b7c88c284bbf8`,tesztelt head `32dcba307e72f2acfcffd494c890afbc2894c57f`.
Időpont:2026-10-30 03:25:44 Europe/Budapest (02:25:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990014063), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990014152), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990013951):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**80 CTest/87 Python**. Port7 FFFFCE csak bemenet,DDR00/maszk00;
256 közvetlen érték,256 CPU-érték/flag és161 időbeli szétvágás PASS.
Port4/6/8/machine izoláció,portazonosító/időbélyeg,readonly írás,szóhozzáférési határ,
hiányzó callback és reentry ellenőrizve. Eredeti MAME:200000 olvasás,0 eltérés.
Rossz porthoz irányító próbahiba769 elvárt kudarc. GPIO4/6/7/8/SCI ASan/UBSan PASS.
515 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
[Forrásazonosság és korlátok](an1x-gpio7.md). Nincs aktív motorjelölt.
Következő:port9 DDR/adat,alap00/maszk C0;majd a többi GPIO-csoport.
Port7+8 szóolvasás,IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #124, 2026-10-30

A [#124 PR](https://github.com/RobCZart82/yaman1x/pull/124) beolvadt:
merge `73a5b5cac78617216901f9db19736902ccd1d0a1`,tesztelt head `33a7a85fc92252d949a7f3867e2d80bbe5b165a8`.
Időpont:2026-10-30 02:45:43 Europe/Budapest (01:45:43 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127930), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127955), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127919):mind az öt feladat sikeres.
[Automatizált kódreview](https://github.com/RobCZart82/yaman1x/pull/124#issuecomment-6512683713):completed,nincs finding az ellenőrzött headen
(2026-10-30T01:31:01.357600Z). Nyitott review-szál nincs;security review-eredmény nem érkezett.
A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**79 CTest/87 Python**. Port8 alap-DDR F0/maszk E0,FFFFCD/FFFFCF;
262144 bit-orákulumos eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>FA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:hiányos maszk268791,hibás alap-DDR130 elvárt kudarc.
GPIO4/6/8/SCI ASan/UBSan PASS;513 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio8.md).
Nincs aktív motorjelölt. Következő:csak olvasható port7 FFFFCE,bemeneti/izolációs/CPU
ellenőrzésekkel;majd a többi GPIO-csoport. IRQ/SYSCR és teljes gépreset nyitott.
Firmware/MIDI/PKS/DSP/hang/REAPER-elfogadás nincs kész.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #122, 2026-10-30

A [#122 PR](https://github.com/RobCZart82/yaman1x/pull/122) beolvadt:
merge `804c8a5100ac281945f643c4c9f8f969f263da14`,tesztelt head `32b36079e0a37f420193b410320f7705d728e29e`.
Időpont:2026-10-30 02:05:46 Europe/Budapest (01:05:46 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044271), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044353), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044283):mind az öt feladat sikeres.
[Automatizált kódreview](https://github.com/RobCZart82/yaman1x/pull/122#issuecomment-6512135987):az ellenőrzött headen completed,
nincs jelzett vagy nyitott finding (2026-10-30T00:50:58.978944Z).
Nyitott review-szál nincs;security review-eredmény nem érkezett.
A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**78 CTest/87 Python**. Port6 DDR80/maszk80,FFFFC9/FFFFCB,
262144 független maszkolt eset,241 CPU-partíció és kimenetváltozás-elnyomás PASS.
CPU:0F/5A/A5 =>AA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Maszkot eltávolító próbahiba:197637 elvárt kudarc;követett kód változatlan.
GPIO4/GPIO6/SCI ASan/UBSan PASS;511 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio6.md).
Nincs aktív motorjelölt. Következő:port8 alap-DDR F0/maszk E0,majd többi GPIO;
IRQ/SYSCR,alternatív pinfunkció és teljes gépreset továbbra is nyitott.
Firmware/MIDI/PKS/DSP/hang/REAPER-elfogadás nincs kész.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #120, 2026-10-30

A [#120 PR](https://github.com/RobCZart82/yaman1x/pull/120) beolvadt:
merge `41f8b6db0d55743495b4953c0ba746dfc4114446`,tesztelt head `a96bfc40bc518f1315aa3c828c0da1548abbd6aa`.
Időpont:2026-10-30 01:25:51 Europe/Budapest (00:25:51 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977946004), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977945988), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977945935):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice csak kapacitási tájékoztatás.

Beolvadt:**77 CTest/87 Python**. Port4 DDR/pins/PCR,buszidőbélyeges callbackek,
262144 bit-orákulumos eset,256 PCR-érték és241 CPU-partíció PASS.
CPU:F0 DDR,A5 latch,3C külső szint =>AC NVRAM-bájt,CCR89.
A kezdeti flag-orákulum N-bitjét javítottuk;CPU-kód nem változott.
Eredeti rögzített MAME-portmódszerek:200000 vegyes művelet,0 eltérés.
Próbahibák:DDR-visszaolvasás65281,pin helyett latch497267 elvárt kudarc.
GPIO4+SCI ASan/UBSan PASS;509 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
[Forrásazonosság és korlátok](an1x-gpio4.md). Nincs aktív motorjelölt.
Következő:port6 DDR/adat regiszterek rögzített alapértékkel/fenntartott bitekkel,
majd a többi portcsoport önálló tesztelt lépcsőkben. IRQ/SYSCR és alternatív pinfunkció nyitott.
Teljes gépreset,LCD/DSP/MEG,firmware/MIDI/PKS/hang/REAPER-elfogadás továbbra is nyitott.
Soros munka:a lezáró dokumentáció beolvadása után indulhat új motorfejlesztés.

## Történeti lezárás — #118, 2026-10-30

A [#118 PR](https://github.com/RobCZart82/yaman1x/pull/118) beolvadt:
merge `6f9e8b8f66d962e968af8d6f9cd4135d2c4d1ab3`,tesztelt head `85baf0a93896468e35f019845d9f9116763ecb29`.
Időpont:2026-10-30 00:46:23 Europe/Budapest (2026-10-29 23:46:23 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239180), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239137), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239160):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**76 CTest/87 Python**. ROM/RAM,SCI0/SCI1,IRQ és másolt RX-idősor közös
CPU-buszidővel;512 TX+512 RX keret,3204 szétvágás,belsőRAM-verem,NVRAM-ISR és SLEEP/RXI
ellenőrzött. Célzott ASan/UBSan PASS;határidő-próbahiba2738 elvárt kudarc.
Reentry belső kivételének elnyelése sem enged folytatást. [Hatókör](an1x-sci-bus.md).
Nincs aktív motorjelölt. Következő:GPIO/IRQ-vezérlő regisztertérkép és a rögzített CPU-profil
hiányainak ellenőrzése,majd egy kis regisztercsoport bekötése független tesztekkel.
Teljes gépreset,LCD/DSP/MEG,firmware/MIDI/PKS/hang/REAPER-elfogadás továbbra is nyitott.
Soros munkamenet:a lezáró dokumentáció beolvadása után indulhat új motorfejlesztés.

## Történeti lezárás — #116, 2026-10-29

A [#116 PR](https://github.com/RobCZart82/yaman1x/pull/116) beolvadt:
merge `7fe1d4bcb602ce96d40f34ec4ccd888430bf70e0`,tesztelt head `1982509b2c4632b7e9198d3ae23656f59e2c2a55`.
Időpont:2026-10-29 23:25:39 Europe/Budapest (22:25:39 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649310), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649254), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649115):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **75 CTest-csoport /87 Python-eset**;CPU-kezelők változatlanul562
(558 változatlan,4 korábbi adaptáció),264 kiválasztott/0 hiányzó/135 kizárt sor.
1MiB saját ROM-másolat,külső256KiB NVRAM és512 bájt belső RAM,24 bites címtükrözés.
Teljes ROM/RAM-söprés,határok/izoláció/NVRAM-roundtrip és241 CPU-partíció PASS.
Helyi Release75/75 CTest,87 Python és célzott ASan/UBSan PASS.
[Bizonyíték és kutatási szabályok](an1x-memory-map.md).
Nincs aktív motorjelölt. MMIO és ROM-írás fail-closed,nem fizikai open-bus modell.
Következő:SCI MMIO és IRQ közös címtérbe,CPU-buszidőbélyeg/periféria-határidők,
reset/izoláció és szétvágástól független RX/TX,SLEEP-ébresztés.
Teljes gép,GPIO/LCD/DSP/MEG,időzítés,firmware/MIDI/PKS/hang/plugin elfogadás nyitott.
Soros fejlesztés:egyszerre egy PR;az alábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #114, 2026-10-29

A [#114 PR](https://github.com/RobCZart82/yaman1x/pull/114) beolvadt:
merge `3172c55e9a1cea766e5326bfd56a98f68d6420f3`,tesztelt head `532c5937cfc2b0b3f142e935cebba8dfa4912000`.
Időpont:2026-10-29 22:45:44 Europe/Budapest (21:45:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229492), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229514), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229555):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **74 CTest-csoport /87 Python-eset;562 upstreamből származó kezelő
(558 változatlan,4 korábbi adaptáció);264 kiválasztott /0 hiányzó /135 kizárt sor**.
A leltározott H8H-kezelők kiválasztása teljes;ez nem teljes CPU/gép-elfogadás.
SLEEP és IRQ/NMI-ébresztés rögzített profillal,standby kizárva. Helyi Release74/74
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
ébresztést tiltó kontroll seed13/slice7-nél felismerve. 512 maszkkombináció és644
szétvágás,periféria-előrehaladás és határidő nélküli alvás ellenőrizve.
[Bizonyíték és korlátok](h8-sleep-wake.md).
Nincs aktív motorjelölt. Következő:AN1x címtér/MMIO és CPU-periféria közös gépütemező,
reset/izoláció/folytathatóság,utána eredeti helyi ROM indítási és MIDI/PKS megfigyelés.
M0 részleges;dinamikus SYSCR,standby,DMA/DTC,trace/retry,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nincs lezárva. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #112, 2026-10-29

A [#112 PR](https://github.com/RobCZart82/yaman1x/pull/112) beolvadt:
merge `b14cd16e8d4d149cdb622371ac29928904bbc295`,tesztelt head `41c5c34be49913dea5dc025f589cfde068a27736`.
Időpont:2026-10-29 22:05:04 Europe/Budapest (21:05:04 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371785), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371726), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371748):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **73 CTest-csoport /87 Python-eset;560 upstreamből származó kezelő
(556 változatlan,4 korábbi adaptáció);263 kiválasztott /1 hiányzó /135 kizárt sor**.
EEPMOV.B/W:bájtonkénti másolás8/16 bites számlálóval,két prefix,nyolc változatlan kezelő.
Helyi Release73/73 CTest,87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás forrásléptetés kontrollja
seed15/slice4-nél felismerve. Max65535 darab,átfedés,címkörbefordulás,362 szétvágás,
131072 prefixőr és12 memóriahiba ellenőrizve. [Bizonyíték](h8-eepmov-block.md).
Nincs aktív motorjelölt. Következő:SLEEP megállás/ébresztés és periféria-előrehaladás
host/ütemező-szerződésével. M0 részleges;teljes CPU/gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #110, 2026-10-29

A [#110 PR](https://github.com/RobCZart82/yaman1x/pull/110) beolvadt:
merge `3d0ce825bb341402f40dab92c0f1ec2f409b5ee7`,tesztelt head `903844cdb2a0466a93b2a61b056191afd0b37146`.
Időpont:2026-10-29 21:25:50 Europe/Budapest (20:25:50 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842276), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842076), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943841959):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **72 CTest-csoport /87 Python-eset;552 upstreamből származó kezelő
(548 változatlan,4 korábbi adaptáció);261 kiválasztott /3 hiányzó /135 kizárt sor**.
MOVFPE/MOVTPE:két művelet,négy változatlan kezelő. Az eredeti `TODO: timing`
megmarad:periféria/E-clock kézfogás és hardveridőzítés nincs implementálva/igazolva.
Helyi végső Release72/72 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás cím-előjelkiterjesztés
kontrollja seed15/slice1-nél felismerve. A túl tág dekódolási maszkot és az
elsőutasítás-leltárt javítottuk,teljes újrafordítás/regresszió után PASS.
[Bizonyíték és korlátok](h8-peripheral-byte-moves.md).
Nincs aktív motorjelölt. Következő:EEPMOV blokkmozgatás,majd SLEEP a szükséges
host/ütemező-szerződéssel. M0 részleges;teljes gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #108, 2026-10-29

A [#108 PR](https://github.com/RobCZart82/yaman1x/pull/108) beolvadt:
merge `3c7005a3a26da4149668f6ab9f4695e1672bdc6e`,tesztelt head `ffef22c9d5794ec78028a0f2d07cb9659ff9d5bb`.
Időpont:2026-10-29 20:25:40 Europe/Budapest (19:25:40 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469453), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469399), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469419):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **71 CTest-csoport /87 Python-eset;548 upstreamből származó kezelő
(544 változatlan,4 korábbi adaptáció);259 kiválasztott /5 hiányzó /135 kizárt sor**.
CCR displacement32:két művelet,két eredeti prefixfolytatás,nyolc változatlan kezelő.
Helyi Release71/71 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;felső eltolásszó elhagyásának
kontrollja seed0/slice1-nél felismerve. [Bizonyíték](h8-ccr-displacement32-memory.md).
Nincs aktív motorjelölt. Következő lépés:MOVFPE/MOVTPE perifériás bájtmozgatások,
majd EEPMOV és SLEEP a szükséges host/ütemező-szerződések tisztázásával.
M0 részleges;teljes gép/MMIO,hardveridőzítés,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #106, 2026-10-29

A [#106 PR](https://github.com/RobCZart82/yaman1x/pull/106) beolvadt:
merge `aee769ef224e2686ccd4a98bbd9d3851f8cc6399`, tesztelt head `e3af0b4453d19194da8d32b24e6d409d4ab8809f`.
Időpont:2026-10-29 19:21:59 Europe/Budapest (18:21:59 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577461), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577203), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577221): mind az öt feladat sikeres.
A jelenlegi GitHub-állapotban nincs rögzített automatikus review-eredmény vagy
review-szál; sikeres automatikus review-t nem állítunk.

Beolvadt: **70 CTest-csoport /87 Python-eset;540 upstreamből származó kezelő
(536 változatlan,4 korábbi adaptáció);257 kiválasztott /7 hiányzó /135 kizárt sor**.
CCR signed16 eltolás:két művelet,négy változatlan kezelő. Helyi Release70/70
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés;hibás előjel-kiterjesztés kontrollja seed1/slice2-nél felismerve.
[Hatókör és bizonyíték](h8-ccr-displacement16-memory.md).
Nincs aktív motorjelölt. Következő lépés:CCR displacement32 címzések.
M0 részleges;teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #104, 2026-10-29

A [#104 PR](https://github.com/RobCZart82/yaman1x/pull/104) beolvadt:
merge `92e52f45307dc1adad37347febd407d0d2106182`,végső tesztelt head `570d7b11d5dde819ac12211671f39fd47163f81b`.
Időpont:2026-10-29 18:30:16 Europe/Budapest (17:30:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264372),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264370),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264281):mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **69 CTest-csoport /87 Python-eset;536 upstreamből származó kezelő
(532 változatlan,4 korábbi adaptáció);255 kiválasztott /9 hiányzó /135 kizárt sor**.
CCR postincrement/predecrement,két művelet,négy változatlan eredeti kezelő.
524288 memóriaszó,24576 határeset,201 szétvágás,512 IRQ/NMI-maszk,
322 megszakításos szétvágás és16 memóriahiba PASS. A host-visszahívás a memóriaelérés
előtt megfigyeli a módosított mutatót és régi CCR-t. Helyi Release69/69 CTest,
87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
a hibás négybájtos növelés kontrollja seed2/slice3-nál felismerve.
[Hatókör és bizonyíték](h8-ccr-autoindex-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR signed16/displacement32 címzések,
majd a fennmaradó H8H-műveletek. M0 részleges;teljes gép/MMIO,firmware/MIDI/
hang/plugin elfogadás nyitott. Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #103 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #102, 2026-10-29

A [#102 PR](https://github.com/RobCZart82/yaman1x/pull/102) beolvadt:
merge `2dab39c1598533fc0a84da9017b3f2975854ff96`, végső tesztelt head `3c0f7947efc5cc170877fa99479f181d527bd195`.
Időpont:2026-10-29 17:50:13 Europe/Budapest (16:50:13 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848068),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848027),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848153): mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **68 CTest-csoport /87 Python-eset;532 upstreamből származó kezelő
(528 változatlan,4 korábbi adaptáció);253 kiválasztott /11 hiányzó /135 kizárt sor**.
Négy CCR abs16/abs32 betöltési/tárolási művelet,nyolc változatlan eredeti kezelő.
131072 memóriaszó,131072 abs16 cím,524288 abs32 címfél-söprés,9216 CCR-határeset,
241 szétvágás,1024 IRQ/NMI-maszk és644 megszakításos szétvágás PASS.
Helyi Release68/68 CTest és87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás alsóbájt-kiválasztás
kontrollja seed3/slice5-nél felismerve. [Hatókör és bizonyíték](h8-ccr-absolute-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR postincrement/predecrement,
majd eltolásos címzések és a fennmaradó H8H-műveletek. M0 részleges;
teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #101 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #100, 2026-10-29

A [#100 PR](https://github.com/RobCZart82/yaman1x/pull/100) beolvadt:
merge `1de8971b8fdd7694a8c3051ae86611a9ff6cd3cd`, végső tesztelt head `ba6db1c61a3ba9d696f8affee1410e5969c0aa9d`.
Időpont:2026-10-29 17:10:15 Europe/Budapest (16:10:15 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424971),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424908),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424743): mind az öt feladat sikeres.
Automatikus review a végső commiton befejeződött, hibamegállapítás és nyitott review-szál nélkül.

Beolvadt: **67 CTest-csoport /87 Python-eset;524 upstreamből származó kezelő
(520 változatlan,4 korábbi adaptáció);249 kiválasztott /15 hiányzó /135 kizárt sor**.
Két MOV.L művelet, eredeti kettős prefix és öt szavas utasításpuffer.
524288 eltolásfél-söprés,55296 határeset,241 szétvágás,65536 prefixfolytatás,
regiszteraliasok és első/második memóriaátviteli hibák PASS. Helyi Release67/67
CTest és87 Python, célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés; a felső eltolásszó elhagyásának hibás kontrollja seed4/slice4-nél felismerve.
[Hatókör és bizonyíték](h8-long-displacement32-memory.md).

Nincs aktív motorjelölt. Következő lépés: a fennmaradó CCR-címzések és további
hiányzó H8H-kódolások, majd teljes gép/MMIO-integráció. M0 részleges;
firmware/MIDI/hang/plugin elfogadás nyitott. Egyszerre egy PR; új fejlesztés csak beolvadás után.
A #99 dokumentációs PR beolvadását a strukturált nyilvántartás már tartalmazza.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #98, 2026-10-29

A [#98 PR](https://github.com/RobCZart82/yaman1x/pull/98) beolvadt:
merge `1c7ba628aceb29fd4c057db73fe31b2be4841558`, tesztelt head `a3c033256ec247555b44b0d820bb228dcaf9bc39`.
Időpont: 2026-10-29 13:36:16 Europe/Budapest (12:36:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870231488),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229509),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229586): mind az öt feladat sikeres.
Automatikus review szolgáltatási kvóta miatt nem állt rendelkezésre; sikeres review-t nem állítunk.

Beolvadt: **66 CTest-csoport /87 Python-eset;518 upstreamből származó kezelő
(514 változatlan,4 korábbi adaptáció);247 kiválasztott /17 hiányzó /135 kizárt kódolássor**.
524288 memóriaszó,24576 cím/CCR-eset,65536 prefixfolytatás,201 szétvágás,
512 IRQ/NMI-maszk és322 megszakításos szétvágás; eredeti referencia200000 szelet,
nulla eltérés. A hibás alsóbájt-kiválasztást a megerősített referencia felismerte.
A helyi korábbi Windows-futás65 futtatható CTest/86 Python-esetet ellenőrzött;
az ismert symlink-jogosultsági eset kizárását nem tekintjük sikeres tesztnek.
[Pontos bizonyíték és korlátok](h8-ccr-indirect-memory.md). M0 részleges; nincs aktív
motorjelölt. Teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.

A fejlesztés soros: egyszerre egy PR; következő fejlesztés csak az aktuális PR
beolvadása után. Következő címzési lépés a hosszúszavas32 bites eltolás,
majd a fennmaradó CCR-címzések és a leltár további hiányzó műveletei.

## Történeti lezárás — #96, 2026-10-29

Ellenőrzött motoralap: [#96](https://github.com/RobCZart82/yaman1x/pull/96),
merge `8b1c37eb569202e394ff1caeb3f64618bae78a69`, végső tesztelt head `fd5d1293a21cf63c4b39293b30ca7e650702efe4`.
Beolvadt: 2026-10-29 11:49:02 Europe/Budapest (10:49:02 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583149),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583161),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583141).
Linux UBSan:32 bites eltolásos bájt/szó-mozgatás és korábbi számtan PASS; eredeti
unsigned-túlcsordulási kontroll PASS. Automatikus kódellenőrzés kvótakorlát miatt
nem fejeződött be; sikeres review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **65 CTest-csoport /87 Python-eset;512 upstreamből származó kezelő
(508 változatlan,4 korábbi adaptáció);245 kiválasztott /19 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Négy MOV-csoport:1048576 eltolásfél-söprés (nem teljes2^32
keresztszorzat),55296 cím/regiszter/CCR-határeset,241 szétvágás,65536 prefixfolytatás,
memóriahibás fail-stop és újraindítás-tiltás PASS. Eredeti referencia:200000 szelet,
nulla eltérés; felső eltolásszó elhagyása kontroll seed6/slice5 felismerve. Helyi64
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
[Pontos hatókör és bizonyíték](h8-displacement32-memory.md). Hosszúszavas32 bites
eltolás külön nyitott. Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/
hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #94, 2026-10-29

Ellenőrzött motoralap: [#94](https://github.com/RobCZart82/yaman1x/pull/94),
merge `442949856a92392986f3bbc5927f36b7884a505b`, végső tesztelt head `9f3cda7fcbe545ee2b0da9cebc23d8dd5ab6b42a`.
Beolvadt: 2026-10-29 10:48:25 Europe/Budapest (09:48:25 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898827),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898826),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898831).
Linux UBSan: szélső előjeles szó-osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a beolvasztáskor még futott;
lezárt review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **64 CTest-csoport /87 Python-eset;502 upstreamből származó kezelő
(498 változatlan,4 korábbi adaptáció);241 kiválasztott /23 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.W:524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,
1024 szélső/CCR-eset,321 szétvágás,128 tiltott ER-szomszéd PASS. Eredeti referencia:
200000 szelet,nulla eltérés; fordított N kontroll seed7/slice7 felismerve. Helyi63
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
INT_MIN/-1: a matematikai +2147483648 hányadost az eredeti32 bites intre szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres mérés.
A forrás szintű szélsőeset-vizsgálat lezárult;[pontos hatókör](h8-signed-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending és korábbi nyitott szélsőeset-bejegyzések történetiek.

## Történeti lezárás — #92, 2026-10-29

Ellenőrzött motoralap: [#92](https://github.com/RobCZart82/yaman1x/pull/92),
merge `1adeb1030871083bb510f4e691da4467cd96a816`, végső tesztelt head `b719cf25b0d0d8b929c5b639d5971ddc424a1a49`.
Beolvadt: 2026-10-29 10:18:06 Europe/Budapest (09:18:06 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532370),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532267),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532335).
Linux UBSan: előjeles bájtos osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a végső headen megállapítás nélkül lezárult.

Beolvadt: **63 CTest-csoport /87 Python-eset;500 upstreamből származó kezelő
(496 változatlan,4 korábbi adaptáció);240 kiválasztott /24 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.B:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás,65536 prefixfolytatás PASS. Eredeti referencia:
200000 szelet,nulla eltérés; hányados+1 kontroll seed7/slice3 felismerve. Helyi62
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
Előjeles szó-osztás INT32_MIN/-1 és csomagolás továbbra is nyitott vizsgálat;
[pontos hatókör és bizonyítékok](h8-signed-byte-divide.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #90, 2026-10-29

Ellenőrzött motoralap: [#90](https://github.com/RobCZart82/yaman1x/pull/90),
merge `a4f52f600d2cfd85f14a5e8822eb5e025d28825d`, végső tesztelt head `f24ca47f3511764dfec4971e84c3c1487aaed443`.
Beolvadt: 2026-10-29 09:38:37 Europe/Budapest (08:38:37 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792043),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792142),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792129).
Linux UBSan: DAA/DAS és korábbi számtan PASS; eredeti unsigned-túlcsordulási kontroll
PASS. Automatikus kódellenőrzés a végső headen lezárult, megállapítás nélkül.

Beolvadt: **62 CTest-csoport /87 Python-eset;496 upstreamből származó kezelő
(492 változatlan,4 korábbi adaptáció);239 kiválasztott /25 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Decimális oracle:40000 érvényes számpár/CCR-eset,2048 regiszter-
határeset,241 szétvágás PASS;200000 eredeti referenciaszelet nulla eltéréssel.
DAA korrekció+1 kontroll seed7/slice0 felismerve. Helyi61 futtatható CTest/86 Python
PASS, ismert Windows-symlink eset kizárva. Érvénytelen BCD/flag kombinációkra nincs
állított teljes független helyességbizonyítás. [Hatókör](h8-decimal-adjust.md).
Nincs aktív motorjelölt. Ez M0-részlépés; M0 továbbra is részleges, teljes gép,
firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #88, 2026-10-29

Ellenőrzött motoralap: [#88](https://github.com/RobCZart82/yaman1x/pull/88),
merge `3c5f7dddfdf54532670d9cdddd0ade0aab7c92bd`, végső tesztelt head `fc0db2778270a5ffdc612ab46ccd7be0b0d9b4fe`.
Beolvadt: 2026-10-29 08:55:24 Europe/Budapest (07:55:24 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738076),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738102),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738069).
Linux UBSan: bájt/szó-osztás és korábbi szorzások PASS; eredeti unsigned-túlcsordulási
kontroll PASS. Beolvadt: **61 CTest-csoport /87 Python-eset;492 upstreamből származó
kezelő (488 változatlan,4 dokumentált adaptáció);237 kiválasztott /27 hiányzó /135 kizárt
kódolássor** (nem CPU-százalék). DIVXU.W:524288 osztó-söprés,30720 alias/CCR-eset,
20000 véletlen pár,241 szétvágás PASS. Nagy értékek független64 bites számítással;
200000 korlátozott eredeti referenciaszelet nulla eltéréssel; hányados+1 kontroll
seed9/slice26 felismerve. Helyi60 futtatható CTest/86 Python PASS, ismert Windows-symlink
eset kizárva. A q/r u32 típusmódosítás dokumentált; az eredeti osztóbit7 N-szabály
megőrzött, hardveresen nem igazolt. [Pontos hatókör](h8-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #86, 2026-10-29

Ellenőrzött motoralap: [#86](https://github.com/RobCZart82/yaman1x/pull/86),
merge `4dd788dadae4b09a21b4611f245640a63c56dcf5`, végső tesztelt head `189eeea02372839e8cf27f02853b990ed0625652`.
Beolvadt: 2026-10-29 08:26:31 Europe/Budapest (07:26:31 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892241),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892230),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892231).
Linux UBSan: bájtos unsigned osztás és korábbi szorzások PASS; az eredeti unsigned
int-promóciós hibát megkövetelő kontroll is PASS.

Beolvadt: **60 CTest-csoport /87 Python-eset;490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott /28 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. DIVXU.B:393216 osztandó-söprés,
2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS.
Eredeti referencia:200000 szelet,nulla eltérés; hányados+1 kontroll seed9/slice18
felismerve. Helyi59 futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
A nullás osztó, hányadoslevágás és osztófüggő N/Z az eredeti forrással egyezik,
nem külön hardvermérés. Nincs aktív motorjelölt. [Hatókör és bizonyíték](h8-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #84, 2026-10-29

Ellenőrzött motoralap: [#84](https://github.com/RobCZart82/yaman1x/pull/84),
merge `2540601aa985513a39d293ffa14fd8e0b76689da`, végső tesztelt head `7aee43a86ea21711254fd3e840728534e01b6a96`.
Beolvadt: 2026-10-29 07:45:33 Europe/Budapest (06:45:33 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383282),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383154),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383285).
Linux UBSan: előjeles bájt/szó és unsigned szó-szorzás PASS; az eredeti unsigned
int-promóciós hibát megkövetelő negatív kontroll is PASS.

Beolvadt: **59 CTest-csoport /87 Python-eset;488 upstreamből származó kezelő
(486 változatlan,2 korábbi adaptáció);235 kiválasztott /29 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. Előjeles szorzás:65536 bájtpár,
327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár,241 szétvágás és
65536 prefixfolytatás PASS. Eredeti referencia:200000 szelet,nulla eltérés;
product+1 kontroll seed9/slice14 felismerve. Helyi58 futtatható CTest/86 Python PASS;
egy ismert Windows-symlink eset kizárva. Nincs aktív motorjelölt.
[Szerződés és pontos bizonyítékok](h8-signed-multiply.md). M0 részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending
bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #82, 2026-10-29

Ellenőrzött motoralap: [#82](https://github.com/RobCZart82/yaman1x/pull/82),
merge `80b237ee7a9ad6b286a13c9253d076aa42bb0214`, végső tesztelt head `4f2cffc88c843cc9d431f5c0a6ec20ee2869d85b`.
Beolvadt: 2026-10-29 07:20:23 Europe/Budapest (06:20:23 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962559),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962568),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962569).
A Linux-lépésben a nagy szorzatok UBSan-tesztje és az eredeti kifejezés
előjeles túlcsordulását megkövetelő külön negatív kontroll is PASS.

Beolvadt: **58 CTest-csoport /87 Python-eset;482 upstreamből származó kezelő
(480 változatlan,2 dokumentáltan adaptált);233 kiválasztott /31 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. MULXU.W:327680 operandus-söprés,
9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS;200000 biztonságos
szorzatokkal végzett eredeti referenciaszelet nulla eltéréssel. A nagy szorzatokat
független64 bites számítás ellenőrzi; product+1 kontroll seed9/slice9 felismerve.
Helyi57 futtatható CTest és86 Python PASS, egy ismert Windows-symlink eset kizárva.
A szó-szorzás C++-promóciós kockázata rendezett a dokumentált unsigned-szélesítéssel;
[pontos hatókör és bizonyíték](h8-word-multiply.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/CI-pending/kockázati bejegyzések történetiek.

## Történeti lezárás — #80, 2026-10-29

Ellenőrzött motoralap: [#80](https://github.com/RobCZart82/yaman1x/pull/80),
merge `bddffa8743eac2e2af8e7fc1a3145046a45e6fa5`, végső tesztelt head `5bf4e97e64fc3ebbe00a42f8215a740fbca72d86`.
Beolvadt: 2026-10-29 06:41:58 Europe/Budapest (05:41:58 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422195),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422322),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422266).

Beolvadt: **57 CTest-csoport / 87 Python-eset;480 eredeti kezelő;
232 kiválasztott /32 hiányzó /135 kizárt profilú kódolássor** (nem CPU-százalék).
MULXU.B:65536 operanduspár,19200 alias/CCR-határeset,161 szétvágott futás PASS;
200000 referenciaszelet nulla eltéréssel; product+1 negatív kontroll felismerve.
Helyi56 futtatható CTest és86 Python PASS; egy ismert Windows-symlink eset kizárva.
Nincs aktív motorjelölt. A szó-szorzás int-promóciós kockázata külön nyitott;
[bekötési feltételek és bizonyítékok](h8-byte-multiply.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás továbbra is nyitott.
Az alábbi jelölt/CI-pending bejegyzések történeti előkészítési állapotok.

## Történeti lezárás — #78, 2026-10-29

Ellenőrzött motoralap: [#78](https://github.com/RobCZart82/yaman1x/pull/78),
merge `d4ef0e45bc416f06216d43559e1f73df1d39caa8`, végső tesztelt head
`ef6adefadb5b390f6822ae34c4db41edab5b1348`. Beolvadt: 2026-10-29 06:26:01 Europe/Budapest
(2026-10-29 05:26:01 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703637),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703598),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703615).

Beolvadt csomag: **56 CTest-csoport, 87 Python-eset; 478 eredeti kezelő;
231 kiválasztott / 33 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
Hat előjeles 16 bites eltolásos memóriautasítás-csoport készült el. Független
referencia: 200000 szelet, nulla eltérés; a szándékos előjelhiba felismerve.
Helyi Windows: 55 futtatható CTest és 86 Python PASS; egy ismert symlink-jogosultsági
eset kizárva. A CI a teljes csomagot ellenőrizte. M0 részleges; teljes AN1x-gép,
firmware-es MIDI, hang és plugin elfogadása nyitott. Nincs aktív motorjelölt.
A korábbi jelölt/CI-pending bejegyzések történetiek; lezárásuk itt és a JSON-naplóban szerepel.

## Történeti lezárás — #77, 2026-10-29

Ellenőrzött motoralap: [#77](https://github.com/RobCZart82/yaman1x/pull/77),
merge `8993a6024b0cedfe0d3f5c48a2819009c98643f1`, végső tesztelt head
`fb2bc0935aad976706b0652d21275e5a55d0efd8`. Beolvadt: 2026-10-29 00:49:52 Europe/Budapest
(2026-10-28 23:49:52 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788596091),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595788),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595917).

Beolvadt csomag: **55 CTest-csoport, 87 Python-eset; 466 eredeti kezelő;
225 kiválasztott / 39 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
A fix profilú IRQ/NMI/TRAPA és a szintetikus CPU–SCI RX/TX/hibakezelés elkészült;
a teljes AN1x-gép, firmware-es MIDI, hang és plugin elfogadása nyitott. M0 részleges.
Az alábbi korábbi jelölt/CI-pending bejegyzések történetiek. Az új eltolásos
memóriautasítások külön jelöltet alkotnak, nem írják felül ezt a lezárt motoralapot.

## Történeti lezárás — #57, 2026-10-28

Ellenőrzött motoralap: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), merge
`f8b6aa20834f209931e3e9bb9463cc47b5b7a540`. Végső tesztelt head:
`bf3e7158a9a4dac6edb36ea4deb659bdf3c54cb1`.
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365642),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365647),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365644).
A beolvadt csomag **39 CTest-csoportot és 84 Python-esetet** tartalmaz.
Helyi Windows-ellenőrzés: 38 futtatható CTest-csoport és 83 Python-eset PASS;
egy symlink-jogosultsági eset kizárva (WinError 1314). A CI a teljes csomagot futtatta.

A normál, rögzített H8H-profil verem-/vezérlésátadási családjai elkészültek;
366 eredeti full/partial kezelő szerepel a részhalmazban. Az RTE szintetikus
veremkép-visszaállítása tesztelt, az IRQ/TRAPA-belépés még nincs bekötve.
M0 részben teljesült; M1 nincs elfogadva; M2–M6 nyitott. Nincs igazolt AN1x-hang
vagy REAPER-plugin. A következő motorfeladatok: hiányzó H8H-utasítások/címzések
leltára, IRQ/TRAPA-belépés és szűrők, majd közös CPU/periféria-ütemezés és gépintegráció.

A korábbi „aktív”, „jelölt”, „CI pending” és „következő” bejegyzések az akkori
állapotot őrzik; a jelenlegi készültségre ez a lezárás és az aktuális mérföldkő-tábla
irányadó. A dokumentáció mostani javítása nem új motorfejlesztés vagy elfogadás.

## Ellenőrzött lezárások — #39–#44

Ellenőrizve: 2026-10-27. Mind a hat PR beolvadt; mindegyik végső headjén öt ellenőrzés PASS. Az alábbi lezárás az irányadó a korábbi, készítéskori „CI pending / review alatt” megjegyzésekkel szemben. Azok történeti pillanatképek, nem aktuális akadályok. A PR-head CI nem hardver-/firmware-/audioelfogadás.

| PR | Ellenőrzött head | Linux/macOS/Windows | MSan | MAME |
|---|---|---|---|---|
| [#39](https://github.com/RobCZart82/yaman1x/pull/39) | `9459a805af0834c5ad66329f0f1b037a9354b08d` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566377) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566528) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292565993) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549567/job/122292568462) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549032/job/122370894035) |
| [#40](https://github.com/RobCZart82/yaman1x/pull/40) | `34cec0074577a1add862772bcf3bc289de735b68` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453074) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453726) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453515) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557238091/job/122376454019) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237934/job/122376453689) |
| [#41](https://github.com/RobCZart82/yaman1x/pull/41) | `387c5e64c0ee9759b3d41dfa58bcda1b35594731` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943734) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943358) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943754) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011629/job/122407456482) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566012882/job/122408084911) |
| [#42](https://github.com/RobCZart82/yaman1x/pull/42) | `a67b599f880f0981d68e04b276b0d904ea3f7d29` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580997) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580920) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580479) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072700/job/122432444681) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072611/job/122432149653) |
| [#43](https://github.com/RobCZart82/yaman1x/pull/43) | `83caf332df3539037187888c60bc35ae422dc2a3` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457473798) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474477) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474285) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107561/job/122456981421) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107642/job/122457680585) |
| [#44](https://github.com/RobCZart82/yaman1x/pull/44) | `1bed694bdc066a66bcebb747ed74a052a4650c25` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074548) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074297) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074723) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879809/job/122480613181) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879880/job/122480584462) |

## A nyilvántartás feladata

Ez a repository-szintű változásjegyzék a fejlesztéseket, javításokat és
kapcsolódó bizonyítékokat köti a PR-ekhez. A részletes mérföldkő-elfogadás és a
PR #16–#36 végső CI-revízióinak táblája továbbra is a
[REAPER-tervben](FIRST_REAPER_SOUND_PLAN.md) marad. Az aktuális készültséget a
[STATUS](STATUS.md), az audit megállapításait a
[2026-10-28-as jelentés](REPOSITORY_AUDIT_2026-10-28.md) tartalmazza.

A korábbi napló tehát már létezett a tervben; ez a dokumentum teljesebb,
külön megtalálható könyvelést ad, és nem nyilvánít késznek nyitott mérföldkövet.
A [strukturált nyilvántartás](development-ledger.json) tartalmazza a teljes
merge- és PR-head SHA-kat, valamint az eredeti UTC-időbélyegeket.

## AUD-2026-10-27 — repository-audit és javítások

Állapot: [#38 PR](https://github.com/RobCZart82/yaman1x/pull/38) beolvadt 2026-10-27 15:36:15 UTC-kor; merge: `47950097b564f7ca48a1a2fd27a125066c3fc7a9`.

A helyben tesztelt kódfa a `de892c3ea278611d1a60e68ccbad8991e174b768`
GitHub-revízió fájlfájával azonos. A követő változás csak e napló metaadatait
rögzíti; a legutóbbi PR-head saját CI-je az irányadó.

- Átvizsgált terület: build/CI, saját C++/Python kód és tesztek, kutatási patch-lánc,
  forrás-/licencleltár, állapotdokumentumok és PR-előzmények.
- Javítás: az élő bridge auditor elfogadja a jelenlegi register-only ABI 2 fejlécet,
  megtartja a régi ABI 1/2 formátumot, és elutasítja az ellentmondó órajeladatot.
- Regresszió: az új teszt a javítás előtt hibázott; utána négy célzott teszt
  normál és `python -O` módban is PASS. Yamaha-adat nem kellett hozzá.
- Dokumentáció: elavult DSP-időléptetési, tesztszám- és változatlan upstream
  forrásra vonatkozó állítások javítva; aktuális és történeti eredmények jelölve.
- Ellenőrzés: tiszta macOS arm64 Release build; alapállapot 22/22 CTest és
  56 Python-eset PASS; a jelölt négy új Python-esetet ad ugyanazon csoporthoz.
  A részletes, végső eredmény az auditjelentésben és a PR saját checkjeiben van.
- Nyitott: SCI IRQ/MMIO és H8-ütemező/CPU-bekötés; firmware-es PKS/MIDI,
  AN1x órajel/EG/táblacímzés, audio és REAPER-elfogadás.

## SCI-MMIO-2026-10-27 — kétcsatornás SCI busz/IRQ-adapter

Állapot: [#39 PR](https://github.com/RobCZart82/yaman1x/pull/39) beolvadt 2026-10-27 18:06:08 UTC-kor; merge: `6c1dd8a0c83eac1d593abd4e70c040f88908bb48`.
A hatregiszteres bájt/szó MMIO és a csatornánkénti ERI/RXI/TXI/TEI útvonal elkészült, DMA-elsőbbséggel. Szintetikus 512 RX bájt/csatorna eset, TX, hibák, reset és izoláció ellenőrizve. Az eredeti PR-head öt platform/build ellenőrzése sikeres volt; a #38 főági dokumentációjával feloldott ütközés után új CI szükséges. A kombinált fa helyben 23/23 CTest-csoporttal és 60 Python-esettel PASS. M0 részben teljesült; CPU-ütemezés és firmware/audio továbbra is nyitott.

## #45 dokumentációs lezárás

Beolvadt 2026-10-27 21:58:12 UTC: `ed966cef79eb33eb309165e9e9d6edca920410ea`; head `eee27cef236fb54cb0d012f1df1716d2d03fa63c`. [Három platform PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542410), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542442), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542551). A strukturált leltár #45-öt is tartalmazza; az alábbi régebbi táblázat #44-ig történeti index.

## Beolvadt változások — a legutóbbi elöl

A következő 39 bejegyzés GitHub PR-metaadatból és az aktuális `main` Git-előzményéből
ellenőrzött visszamenőleges könyvelés (#6–#44). A beolvadás bizonyított; a
felsorolás önmagában nem állít minden PR-re újrafuttatott hardveres vagy helyi
validációt. A kezdeti importot a [történeti összefoglaló](HISTORY.md) őrzi.

| Beolvasztás (Budapest) | PR | Fejlesztés / javítás | Merge commit |
|---|---|---|---|
| 2026-10-27 22:43:08 | [#44](https://github.com/RobCZart82/yaman1x/pull/44) | H8 ciklustartozás és perifériahatáridők | `b644b6e1c471f8632ea086d75bd37d20b5ebbeb4` |
| 2026-10-27 21:49:51 | [#43](https://github.com/RobCZart82/yaman1x/pull/43) | H8H buszsegédek és eredetimetódus-összevetés | `f8b1f4fbd4a422eab33d633e2384f1c82c11623d` |
| 2026-10-27 21:33:08 | [#42](https://github.com/RobCZart82/yaman1x/pull/42) | H8 CPU gazdaszolgáltatás-határ és reprodukálható leltár | `e3f71a89f6d967034a3c05926a7a86c595ab9a49` |
| 2026-10-27 20:47:56 | [#41](https://github.com/RobCZart82/yaman1x/pull/41) | Kétcsatornás SCI-idővonal | `259338dbffed8b5b10b606c5f8f907bd791b1b74` |
| 2026-10-27 19:56:09 | [#40](https://github.com/RobCZart82/yaman1x/pull/40) | Korlátozott ciklus-eseményfuttató | `76787a6ee5450fa29a7403b658fc861ef908a297` |
| 2026-10-27 19:06:08 | [#39](https://github.com/RobCZart82/yaman1x/pull/39) | Két SCI-csatorna MMIO/IRQ-adaptere | `6c1dd8a0c83eac1d593abd4e70c040f88908bb48` |
| 2026-10-27 16:36:15 | [#38](https://github.com/RobCZart82/yaman1x/pull/38) | Repository-audit, bridge-kompatibilitási javítás és fejlesztési napló | `47950097b564f7ca48a1a2fd27a125066c3fc7a9` |
| 2026-10-27 13:32 | [#37](https://github.com/RobCZart82/yaman1x/pull/37) | Reconcile development plan with verified PR closures and historical snapshots | `276399870400` |
| 2026-10-27 13:10 | [#36](https://github.com/RobCZart82/yaman1x/pull/36) | Unify asynchronous H8 SCI transmit and receive under one channel | `33dc7e97c174` |
| 2026-10-27 12:22 | [#35](https://github.com/RobCZart82/yaman1x/pull/35) | Compose H8 asynchronous SCI receiver with sampled input and errors | `e7257ebe2bbc` |
| 2026-10-27 11:59 | [#34](https://github.com/RobCZart82/yaman1x/pull/34) | Compose H8 asynchronous SCI transmitter with status and cycle clock | `389594251b9b` |
| 2026-10-27 11:43 | [#33](https://github.com/RobCZart82/yaman1x/pull/33) | Update LICENSE | `6e5da15024e3` |
| 2026-10-27 11:27 | [#32](https://github.com/RobCZart82/yaman1x/pull/32) | Track reviewed third-party license coverage and enforce source notices | `cded711258a3` |
| 2026-10-27 11:00 | [#31](https://github.com/RobCZart82/yaman1x/pull/31) | Require replaceable PKS backends and a versioned firmware upgrade path | `e1bb9206b024` |
| 2026-10-27 10:56 | [#30](https://github.com/RobCZart82/yaman1x/pull/30) | Record PKS replacement ideas as unverified research options | `bc946bd39d2b` |
| 2026-10-27 10:47 | [#29](https://github.com/RobCZart82/yaman1x/pull/29) | Extract H8 SCI status latch with exhaustive flag transition checks | `3890122ffddf` |
| 2026-10-27 10:11 | [#28](https://github.com/RobCZart82/yaman1x/pull/28) | Extract internal H8 SCI cycle clock and refresh milestone plan | `019070d95f82` |
| 2026-10-27 09:46 | [#27](https://github.com/RobCZart82/yaman1x/pull/27) | Adapt H8H IRQ registers and pins preserving upstream publication | `3ec574095dfa` |
| 2026-10-27 09:09 | [#26](https://github.com/RobCZart82/yaman1x/pull/26) | Extract standalone H8H internal interrupt arbitration | `3a6cf4832025` |
| 2026-10-27 08:48 | [#25](https://github.com/RobCZart82/yaman1x/pull/25) | Record completed H8 peripheral CI evidence | `26dce3d3ceff` |
| 2026-10-27 08:44 | [#24](https://github.com/RobCZart82/yaman1x/pull/24) | Prepare H8 source boundary and extract standalone digital port | `14635883d5e3` |
| 2026-10-27 08:15 | [#23](https://github.com/RobCZart82/yaman1x/pull/23) | Prepare paired local M1 serial experiment for Mac continuation | `54757fbcb340` |
| 2026-10-27 07:35 | [#22](https://github.com/RobCZart82/yaman1x/pull/22) | Initialize VOP3 decoded operations before the first comparison | `94482f2e05a9` |
| 2026-10-27 07:08 | [#21](https://github.com/RobCZart82/yaman1x/pull/21) | Record verified MAME build evidence and remaining M1 acceptance | `c42c6f244631` |
| 2026-10-27 07:02 | [#20](https://github.com/RobCZart82/yaman1x/pull/20) | Pin MAME research source and compile the full patched target in CI | `4bca07273afa` |
| 2026-10-27 06:32 | [#19](https://github.com/RobCZart82/yaman1x/pull/19) | Add timestamped SCI access observation for M1 startup research | `291e04554c59` |
| 2026-10-27 06:23 | [#18](https://github.com/RobCZart82/yaman1x/pull/18) | Track development plan progress with dated validation evidence | `064ddbe56dd7` |
| 2026-10-27 06:19 | [#17](https://github.com/RobCZart82/yaman1x/pull/17) | Record M0 engine architecture and dependency baseline | `865e47930497` |
| 2026-10-27 06:17 | [#16](https://github.com/RobCZart82/yaman1x/pull/16) | Keep ROM-free ABI checks active under Python -O | `d70a309ca08a` |
| 2026-10-27 05:18 | [#15](https://github.com/RobCZart82/yaman1x/pull/15) | Plan development through the first audible voice in REAPER | `d24237292908` |
| 2026-10-27 00:03 | [#14](https://github.com/RobCZart82/yaman1x/pull/14) | Observe both H8 SCI devices and keyboard-port traffic | `1f9163c2866e` |
| 2026-10-26 23:53 | [#13](https://github.com/RobCZart82/yaman1x/pull/13) | Audit MIDI input at H8 SCI0 | `216af8b06070` |
| 2026-10-26 23:38 | [#12](https://github.com/RobCZart82/yaman1x/pull/12) | Route MAME MIDI input to H8 SCI0 | `db9028297e05` |
| 2026-10-26 23:29 | [#11](https://github.com/RobCZart82/yaman1x/pull/11) | Add opt-in timed H8 VOP3 experiment | `b735bd3e8cc6` |
| 2026-10-26 23:02 | [#10](https://github.com/RobCZart82/yaman1x/pull/10) | Document AN1x table-ROM findings and audit tool | `f19674cebede` |
| 2026-10-26 22:34 | [#9](https://github.com/RobCZart82/yaman1x/pull/9) | Add ROM-free cross-platform CI | `cdfce265590e` |
| 2026-10-26 22:26 | [#8](https://github.com/RobCZart82/yaman1x/pull/8) | Align branch ruleset docs with zero required approvals | `f79779625139` |
| 2026-10-26 21:29 | [#7](https://github.com/RobCZart82/yaman1x/pull/7) | Record public research release and active main-branch protection | `9978fb71b0ea` |
| 2026-10-26 20:54 | [#6](https://github.com/RobCZart82/yaman1x/pull/6) | Prepare main-branch protection ruleset; activation requires eligible plan | `89e54b953089` |

## Következő bejegyzések vezetése

Minden új fejlesztés és javítás saját dátumozott bejegyzést kap ugyanabban a
PR-ben. Kötelező adatok: probléma/cél, változás, érintett mérföldkő, PR vagy
commit, ellenőrzések és platform, maradó korlát, következő feladat. A PASS,
FAIL, NOT RUN és környezeti akadály külön szerepeljen. A végső merge SHA a
beolvasztás utáni egyeztetéskor kerül be; ismeretlen adat maradjon függőben.

A részletes vezetési szabály: [CONTRIBUTING](../CONTRIBUTING.md). A PR-sablon
emlékeztet a frissítésre; ez nem automatikus naplózási szolgáltatás. A történeti
adatokat megőrizzük, a téves következtetést új, indokolt korrekcióval javítjuk.

## SCHED-2026-10-27 — korlátozott ciklus-eseményfuttató

PR: [#40](https://github.com/RobCZart82/yaman1x/pull/40), ág: `feat/bounded-cycle-dispatcher`; [szerződés](cycle-dispatcher.md). Új, saját komponens, nem upstream átvétel. Költségkeret, folytatás, időmonotonitás, hibahatár és izoláció tesztelve; 1000 szintetikus eseménysor felosztásfüggetlen. Külön PR a #39-től; annak státuszát nem zárja le. A teljes CPU/periféria-ütemezés és M0 elfogadása nyitott.

Helyi záróellenőrzés: macOS arm64 Release, 23/23 CTest-csoport és 60 Python-eset PASS; natív ASan/UBSan PASS. Végső platform-CI a PR-head checkjeiben.

2026-10-27, #39 utáni szinkronizálás: a build- és naplóütközések feloldva, mindkét komponens megtartva. Az egyesített kódfa helyben 24/24 CTest-csoporttal és 60 Python-esettel PASS. Az előző 23-as eredmény a különálló ág történeti bizonyítéka. Új GitHub CI a szinkronizált #40-headen szükséges.

## SCI-TIMELINE-2026-10-27 — közös SCI-eseménytulajdonos

PR: [#41](https://github.com/RobCZart82/yaman1x/pull/41), ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. Saját új komponens, új upstream átvétel nélkül. [Részletes szerződés és határok](sci-cycle-timeline.md). Időbélyegzett bemenet, két csatorna, eseményenkénti keret és determinisztikus sorrend; 256 teljes duplex minta három futtatási felosztása egyezik. CPU-végrehajtás és M0 lezárása nyitott.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.

## H8-HOST-2026-10-27 — CPU gazdaszolgáltatás-határ

PR: [#42](https://github.com/RobCZart82/yaman1x/pull/42), ág: `feat/h8-host-services-boundary`, alap: beolvadt #41. Új ROM-mentes elemző és [áttekintett integrációs szerződés](h8-host-services-boundary.md). 34 forrás-/generált fájl, hash-ek és tokenhelyek, két független bájtazonos regenerálás. Nyolc új Python-regresszió; CPU-fordítás/végrehajtás és függőségzár továbbra is nyitott.

Local validation / helyi ellenőrzés: macOS arm64 Release, 25/25 CTest groups and 68 Python cases PASS; source/notice audits PASS. Final PR-head platform CI pending.

## H8H-BUS-2026-10-27 — CPU buszsegédek

PR: [#43](https://github.com/RobCZart82/yaman1x/pull/43), ág: `feat/h8h-bus-access`, alap: beolvadt #42. [Szerződés és bizonyíték](h8h-bus-access.md). BSD-forráseredet rögzítve, licenclefedettség frissítve. Öt önálló buszsegéd; 200000 műveletes eredetimetódus-összevetés 0 eltéréssel, 65536 szóérték és hibahatárok. CPU-futtató, firmware és hang továbbra is nyitott.

Local validation / helyi ellenőrzés: macOS arm64 Release, 26/26 CTest groups and 72 Python cases PASS; new native ASan/UBSan PASS. Final PR-head platform CI remains separate evidence.

## H8-BUDGET-2026-10-27 — végrehajtási keret számtana

PR: [#44](https://github.com/RobCZart82/yaman1x/pull/44), ág: `feat/h8-execution-budget`, alap: beolvadt #43. [Szerződés és bizonyíték](h8-execution-budget.md). Ciklustartozás, határidő és stabil CPU-counter a buszsegédhez; 1000 felosztási történet és 400000 eredetimetódus-összevetés 0 eltéréssel. Licenc- és eredetnyilvántartás frissítve. Teljes CPU-utasításvégrehajtás, firmware és hang nincs.

Local validation / helyi ellenőrzés: macOS arm64 Release, 27/27 CTest groups and 74 Python cases PASS; new native ASan/UBSan and source/notice audits PASS. Final PR-head CI pending.

## H8-RUNNER-2026-10-28 — végrehajtási sorrend

PR: [#46](https://github.com/RobCZart82/yaman1x/pull/46). Ág: `feat/h8-execution-order`, alap: #45. Korabeli jelöltállapot: H8 full/partial/periféria-sorrend a meglévő cikluskeret fölött, nem teljes CPU. 200000 eseményes referencia-szelet: 338598 update, 761211 full, 276336 partial callback, nulla eltérés. Natív 1000 felosztás és hibahatárok; két új Python regresszió. A Windows helyi teljes Python-csoport symlink-jogosultsági korlátját külön kezeljük; végső PR-head CI szükséges. Következő: tényleges generált kezelők és CPU-regiszterállapot. M0 részben teljesült.

## H8-FIRST-2026-10-28 — első generált utasítások

Előző lépcső lezárása: [#46](https://github.com/RobCZart82/yaman1x/pull/46): merge `00e16ce805d542e4a654923ddde2d462ebac78bb`, végső head `478294b0570b0140b0ba2c5f78b65fcc99fecaa4`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467656), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467504), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467588).

PR: [#47](https://github.com/RobCZart82/yaman1x/pull/47). Ág: `feat/h8-first-generated-handlers`, alap: beolvadt #46. Korabeli jelöltállapot: nyolc eredeti generált full/partial kezelő (reset, NOP, MOV.B immediate, BRA rel8) fordul és fut saját CPU-állapottal a busz-/cikluskereten. Az összes többi opcode explicit hiba. 4096 regiszter/érték, 65536 jelzőbit-eset, 256 elágazási eltolás és 1000 futásfelosztás tesztelve. Eredeti H8H-dekóderrel/kezelőkkel/futtatóval 200000 szakasz és 899400 olvasás, nulla eltérés; szándékos regiszterhibát a kontroll észleli. Hat új Python regresszió. Részletek: [komponensszerződés](h8-first-instructions.md). Teljes CPU, IRQ/SCI integráció és firmware/audio nincs; M0 részben teljesült. A jelölt végső PR-head CI-je külön ellenőrizendő.

## H8-MEMORY-BRANCH-2026-10-28 — adatmemória és feltételes elágazások

Előző lépcső lezárása: [#47](https://github.com/RobCZart82/yaman1x/pull/47): merge `343f4c32c81229e89ec092b0d8e324f2a0305f8a`, végső head `6f8107ec35e511c6aa9d347295e67a882a7826d3`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640131062), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130940), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130959).

PR: [#48](https://github.com/RobCZart82/yaman1x/pull/48). Ág: `feat/h8-memory-conditional-branches`, alap: beolvadt #47. Korabeli jelöltállapot: 44 eredeti generált full/partial kezelő; MOV.L immediate32, indirekt bájtolvasás/-írás és mind a 16 rel8 feltétel. 65536 elágazási és 65536 dekódereset, 32768–32768 load/store eset, 1000 felosztás; két új Python regresszió. Eredeti H8H-kóddal 200000 szakasz, 844255 utasítás-, 27571 adatolvasás, 27574 adatírás: nulla eltérés; szándékos adatírási hibát a kontroll észleli. [Szerződés és korlátok](h8-memory-branches.md). M0 részben teljesült; IRQ/SCI CPU-bekötés, teljes CPU és firmware/audio továbbra is nyitott. A végső PR-head CI-je külön ellenőrizendő.

## H8-WORD-LONG-2026-10-28 — szó- és hosszúszó-átvitel

Előző lépcső lezárása: [#48](https://github.com/RobCZart82/yaman1x/pull/48): merge `800f8b4eabfe484b54e9918a652096c0acf50986`, végső head `a2da3e66797ec5d9f617e3e85b1e6505ccf26beb`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345253), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345183), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345179). Az automatikus review a beolvasztáskor még futott; nem lezárt eredményként szerepel. A beolvasztás a kézi forrásellenőrzés, eredeti kódos kontrollok és mind az öt sikeres CI alapján történt.

PR: [#49](https://github.com/RobCZart82/yaman1x/pull/49). Ág: `feat/h8-word-long-memory`, alap: beolvadt #48. Korabeli jelöltállapot: 56 eredeti generált full/partial kezelő; MOV.W immediate, indirekt word/long load/store és eredeti 0100 prefixállapot. Mind a 65536 szóérték, regiszter-/címátfedések, 65536 prefixkiterjesztés és 1000 felosztás ellenőrizve. Eredeti H8H-kóddal 200000 szakasz: 745100 utasításolvasás, 19304 bájtolvasás, 19310 bájtírás, 57833 szóolvasás, 57853 szóírás; nulla eltérés. Szándékos szó-bájtsorrendhibát a kontroll észleli. [Szerződés és korlátok](h8-word-long-memory.md). M0 részben teljesült; aritmetika, IRQ/SCI és teljes CPU/firmware/audio továbbra is nyitott. Végső PR-head CI külön ellenőrizendő.

## H8-ARITHMETIC-2026-10-28 — ADD/SUB/CMP és jelzőbitek

Előző lépcső lezárása: [#49](https://github.com/RobCZart82/yaman1x/pull/49): merge `4567d7bfe852100f5483a8104bcba81b50ae26bb`, végső head `f69503487de2fbbf3c575d81f54ca5a91e965f90`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971856), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971826), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971835). Az automatikus review a `5aff015d6157f166d9f26d0591e7b13d067cd171` forrásrevízión hibát nem jelzett; a követő commit csak PR-metaadatot rögzített a dokumentációban.

PR: [#50](https://github.com/RobCZart82/yaman1x/pull/50). Ág: `feat/h8-arithmetic-flags`, alap: beolvadt #49. Korabeli jelöltállapot: 78 eredeti generált full/partial kezelő; ADD/CMP byte immediate, ADD/SUB/CMP byte register és word/long immediate formák. 327680 byte-bemenetpár, 12288 regiszterátfedés, 100800 szélesebb eset és 1000 felosztás tesztelve. Matematikai overflow/borrow orákulum; CMP nem ír regisztert. Eredeti H8H-kóddal 200000 szakasz: 791089 utasításolvasás, 13555 byte-olvasás, 13557 byte-írás, 40593 word-olvasás és 40606 word-írás; nulla eltérés. Szándékos half-carry-hibát a kontroll észleli. [Szerződés és korlátok](h8-arithmetic-flags.md). M0 részben teljesült; további aritmetika/logika/shift, stack/IRQ és CPU/SCI-integráció nyitott. Végső PR-head CI külön ellenőrizendő.

## H8-CARRY-LOGIC-2026-10-28 — carry, regiszterek és logika

Előző lépcső lezárása: [#50](https://github.com/RobCZart82/yaman1x/pull/50): merge `0f8d3ae96eb32dcabb3a039f40e5c0fd0315a1b9`, végső head `4bbea3449f5d73f15031332b537919b493a5defc`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119562), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119555), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119545). Automatikus review a `ddc8d51221838e64c9e849101756c1aeca260dce` forrásrevízión: kész, nincs találat; a követő commit csak PR-metaadatot rögzített.

Ág: `feat/h8-carry-register-logic`, alap: beolvadt #50. Korabeli jelöltállapot: 124 eredeti full/partial kezelő; wide register ADD/SUB/CMP, byte ADDX/SUBX, byte register/immediate és wide immediate logika, LDC CCR. 1048576 carry/Z eset, 30720 wide register eset, 393216 byte logika, 12288 logikai átfedés, 36864 wide logika, 20000 négybyte-os lánc, 1000 felosztás. Eredeti kóddal 200000 szakasz, nulla eltérés; hibás Z-assertet a kontroll észleli. [Szerződés és korlátok](h8-carry-register-logic.md). M0 részben teljesült; LDC csak CCR-állapot, nem IRQ-elfogadás. Shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott. Végső PR-head CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: carry/regiszter/logika fejlesztés: [PR #51](https://github.com/RobCZart82/yaman1x/pull/51), `feat/h8-carry-register-logic`. Helyben 32 CTest-csoport és 83 Python-eset PASS; egy symlink-jogosultsági eset kihagyva. A végső head teljes CI-je külön ellenőrzendő; M0 részben teljesült.

## H8-WIDE-LOGIC-2026-10-28 — széles regiszteres logika

Előző lépcső lezárása: [#51](https://github.com/RobCZart82/yaman1x/pull/51), merge `ba2c91d8a156972bcf3c7b9adab98e36f7279b0c`, végső head `931a5a50dfe57c575d45d9d8b6b817889501a5a0`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821739), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821601), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821621). A forrásrevízió (`df6ce5574346e32b57a81dffb5c5b5f0e7929bbf`) automatikus review-ja kész, nincs találat; a követő commit dokumentációs PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-wide-register-logic`, alap: beolvadt #51. 138 eredeti generált full/partial kezelő: word/long regiszteres OR/XOR/AND és eredeti 01f0 prefix. 196608 word-eset, 245760 regiszter/CCR-kombináció, 65536 prefixkiterjesztés és 1000 felosztás PASS. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; XOR helyett OR hibát a kontroll észleli. [Szerződés és korlátok](h8-wide-register-logic.md). Helyben 33 natív CTest-csoport PASS; a teljes 34 csoport / 84 Python-eset végső CI-je külön ellenőrizendő. M0 részben teljesült; shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott.

Korabeli jelöltbejegyzés: széles regiszteres logika: [PR #52](https://github.com/RobCZart82/yaman1x/pull/52), `feat/h8-wide-register-logic`. Helyi 33 natív csoport / 83 Python-eset PASS; a teljes végső head CI-je külön ellenőrizendő.

## H8-SHIFT-ROTATE-2026-10-28 — egybites eltolás és forgatás

Előző lépcső lezárása: [#52](https://github.com/RobCZart82/yaman1x/pull/52), merge `88e88bd8551f78bc312230d459ea3c92d7b66f01`, végső head `eac6be6a95e22f1266730c6941efa0c766658913`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124816), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124527), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124813). A `c726582b72c43bc87cc917f1ff60c10f762a768a` forrásrevízió review-ja kész, nincs találat; a későbbi dekódersorrend-védelmet célzott és eredeti kódos teszt igazolja. Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-shift-rotate`, alap: beolvadt #52. 186 eredeti generált full/partial kezelő; SHLL/SHAL/SHLR/SHAR/ROTXL/ROTL/ROTXR/ROTR 8/16/32 biten. Független bitenkénti orákulum, signed-range overflow, 1744896 operandus/CCR/regisztersáv-eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; a megerősített explicit carry-program a kihagyott ROTXR carry-t seed 2/slice 8 helyen észleli. [Szerződés és korlátok](h8-shift-rotate.md). M0 részben teljesült; kétbites változatok, bitműveletek, stack/IRQ és CPU/SCI-integráció nyitott. A jelölt teljes CI-je külön ellenőrizendő.

Korabeli jelöltbejegyzés: egybites shift/rotate fejlesztés: [PR #53](https://github.com/RobCZart82/yaman1x/pull/53), `feat/h8-shift-rotate`. Helyi 34 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-REGISTER-BITS-2026-10-28 — regiszteres bitműveletek

Előző lépcső lezárása: [#53](https://github.com/RobCZart82/yaman1x/pull/53), merge `806ddf86a6373731f7b138c985a55ed41c583d6a`, végső head `b234b460a19d63d9a6c80e2086a4172a9a313a26`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288385), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288495), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288416). A `8ea376e0ba91bbc83ff94b521c5f0082bb093811` forrás review-ja kész, nincs találat; követő commit csak PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-register-bit-operations`, alap: beolvadt #53. 222 eredeti generált full/partial kezelő; regiszteres/immediate BSET/BNOT/BCLR/BTST, BST/BIST és carry logikai/load formák. 8126464 független operandus/bitindex/CCR/átfedési eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a BNOT helyett BSET hibát seed 3/slice 51 helyen felismeri a kontroll. [Szerződés és korlátok](h8-register-bit-operations.md). **Tervpontosítás:** a kétbites shift/rotate az eredeti listában `s20` (H8S/2000), a H8H-dekóderben nincs; nem H83002-fejlesztési feladat. Memóriás bitműveletek, stack/control flow és IRQ/SCI-integráció nyitott; M0 részben teljesült. Végső jelölt-CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: regiszteres bitműveleti fejlesztés: [PR #54](https://github.com/RobCZart82/yaman1x/pull/54), `feat/h8-register-bit-operations`. Helyi 35 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-MEMORY-BITS-2026-10-28 — indirekt memóriás bitműveletek

Előző lépcső lezárása: [#54](https://github.com/RobCZart82/yaman1x/pull/54), merge `efdb6df59ba53e8497d313fb33c7b40dcd9d76c4`, végső head `f9cab7c831a6aa2e880e2131b52a3623c049347c`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059555), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059577), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059727). Automatikus review-eredmény a lezáráskor nem volt; helyi forrás-/független/eredeti kódos kontroll ellenőrizve. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-memory-bit-operations`, alap: beolvadt #54. 262 eredeti generált full/partial kezelő: 7c/7d prefix és 18 indirekt bájtos bitművelet. 7340032 immediate eset, 131072 cím/indexátfedés, 131072 prefixkiterjesztés, 31 megszakítási pont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; read/prefetch/write sorrend és pontosan egyszeri mellékhatás ellenőrizve. [Szerződés és korlátok](h8-memory-bit-operations.md). M0 részben teljesült; abszolút memóriás formák, stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: indirekt memóriás bitműveleti fejlesztés: [PR #55](https://github.com/RobCZart82/yaman1x/pull/55), `feat/h8-memory-bit-operations`. Helyi 36 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-ABSOLUTE-BITS-2026-10-28 — abszolút memóriás bitműveletek

Előző lépcső lezárása: [#55](https://github.com/RobCZart82/yaman1x/pull/55), merge `e53b6fe7e2906b472e898c57d59ccfe1b791fa96`, végső head `452f9585afe1e67ceb57cd9cc8ac258202cd3cf7`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897203), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897191), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897202). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-absolute-bit-operations`, alap: beolvadt #55. 302 eredeti generált full/partial kezelő; 7e/7f prefix és 18 abs8 bitművelet. 7340032 immediate eset, 262144 cím/regisztersáv eset, 131072 prefixkiterjesztés, 31 futtatási vágáspont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés. [Szerződés és korlátok](h8-absolute-bit-operations.md). M0 részben teljesült; stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: abszolút memóriás bitműveleti fejlesztés: [PR #56](https://github.com/RobCZart82/yaman1x/pull/56), `feat/h8-absolute-bit-operations`. Helyi 37 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-STACK-CONTROL-2026-10-28 — veremkezelés és vezérlésátadás

Előző lépcső lezárása: [#56](https://github.com/RobCZart82/yaman1x/pull/56), merge `a3c2eb8ba25fc40eaa5057cc388260e4fc58b9df`, végső head `dee08721b3c2d3ccb16e1dfa4b4475625889c962`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502703), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502638), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502838). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-stack-control-flow`, alap: beolvadt #56. 366 eredeti full/partial kezelő; predecrement/postincrement MOV byte/word/long, JMP/JSR mindhárom címzése, BSR rel8/rel16, mind a 16 rel16 feltétel, RTS és szintetikus RTE-veremkép-visszaállítás. Független előjeles eltolás-/alias-/címhatár-/CCR-próbák, 201 beágyazott és 388 további hívás/RTE vágáspont, buszhiba-kontroll. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a hibás RTS SP+2 értéket seed 6/slice 5 helyen felismeri a kontroll. [Hatókör, bizonyíték és fennmaradó korlátok](h8-stack-control-flow.md). A normál verem/vezérlés utasításcsaládok ebben a rögzített H8H-profilban megvalósítva; IRQ/TRAPA-belépés, teljes CPU és CPU/SCI-integráció nyitott, M0 részben teljesült. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: verem/vezérlésátadási fejlesztés: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), `feat/h8-stack-control-flow`. Helyi 38 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő. Normál utasításcsaládok készültek el; IRQ/TRAPA-belépés továbbra is nyitott.

## DOC-RECONCILE-2026-10-28 — aktuális és történeti állapot egységesítése

A #57 lezárása a motoralaphoz rögzítve, a strukturált nyilvántartásban nincs nyitott motorjelölt. STATUS és roadmap egységesítve; korábbi jelöltbejegyzések megőrizve és történetiként jelölve. Az M0 következő feltétele már a hiányzó lefedettségre és gépintegrációra vonatkozik, nem az első CPU-fordításra. Dokumentációs javítás, új motor-/firmware-/audioelfogadás nélkül. Ellenőrzés: élő #57 merge/head és mind az öt CI-feladat, JSON-lezárás és egyedi PR-azonosítók, UTF-8 és helyi fájlhivatkozások, forrás- és értesítéslefedettségi audit.

A dokumentációs egységesítés PR-je: [#58](https://github.com/RobCZart82/yaman1x/pull/58). A motoralap továbbra is a lezárt #57; a dokumentációs PR saját aktuális CI-/merge-állapotát a hivatkozott PR mutatja.

## AUD-2026-10-28 — teljes repository-áttekintés

Ág: `audit/repository-2026-10-28`; átvizsgált dokumentációs főág: #58, `863d21abcde448d746b3b44c39a1eb61be455a15`. [Jelentés](REPOSITORY_AUDIT_2026-10-28.md). Helyi macOS: 39/39 CTest, 84/84 Python, 200000 eredetimetódus-szelet, célzott négy CPU ASan/UBSan-csoport PASS; forrás/licenc/napló/link ellenőrizve. Reprodukálható motorhiba nem jelentkezett. STATUS elavult „current 27” bekezdése javítva. A jelentés két CI-fejlesztési javaslatot és a nyitott CPU/firmware/audio határokat rögzíti; motorkód és mérföldkő-készültség változatlan.

## H8-COVERAGE-CCR-2026-10-28

Ág: `feat/h8-coverage-ccr`; alap: beolvadt #59. [Komponensjegyzőkönyv](h8-coverage-and-ccr.md). Reprodukálható 399 soros H8H-leltár és öt új CCR-család; 376 eredeti full/partial metódus, 196608 immediát CCR-eset, regiszterlane- és partíciópróbák. Eredeti kód 200000 szelet, 0 eltérés; ASan/UBSan PASS. A régi dekóderteszt 800 újonnan támogatott kódolással frissítve. M0 részben teljesült. A #59 saját négy ellenőrzése zöld volt, nyitott review-thread nélkül beolvasztva a felhasználó sorozatos merge-engedélye alapján.

## H8-REGISTER-MOV-2026-10-28

Ág: `feat/h8-register-transfers`; [jegyzőkönyv](h8-register-transfers.md). Bájt/szó/hosszúszó regiszterközi MOV, hat új eredeti full/partial metódus (382 összesen), 576 további elsőszavas kódolás. 147456 lane/alias/flag eset, 1000 felosztási történet; 41/41 helyi CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. Szülő: #60; saját PR csak annak beolvasztása után, a főágra újraalapozva.

### Sorozatos lezárás — #60 beolvadt

#60 merge: `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db`, 2026-10-28 15:04:13 UTC. Végső head `e8dee6975054c4143abe36b3b8731b7cea510cf4`; három platform, MSan és MAME mind PASS. A korábbi referenciafixture elérhetőségi hibája beolvadás előtt javítva. #61 a friss mainre újraalapozott, önálló CI-re kész regiszter-MOV jelölt; #62–#64 szülőfüggőség miatt draft.

## H8-SMALL-ARITHMETIC-2026-10-28

Ág: `feat/h8-small-arithmetic`; [jegyzőkönyv](h8-small-arithmetic.md). 16 új csoport, 32 generált metódus (414 összesen). INC/DEC N/Z/V frissítés H/C megőrzéssel, ADDS/SUBS teljes CCR-megőrzéssel. Teljes szó/bájt tartomány és hosszúszó-határok, 1000 felosztás; 200000 eredetimetódus-szelet. Szülője a regiszter-MOV ág; PR csak annak beolvadása és újraalapozás után.

### #61 lezárása, #62 előkészítése

#61 mind az öt ellenőrzése zöld, nyitott review-thread és változtatáskérés nélkül beolvasztva: `9cedcdf92d0330103fbeac24932c5c13e3924b3a`, ellenőrzött head `c36473985c685147867f9b56ecd250b9377d037a`. A korábbi piros macOS-futások futtatóhiány miatt szakadtak meg; az újrafuttatások sikeresek. #62 önálló változtatása a friss mainre újraalapozva; saját végső CI-je még külön kapu. #63–#66 továbbra is függőségi draft.

### #62 CI-indítás javítása

A főágra célzás és ready állapot önmagában nem indított workflow-t: az újraalapozott commit feltöltésekor a PR még a korábbi feature ágat célozta, a workflow-k pedig csak main célágra indulnak. Az élő Actions API az `1d0cd3d2df4a6dfcb319822ec5a034c1aece8bc0` headhez nulla futást mutatott. Ez a naplófrissítés a már mainre célzott PR szinkronizálásával indítja a szükséges ellenőrzéseket. A következő lépcsőknél a main célág beállítása után történjen a végső feltöltés. Motorkód változatlan; nincs CI- vagy merge-szabály megkerülés.

### #62 Windows-fordítási javítás

A `9222223a7c4c4c57c3d214b34e7af87eac4410f9` head Windows CI-je MSVC C1061 hibát jelzett: a 127 ágból álló utasításválasztó else-if lánc túllépte a fordító beágyazási korlátját. A legkülső lánc sorrendtartó, korai visszatéréses if ágakká alakult; minden kiválasztott kezelő után pontosan egy kilépés történik, így a kezelő által módosított állapotra nem történik újabb kiválasztás. A generált utasítástörzsek és kódolások változatlanok. A macOS normál feladata külön futtatóhiány miatt szakadt meg; a MAME macOS referencia-, Linux- és MSan-feladat sikeres volt. Az új head saját teljes CI-je továbbra is kötelező.

A lapos választó helyi ellenőrzése: 42/42 CTest, 87 Python-eset, 200000 eredeti dekóder/execute_run referenciaszelet nulla eltéréssel; forrás-/licenc-/feltöltési audit PASS. A Windows-fordító eredménye az új CI futásától függ, helyi macOS-eredményből nincs Windows PASS állítás.

## H8-UNARY-2026-10-28

Ág: `feat/h8-unary-extension`; [jegyzőkönyv](h8-unary-extension.md). NOT/NEG/EXTU/EXTS tíz csoportban, 20 eredeti metódus, 434 összesen. 43/43 helyi CTest, 87 Python, független flag/adat orákulum, ASan/UBSan PASS. Elérhető referenciaciklus, minden új családra bejárási őrfeltétellel; 200000 szelet 0 eltérés. Szülője az INC/DEC ág; a szülők merge/CI feltételei miatt egyelőre egymásra épülő jelölt, nem main-elfogadás.

### #62 lezárása, #63 előkészítése

#62 végső head `3e1652252342b0b58d31f9f475af2a6bf14cd909`: mind az öt CI PASS, nyitott review-thread és változtatáskérés nincs. Merge: `f0b37f3e10e02248b8c9d0f52b3a555ae944c8d5`, 2026-10-28 17:44:11 UTC. A GitHub új stack #67 miatt a szinkron merge API nem támogatott; a webes felület kizárólag #62-re indított aszinkron squash merge-je sikeres, API-val visszaellenőrizve. #63 önálló unáris változtatása a friss mainre újraalapozva, a Windows-javítás lapos választójának megőrzésével. Saját végső CI külön kapu; #64–#66 draft függőség marad.

#63 újraalapozás utáni helyi ellenőrzés: 43/43 CTest, 87 Python, 200000 eredetimetódus-szelet nulla eltéréssel, forrás-/licenc-/feltöltési audit PASS. A főágra célzás már megtörtént; a végső feltöltés indítja a PR saját CI-jét.

## H8-TRAPA-2026-10-28

Ág: `feat/h8-trapa-entry`; [jegyzőkönyv](h8-trapa-entry.md). Négy szinkron trapkódolás, 436 eredeti metódus. Rögzített reset-SYSCR, vektor8–11, stack-sorrend és observerhibák. 44/44 helyi CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. A stackbe írt m_NPC viselkedése forrásazonosság, nem hardverbizonyíték. Külső IRQ/NMI külön következő lépés; szülője az unáris ág.

### #63 lezárása és a #64–#66 Windows-javítása

#63 a GitHub szerint beolvadt: `dfa7af9bc21c8fe2b91367ad88b91bd7a56ba498`, 2026-10-28 18:12:45 UTC, végső head `ff7fa98a801ad33e0976206606673c7db0b17eca`, öt zöld CI. #64 újraalapozva a friss mainre; a régi Windows C1061 hibát okozó választólánc helyett a már ellenőrzött sorrendtartó korai kilépések megőrizve. #65 és #66 ugyanebben a függőségi sorrendben kapja meg ezt a javítást. A generált metódustörzsek változatlanok; minden új head saját CI-je szükséges.

## H8-EXTERNAL-IRQ-2026-10-28

Ág: `feat/h8-external-irq`; [jegyzőkönyv](h8-external-irq.md). Két eredeti state_irq metódus (438 összesen), opcionális kontrollerkötés, rögzített reset-SYSCR CCR.I szűrés, normál/noirq prefetch különválasztása. 512 maszkolás/NMI/RTE eset, 322 felosztás, külső edge pin, stackhibák és példányizoláció. 45/45 CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. Szülő #64; a merge kizárólag a szülők és saját végső CI miatt vár. #61 Linux/Windows/MSan zöld, két macOS-feladat még sorban áll; nem újrafuttatott hibák. Teljes gép- és audioelfogadás továbbra sincs.

#65 Windows-javítás: a #64 javított ágára újraalapozva, a régi mély else-if választó lecserélve sorrendtartó, azonnal kilépő ágakra. Új opcode vagy metódustörzs-változás nincs. A #64 függőség és a #65 saját végső CI-je továbbra is beolvasztási kapu.

### #64 lezárása, #65 aktuális jelölt

#64 beolvadt `ad1df979f17d7806d24f997f84d63fb087803675`, öt sikeres ellenőrzéssel, nyitott review-thread nélkül. A GitHub a #65 és #66 stackágakat automatikusan újraalapozta; #65 `d4fbd149d7141e05b47aee8262ce9973c9b5f2c8` teljes fájltartalma azonos a helyben validált `c091847` változattal. #65 már mainre céloz és review-ra kész; Windows/Linux/MSan zöld, két macOS-feladat vár. E naplófrissítés után az új head CI-je a merge-kapu. #66 még szülőfüggőség miatt draft.

## H8-SCI-CPU-RECEIVE-2026-10-28

Ág: `feat/h8-sci-cpu-receive`; [jegyzőkönyv](h8-sci-cpu-receive.md). Meglévő CPU/IRQ/SCI összekötése szintetikus teszthostban: 512 bájt/csatorna eset, 1602 felosztás; vezetéki él → RXI → ISR → RAM → RDRF-törlés → RTE. Új motorkód nincs. A host kizárólag előre konfigurált RX-et és időfüggetlen SSR-nyugtázást támogat; általános MMIO-írás időbélyeg és új határidő publikálása külön következő feladat. Szülő #65, merge/CI függőség miatt jelölt.

Helyi lezárás: 46/46 CTest és ASan/UBSan PASS; változatlan eszközök 87 Python-esete újra ellenőrizve. #60 beolvasztva `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db` (öt zöld ellenőrzés), #61 saját macOS-feladatai még várakoznak. #65 az IRQ/NMI-jelölt PR-je.

#66 a javított #65 ágára újraalapozva; örökli a Windows-fordítási javítást. A három javított head közös teljes helyi tesztje és a legutolsó eredetimetódus-referenciája következik; a szülői merge-kapuk változatlanok.

A #64–#66 javított lánc helyi lezárása: 46/46 CTest, 87 Python-eset, 200000 eredeti dekóder/execute_run referenciaszelet nulla eltéréssel; forrás-/licenc-/feltöltési audit és diffellenőrzés PASS. #64 külön TRAPA-próbája 1024 eset/81 vágáspont mellett is sikeres. Windows PASS csak az új headek GitHub-fordítása után állítható; a régi piros headek történeti eredmények.

### #65 lezárása, #66 utolsó jelölt

#65 beolvadt `f59661ed79860460ba2989f6d2177554e0b56a05`, 2026-10-28 19:05:03 UTC, head `18f1fa382c8768e2424644464d7556170c9a8501`, öt sikeres CI, nyitott review-thread és változtatáskérés nélkül. #66 önálló fogadási tesztje a friss mainre újraalapozva; csak napló/ledger konfliktus volt. A végső head saját ellenőrzése még szükséges.

## H8-BUS-CYCLE-TIME-2026-10-28

Ág: `feat/h8-bus-cycle-time`, alap: #66 beolvadt `817e9ce91db22c770291fe2acf666f86f29e1896`, head `66687f2313c5824e629cff570aeffd365da71d73`, négy sikeres CI, nyitott review-thread nélkül. A korábbi #66 jelöltleírás történeti. [Új szerződés](h8-bus-cycle-time.md): opcionális bus_cycle időbélyeg minden kétciklusos buszművelet után, a memóriaművelet előtt, túlcsordulás- és hibakezeléssel. Öt művelettípus és 201 felosztás független ellenőrzése sikeres. Ez a CPU-vezérelt soros adás előfeltétele; az MMIO által létrehozott új határidő beillesztése külön következő lépés.

Helyi lezárás: 47/47 CTest, 87 Python, 200000 eredetimetódus-szelet nulla eltéréssel; új célzott teszt és ASan/UBSan PASS, forrás-/licenc-/feltöltési audit PASS. Hiányzó időalap és negatív abszolút idő külön visszautasítva. Saját végső CI/review még külön merge-kapu.

## H8-SCI-CPU-TX-2026-10-28

Ág: `feat/h8-sci-cpu-transmit`, szülő #68. [Szerződés](h8-sci-cpu-transmit.md): minden sikeres buszművelet után opcionális tiszta next_bus_event lekérdezés frissíti a futtató és az aktuális generált kezelő eseményhatárát. CPU által konfigurált SCI-adás két csatornán, összesen 512 bájteset és 1602 felosztás; start/adat/stop/végső idle, 32 ciklusos bitköz. Eredeti metódustörzsek változatlanok; teljes teszt/ref/sanitizer lezárás folyamatban, szülőmerge és saját végső CI külön kapu.

Helyi lezárás: 48/48 CTest, 87 Python, ASan/UBSan, 200000 eredetimetódus-szelet nulla eltéréssel; forrás-/licenc-/feltöltési audit PASS. #68 mind az öt CI-je zöld, nyitott review-thread/változtatáskérés nélkül beolvasztva: `0fa40aaba818a0efd7265a1861650479795cbba1`, head `7867ec70308e4e06ad794758e4f7c05008534f38`. A TX-jelölt saját végső CI-je külön kapu.

## H8-SCI-CPU-DUPLEX-2026-10-28

Ág: `feat/h8-sci-cpu-duplex`; [szerződés](h8-sci-cpu-duplex.md). Motorváltoztatás nélkül egyidejű TX/RX, mind a négy csatornapárosítás, 1024 bájteset és 3604 felosztás. Független wire/adat/IRQ/RDRF/RTE ellenőrzés; a frame-ek átfedik egymást, de aktív TX közbeni RXI-preempciót nem állítunk. Szülő #69 öt sikeres CI és üres review-thread/változtatáskérés lista után beolvadt: `298b8465aee4e74cfe7ec8e1459402b21f2ce9aa`, head `08a18b492a2c7df3abc5c33edac1f37a994a080f`. Teljes helyi teszt/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 49/49 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Negatív kontroll: RXI engedélyezés nélkül a teszt hibát jelez, tehát a RAM/IRQ elfogadás nem pusztán a TX befejezését ellenőrzi. Eredeti CPU-/UART-motorkód változatlan; a #69 eredetimetódus-referenciája továbbra is a motoralap bizonyítéka.

## H8-SCI-CPU-FER-2026-10-28

Ág: `feat/h8-sci-cpu-framing`; [jegyzőkönyv](h8-sci-cpu-framing.md). Motorváltoztatás nélküli ERI/FER-integráció: 1024 bájt/csatorna eset, 3604 felosztás, 512 következő jó frame helyreállítási eset. CPU SSR-kiolvasás/RAM/nyugtázás/RTE, majd RXI és helyes adat. A hiányzó idle-high stimulus mind512 recovery esetben hibát jelzett, a tesztbemenet javítva. #70 négy zöld CI, üres review-thread/változtatáskérés lista után beolvadt `860815eb62afb2d9b55e1e2cc8318a85967d4d04`, head `a063232d4f877fc2d3eea43303b73273a8e872eb`. Saját végső CI külön kapu; overrun/paritás még nyitott.

Helyi lezárás: 50/50 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Negatív kontroll: FER-törlés kihagyása 2048 elfogadási hibát eredményez, tehát a teszt a nyugtázást és recovery-t is ellenőrzi. A közös fixture kiemelése után a korábbi duplex teszt változatlan alapbeállításokkal továbbra is sikeres. Saját végső CI/review még szükséges.

## H8-SCI-CPU-PARITY-2026-10-28

Ág: `feat/h8-sci-cpu-parity`; [jegyzőkönyv](h8-sci-cpu-parity.md). SMR0x20/0x30, jó/rossz paritás mindkét csatornán, 2048 bájteset és 8808 felosztás. CPU RXI/ERI, RAM/SSR, PER-törlés és RTE; független bit-count bemenet és TX-paritás/időzítés. A rögzített forrás paritáspolaritását követjük, hardver-validált even/odd állítás nélkül. #71 négy zöld CI, nyitott review-thread/változtatáskérés nélkül beolvadt `8e9c431f1a0629cc20db136c50c56eba81854202`, head `e49e6c0053c8c5179e4475fc51ee3628478b9177`. Teljes helyi teszt/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 51/51 CTest, 87 Python, célzott ASan/UBSan, forrás-/licenc-/feltöltési audit PASS. PER-törlés nélküli negatív kontroll hibát jelez. A közös fixture alapértelmezett no-parity és FER-próbái továbbra is sikeresek. Motorkód változatlan; saját végső CI/review még szükséges.

## H8-SCI-CPU-OVERRUN-2026-10-28

Ág: `feat/h8-sci-cpu-overrun`, szülő #72. [Jegyzőkönyv](h8-sci-cpu-overrun.md). Két kiolvasatlan frame maszkolt CPU mellett, ORER+RDRF és első adat megőrzése; késleltetett LDC0 után CPU SSR/RDR mentés, törlés és RTE. A korábban latch-elt RXI az ERI után külön RTE-kezelőt kap; ez rögzített modellviselkedés, nem hardverbizonyíték. 512 bájteset, 4802 felosztás célzott PASS. #72 Linux/Windows/MSan zöld, macOS futtatóhiány miatt megszakadt, újraindítva. Az overrun PR a szülő CI/merge miatt visszatartandó, nem több fejlesztési lépcső miatt.

Helyi lezárás: 52/52 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Második frame nélküli negatív kontroll 2048 elfogadási hibát jelez. Motorkód változatlan. #72 macOS újrafutása folyamatban; az overrun jelölt szülőmerge és saját végső CI miatt draft.

#72 lezárása: macOS újrafutás sikeres, négy zöld CI, üres review-thread/változtatáskérés lista; merge `f29f0373be4dc6ca866744c2a0d832d82eb7dfa6`, head `dceda74cfa1402f050649b14b91de531aa8d9fdf`. #73 tisztán újraalapozva a mainre és review-ra kész; a korábbi draft/szülővárakozás bejegyzés történeti. A végső head saját CI-je még merge-kapu.

## H8-ABSOLUTE-MEMORY-2026-10-28

Ág: `feat/h8-absolute-memory`; [jegyzőkönyv](h8-absolute-memory.md). Hat abs8/abs16 MOV-csoport, 12 eredeti full/partial metódus (450 összesen), leltár217 kiválasztott/47 hiányzó/135 kizárt. Összes abs16 cím, abs8 regisztersáv/érték/CCR, 201 felosztás és hat buszhibahely; elérhető eredeti referenciaciklus mindhat formára fetch-őrrel. #73 négy zöld CI és üres review-thread/változtatáskérés lista után beolvadt `8446d10b65c698b5fbc749279f696d69c100e5fc`, head `c37346094ed6b35f068d8969a2401df40f010636`. Teljes helyi teszt/ref/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 53/53 CTest, 87 Python, célzott ASan/UBSan, forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run: 200000 szelet nulla eltéréssel, mindhat abszolút forma fetch-őre teljesült. A végső adatbuszhiba-tesztekkel az új cél külön újrafordítva és újrafuttatva. A 65536 elsőszavas dekóder-orákulum az újonnan támogatott abszolút kódolásokkal frissítve; szomszédos tiltott formák továbbra is elutasítva. Saját végső CI/review szükséges.

## H8-ABSOLUTE32-MEMORY-2026-10-28

Ág: `feat/h8-absolute32-memory`; [jegyzőkönyv](h8-absolute32-memory.md). Négy bájt/szó abs32 MOV-csoport, nyolc eredeti metódus (458 összesen), leltár221 kiválasztott/43 hiányzó/135 kizárt. 15 címhatár minden regisztersávval, formánként65536 korrelált32-bites címminta (nem teljes32-bites tartomány), 201 felosztás és négy buszhibahely. Külön elérhető eredeti referencia mindnégy fetch-őrrel. #74 öt zöld CI, nyitott review-thread/változtatáskérés nélkül beolvadt `758ad981253f8d3b5c72915f2fe5b60df560de97`, head `642977224868632203a46b1d220b926e506b0b5c`. Teljes helyi teszt/ref/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 54/54 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run: 200000 szelet nulla eltéréssel; mindnégy abs32 fetch-őr és minden korábbi család elérhetőségi őre teljesült. Teljes32-bites tartomány helyett határpontokat és65536 mintát ellenőriztünk formánként. Saját végső CI/review szükséges.

## DOC-CLOSE-ABSOLUTE-MEMORY-2026-10-28

#75 lezárása az élő GitHub merge/head/időpont és öt sikeres ellenőrzés alapján rögzítve. A napló eleje, STATUS, REAPER-terv, README és strukturált ledger egységesítve; a korábbi jelöltleírások történetiek. A motoralap #75, nincs új motor-/firmware-/audioelfogadás. A dokumentációs PR saját CI-je külön követendő.

## H8-LONG-ABSOLUTE-MEMORY-2026-10-29

Ág: `feat/h8-long-absolute-memory`; [jegyzőkönyv](h8-long-absolute-memory.md). Négy abs16/abs32 MOV.L csoport, nyolc eredeti metódus (466 összesen),225 kiválasztott/39 hiányzó/135 kizárt sor. Négy szavas IR-puffer és snapshot/reference,0100 prefix ER-őrök; eredeti két szavas sorrend/átfordulás. Célzott összes abs16 cím, ER/CCR határpontok,201 felosztás és első/második írási hibák PASS. #76 dokumentációs PR még CI alatt; a mostani motorjelölt ettől külön kezelendő. Teljes teszt/ref/sanitizer lezárás folyamatban.

Helyi lezárás: 55/55 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run:200000 szelet nulla eltéréssel, mindnégy long-abs fetch-őr és minden korábbi őr teljesült. A régi65536 prefixkiterjesztés-orákulum32 új támogatott kódolással frissítve; érvénytelen ER8..15 szomszédok továbbra is tiltottak. #76 macOS várakozik; az új motor-PR csak a szülőmerge és saját végső CI miatt draft.

#76 lezárva: négy sikeres CI, üres review-thread/változtatáskérés lista; merge `0c67fce0e32450709523e4ac01ae477a64633916`, head `ae753a6bc1418be4885645a38ae3fc757e7e293c`. #77 tisztán újraalapozva a mainre; korábbi szülővárakozása megszűnt. Saját végső CI/review még merge-kapu.

## H8-DISPLACEMENT16-2026-10-29 — 16 bites eltolású memóriaátvitel

Ág: `feat/h8-displacement16-memory`, alap: beolvadt #77. Hat eredeti MOV byte/word/long
load/store csoport, 12 változatlan full/partial metódus: 478 összesen;
231 kiválasztott / 33 hiányzó / 135 kizárt kódolássor. Mind a 65536 eltolás,
regiszterátfedések, teljes ER-címkörbefordulás, CCR és szóigazítás tesztelve.
201 felosztás és kétlépcsős store-hiba; eredeti dekóderrel 200000 szakasz nulla eltéréssel.
Mind a hat új forma és a korábbi referencia-családok elérésellenőrzése megmarad.
[Részletes bizonyíték és korlátok](h8-displacement16-memory.md). A jelölt teljes csomagja
56 CTest / 87 Python; firmware/audio-elfogadás továbbra sincs. Végső PR-CI külön bizonyíték.

Az eltolásos memóriautasítások PR-je: [#78](https://github.com/RobCZart82/yaman1x/pull/78). A készítéskori jelöltállapot és a végső CI-/merge-bizonyíték külön kezelendő.

## 2026-10-29 — Előjel nélküli bájtszorzás jelölt

Az eredeti MULXU.B full/partial kezelők kiválasztva; fix H8H MAC nélküli időzítés.
65536 operanduspár,19200 alias/CCR-határeset és161 szétvágott futás PASS;
200000 eredeti referencia-szelet nulla eltéréssel; szándékos product+1 hiba felismerve.
Helyi56 CTest/86 Python PASS, ismert Windows-symlink eset kizárva; távoli CI függőben.
Jelölt:57 CTest/87 Python,480 eredeti módszer,232 kiválasztott/32 hiányzó/135 kizárt sor.
A szó-szorzás külön nyitott: az eredeti u16 operandusok int-promóciója nagy szorzatnál
C++-túlcsordulási kockázatot jelent. Bekötés előtt független nagyértékű teszt,
UBSan és dokumentált unsigned-szélesítés felülvizsgálata szükséges.
Ez lehetőség és igazolandó javítási feladat; nem hardveres hibamegállapítás.
Részletek: [szerződés és bizonyíték](h8-byte-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-szorzás jelölt

MULXU.W: két eredeti eredetű full/partial kezelő minimális, dokumentált javítással:
szorzás előtt u32-szélesítés, hogy a u16 operandusok int-promóciója ne csorduljon túl.
A korábbi480 kezelő változatlan;482 eredeti eredetű kezelőből2 adaptált.
327680 operandus-söprés,9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS.
200000 eredeti referenciaszelet nulla eltéréssel, csak biztonságos szorzatokkal;
a nagy szorzatokat független64 bites oracle ellenőrzi. Product+1 kontroll:seed9/slice9.
Helyi57 CTest/86 Python PASS, ismert Windows-symlink eset kizárva. Jelölt:58 CTest/87 Python,
233 kiválasztott/31 hiányzó/135 kizárt kódolássor. Linux UBSan és külön eredeti-hibakontroll
bekötve; tényleges távoli eredmény még függőben. A szó-szorzás nyitott kockázata
csak ezek sikeres lezárása után tekinthető rendezettnek; ez nem hardverelfogadás.
[Szerződés, pontos adaptáció és bizonyítékok](h8-word-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjeles bájt- és szó-szorzás jelölt

Eredeti01c0 prefix és MULXS.B/MULXS.W full/partial kezelők; külön őrzött folytatás.
Az eredeti set_nz16/set_nz32 csak N/Z-t módosít. Független64 bites számítás:
65536 bájtpár,327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár PASS;
241 szétvágott futás és65536 prefixfolytatás ellenőrzött. Eredeti referenciaszeletek:
200000,nulla eltérés; mindkét forma elérése kötelező. Product+1 kontroll seed9/slice14.
Jelölt:59 CTest/87 Python,488 upstreamből származó kezelő (486 változatlan,2 korábbi
unsigned-adaptáció);235 kiválasztott/29 hiányzó/135 kizárt kódolássor. Linux UBSan
mindkét új formát is teszteli; távoli CI függőben, M0 továbbra is részleges.
[Szerződés és részletes bizonyítékok](h8-signed-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli bájtos osztás jelölt

Eredeti DIVXU.B full/partial kezelők, külön5100/ff00 dekóderág. Nullás osztónál
célregiszter-megőrzés; hányados alacsony8 bitje és maradék külön bájtban; N/Z
az eredeti osztófüggő szabályt követi. Ez forrásegyezés, nem hardveres bizonyíték.
393216 osztandó-söprés,2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár
és241 szétvágás PASS;200000 referenciaszelet nulla eltéréssel. Hányados+1 kontroll
seed9/slice18 felismerve. Jelölt:60 CTest/87 Python,490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott/28 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve osztásra is; távoli CI függőben. M0 továbbra is részleges.
[Szerződés és ellenőrzési hatókör](h8-byte-divide.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-osztás jelölt

DIVXU.W, külön5300/ff08 dekóderág. A hányados/maradék u32 típusa explicit;
csomagolás unsigned32 marad. Két dokumentált típusadaptáció, nem C++20 UB-mérés.
Az eredeti N az osztó7. bitjét követi, nem a15.-et; hardvermérés nélkül ezt megőrizzük.
524288 osztó-söprés,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS;
200000 biztonságosan korlátozott referenciaszelet nulla eltéréssel. Nagy értékek
független64 bites számítással ellenőrizve; hányados+1 kontroll seed9/slice26.
Jelölt:61 CTest/87 Python,492 upstreamből származó kezelő (488 változatlan,4 adaptált);
237 kiválasztott/27 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve szó-osztásra;
távoli CI függőben, M0 továbbra is részleges. [Szerződés](h8-word-divide.md).
Yamaha-adat nem került be; firmware/MIDI/hang/plugin elfogadás nincs.

## 2026-10-29 — M0 decimális korrekciós részlépés: DAA/DAS jelölt

Négy változatlan eredeti full/partial kezelő, külön0f00/fff0 és1f00/fff0 dekóderág.
Független tízes számrendszerű számítás: minden0–99 operanduspár összeadás/kivonás,
két CCR-minta (40000 eset);2048 regiszterhatáreset és241 szétvágott futás PASS.
Érvénytelen BCD/flag kombinációkra nem állítunk teljes független helyességbizonyítást.
200000 eredeti referenciaszelet nulla eltéréssel, mindkét új forma és korábbi családok
elérése kötelező; DAA korrekció+1 kontroll seed7/slice0 felismerve. Jelölt:62 CTest/87 Python,
496 upstreamből származó kezelő (492 változatlan,4 korábbi adaptáció);239 kiválasztott/
25 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve. M0 továbbra is részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. [Szerződés](h8-decimal-adjust.md).
Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles bájtos osztás jelölt

Változatlan eredeti01d0 prefix és DIVXS.B kezelők, őrzött0xf0000 folytatás.
Független64 bites számítás:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás és65536 prefixfolytatás PASS. A hányados nullához
csonkolódik,maradék az osztandó előjelét követi; N a teljes hányados előjele,Z a nullás
osztó. Eredeti kommentek helyett a matematikát és futó kódot ellenőrizzük; hardverigazolás nincs.
200000 referenciaszelet nulla eltéréssel; hányados+1 kontroll seed7/slice3 felismerve.
Jelölt:63 CTest/87 Python,500 upstream-kezelő (496 változatlan,4 korábbi adaptáció),
240 kiválasztott/24 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve; CI függőben.
Szó-osztás INT32_MIN/-1 és csomagolás külön nyitott vizsgálat. [Szerződés](h8-signed-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles szó-osztás és szélső hányados jelölt

Két változatlan eredeti DIVXS.W kezelő, őrzött meglévő01d0 folytatás;32 bites int
fordítási feltétel. Az eredeti unsigned32 segédértékek elkerülik a közvetlen signed
INT_MIN/-1 osztást. A matematikai +2147483648 hányadost az eredeti int32-re szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres hibajavítás vagy mérés.
524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,1024 szélső/CCR-eset,321
szétvágás és128 tiltott ER-szomszéd PASS. Független64 bites számítás és külön
szűkítési flagmodell.200000 eredeti referenciaszelet nulla eltéréssel; fordított
N kontroll seed7/slice7 felismerve. Jelölt:64 CTest/87 Python,502 upstream-kezelő
(498 változatlan,4 korábbi adaptáció),241 kiválasztott/23 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve; CI függőben. [Hatókör és szélső eredmény](h8-signed-word-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nincs.

## 2026-10-29 — M0 32 bites eltolásos bájt/szó-memóriamozgatás jelölt

Eredeti7800 prefix és négy MOV-csoport,tíz változatlan full/partial kezelő.
Meglévő négyszavas IR,őrzött160000 folytatás,full32 cím és külön24 bites fizikai
leképezés.1048576 eltolásfél-söprés,55296 cím/regiszter/CCR-határeset,241 szétvágás,
65536 prefixfolytatás és memóriahibás fail-stop tesztek PASS. Nem teljes2^32 keresztszorzat.
A prefixkontroll nyolc hibás belső-állapot újradekódolást fogott; az elsőszó-maszk
szűkítése rendezte.200000 eredeti referenciaszelet nulla eltéréssel; felső eltolásszó
elhagyása kontroll seed6/slice5 felismerve. Jelölt:65 CTest/87 Python,512 upstream-kezelő
(508 változatlan,4 korábbi adaptáció),245 kiválasztott/19 hiányzó/135 kizárt sor.
Hosszú szó32-eltolással továbbra is nyitott. Linux UBSan kiterjesztve; CI függőben.
[Szerződés és hatókör](h8-displacement32-memory.md). M0 részleges; firmware/MIDI/hang/plugin
elfogadás nincs. Yamaha-adat nem került be.

## #200 main infrastructure retry — 2026-11-03

Research run41700030181/job126140567756 was not acquired by hosted runner
after multiple attempts;annotations explicitly report macOS arm64 capacity.
After containing workflow completed,requested only that cancelled job retry.
Retry job126151506376 succeeded;Ubuntu also passed without restart.
All five main checks now successful. No code failure established.

## #210 macOS capacity retries — 2026-11-04

Both platform macOS job126658587970(run41854292121) and research job
126658578105(run41854290293) were not acquired by hosted runners;annotations
explicitly report arm64 capacity. After each containing workflow completed,
requested only its cancelled job retry. Results pending;three other exact-head
checks passed. Automatic review result not recorded. No code failure established.

## #210 second platform macOS capacity retry — 2026-11-04

Job126680596924 again was not acquired;annotations confirm hosted arm64
capacity. Containing platform workflow completed before requesting only this
cancelled job retry. Research job126680629483 remains queued and was not
restarted. No code failure or automatic review result recorded.

## #210 persistent external runner blocker — 2026-11-04

Platform job126702161088 again not acquired;annotations confirm hosted macOS
arm64 capacity. This is the third confirmed platform acquisition failure;
no further retry requested. Research job126708964536 currently queued,untouched.
Ubuntu/Windows/MSan passed;automatic review result absent. PR210 remains open
and unmerged. Stop recurring automation per user persistent-blocker instruction;
resume requires checking current remote state after capacity recovers.

## #210 merge reconciled — 2026-11-04

User reported all PRs merged;live GitHub confirms no open PRs and #210 merged
at2026-11-04T19:40:14Z,commitf37ed085c5f37e09cc84056e91d26eae1946ec3f.
All five checks on PR head4e82f082c8b1870487bd1eabc999d4f5897fadce now
successful. No automatic review result recorded. Prior runner-capacity blocker
has recovered;historical retry records retained. Fresh main pulled. Four main
checks passed;Ubuntu still running. Next bounded deadlines/IRQ dispatch waits
for that result. Previous recurring automation remains deleted,not recreated.
