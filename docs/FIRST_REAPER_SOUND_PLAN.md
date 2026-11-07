# YamAN1x: fejlesztési terv az első REAPER-ben megszólaló hangig

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

## Slow ADC scan on the AN1x bus — merged in #235, 2026-11-06

An explicit scan callback supplies each AN0–AN3 sample at the machine
deadline. The 0x33 profile exposes aligned ADDR words and byte lanes,
preserves the prior AN3 result until its first scan commit, and updates
ADF at each completed sweep. Reset cancels pending scan events.

The bus test covers 1,035 time partitions with independent sample values,
lane expectations and exact deadlines. The pinned H8_ADC_3337 model below
defines the timing. A fresh firmware probe with explicit scan inputs is
the next step. Board calibration, boot and audio acceptance remain open.

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
Candidate CI and automated review are pending.


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


## Pure ITU next-event deadline — candidate,2026-11-04

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


## Internal-clock ITU auto-clear events — candidate,2026-11-04

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


## Free-running ITU counter events — candidate,2026-11-04

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


## Isolated H8H ITU status latch — candidate,2026-11-04

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


## Isolated internal-clock ITU counter — candidate,2026-11-04

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


## Active ITU source contract — candidate,2026-11-04

Three locked original files/19 exact method identities checked through the
private verified archive snapshot. TSTR settles counters before enable changes;
compare flags may change with IRQ disabled;absolute divider/phase time,clear/
wrap boundaries,TGRFFFF u16 comparison and reset-after-children order require
independent tests. No active timer behavior enabled. Next:isolated monotonic
internal-clock counter,then flags/IRQ/deadlines,before MMIO/firmware acceptance.
[Contract and gates](an1x-active-itu-contract.md). Own CI/review pending.


## Firmware level-read experiment — candidate,2026-11-04

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


## Explicit experimental native level readback — candidate,2026-11-04

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


## Reg22 readback contract — candidate,2026-11-04

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


## Native VOP3 firmware-probe profile — candidate,2026-11-03

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


## VOP3 CPU word-bus integration — candidate,2026-11-03

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


## Native VOP3 register backend — candidate,2026-11-03

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


Rögzítve: 2026-10-27. Kiindulópont: a #14 PR-ig beolvadt kutatási eredmények.
Ez végrehajtási terv; az alábbi, még nyitott mérföldkövek nem kész funkciók.
Állapotellenőrzés:2026-10-30,motoralap:#143 (`301eec75c72028b8f377ee4e099fce0b1df4de5e`).
A működő H8H-részhalmaz nem teljes CPU- vagy firmware-elfogadás.

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

## Megtartott BRCR profil — jelölt, 2026-11-02

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
állapotot őrzik; a jelenlegi készültségre a fenti #116 lezárás és az aktuális mérföldkő-tábla
irányadó. A dokumentáció mostani javítása nem új motorfejlesztés vagy elfogadás.

## Aktuális mérföldkő-állapot — 2026-10-29

| Mérföldkő | Aktuális állapot | Következő elfogadási feltétel |
|---|---|---|
| M0 | **Részben teljesült.** Architektúra és forrás-/licencnyilvántartás; tesztelt GPIO-, IRQ- és SCI-komponensek; működő önálló H8H-részhalmaz 562 upstreamből származó kezelővel (558 változatlan,4 adaptált), rögzített SYSCR-es IRQ/NMI/TRAPA-belépéssel, buszidőbélyeggel és szintetikus CPU–SCI RX/TX/hibakezeléssel. | A264 leltározott H8H-kódolássor kezelője kiválasztva; a tesztelt CPU/periféria-ütemezés teljes AN1x címtér/MMIO-gépbe integrálása és gépizoláció. A részhalmaz már fordul és fut; a teljes CPU és a terjesztési licenc lezárása nyitott. |
| M1 | **Előkészítve, nem teljesült.** A külön macOS MAME-build és a párosított helyi futtató rendelkezésre áll. | Friss Mac-firmware-próba: SCI0 tényleges bájtfogadás és note-on/note-off feldolgozás bizonyítása; PKS-kézfogás feltárása. |
| M2–M3 | **Kutatási feladatok nyitva.** Nincs igazolt referenciahang. | AN1x órajel, táblacímzés, EG és soros hangút bizonyítása, majd megismételhető offline hang. |
| M4–M6 | **Tervezett.** Nincs elkészült beágyazható hangszer vagy REAPER-elfogadás. | Validált offline referencia után audio-blokkmotor, VST3 és helyi REAPER-próba. |

Az önálló komponens tesztje nem teljes CPU-/firmware-elfogadás. A jelenlegi
állapotot ez a táblázat és az M0/M1 részletes szakasza mutatja. A lezárási tábla
az ellenőrzött végső PR-revíziókat és CI-futásokat rögzíti; a korabeli napló
az akkori munkamenet állapotát őrzi.

Korábbi, #36-hoz tartozó platformeredmény: [#36](https://github.com/RobCZart82/yaman1x/pull/36),
`6f3fbf6c22b9f3bfc28f78d0fc1bad55ed98ed8c`: Linux/macOS/Windows platformonként
**22/22 tesztcsoport PASS**, MSan és teljes macOS MAME-build PASS. A konkrét
futások a lezárási táblában vannak. Helyben Windows alatt 21 futtatható csoport
PASS; a Python unittest-csoportot a korábban igazolt symlink-jogosultsági korlát
miatt kizártuk a helyi futásból. A teljes csoport CI-ben sikeres. Ez nem teljes
helyi tesztcsomag-PASS, és nem firmware-es MIDI-elfogadás.

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


## A terv folyamatos vezetése

Ez a dokumentum a fejlesztés során folyamatosan karbantartandó terv.
Minden, a tervet érintő fejlesztési PR ugyanabban a változtatásban frissítse
az érintett mérföldkövet és az alábbi előrehaladási naplót. Rögzíteni kell:

- a dátumot, a konkrét eredményt és a kapcsolódó PR-t vagy commitot;
- az állapotot: tervezett, folyamatban, részben teljesült vagy igazoltan kész;
- a végrehajtott ellenőrzéseket és eredményüket, a platformot és a vizsgált
  revíziót; a nem futtatott és környezeti okból akadályozott próbákat külön;
- a megmaradó feltételeket, függőségeket és a következő konkrét lépést.

A mérföldkő csak a saját elfogadási feltételeinek bizonyítása után jelölhető
késznek. A ROM-mentes CI sikere nem helyettesíti a hardver-, firmware-, audio-
vagy REAPER-próbát. Tervváltozásnál az indokot és az új sorrendet is rögzíteni
kell. A történeti eredményeket megőrizzük dátumozott bizonyítékként;
az aktuális állapotot az új bejegyzés és az érintett mérföldkő együtt mutatja.

A PR készítésekor a végső CI még nem ismert. Beolvasztás után a következő
nyilvántartási frissítésben kötelező a merge SHA, végső head és tényleges CI-eredmény
lezárása; az előző jelölt nem maradhat aktuális aktív fejlesztésként. A strukturált
nyilvántartás motoralapja az utolsó ellenőrzött motorcommit, nem szükségszerűen a
legújabb dokumentációs commit; `active_change: null` azt jelenti, nincs nyilvántartott
nyitott motorjelölt. A dokumentációs PR saját CI-jét a PR lezárása rögzíti.

ROM, bank és privát trace helyett csak publikálható azonosító és összefoglaló
kerülhet a tervbe. A részletes mérési jegyzőkönyvekre hivatkozás mutasson.

Az új szerkezet vezetési szabálya: minden új fejlesztésnél az aktuális
állapottáblát és az érintett részletes mérföldkövet együtt kell frissíteni.
A munkanapló új sora kapjon dátumot, PR-hivatkozást és „Korabeli állapot” jelölést.
A végső ellenőrzések lezárásakor a PR-lezárási táblába a valóban tesztelt
PR-revízió és annak saját futásai kerüljenek; más revízió CI-je nem helyettesíti.
A korabeli mérési számokat és korlátokat megőrizzük, a már teljesült előfeltételt
pedig az aktuális feladatlistából átvezetjük az elkészült eredmények közé.

## Előrehaladási napló

### Repository-vizsgálat és központi változásjegyzék — 2026-10-27

Az `AUD-2026-10-27` vizsgálat a #37 merge állapotáról indult. Elkészült a
[repository-audit](REPOSITORY_AUDIT_2026-10-27.md), a PR-eket indexelő
[fejlesztési napló](DEVELOPMENT_LOG.md) és a hozzá tartozó strukturált leltár.
A korabeli bejegyzések és a fenti végső PR-ellenőrzési táblázat megmaradnak.
Javítás készült a bridge-auditor mai register-only fejlécének kezelésére;
ez nem változtatja a DSP-modellt vagy M0/M1 készültségét. A vizsgálat és a
vezetési szabály a naplóban szerepel, a jelölt végső ellenőrzései a saját PR-ben.

### Dokumentációs egységesítés — 2026-10-27

[#37](https://github.com/RobCZart82/yaman1x/pull/37): az M0/M1 részletes állapota,
a lezárt PR-ek bizonyítékai és a korabeli pillanatképek értelmezése egységesítve.
Az ellenőrzés élő GitHub PR-/workflow-/jobadatokra és a fenti kódbázisra épült.
A módosítás csak dokumentáció; M0/M1 készültségét és az elfogadási feltételeket
nem változtatja meg. A dokumentációs PR saját CI-je a saját checkjeinél követhető.

### Ellenőrzött PR-lezárások — legutóbbi elöl

A #16–#36 PR-ek mind **beolvadtak**. Az alábbi SHA a végső tesztelt PR-head,
nem a squash után keletkezett `main` commit; a beolvasztás a PR hivatkozásánál
ellenőrizhető. Minden felsorolt alap-CI mindhárom platformjobja sikeres.
`—`: az adott PR végső revízióján nem volt ilyen PR-triggerelt workflow-futás;
nem FAIL és nem más revízió eredményének átvétele. A korábbi hibakereső vagy
köztes reprodukciós revíziók a korabeli naplóban megmaradnak.

| Beolvadt PR / eredmény | Végső tesztelt PR-revízió | Lezárt ROM-mentes ellenőrzések |
|---|---|---|
| [#36](https://github.com/RobCZart82/yaman1x/pull/36) — Közös aszinkron SCI-csatorna | `6f3fbf6c22b9f3bfc28f78d0fc1bad55ed98ed8c` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706565); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706683); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706500) |
| [#35](https://github.com/RobCZart82/yaman1x/pull/35) — Aszinkron RX | `eeceb8282e920e163c5ff649068b90952d753671` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506274); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506107); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506093) |
| [#34](https://github.com/RobCZart82/yaman1x/pull/34) — Aszinkron TX | `da5fcd3dd9301f06e68c489c879989661a3b3428` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811757); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811799); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811994) |
| [#33](https://github.com/RobCZart82/yaman1x/pull/33) — LICENSE szövegtördelése; státusz változatlan | `22cb57c8f1a617b6a9ddca90bad6fc8962671327` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40489564815); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40489564814); MAME: — |
| [#32](https://github.com/RobCZart82/yaman1x/pull/32) — Licenclefedettség-ellenőrzés | `fc6f4ed47774ba2b1a17394255a1427e2e7249fb` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40488326442); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40488326433); MAME: — |
| [#31](https://github.com/RobCZart82/yaman1x/pull/31) — Cserélhető PKS-megvalósítás követelménye | `41ff1d08c834632417efb4c6bbdca2a5c3754c24` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484817843); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484817846); MAME: — |
| [#30](https://github.com/RobCZart82/yaman1x/pull/30) — PKS ötletfeljegyzés | `955dd73afd3760df1535de60a94503c016161925` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484348640); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484348645); MAME: — |
| [#29](https://github.com/RobCZart82/yaman1x/pull/29) — SCI SSR-latch | `b169bbe9b3e995328b75d6190f7458138c154722` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540061); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540084); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540083) |
| [#28](https://github.com/RobCZart82/yaman1x/pull/28) — SCI ciklusóra | `747cd98fac4ae300216f0bd5021dbd0cc2eae6ea` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835302); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835203); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835254) |
| [#27](https://github.com/RobCZart82/yaman1x/pull/27) — IRQ-regiszter/láb adapter | `d33a27bdef963ddc8b1385d87fa324878af51f89` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786571); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786580); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786569) |
| [#26](https://github.com/RobCZart82/yaman1x/pull/26) — Belső IRQ-választó | `d5d80e49f4664ce00f5df33d2d3408e74c2aa64a` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767512); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767565); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767326) |
| [#25](https://github.com/RobCZart82/yaman1x/pull/25) — H8 CI-bizonyíték dokumentálása | `a917c9779f64ce5afacae5fa2fc615d194c8abaa` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40469158573); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40469158626); MAME: — |
| [#24](https://github.com/RobCZart82/yaman1x/pull/24) — H8-leltár, generátor és GPIO | `896bd7882309bb03868c5c3816f4f9b7b068b858` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442) |
| [#23](https://github.com/RobCZart82/yaman1x/pull/23) — Mac párosított próba előkészítése | `e0ad4df4884af8d0926cd68e451ebcbc280ed4d0` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40463469779); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40463469732); MAME: — |
| [#22](https://github.com/RobCZart82/yaman1x/pull/22) — VOP3 konstruktorjavítás | `d363d6a530f03aef5e6167759357874d7b4650b0` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40461780030); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40461779887); MAME: — |
| [#21](https://github.com/RobCZart82/yaman1x/pull/21) — Kutatási buildbizonyíték dokumentálása | `cfb87b521f601daff01c15befb09af88066239e4` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40459057121); MSan: —; MAME: — |
| [#20](https://github.com/RobCZart82/yaman1x/pull/20) — Rögzített kutatási forrás és build | `31664c74f2726091eb9f403c1621ef949479ddbb` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062); MSan: —; [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) |
| [#19](https://github.com/RobCZart82/yaman1x/pull/19) — SCI-hozzáférés-megfigyelő | `748d8495b1e75ed43bd69d456bfd519ffca85557` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40455618012); MSan: —; MAME: — |
| [#18](https://github.com/RobCZart82/yaman1x/pull/18) — Tervvezetési szabályok | `e503321ad31b2d0a36e0ae419215034cbf82b74f` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454887762); MSan: —; MAME: — |
| [#17](https://github.com/RobCZart82/yaman1x/pull/17) — ADR és függőségalap | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768); MSan: —; MAME: — |
| [#16](https://github.com/RobCZart82/yaman1x/pull/16) — Python -O / AsmJit javítások | `ac56d8c7144ef8cb478f3a714578203c440b841e` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792); MSan: —; MAME: — |

A zöld CI a forrás-/komponenstesztek és a jelzett kutatási build eredménye.
Nem igazol teljes H8-portot, friss firmware-es MIDI-vételt, hardverazonosságot,
audio- vagy REAPER-elfogadást, és nem jelent jogi/licenc- vagy bináris kiadási
jóváhagyást. A csak dokumentációs PR-ek sem új működési bizonyítékok.

### Korabeli munkanapló — PR-sorrendben

Az alábbi sorok az adott fejlesztési lépés **akkori** állapotát és akkori
következő feladatát rögzítik. A `NOT RUN`, „PR megnyitva”, „platform-CI a PR-ben”
és a még külön TX/RX modellekre utaló mondatok itt történeti állapotok.
Platformlezáráshoz a fenti táblát, jelenlegi feladathoz az aktuális M0/M1 részt
kell olvasni. A firmware-/hardver-/audio-elfogadás hiányát nem írja felül az,
hogy egy korábban még nem futott ROM-mentes platformteszt azóta sikeres lett.
A helyi szintetikus és upstream-összehasonlító mérések eredeti számai megmaradnak.

| Dátum | Korabeli változás és állapot | Akkori ellenőrzés / bizonyíték | Akkori következő lépés |
|---|---|---|---|
| 2026-10-27 | **Korabeli állapot:** [#16](https://github.com/RobCZart82/yaman1x/pull/16), beolvadt: Python `-O` mellett is aktív ABI-ellenőrzések, AsmJit tesztforrások kizárása, harmadik fél patch-darabszám javítása | `ac56d8c7144ef8cb478f3a714578203c440b841e`: Linux x64, macOS arm64, Windows x64 CI 11/11 tesztcsoport PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792). Helyi Windows: build PASS, CTest 10/11; egy meglévő symlinkteszt WinError 1314 miatt akadályozott. [Auditkövetés](AUDIT_FOLLOWUP_2026-10-27.md). | A VOP3 inicializálás/`memcmp` kockázat célzott vizsgálata; EG target-hold változtatás csak hardveres bizonyíték után. |
| 2026-10-27 | **Korabeli állapot:** [#17](https://github.com/RobCZart82/yaman1x/pull/17), beolvadt: [ADR-0001](ADR-0001-engine-boundary.md), függőségazonosítók és [buildalap](BUILD_BASELINE.md). **M0 részben teljesült.** | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1`: mindhárom platform CI PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768). Helyi CMake-konfiguráció és forrásaudit PASS, 201 változatlan upstream fájl ellenőrizve. | H8-port függőség- és licenclezárása, példányizolációs/időléptetési próbák, teljes referencia-toolchain rögzítése; saját terjesztési licenc döntése. M1 a külön MAME-kutatási úton folytatható. |
| 2026-10-27 | **Korabeli állapot:** [#19](https://github.com/RobCZart82/yaman1x/pull/19): **M1 folyamatban:** időbélyeges SCI-hozzáférés-megfigyelő és naplóellenőrző elkészült; firmware-es elfogadás még nem futott. [Részletek](m1-sci-access-milestone.md). | Natív megfigyelő és célzott Python-tesztek Windows x64-en PASS. A teljes Python-csomag meglévő symlinktesztjét WinError 1314 akadályozza. Tizenegy patch alkalmazása a rögzített forrásra PASS; teljes új MAME-build és friss firmware-próba NOT RUN. | Külön macOS kutatási build, SCI-beállítások és rövid engedélyezési ablakok rögzítése, majd a PC-k követése privát disassemblyvel. PKS-válasz továbbra sem feltételezhető. |
| 2026-10-27 | **Korabeli állapot:** [#20](https://github.com/RobCZart82/yaman1x/pull/20), beolvadt: **M0/M1 folyamatban.** Upstream forrásazonosítók visszanyerése, ismételhető patch-előellenőrző és teljes macOS MAME-build CI. [Bizonyíték és határok](source-identity-recovery.md#verified-result). | `31664c74f2726091eb9f403c1621ef949479ddbb`: [Linux/macOS/Windows alapcsomag](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062) 12/12 csoport PASS platformonként; [teljes MAME-build és arm64/regisztrációs próba](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) PASS Xcode 16.2-vel. 11 patch, 4 egyező projektfejléc; firmware/SCI/MIDI-elfogadás NOT RUN. A kezdeti fordító- és Windows-sorvéghiba dokumentálva. | Friss, helyi firmware-felvétel és SCI-vezérlésiút-elemzés. A teljes kutatási build előfeltétele teljesült; H8-port és példányizoláció továbbra is nyitott. |
| 2026-10-27 | **Korabeli állapot:** [#22](https://github.com/RobCZart82/yaman1x/pull/22): **M0 alapstabilitás, bizonyított konstruktorhiba javítása.** VOP3 `ops` inicializálása az első dekódolás előtt; [bizonyíték és visszafordítható provenance](vop3-initial-decode-fix.md). | `44d5e8c8d2708e6736ce88e8cbe48be67a7b314c`: az eredeti fejléc inicializálatlan `memcmp` olvasását a [MemorySanitizer reprodukálta](https://github.com/RobCZart82/yaman1x/actions/runs/40461084899), az inicializált jelölt tiszta. Natív konstrukció/dekódolás és provenance-regressziók hozzáadva; végső platform- és sanitizer-eredmények a PR checkjeinél. | H8-port és friss helyi firmware/SCI/MIDI-elfogadás továbbra is nyitott. Az EG target-hold változatlan, hardveres bizonyíték szükséges. |
| 2026-10-27 | **Korabeli állapot:** [#23](https://github.com/RobCZart82/yaman1x/pull/23): **M1 Mac-folytatás előkészítve.** Generált két másodperces MIDI-hang és két friss, egyetlen státuszprofilban eltérő firmware-megfigyelés futtatója; [Mac M1 átadás](MAC_M1_HANDOFF.md). | Szintetikus MIDI-időzítés, párosítási és hibakezelési próbák helyben PASS; forrásaudit PASS. A végső platform- és sanitizer-eredmények a PR checkjeinél. Valódi firmware/SCI/MIDI-futás NOT RUN. | A Macen eszköz- és forrásellenőrzés, build, majd a helyi párosított próba. A befejezett suite nem jelent automatikusan igazolt MIDI-feldolgozást vagy bootot. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 H8-port előkészítés:** 30 fájlos, hash-elt include/implementáció leltár és generált/keretrendszer-határok. [Részletek](h8-port-source-boundary.md). | A rögzített archívum read-only feldolgozása PASS; ciklus-, párosítás-, hiányzó forrás-, hash- és útvonal-regressziók PASS. Ez nem teljes fordítói függőségzárás vagy licenclezárás; beágyazott H8-build NOT RUN. | Generátor/lista/buildszabály rögzítése, framework-szolgáltatások feltárása és minimális hostadapter; a Macen külön friss firmware-próba. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 H8-generátor előkészítés:** három buildbemenet és négy generált fájl hash-rögzítése; [ismételt generálási próba](h8-port-source-boundary.md#repeated-generation-experiment). | Windows Python 3.13.3: négy kimenet két független generálása PASS; bemenetazonosság- és kimenettartalom-regressziók PASS; forrásaudit PASS. Ág: `feat/h8-source-inventory`; PR megnyitva; végső platform-CI a #24 checkjeinél. | Keretrendszer-szolgáltatások és generált kód függőségeinek feltárása, minimális hostadapter és fordítás. CPU/firmware-végrehajtás nem történt. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 első önálló H8-periféria:** MAME-független digitális portmodell; [forrásazonosság, határok és teszt](h8-digital-port.md). | MSVC 19.44 Release build és natív teszt PASS: 786432 DDR/adat/maszk/forced-input kombináció négy bemeneti mintával; értesítéssorrend, reset és példányizoláció PASS. Platform-CI, firmware-integráció és hardverpróba NOT RUN; ág: `feat/h8-source-inventory`. | PR és háromplatformos ellenőrzés; utána a SCI/IRQ/timer időzítési és hostszolgáltatás-határának kialakítása. A CPU-port még nincs kész. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): digitális port helyi upstream összehasonlítása. | A rögzített MAME 11 eredeti regiszter/reset metódustörzsével 600000 determinisztikus művelet: **0 eltérés**, MSVC Release. [Próba határai](h8-digital-port.md#validation). | Végső platform-CI és teljes kutatási build ellenőrzése. Teljes MAME-eszköz-, CPU- és hardvervalidáció továbbra sem történt. |
| 2026-10-27 | **Korabeli állapot:** **#24 beolvadt, H8-periféria platformellenőrzés lezárva.** [Végső bizonyíték](h8-digital-port.md). | `896bd7882309bb03868c5c3816f4f9b7b068b858`: [Linux/macOS/Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477), [MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462) és [teljes kutatási MAME-build](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442) PASS. A korábbi NOT RUN platformbejegyzések történeti állapotok. | SCI/IRQ/timer hostadapter és CPU-leválasztás. Az új GPIO firmware-integrációja és a friss Mac-firmware-próba továbbra is NOT RUN. M0/M1 még nem teljesült. |
| 2026-10-27 | **Korabeli állapot:** [#26](https://github.com/RobCZart82/yaman1x/pull/26): **M0 H8H belső IRQ-választó elkészült külön komponensként.** [Szemantika és hiányzó adapter](h8-internal-irqs.md). | Helyi MSVC Release build/teszt PASS: 11 prioritáscsoport és 44 belső vektor, 990 vektorpár, DMA-elfogyasztás, szűrők, NMI-latch, reset és izoláció. Platform-CI a külön PR-ben; CPU-/SCI-/firmware-integráció NOT RUN. | Teljes IRQ-adapter, külső lábak és CPU-szűrők bekötése; időzített SCI-modell és friss Mac-firmware-próba. Az NMI-élt és időfeltételt a tulajdonosnak kell igazolnia. |
| 2026-10-27 | **Korabeli állapot:** [#27](https://github.com/RobCZart82/yaman1x/pull/27): **M0 H8H IRQ-regiszter/láb adapter elkészült.** [Szemantika, ICR-kockázat és inbox-értékelés](h8h-irq-controller.md). | Helyi MSVC Release tesztek PASS; az eredeti upstream metódustörzsekkel 300000 determinisztikus művelet, **0 eltérés**. A PIN/NMI/IER/ISR/ISCR/ICR és publikációs határok megőrizve; CPU/SCI/firmware-bekötés NOT RUN. | Platform-CI és kutatási build; utána időzített SCI- és CPU-adapter. Az új PKS-HLE/dummy-ROM jegyzet nem tekinthető protokollbizonyítéknak; feltételezett választ nem vezetünk be. |
| 2026-10-27 | **Korabeli állapot:** [#28](https://github.com/RobCZart82/yaman1x/pull/28): **M0 SCI belső ciklusóra-adapter.** [Részletes szerződés, tesztek és következő lépés](h8-sci-cycle-clock.md). | Helyi MSVC Release próba: 1024 osztó, TX/RX ciklusnézet, blokkfelosztás, standby és hibahatárok PASS. Eredeti upstream ütemezési metódusokkal 200000 determinisztikus művelet, **0 eltérés**. Végső platform-CI a külön PR-ben; bájtállapotgép és CPU-/firmware-integráció NOT RUN. | SCI TX/RX és SSR/RDR mellékhatások, IRQ-kapcsolat, majd közös gépütemező és tényleges H8-végrehajtás. Külső SCI-órás módok még nem támogatottak. |
| 2026-10-27 | **Korabeli állapot:** [#29](https://github.com/RobCZart82/yaman1x/pull/29): **M0 részben teljesült:** önálló [SCI SSR státusz-latch](h8-sci-status.md), olvasás utáni törlés, debugger/DMA mellékhatások és upstream reset-viselkedés. A teljes UART még nincs bekötve. | Windows MSVC Release: 33 554 432 átmenet független bitenkénti orákulummal PASS; külön szekvenciatesztek PASS. A végső revízió és platformeredmények a kapcsolódó PR checkjeinél. Firmware/MIDI/hardver: NOT RUN. | SCI TX/RX bájtállapotgép, TDR/RDR és megszakítások; az SSR-write utáni tx_start/sync_rx_start tulajdonosi lépések és ciklusóra összekötése, majd CPU-bekötés. |
| 2026-10-27 | **Korabeli állapot:** [#30](https://github.com/RobCZart82/yaman1x/pull/30): **Ötletfeljegyzés:** PKS HLE, fokozatos protokollbizonyítás, dummy ROM korlátai és távlati hardveres helyettesítés lehetőségei rögzítve az M1 alatt. | Dokumentációs értékelés; új működési vagy hardveres bizonyíték nincs, M0/M1 állapota változatlan. | Firmware-elvárások és tényleges kommunikáció bizonyítása; csak utána döntés egy külön tesztelhető PKS-modellről. |
| 2026-10-27 | **Korabeli állapot:** [#31](https://github.com/RobCZart82/yaman1x/pull/31): **Architekturális követelmény:** cserélhető PKS HLE/firmware megvalósítás, közös időzített perifériafelület és verziózott mentett állapot; áttérési út az M1 alatt és ADR-0001-ben. | Dokumentációs döntés; nincs megvalósított backend vagy új dump, M0/M1 változatlan. | A bizonyított protokoll alapján interfész és szintetikus tesztek; dump esetén MCU-integráció és összehasonlító elfogadás. |
| 2026-10-27 | **Korabeli állapot:** [#32](https://github.com/RobCZart82/yaman1x/pull/32): **M0 licencnyilvántartás megerősítve:** javított harmadikfél-összesítő; 219 fájl, 5 licencszöveg nyilvántartása és CTest-ellenőrzése. [Hatókör és nyitott feltételek](../THIRD_PARTY.md#reviewed-sourcenotice-coverage). | Helyi Windows: 7 célzott pozitív/negatív teszt PASS; forrás-/értesítés-lefedettség PASS. A végső revízió és háromplatformos eredmények a kapcsolódó PR checkjeinél. Ez nem jogi vagy bináris kiadási jóváhagyás. | Új átvételnél kötelező nyilvántartás és licencvizsgálat; saját projektlicenc döntése, teljes H8/SDK függőségzár és tényleges kiadási csomag értesítéseinek ellenőrzése továbbra is nyitott. |
| 2026-10-27 | **Korabeli állapot:** [#34](https://github.com/RobCZart82/yaman1x/pull/34): **M0 részben teljesült:** [aszinkron SCI TX](h8-sci-async-tx.md), TDR/puffer, SSR/ciklusóra összekötése, adási vonal és TXI/TEI események. RX/CPU még nincs bekötve. | Windows MSVC Release: 4096 adat/keretezés kombináció és célzott tulajdonosi szekvenciák PASS; eredeti upstream metódusokkal 200000 művelet, 82931 TX-tick, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. Firmware/hardver/audio NOT RUN. | Aszinkron RX/RDR és hibák, közös TX/RX óra/státusz, IRQ/CPU MMIO; utána friss firmware-es MIDI-elfogadás. Paritás upstream szerint megőrizve, nem igazolt hardvertény. |
| 2026-10-27 | **Korabeli állapot:** [#35](https://github.com/RobCZart82/yaman1x/pull/35): **M0 részben teljesült:** [aszinkron SCI RX](h8-sci-async-rx.md), vonalmintavétel, RDR és RXI/ERI kérések, túlcsordulás/paritás/keretezési hibák. Külön RX-tulajdonos; közös TX/RX és CPU még nincs bekötve. | Windows MSVC Release: 4096 adat/keretezés kombináció és célzott hibás szekvenciák PASS; eredeti upstream metódusokkal 896623 összehasonlítás, 663805 RX-tick, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. Firmware/hardver/audio NOT RUN. | Közös TX/RX regiszter/óra/státusz és azonos idejű események; IRQ/CPU MMIO; utána friss firmware-es MIDI-elfogadás. A külön TX/RX osztályok együtt még nem teljes SCI-csatorna. |
| 2026-10-27 | **Korabeli állapot:** [#36](https://github.com/RobCZart82/yaman1x/pull/36): **M0 részben teljesült:** [közös aszinkron SCI-csatorna](h8-sci-async-channel.md), egy regiszter/óra/SSR-tulajdonos TX/RX állapotgépekkel. Régi felületek kompatibilitási rétegként megmaradtak, a bitlogika közös. | Windows MSVC Release: 4096 teljes duplex adat/keretezés kombináció és célzott sorrend-/státusz-/izolációs tesztek PASS; eredeti upstream óra/TX/RX metódusokkal 1153468 összehasonlítás, 804316 közös él, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. CPU/firmware/hardver/audio NOT RUN. | Négy SCI IRQ-kérés útvonalazása és hatregiszteres MMIO-adapter, két SCI-csatorna tesztje; majd CPU/periféria-ütemező és friss firmware-es MIDI-elfogadás. |

## A cél pontos meghatározása

Az első cél egy macOS Apple Siliconon futó **VST3 hangszer**, amely REAPER-ben
betöltve, MIDI note-on hatására egy AN1x-hangszínt megszólaltat, note-off után
pedig annak elengedését végrehajtja. A hangot az eredeti, változatlan helyi
firmware által vezérelt DSP-program állítja elő. Egy scene és egy egyszerre
megszólaló hang elegendő az első bemutatóhoz. A szükséges chipek számát a
feltárt hangút határozza meg; a kétchipes működést nem tekintjük késznek.

A demonstrációhoz rögzített firmware-, hangszín- és konfigurációazonosító,
ismételhető indítás és helyi REAPER-hangfelvétel tartozik. A kísérleti státusz-,
EG- és órajelprofil minden eredmény mellett szerepel. Egy sikeres megszólalás
önmagában nem igazolja az AN1x teljes bootját vagy hangzásazonosságát.

Az első változathoz minimális beállítási lehetőség kell a helyi ROM-fájlok
kiválasztására és az indítás állapotának jelzésére. Az egyedi grafikus skin,
AU, Windows, teljes polifónia, bankböngésző és MEG-effektek későbbi feladatok.

## Jelenlegi helyzet és kutatási előzmények

A ROM-mentes motoralap aktuális állapota a dokumentum eleji #136 motorlezárásban
és az alábbi M0-szakaszban szerepel. A korábbi táblázat és a #36 lezárása történeti adat. Az alábbi firmware-/DSP-megfigyelések a hivatkozott kutatási
jegyzőkönyvek korábbi felvételei; a dokumentáció egységesítésekor nem futtattunk
új firmware-t vagy hardvermérést. Nem tekinthetők a közös SCI-adapter friss
firmware-integrációs bizonyítékának.

| Terület | Bizonyított eredmény | Nyitott feladat |
|---|---|---|
| H8 és firmware | A kutatási MAME-target futtatja a helyi 1.04 ROM-ot; kísérleti státuszprofil mellett mindkét DSP programját feltölti | Megismételhető indulás a hangszín- és MIDI-kezeléshez szükséges állapotig |
| MIDI és PKS | RXD0 a MIDI bemenet; a jel eléri a modellt. SCI1-en 30 TDR-írás és nulla RDR-olvasás látszik a párosított próbákban | A PKS-protokoll és az SCI0 bekapcsolásának vezérlési útja; tényleges MIDI-bájtvétel |
| DSP és időzítés | Két VOP3-modell és mintaléptetés működik; a feltöltött mezők auditálhatók | Órajel, EG-viselkedés, feltöltés alatti futás, táblarom-hozzáférés és soros hangút |
| Táblarom | Külső 128 KiB-os kép egyes táblái egyeznek a CPU-ROM-ban lévő másolattal | Indexskála, modellbe kötés és a második panel ROM-jának azonossága |
| Hang és plugin | Szintetikus DSP-soros próba és ROM-mentes CI van | AN1x offline hang, termékmotor, VST3 és REAPER-próba |

Részletek: [STATUS](STATUS.md), [SCI-megfigyelések](sci-state-milestone.md),
[táblarom és órajel](an1x-clock-and-table-rom.md),
[időzített DSP-kísérlet](timed-vop3-experiment.md).

## Mérföldkövek és elfogadási feltételek

### M0 — Rögzített fejlesztési alap és motorarchitektúra

2026-10-30: **részben teljesült**. A következő alapok elkészültek:

- Az [ADR-0001](ADR-0001-engine-boundary.md) kiválasztotta a motorhatárt, a
  külön MAME kutatási utat és a tervezett VST3 SDK-t. Az architekturális választás
  rögzített; a CPU és több periféria szintetikus integrációja elkészült,
  a teljes gép és a firmware-es indulás elfogadása még hátravan.
- A [függőségazonosítók](engine-dependencies.json), a forráslockok, a H8-leltár
  és a generátor bemenetei/kimenetei rögzítettek. A teljes fordítói/licenc-
  függőségzár ezzel még nincs lezárva.
- A [buildalap](BUILD_BASELINE.md) dokumentálja a helyi Windows referencia-
  környezetet és a külön teljes macOS arm64 MAME-build rögzített adatait.
  A kutatási build előfeltétele a #20 óta teljesült, a #36-nál is PASS.
  A hosztolt CI-runner verziói ettől nem válnak állandó toolchain-lockká.
- Önálló GPIO, IRQ-adapter és [közös aszinkron SCI TX/RX-csatorna](h8-sci-async-channel.md)
  áll rendelkezésre. Komponensszintű időléptetési/izolációs tesztjeik sikeresek;
  ez még nem teljes CPU-/gépizolációs vagy firmware-elfogadás.
- A forráseredet és a [licenc-/értesítés-lefedettség](../THIRD_PARTY.md#reviewed-sourcenotice-coverage)
  ellenőrzése automatizált. A saját projektlicenc és a tényleges kiadási csomag
  megfelelőségének ellenőrzése továbbra is nyitott.

A [kétcsatornás SCI MMIO/IRQ-adapter](h83002-sci-mmio-irq.md) szintén elkészült: bájt-/szóhozzáférés, DMA-elsőbbség és csatornánkénti vektorkiosztás.

Az önálló SCI-idővonal (#41), CPU-hoszthatár (#42), buszsegédek (#43),
cikluskeret (#44) és [végrehajtási sorrend](h8-execution-runner.md) elkészült.
A [beolvadt H8H-részhalmaz](h8-first-instructions.md) már fordul és eredeti
full/partial kezelőket hajt végre. A #57-kori 366 kezelő támogatott műveletei (történeti részhalmaz):
memória és prefixek, rel8/rel16 feltételek, kiválasztott aritmetika/logika,
egybites shift/rotate, regiszteres/indirekt/abszolút bitműveletek, normál
[veremkezelés és vezérlésátadás](h8-stack-control-flow.md). Ez részhalmaz,
nem teljes CPU. A korábbi bővítések és mérések az előrehaladási naplóban maradnak.

Nyitott feladatok, végrehajtási sorrendben:

1. A 264 kiválasztott H8H-kódolássor kezelői már bekerültek, a kiválasztott
   leltárban 0 hiányzó sor van (135 kizárt sor külön hatókör). Ez nem teljes
   CPU- vagy hardverelfogadás. A közös ROM/RAM/SCI/GPIO/IRQ-busz szintetikus
   integrációja elkészült. A közös korlátozott SYSCR-módok és a támogatott részreset
   már teszteltek. Következő: az ITU önálló számláló/esemény/deadline/dispatch
   komponenseinek aktív gépütemező/MMIO- és CPU IRQ-bekötése, majd friss helyi
   firmware-diagnosztika. Teljes hardver-reset, további perifériák és a teljes
   címtér/időzítés igazolása továbbra is nyitott.
2. A teljes gép CPU-/RAM-/periféria-/DSP-/MIDI-állapotának példányizolációját,
   resetjét és blokkfelosztástól független időléptetését igazolni.
3. Az újonnan ténylegesen beépített függőségek licenceit lezárni; a saját kód
   licencéről a bináris terjesztés előtt dönteni. A kiválasztott SDK tényleges
   integrációját és összekapcsolt részeit akkor ellenőrizni; az SDK még tervezett.
4. Megőrizni a ROM-mentes alapbuildet és a külön kutatási utat; a beágyazott
   motorhoz saját, revízióhoz kötött referencia-buildet rögzíteni.

**Kész, ha:** van rövid architektúradöntés, függőséglista és megismételhető
buildutasítás. A kiválasztott motor CPU-, periféria-, DSP- és MIDI-állapota
példányonként elkülöníthető, időléptetése külső audiofolyamhoz illeszthető.

### M1 — Indulási út, PKS és valódi MIDI-vétel

2026-10-27: **előkészítve, nem teljesült**. Elkészült az
[időbélyeges SCI-hozzáférés-megfigyelő](m1-sci-access-milestone.md), a naplóellenőrzés
és a [Mac M1 párosított helyi próba futtatója](MAC_M1_HANDOFF.md). Ezek szintetikus
szervezési/hibakezelési tesztjei sikeresek; nem friss firmware-es elfogadások.

**Teljesült előfeltétel:** a [rögzített forrású teljes kutatási build](source-identity-recovery.md)
#20 óta sikeres, és a #36 külön macOS buildje/regisztrációs próbája is PASS.
A build sikere önmagában nem bizonyít tényleges SCI-bájtvételt.
A közös SCI-modell szintetikus CPU-/buszintegrációja már az M0 része. Ez nem
bizonyítja a valódi firmware MIDI-bájtfogadását; az M1 elfogadása továbbra is nyitott.

**Következő elfogadási lépés:** a Mac M1 átadási útmutató szerint friss helyi
firmware-futást és párosított megfigyelést készíteni, majd a tényleges SCI0
bájtfogadást és a note-on/note-off feldolgozását bizonyítani. A friss elfogadáshoz
nincs rögzített eredmény; dokumentált státusza **NOT RUN**. A PKS-kézfogás és az
SCI0 engedélyezésének okozati útja továbbra is kutatási feladat; a ROM-mentes CI
nem váltja ki.

- Visszakövetni a firmware SCI1/PKS-inicializálását és az SCI0 engedélyezését.
  A mostani megfigyelés alapján a PKS-válasz hiánya lehetséges akadály, de
  okozati összefüggését még igazolni kell.
- Megállapítani a szükséges PKS-kézfogást firmware-elemzéssel és elérhető
  hardveres bizonyítékokkal. Ha a hiányzó ROM helyett HLE szükséges, annak
  minden válaszát, állapotát és időzítési feltételezését dokumentálni.
- Ellenőrizni az SCI0 konfigurációját, a beérkezett bájtokat és a firmware
  eseménykezelőjéig vezető utat; az RXD-vonalátmenetek önmagukban nem elegendők.
- Rögzíteni egy determinisztikus induló hangszínt és rendszerállapotot.
  Az üres NVRAM, gyári inicializálás és normál indulás útját külön tisztázni.

**Kész, ha:** friss indulás után a firmware bizonyíthatóan fogad egy ismert
note-on/note-off bájtsorozatot és annak megfelelő voice/EG/regiszterváltozást
végez. A próba megismételhető, a használt perifériaprofilok rögzítettek.

**Ha elakad:** a hiányzó protokollt vagy hardvermérést konkrét feladatként
rögzíteni. A további offline DSP-kutatás folytatható, de egy közvetlenül
feltöltött DSP-kép megszólaltatása még nem teljesíti a firmware-es MIDI-célt.

#### Kutatási ötletek: a hiányzó PKS-program lehetséges helyettesítése

Feljegyezve: 2026-10-27, az `AN1x_DUMP-hiánypótlás.txt` ötleteinek
értékelése alapján. **Lehetőségek és hipotézisek; nem igazolt működés,
nem megvalósított funkciók és nem új elfogadási bizonyítékok.** M1 állapota
és elfogadási feltételei ettől nem változnak.

- **Minimális PKS HLE-modell lehetősége.** Elképzelhető, hogy az eredeti
  HD63B05-program futtatása helyett a fő CPU által megfigyelhető kommunikáció
  modellezése elegendő. Előbb azonosítani kell a kéréseket, válaszbájtokat,
  állapotátmeneteket, reset- és időzítési szabályokat. A PKS-válasz hiánya
  jelenleg csak lehetséges indulási akadály; az okozati kapcsolat nem igazolt.
- **Fokozatos bizonyítás.** Először firmware-elemzéssel követni a fő CPU
  elvárásait; lehetőség szerint működő hangszer privát buszmegfigyelésével
  ellenőrizni. Ezután kis, külön kapcsolható protokollmodellt és pozitív/negatív
  teszteket készíteni. Meg kell különböztetni a minimális indulási kézfogást
  a teljes kezelőpanel-viselkedéstől. A siker mércéje továbbra is a tényleges
  firmware-es bájtfogadás és eseményfeldolgozás, nem egy kényszerített státusz.
- **Dummy ROM csak betöltési kísérletként merülhet fel.** Egy 4096 bájtos
  üres fájl nem helyettesíti a programot vagy a protokollt. Csak elkülönített,
  egyértelműen szintetikus tesztadatként lenne értelme, ha konkrét betöltési
  kérdést vizsgálunk; nem nevezhető valódi dumpnak és nem bizonyít bootot.
  Előnyösebb lehet egy kifejezetten ROM nélküli HLE-mód, ha a protokoll már
  bizonyított. Jelenleg egyik megoldás bevezetéséről sincs döntés.
- **Fizikai mikrokontrolleres helyettesítés: külön távlati irány.** Csak
  bizonyított protokoll, elektromos illesztés és időzítés alapján mérlegelhető.
  A szervizrajz önmagában nem tárja fel a program működését; a javasolt Hitachi
  változatok láb-, programozási és működési kompatibilitása nincs igazolva.
  Ez nem előfeltétele a jelenlegi szoftveres fejlesztésnek.

**Továbbfejleszthetőségi követelmény — 2026-10-27.** Ha bizonyított HLE
áthidalást vezetünk be, azt cserélhető PKS-megvalósításként kell tervezni.
A későbbi eredeti dumpot futtató MCU-modell ugyanahhoz az időbélyeges
perifériafelülethez kapcsolódjon; a plugin és a fő H8 firmware ne függjön a
HLE belső megoldásaitól. Ez tervezési követelmény, nem elkészült funkció.
Részletek: [ADR-0001: cserélhető PKS-megvalósítás](ADR-0001-engine-boundary.md#replaceable-pks-implementation-and-future-dump-integration).

Tervezett áttérési út és elfogadási feltételek:

1. Bizonyított protokoll alapján rögzíteni a közös bemenet/kimenet, reset és
   időléptetés szerződését; szintetikus tesztekkel ellenőrizni az izolációt.
2. A HLE-t külön azonosított kísérleti profilként megtartani. A mentett állapot
   tartalmazza a megvalósítás/profil azonosítóját, sémaverzióját és a szükséges
   ROM tartalmi azonosítóját; Yamaha bájtokat vagy helyi abszolút utat ne.
3. Ha elérhető lesz a dump, ellenőrizni az eredetét/azonosítóját, a pontos MCU-t,
   memóriatérképet és perifériákat; csak ezután készíteni firmware-t futtató
   modellt. Az eredeti program megléte önmagában nem bizonyít hardverhűséget.
4. Azonos időzített ingerekkel összevetni a HLE-t és az új modellt; indulás,
   kezelőpanel, reset, firmware-es MIDI és audio elfogadását újrafuttatni.
   A bizonyíték alapján tudatosan kijelölni az új alapértelmezett profilt.
5. Váltáskor ellenőrzött újrainicializálást használni; futó audio közbeni csere
   nem követelmény. Régi HLE-állapotot tilos észrevétlenül MCU-állapotként
   betölteni. Hiányzó ROM/profil esetén egyértelmű jelzés szükséges; migráció
   csak külön dokumentált és tesztelt támogatásként ígérhető.

Korlátok: a jelenlegi LCD-buszmodell és SysEx-eszközök nem helyettesítik a
PKS-kézfogást. A PKS HLE ötlete a külön VOP3 ROM-ok hiányát nem oldja meg.
Nem vezetünk be feltételezett sikeres válaszokat vagy firmware-módosítást
pusztán az ötletjegyzet alapján. A hardveres bizonyítékhoz kötött EG target-hold
szabály változatlan. A feljegyzés nem teszi feladattá ROM, bank vagy privát
trace repository-ba másolását.

### M2 — Egy hangszínhez elegendő DSP- és táblarom-modell

- Külső bemenetként betölteni és azonosítani a szükséges táblaromot; rögzíteni
  az előjel-, szó- és címezési szabályokat. A DMS ROM azonosságát külön kezelni.
- Feltárni a választott hangszínhez szükséges indexskálát és táblahasználatot.
  A táblák tartalmi egyezése nem helyettesíti a címezés ellenőrzését.
- Megvizsgálni az EG indítását, célkeresztezését, tartását és elengedését,
  valamint a firmware által használt 0x21/0x22 visszaolvasásokat.
- Rendezni az upload alatti megállítás/újraindítás és a CPU–DSP eseménysorrend
  modelljét. A regiszterfeltöltés sikere nem jelenti ezek helyességét.
- Meghatározni az első demonstráció motoróráját. Ha csak feltételezett érték
  áll rendelkezésre, névvel jelölt kísérleti profilt használni, és ebből a
  megfelelő hangmagasságra vagy hardveres időzítésre nem következtetni.

**Kész, ha:** a kiválasztott voice determinisztikusan fut, az érintett DSP-állapotok
végesek, note-on és note-off hatására a szintek változnak. Minden még feltételezett
órajel-, EG- és címzési szabály szerepel a mérési jegyzőkönyvben.

### M3 — Első száraz hang offline rendereléssel

- Követni a voice kimeneti soros slotjait és az esetleges chipközi kapcsolatot.
  A szükséges útvonalat a rajz, a firmware és a DSP-program alapján azonosítani.
- Kialakítani egy megfigyelhető száraz kimeneti pontot. A MEG elhagyása csak
  dokumentált kutatási száraz út lehet; nem állítjuk róla, hogy a teljes gyári
  kimenet másolata. Ha a hanghoz további chip kötelező, azt be kell kötni.
- Meghatározni a szóformátumot, skálázást, jelkésést és a csatornák jelentését.
  Az upstream FS1R DAC-segédfüggvény AN1x-re való alkalmazását külön igazolni.
- Ugyanazon firmware/MIDI útvonalból helyi WAV-fájlt renderelni: kezdeti csend,
  note-on, tartott hang, note-off és lecsengés. Diagnosztikához hibás számértéknél
  némítani; kezdetben alacsony kimeneti szintet használni.

**Kész, ha:** a WAV-ban hallható, nemnulla, véges jel van; a hang indulása és
elengedése a MIDI-eseményekhez kötődik. Nincs tartós túlvezérlés vagy váratlan
egyenkomponens. Azonos bemenet ismételt renderelése egyezik az előre rögzített
numerikus tolerancián belül. A felvételhez profil-, ROM- és buildazonosító tartozik.

### M4 — Beágyazható, audio-blokkokkal működő motor

- Kialakítani az init, reset, időbélyeges MIDI-bemenet, render és állapotkezelés
  felületeit. A firmware indítása és a fájlbetöltés az audiofeldolgozáson kívül történjen.
- Egy idővonalra illeszteni a CPU-t, perifériákat és DSP-ket. A DAW-események
  blokkon belüli helyét megőrizni; a modellezett soros átvitel késleltetését is kezelni.
- A belső motoróra és a hoszt mintavételi frekvenciája közé szükség esetén
  mintavétel-konverziót építeni; a hoszt frekvenciája ne írja át észrevétlenül a DSP-modellt.
- Előre lefoglalni a szükséges memóriát. Az audioútból eltávolítani a fájlműveletet,
  dinamikus foglalást, blokkoló várakozást és korlátlan naplózást.
- Kezelni az all-notes-off, reset, leállítás és újraindítás eseteit, valamint a
  MIDI-eseménysor túlcsordulását dokumentált hibaviselkedéssel.

**Kész, ha:** a beágyazott motor az M3 referenciafolyamát különböző blokkfelosztásokkal
is reprodukálja. Az első célbeállításon, például 48 kHz/256 mintás hosztblokkal,
tartósan az audiohatáridőn belül fut. A mért idő és tartalék dokumentált;
44,1 kHz és más blokkméretek támogatását külön ellenőrizzük.

### M5 — Minimális VST3 hangszer

- A motorhoz VST3 adaptert készíteni a rögzített SDK/keretrendszer-verzióval;
  audio-kimenetet, MIDI-eseményfogadást és példányonkénti állapotot biztosítani.
- GUI nélkül, hosztparaméterrel vagy helyi konfigurációval biztosítani a ROM-kiválasztást, indítási állapotot és hangerőt.
  A hiányzó/ismeretlen ROM kezelése legyen egyértelmű, a plugin maradjon némítható.
- A hoszt pluginkeresése ne indítson hosszan futó firmware-próbát. A motor
  elkészültéig a feldolgozás determinisztikus csendet adjon.
- Projektmentéshez tárolni a szükséges profilokat, hangszínállapotot és
  ROM-azonosítókat. Visszatöltéskor egyező helyi fájlokat keresni/kérni;
  a ROM-tartalom ne kerüljön a pluginba vagy a projektállapotba.
- Megépíteni a REAPER-rel egyező architektúrájú helyi `.vst3` csomagot,
  ellenőrizni a formátum validátorával, és előkészíteni a helyi telepítést.

**Kész, ha:** a VST3 betölthető, MIDI-t fogad és ugyanazt a referenciahangot adja,
mint a motor önálló renderelése. A projekt mentése/újranyitása megőrzi a szükséges
beállításokat; hiányzó ROM esetén érthető hibajelzés és csend az eredmény.

### M6 — Az első REAPER-ben megszólaló hang igazolása

1. Rögzíteni a macOS-, gép-, REAPER-, pluginbuild- és architektúra-adatokat,
   az audioeszközt, frekvenciát, blokkméretet, ROM-hasheket és a választott voice-t.
2. A helyi VST3-at felismertetni, üres projekt hangszer-sávjára betölteni,
   a helyi ROM-okat kiválasztani és megvárni az induláskész állapotot.
3. Beállítani a sáv audioútját a master kimenetre. Rövid MIDI-itemmel indítani
   egy ismert hangot, például a 60-as MIDI-hangszámot, majd note-offot küldeni.
4. Ellenőrizni a hangot lehallgatással és mérővel: indulás, tartás, elengedés,
   ismétlés és leállítás. A note-off/all-notes-off ne hagyjon beragadt hangot.
5. A REAPER-kimenetet helyi WAV-ba menteni, összevetni az offline referenciával,
   majd a projektet újranyitva megismételni a próbát. A különbségekhez rögzített
   késleltetés- és numerikus tolerancia tartozzon.

**Kész, ha:** a fenti jegyzőkönyv és felvétel alapján a REAPER-ben a YamAN1x
firmware-vezérelt DSP-hangja hallható és reprodukálható. Rögzíteni kell, hogy
mely tulajdonságok kísérletiek. A ROM-mentes forrás és a helyi kutatási adatok
publikálási határa továbbra is megmarad.

## Végrehajtási sorrend és döntési pontok

Fő függőségi lánc: **M0 → M1 → M2 → M3 → M4 → M5 → M6**.
M1 közben az M2 táblarom-/órajel-kutatása önállóan haladhat. Egy ROM-mentes
VST3 szintetikus próbája később segítheti a hosztadapter ellenőrzését, de a
végcélt a firmware-vezérelt jelút teljesíti.

Minden mérföldkőhöz kis, önálló PR, reprodukciós parancsok, elvégzett próbák és
frissített státusz tartozik. A repository CI a ROM-mentes kódot ellenőrzi;
a helyi firmware-, audio- és REAPER-próbát külön jegyzőkönyv igazolja.

A legnagyobb bizonytalanság M1–M3: PKS-protokoll, DSP-modell, táblarom-címezés
és hangút. Ha ezekhez új hardveradat kell, annak pontos kérdését és beszerzési
lépését rögzíteni kell. Naptári kiadási dátumot az offline referenciahang
megszületése előtt nem vállalunk; M3 után a motor- és plugincsomagolás becsülhető.

## 2026-10-27 — Korlátozott ciklus-eseményfuttató

PR: [#40](https://github.com/RobCZart82/yaman1x/pull/40), ág: `feat/bounded-cycle-dispatcher`, alap: main #38; a nyitott #39-től független. [Komponensszerződés és tesztek](cycle-dispatcher.md): monoton ciklusidő, befoglaló határidő, folytatható lépéskeret, hibás tulajdonos elkülönítése. 1000 eseménysor blokk-/keretfüggetlensége és határesetei helyben PASS. Ez az ütemező alapja; CPU/periféria-tulajdonos, hardveres eseménysorrend és firmware-végrehajtás továbbra is nyitott. M0 részben teljesült.

## 2026-10-27 — SCI busz- és IRQ-integráció fejlesztési bejegyzés

PR: [#39](https://github.com/RobCZart82/yaman1x/pull/39), ág: `feat/h8-sci-mmio-irq`, alap: main #37. Két SCI-csatorna hatregiszteres MMIO-ja és 52–59 IRQ-vektorai bekötve a különálló IRQ-vezérlőbe. Helyi macOS arm64 Release: 23/23 tesztcsoport PASS; 512 RX bájt/csatorna eset, TX, DMA, hibák, reset és izoláció. ASan/UBSan PASS. Részletes [komponensjegyzőkönyv](h83002-sci-mmio-irq.md). Platform-CI a kapcsolódó PR-ben ellenőrizendő. M0 részben teljesült; CPU-/firmware-/MIDI-/audioelfogadás nincs. Következő lépés: közös CPU/periféria-ütemező és tényleges CPU-port.

### #40 szinkronizálása a beolvadt #39-cel

2026-10-27: a közös build-/naplófájlok ütközése feloldva, mindkét komponens és tesztcsoport megtartva. Az egyesített csomag 24 CTest-csoportot és 60 Python-esetet tartalmaz. A korábbi 23-as eredmények történeti bejegyzések; új PR-head CI szükséges. A CPU/periféria-tulajdonos és M0 lezárása továbbra is nyitott.

## 2026-10-27 — Közös kétcsatornás SCI-idővonal

PR: [#41](https://github.com/RobCZart82/yaman1x/pull/41), ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. [Komponensszerződés](sci-cycle-timeline.md): időbélyegzett RX-bemenet és két SCI-ciklusóra, korlátozott és folytatható eseményfeldolgozás. 256 kétcsatornás teljes duplex minta naplója azonos egyben, ciklusokra bontva és egyesével engedélyezett eseményekkel; 512 ismert RX-bájt ellenőrizve. Azonos ciklusban bemenetek forrássorrendben, majd SCI0/SCI1; szoftveres konvenció, nem hardveres bizonyíték. CPU/periféria-ütemezésből a CPU-rész és buszidőzítés továbbra is hiányzik; M0 részben teljesült.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.

## 2026-10-27 — H8 CPU gazdaszolgáltatás-határ

PR: [#42](https://github.com/RobCZart82/yaman1x/pull/42), ág: `feat/h8-host-services-boundary`, alap: beolvadt #41. [Részletes határ és végrehajtási sorrend](h8-host-services-boundary.md). Hash-ellenőrzött leltár 30 forrásból és négy kétszer generált kimenetből; két teljes futás bájtazonos. Memóriabusz, szóigazítás, ciklustartozás/részutasítás, IRQ/DMA, idő, életciklus, standby/mentés és diagnosztika követelményei rögzítve. Ez előkészítés: a CPU még nincs lefordítva vagy végrehajtva, M0 részben teljesült.

Local validation / helyi ellenőrzés: macOS arm64 Release, 25/25 CTest groups and 68 Python cases PASS; source/notice audits PASS. Final PR-head platform CI pending.

## 2026-10-27 — H8H CPU buszhozzáférési réteg

PR: [#43](https://github.com/RobCZart82/yaman1x/pull/43), ág: `feat/h8h-bus-access`, alap: beolvadt #42. [Komponensszerződés](h8h-bus-access.md): öt H8H buszsegéd, utasítás/adat út, szóigazítás és hozzáférés előtti kétciklusos levonás. Eredeti upstream metódustörzsekkel 200000 vegyes művelet, 0 eltérés; minden 16 bites szóérték és hiba-/izolációs határ tesztelve. Ez nem CPU-utasításvégrehajtás: a részutasítás-/keretkezelés, valódi címtér és IRQ/SCI CPU-bekötés nyitott. M0 részben teljesült.

Local validation / helyi ellenőrzés: macOS arm64 Release, 26/26 CTest groups and 72 Python cases PASS; new native ASan/UBSan PASS. Final PR-head platform CI remains separate evidence.

## 2026-10-27 — H8 végrehajtási cikluskeret

PR: [#44](https://github.com/RobCZart82/yaman1x/pull/44), ág: `feat/h8-execution-budget`, alap: beolvadt #43. [Komponensszerződés](h8-execution-budget.md): ciklustartozás továbbvitele, perifériahatáridő és explicit szeletfázisok. Eredeti execute_run módszerrel 200000 szintetikus, esemény nélküli szelet; recompute_bcount módszerrel 200000 határidő, 0 eltérés. Natív buszkapcsolat 1000 felosztási történettel. Ez számtani komponens; a valódi full/partial utasításkezelők, CPU-állapot és IRQ/SCI ütemezési kapcsolata még nyitott. M0 részben teljesült.

Local validation / helyi ellenőrzés: macOS arm64 Release, 27/27 CTest groups and 74 Python cases PASS; new native ASan/UBSan and source/notice audits PASS. Final PR-head CI pending.

2026-10-27 dokumentációs lezárás: #44 beolvadt, végső head mind az öt CI-je PASS. A motor legutóbbi helyi eredménye 27/27 CTest-csoport és 74 Python-eset PASS. CPU-opcode végrehajtás, M0 teljes lezárása és M1/audio továbbra is nyitott.

## 2026-10-28 — H8 teljes/részutasítás-futtatási sorrend

PR: [#46](https://github.com/RobCZart82/yaman1x/pull/46). Ág: `feat/h8-execution-order`, alap: beolvadt #45. A [futtatókeret](h8-execution-runner.md) összeköti a ciklustartozást, perifériahatáridőket és full/partial callback-sorrendet. 200000 eseményes referencia-szelet, nulla eltérés az eredeti execute_run metódussal szintetikus kezelők mellett. Natív sorrend-/hiba-/1000 felosztási teszt. M0 részben teljesült; valódi CPU-utasítások, IRQ-belépés, firmware/MIDI/audio nem igazolt. A jelölt saját platform-CI-je külön ellenőrizendő.

## 2026-10-28 — Első valódi generált H8-kezelők

PR: [#47](https://github.com/RobCZart82/yaman1x/pull/47). Ág: `feat/h8-first-generated-handlers`, alap: beolvadt #46. [Részletes bizonyíték és korlátok](h8-first-instructions.md). Saját regiszter-/prefetch-/részutasítás-állapot, eredeti generált reset/NOP/MOV.B/BRA kód, explicit nem támogatott opcode-hiba. 200000 szintetikus szakasz összevetve az eredeti H8H-dekóderrel és futtatóval, 899400 olvasás, nulla eltérés; szándékos hibát a kontroll észleli. M0 részben teljesült. Következő: adatmemória-műveletek, feltételes elágazások, IRQ-belépés és CPU/SCI-sorrend. A teljes CPU-/firmware-/MIDI-/audioelfogadás továbbra is nyitott.

Előző lépcső lezárása: [#46](https://github.com/RobCZart82/yaman1x/pull/46): merge `00e16ce805d542e4a654923ddde2d462ebac78bb`, végső head `478294b0570b0140b0ba2c5f78b65fcc99fecaa4`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467656), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467504), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467588).

## 2026-10-28 — H8H memória és rel8 feltételek

PR: [#48](https://github.com/RobCZart82/yaman1x/pull/48). Ág: `feat/h8-memory-conditional-branches`, alap: beolvadt #47. [Részletes tesztek és korlátok](h8-memory-branches.md). 44 eredeti generált full/partial metódus, háromszavas IR, teljes ER-címek és kétciklusos adatbusz-hozzáférés. 200000 eredeti referencia-szakasz, 844255 utasításolvasás, 27571 adatolvasás, 27574 adatírás: nulla eltérés. M0 részben teljesült. Következő: szó-/hosszúszó-műveletek, aritmetikai/összehasonlítási jelzőbitek, majd IRQ és CPU/SCI-sorrend. Teljes CPU-/firmware-/MIDI-/audioelfogadás továbbra is nyitott.

Előző lépcső lezárása: [#47](https://github.com/RobCZart82/yaman1x/pull/47): merge `343f4c32c81229e89ec092b0d8e324f2a0305f8a`, végső head `6f8107ec35e511c6aa9d347295e67a882a7826d3`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640131062), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130940), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130959).

## 2026-10-28 — H8H szó-/hosszúszó és prefixállapot

PR: [#49](https://github.com/RobCZart82/yaman1x/pull/49). Ág: `feat/h8-word-long-memory`, alap: beolvadt #48. [Részletes bizonyíték és korlátok](h8-word-long-memory.md). Eredeti 0100 prefix, külön belső dekódállapot, igazított word busz és kétlépcsős long átvitel. 200000 referencia-szakaszon nulla eltérés; mid-transfer folytatás/hiba és unsigned címkörbefordulás tesztelve. M0 részben teljesült. Következő: aritmetikai/összehasonlítási jelzőbitek, szélesebb utasításkészlet, majd IRQ és CPU/SCI-sorrend. Firmware/MIDI/audio-elfogadás változatlanul nyitott.

Előző lépcső lezárása: [#48](https://github.com/RobCZart82/yaman1x/pull/48): merge `800f8b4eabfe484b54e9918a652096c0acf50986`, végső head `a2da3e66797ec5d9f617e3e85b1e6505ccf26beb`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345253), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345183), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345179). Az automatikus review a beolvasztáskor még futott; nem lezárt eredményként szerepel. A beolvasztás a kézi forrásellenőrzés, eredeti kódos kontrollok és mind az öt sikeres CI alapján történt.

## 2026-10-28 — H8H aritmetika és összehasonlítás

PR: [#50](https://github.com/RobCZart82/yaman1x/pull/50). Ág: `feat/h8-arithmetic-flags`, alap: beolvadt #49. [Részletes bizonyíték és korlátok](h8-arithmetic-flags.md). Byte ADD/CMP immediate, byte ADD/SUB/CMP register, word/long ADD/SUB/CMP immediate; H/N/Z/V/C és CMP nem-írás ellenőrizve. 200000 eredeti referencia-szakasz, nulla eltérés; független signed-range/maradékos orákulum és half-carry negatív kontroll. M0 részben teljesült. Következő: további regiszteres/carry-input aritmetika, logika/shift, stack/IRQ és CPU/SCI-sorrend. Firmware/MIDI/audio-elfogadás változatlanul nyitott.

Előző lépcső lezárása: [#49](https://github.com/RobCZart82/yaman1x/pull/49): merge `4567d7bfe852100f5483a8104bcba81b50ae26bb`, végső head `f69503487de2fbbf3c575d81f54ca5a91e965f90`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971856), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971826), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971835). Az automatikus review a `5aff015d6157f166d9f26d0591e7b13d067cd171` forrásrevízión hibát nem jelzett; a követő commit csak PR-metaadatot rögzített a dokumentációban.

## 2026-10-28 — Carry, regiszteres aritmetika és logika

Ág: `feat/h8-carry-register-logic`, alap: beolvadt #50. [Részletes bizonyíték és korlátok](h8-carry-register-logic.md). ADDX/SUBX bemenő carry és sticky Z; wide register ADD/SUB/CMP; byte és wide immediate logika; LDC CCR. 200000 eredeti referencia-szakasz: nulla eltérés; több mint egymillió carry/Z és 20000 többbyte-os lánc matematikai kontrollal. M0 részben teljesült. Következő: wide register logika, shift/rotate és bitműveletek, majd stack/IRQ és CPU/SCI. Firmware/MIDI/audio-elfogadás továbbra is nyitott.

Előző lépcső lezárása: [#50](https://github.com/RobCZart82/yaman1x/pull/50): merge `0f8d3ae96eb32dcabb3a039f40e5c0fd0315a1b9`, végső head `4bbea3449f5d73f15031332b537919b493a5defc`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119562), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119555), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119545). Automatikus review a `ddc8d51221838e64c9e849101756c1aeca260dce` forrásrevízión: kész, nincs találat; a követő commit csak PR-metaadatot rögzített.

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

## 2026-10-28 — repository-audit a #58 főágon

[Auditjelentés](REPOSITORY_AUDIT_2026-10-28.md): a 39 csoportos/84 Python-esetes csomag Macen teljesen PASS, 200000 eredetimetódus-szelet eltérés nélkül, célzott CPU ASan/UBSan PASS. Az ellenőrzés nem igazol firmware-, hardver- vagy audioelfogadást; M0/M1 állapota változatlan. A következő feladatok a megmaradt utasítás/címzés-lefedettség, IRQ/TRAPA és a közös gépütemezés.

## 2026-10-28 — lefedettség és CCR

[Jegyzőkönyv](h8-coverage-and-ccr.md): forrásazonosított kódolásleltár (181 kiválasztott / 83 hiányzó / 135 másprofilú sor), STC/LDC regiszteres és ORC/XORC/ANDC immediát család. Ez nem ISA-készültségi százalék és nem IRQ-belépési bizonyíték. Következő a fennmaradó utasítás/címzéscsaládok, majd IRQ/TRAPA és gépütemezés.

## 2026-10-28 — regiszterközi MOV

[Jegyzőkönyv](h8-register-transfers.md): mindhárom szélesség, teljes lane/alias megőrzés és N/Z/V szemantika. A forrásleltárban 80 hiányzó H8H-sor marad. CPU IRQ/TRAPA és gépütemezés továbbra is nyitott.

## 2026-10-28 — INC/DEC és kis cím-aritmetika

[Jegyzőkönyv](h8-small-arithmetic.md): 16 csoport, flagmegőrzés és részutasítás-felosztás tesztelve. A leltárban 64 hiányzó H8H-kódolási sor marad. M0/M1 készültsége ettől nem teljes.

## 2026-10-28 — unáris műveletek és kiterjesztés

[Jegyzőkönyv](h8-unary-extension.md): tíz új csoport, 434 full/partial kezelő; 54 hiányzó H8H-sor marad. Az eredetimetódus-próbák új családjai elérhető ciklusban és bejárási őrfeltétellel futnak. A mérföldkő nem IRQ-, firmware- vagy hangelfogadás.

## 2026-10-28 — szinkron TRAPA-belépés

[Jegyzőkönyv](h8-trapa-entry.md): default SYSCR-profil, négy vektor, eredeti stack/busz/időzítés. Külső IRQ/NMI, szűrők és gépütemezés továbbra is nyitott; hardveres trap-visszatérés nincs igazolva.

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
