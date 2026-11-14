# First REAPER Sound Plan

For a Hungarian milestone summary, see [FIRST_REAPER_SOUND_PLAN_HUN.md](FIRST_REAPER_SOUND_PLAN_HUN.md). Historical Hungarian passages moved from this file are preserved in the [Hungarian notes archive](HUNGARIAN_NOTES_HUN.md).

## Current verified main — PR #292 — 2026-11-14

PR [#292](https://github.com/RobCZart82/YamAN1x/pull/292) merged by squash at
`2026-11-14T14:52:58Z` from exact head
`7ae2fccee343257a67acb7541d30f191d2f925fe` to main
`6039cc3833c35a278517e4541ed3d84fb527bb2b`. PR-head Linux, macOS and Windows
checks passed in ROM-free CI run `43603781411`; initial-decode MSan passed in
`43603781407`. On exact main, Linux, macOS and Windows passed in ROM-free CI
run `43607171460`, and initial-decode MSan passed in `43607171485`. The main
Ubuntu job completed its extended UBSan/ASan checks successfully. MAME research
was path-filtered for this documentation-only change.

Codex's exact-head review completed without findings (summary comment
`6878585874`; receipt `6878605101`). GitHub review submissions, inline review
comments and review threads were zero. Ruleset `25910947` required zero
approvals and resolved review threads; no quota error was reported and no
merge bypass was used. PR #292 reconciles PR #291's merge evidence and the
current main baseline. It adds no runtime behavior or hardware, boot, MIDI,
DSP, audio or REAPER acceptance.

## Fresh local bounded firmware observation — 2026-11-14

A read-only AN1x 1.04 probe using the merged main build reached its requested
20,000,000-cycle limit with no CPU failure or access fault. It recorded
3,184,647 CPU callbacks, peripheral cycle `20,000,000`, PC `247044`, MEG
controls/status reads `5/1`, VOP3 writes `[11557,516]`, reads `[448,0]`, sample
passes `[0,0]`, GPIO writes `6329`, and zero ADC samples. Both
`audio_accepted` and `firmware_boot_accepted` remained false. This is only
bounded progress under the selected synthetic profile; it is not complete boot,
hardware agreement, MIDI, audio or REAPER acceptance. The ROM SHA-256 before
and after was `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`;
it remains unchanged and outside Git. No new MAME build or run was made.

## PKS caller-input timing windows — local candidate — 2026-11-14

A bounded read-only observer now records the statically identified aligned
word `2349EA` in addition to the previously observed bases. Two fresh
nine-second passive/default-input probes on the pinned MAME source each
captured six `39AC` source-buffer writes. The `2349EA` read at post-fetch PC
`002A08` returned `4000/FF00` (`40` in the selected high byte) and preceded
the first three writes by 12,625, 20,125 and 27,625 ns; their byte values were
`E0 00 20`. The `237204` aligned-word read at post-fetch PC `001FD8` returned
`FF00/00FF` (selected low byte `00`) and preceded the next writes by 10,375,
17,875 and 25,375 ns; their values were `E0 00 00`. CCR snapshots are retained
as raw evidence without interpretation. Both run manifests matched on 26
non-trace fields. These timing windows do not establish causality, data meaning,
hardware agreement, complete boot, MIDI, DSP samples, audio or REAPER
acceptance. The ROM was unchanged and outside Git. The next candidate step is
to trace the upstream instruction/value path and independently recheck address,
value, lane, flags, timing and continuation.

## PR #279 merge verification — 2026-11-13

PR [#279](https://github.com/RobCZart82/YamAN1x/pull/279) merged at
2026-11-13T17:34:43Z from exact head
`bcdb21bada7c02912ebf8966fdadd99f08a25be5` to main
`40d7847ad23c05a1c11a52659dfbc4181dc0ebe2`. Linux, macOS, Windows, MSan and
MAME research checks passed on the PR head (jobs `132026604026`, `132026603408`,
`132026604067`, `132026603850`, `132026603857`) and on exact main
`40d7847` (jobs `132053535173`, `132053535251`, `132053534783`,
`132053535103`, `132053534152`). Codex's exact-head code review completed
without findings (issue comment `6859252684`); GitHub reviews and review threads
were zero. Active ruleset `25910947` requires zero approvals and resolved
threads; no quota error or bypass was involved.

The bounded probe now accepts the explicit eight-channel constant scan profile
and uses its AN3 entry for the preceding single conversion. Four read-only
100-million-cycle runs showed equal scan callback deadlines/counts across slice
sizes; AN3=0 versus 1023 changed the final CPU/GPIO trace, while VOP3 sample
passes remained zero. This is synthetic firmware progress only, not hardware,
boot, MIDI, audio or REAPER acceptance. ROM hash
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe` is
unchanged and outside Git; the MAME source remains pinned at
`706dbb73281320c54d7c2f16262c1c3786f2be01` with no new build/run.

## PR #280 merge verification — 2026-11-13

PR [#280](https://github.com/RobCZart82/YamAN1x/pull/280) merged at
2026-11-13T19:02:41Z from exact head
`8eb0eaaac1b7390a43f323f09ba0adfee792e436` to main
`284c0f1a273b6212272b80458eba7b6417e4dfee`. Linux, macOS, Windows, MSan
and MAME research checks passed on the PR head (jobs `132081161291`,
`132081161363`, `132081160837`, `132081161511`, `132081161303`) and exact main
(jobs `132102887643`, `132102887232`, `132102887927`, `132102887298`,
`132102887771`). Codex's exact-head review completed without findings (issue
comment `6861430543`); GitHub review objects and threads were zero. Active
ruleset `25910947` required zero approvals and resolved threads; no quota error
or bypass was involved.

The bounded probe accepts an explicit constant input for all eight ADC channels
and has a 250,000,000-cycle / 50,000,000-callback finite ceiling. Four fresh
read-only runs reached that ceiling with no bus fault, `audio_accepted:false`
and VOP3 sample passes `[0,0]`. AN0–AN3 and AN4–AN7 scan start deadlines were
stable across slice sizes. The six-vector 100-million-cycle sweep found that
only AN3 changed the trace among the tested zero, maximum and one-hot values.
This is synthetic firmware progress only, not hardware agreement, complete
boot, MIDI, audio or REAPER acceptance. The ROM SHA remains unchanged and
outside Git; no new MAME build/run was made.

## Bounded firmware probe — merged in PR #286 — 2026-11-14

PR #286 raises the explicit boot probe's finite ceilings to 600,000,000
cycles and 125,000,000 callbacks; the slice ceiling remains 1,000,000. Four
fresh read-only AN1x 1.04 runs (AN3=0/1023 by 4,096/1,000,000-cycle slices)
all reached the cycle limit with no access fault. The first AN3 conversion was
at cycle `95,706,795`; AN0–AN3 scanning began at `95,707,121` with subsequent
first deadlines `[95707387,95707643,95707899]`, and AN4–AN7 began at
`127,793,965` with subsequent deadlines `[127794231,127794487,127794743]`.
These deadlines matched across all four runs. Final PC was `62010` for AN3=0
and `1896` for AN3=1023. Callback totals ranged from `94,217,090` to
`94,511,131` and varied by slice size. Each run reported VOP3 sample passes
`[0,0]`, `audio_accepted:false`, and `firmware_boot_accepted:false`. The probe
remains bounded and fail-stop. This does not establish physical control
agreement, complete boot, MIDI, audio or REAPER acceptance. ROM SHA-256
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe` is
unchanged and outside Git; the pinned MAME source remains
`706dbb73281320c54d7c2f16262c1c3786f2be01`, with no new build/run.

## PR #277 merge verification — 2026-11-13

PR [#277](https://github.com/RobCZart82/YamAN1x/pull/277) merged on
2026-11-12T20:21:06Z from exact head
`6fa64ebcb2bfbb5cb72920c251117319d2819ed4` to main
`fce5f65ce5e43c36ceb7eb67a13f9f2040dbcb5a`. Its exact-head Linux, macOS,
Windows and MSan checks passed (workflow runs `43259845185` and
`43259845433`). MAME research was not run: this documentation-only change did
not match that workflow's `research/**`, `src/**`, tool or workflow path filters.
The exact-main Linux, macOS, Windows and MSan checks also passed (runs
`43299437756` and `43299436402`). The active ruleset requires all three platform
checks, zero approving reviews and resolved review threads. Codex's exact-head
review completed without findings (comment `6829550926`); GitHub recorded zero
review objects, inline comments and review threads. No quota error was reported
and no merge bypass was used. This PR only reconciles the #276 merge evidence
in the documentation and ledger; it adds no runtime behavior or acceptance.

## PKS operand CPU context — merged in #264

A bounded, read-only MAME observer now captures the post-fetch PC, virtual time, CCR and ER0–ER7 when firmware reads/writes the three previously observed aligned RAM word bases `237202`, `237204` and `2349E8`. The observer has a 4,096-event cap, reports overflow, filters all other addresses and does not change bus values. It was built against the pinned MAME source `706dbb73281320c54d7c2f16262c1c3786f2be01`; instrumented binary SHA-256 `0a7115cb99760c3644affe2a8f22a4491a180a824ab21b1d2b18d700174c1768`. The matching uninstrumented build hash is `6269f609a41a7e9e7202d2be9d3edc03f8744b48219fac0c9ebdfe1fea0746ff`.

Two nine-second passive/default-input runs produced byte-identical context traces (1,928 events; SHA-256 `8acf5f8c23ce9a912be8fa0a25323be342ab3f37084d3fbed012a18b5133172e` each). Per run, `2349E8` had 480 reads/483 writes, `237202` had 478 reads/4 writes, and `237204` had 478 reads/5 writes. All events fit within the cap without drops. The baseline and instrumented `run.json` matched on all 26 non-trace fields; only command path, binary hash and trace-line count were excluded. ROM identity remained AN1x 1.04 SHA-256 `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; no ROM contents were changed or added to the repository.

This is limited repeatability evidence for one nine-second passive profile. The callback PC remains the CPU-reported post-fetch PC, not an instruction-start PC. The run does not establish operand meaning, ADC arithmetic, PKS behavior, complete boot, MIDI acceptance, hardware agreement or sound. VOP3 samples remained zero. Next: use these bounded snapshots to follow the preceding value writes and their instruction context, retaining independent value/address/flag/timing and continuation checks.

## Main #264 — current, verified baseline

PR [#264](https://github.com/RobCZart82/yaman1x/pull/264) merged by squash at `a2f52d020ebc6d70e7ade57d9b78a920a9e2af4c` on 2026-11-09T17:35:18Z from verified head `1092df38cb5278e3db50ef29ac6c99fa621c85ce`. PR-head checks passed (ROM-free CI `42652447163`, MSan `42652447223`, MAME research `42652447044`); merge-commit checks passed (ROM-free CI `42666087454`, MSan `42666087385`, MAME research `42666087403`). Codex review summary completed without findings on the exact head; no GitHub review objects or threads were present, and no quota issue was reported. The active ruleset required zero approvals and resolved review threads. Two macOS hosted-runner acquisition attempts failed with explicit hosted-capacity annotations; after each workflow completed, only the failed job was retried, and the third attempt passed. Squash merge used the verified head without bypass.

## PKS control and ramp RAM reads — merged in #262, 2026-11-09

A local MAME binary with read-only taps was built using the explicit macOS OSD target (`OSD=mac`); SDL3 was not required. Two identical nine-second passive/default-input runs at the locked source commit `706dbb73281320c54d7c2f16262c1c3786f2be01` produced byte-identical traces, each 65,370 lines. The unchanged AN1x 1.04 ROM SHA-256 was `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; the diagnostic binary SHA-256 was `bd762ab222095721ea0843e7a765b8c844bcf47cc13bc182fcaffcf7d050aa88`.

The taps observed 956 reads across aligned word bases `237202` and `237204` (478 each), nine writes across those bases, and 480 reads plus 483 writes at `2349E8`. Reads at `2349E8` returned zero in this profile. At base `237204`, the low-byte read lane (`mask=00ff`) is consistent with the static disassembly `mov.b @h'237205,r2l` at `001fd2`, with the callback reporting `pc=001fd8`. Callback PC is the H8 CPU's reported post-fetch PC, not an instruction-start address. These are RAM address/lane observations only; they do not establish what the bytes mean or whether they are ADC/PKS inputs.

Both run metadata objects matched. The profile used passive DSP, no DMS board, no MIDI input and no DSP sample execution. VOP3 samples stayed at zero and complete boot remained false. The tap callbacks log address/data/mask/PC and do not modify read data. This is limited evidence from one profile and horizon; no alternate-input or hardware comparison was run. It does not establish ADC arithmetic, PKS behavior, hardware agreement or audio. Next: capture bounded CPU context or instruction-fetch history around these operand accesses and their preceding writes to follow value provenance.

PR [#262](https://github.com/RobCZart82/yaman1x/pull/262) merged at `1cdeb8f7aa629aa969beae9cce55c9292502522a` from head `53c91d5855aded332c4bd457acd766e143eb800f` on 2026-11-09T06:17:57Z. All four PR-head checks passed (ROM-free CI `42520377872`, initial-decode MSan `42520377977`); all four required main checks passed on the merge commit (ROM-free CI `42572182568`, MSan `42572182585`). The active ruleset required zero approvals and resolved review threads. No reviews, comments or threads were recorded; no automatic-review outcome or quota evidence is claimed. Squash merge used the verified head without bypass.

## PKS writer caller sites — merged in #261, 2026-11-08

PR [#261](https://github.com/RobCZart82/yaman1x/pull/261) merged at `9c89f291a630c86b959c3e173cfabbae1b54164d` from head `f3ce99d10fe34f33e668cfad3c2a62d3c03cba84`. Its four PR-head checks passed (ROM-free CI run `42512461512`, initial-decode MSan run `42512461563`); all four required checks passed on the merge commit (ROM-free CI `42515701366`, MSan `42515701585`). The active repository ruleset required zero approvals and resolved review threads. No review, comment or thread was recorded; no automatic-review result or quota evidence is claimed. The merge used squash without bypass.

## PKS direct RAM source — merged in #260, 2026-11-08

The immediate RAM source for six observed bytes at `22D9EF` is `234ACC+ER0`,
copied through R0L; the source buffer was written from R1L by the routine at
last fetch `0039A4`. This is a passive/default-input observation, not an ADC
or PKS interpretation. See [bounded source path observations](pks-payload-path.md).

## PKS caller source chain — merged in #259, 2026-11-08

Twelve read-only probes connected earlier caller reads to writes and verified
the writer register context. See [combined source-chain observations](pks-payload-path.md).
The ADC calculation remains unproven. Boot, MIDI and audio remain unaccepted.

## Earlier PKS entry predecessor — merged in #258, 2026-11-08

Eight read-only probes identified an earlier path carrying the same source
byte at fourteen paired observations. Baseline JSON stayed identical;
same-input logs matched across slices. See [predecessor observations](pks-payload-path.md).
No ADC-data source was established in this bounded window. Next: follow
the value before that path. Boot, MIDI and audio remain unaccepted.

## PKS source operand identified — merged in #257, 2026-11-08

Four read-only probes verified the R1L-to-R4L byte transfer and found the
same values already present at the earlier entry boundary. Baseline JSON
remained identical and same-input logs matched across slices. See
[source operand observations](pks-payload-path.md). Next: trace the callers
that provide R1L. ADC arithmetic, boot, MIDI and audio remain unaccepted.

## PKS value-setting interval — merged in #256, 2026-11-08

Four read-only probes narrowed the observed register-value change before
source-byte writes. All baseline JSON bytes remained identical and
same-input logs matched across slices. See [transition observations](pks-payload-path.md).
The source operand and ADC arithmetic remain open. Next: trace that operand
back to ADC intermediates. Boot, MIDI and audio remain unaccepted.

## PKS source-write register context — merged in #255, 2026-11-08

Eight read-only probes identified the destination and byte value in CPU
snapshots at the source writes. All previous JSON bytes remained identical;
same-input logs matched across slices. See [register observations](pks-payload-path.md).
The ADC-to-payload arithmetic is still open. Next: trace the register value
into this write context. Boot, MIDI and audio remain unaccepted.

## Earlier PKS staging source — merged in #254, 2026-11-08

Eight read-only probes identified an earlier mutable source region feeding
the staging bytes. Baseline JSON remained identical and same-input logs
matched across slices. See [source observations](pks-payload-path.md).
The observed copy path still does not prove the ADC-to-payload calculation.
Next: follow its input reads and callers. Boot, MIDI and audio remain open.

## PKS staging data path — merged in #253, 2026-11-08

Eight read-only probes linked observed staging-byte writes and producer
reads to the previously verified transmit-buffer path. All baseline JSON
bytes stayed unchanged and same-input logs matched across slices. See
[staging observations](pks-payload-path.md).
The ADC-to-staging calculation is still open; no protocol or audio
acceptance is implied. Next: trace data inputs to the staging-write path.

## Distinct PKS enqueue paths — merged in #252, 2026-11-08

Four read-only probes reproduced distinct predecessor fetches for the D0
and E0 bytes before the common buffer writer. Baseline JSON remained
identical and same-input logs matched across slices. See
[bounded payload path observations](pks-payload-path.md) for addresses,
method and limits. Next: connect producer input reads to ADC intermediates.
Full boot, MIDI and audio remain unaccepted.

## PKS buffer writer — merged in #251, 2026-11-08

Four read-only probes extended the previous AN2/AN6 traces with byte-write
observation for 2360CC–2360D3 and overlapping word-write observation for
2360CA–2360D4. The fixed horizon and both slice sizes were unchanged.
Every baseline JSON remained byte-identical; same-input logs matched.

All eight observed outgoing AN2 bytes and all six AN6 bytes had a preceding
matching byte write at the same RAM address, with last instruction-word
fetch FAC6. Each was then read at last fetch 18B0 and written to TDR1 five
cycles later. Earlier word writes zeroed the observed region during startup
(last fetch 9ADEA); these were initialization, not payload-byte writes.

This identifies a common enqueue/write site for the observed bytes, not
the higher-level payload generator. Last-fetch addresses remain distinct
from asserted instruction-start PCs. The bounded region and short horizon
do not establish queue capacity, wraparound or complete producer semantics.
Next: identify callers of the FAC6 path and follow their input values;
outgoing bytes still provide no basis for inventing a PKS reply.

ROM hash stayed
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
Original MAME reference remains
706dbb73281320c54d7c2f16262c1c3786f2be01. No ROM contents are included.
VOP3 samples remain zero; full boot, MIDI and audio stay unaccepted.

## ADC RAM intermediates and transmit reads — merged in #250, 2026-11-08

Four read-only probes added bounded write observers for last-fetch range
1B00–1DFF and byte-read observers for 1880–18B2 to the previous AN2/AN6
profiles. Each used 130 million requested cycles and both slice sizes.
All previous JSON outputs stayed byte-identical; same-input logs matched
across slices. Last-fetch addresses are not instruction-start PCs.

In the AN2-only case, nine writes each stored 03FF at 2349FA (last fetch
1BF0) and 234A0A (1D2A). In the AN6-only case, nine writes stored 03FF at
234A02 (1C98), and eight at 234A12 (1DBC). Both cases also stored 03FF once
at 234910 (1B7E). These observations locate intermediates; they do not
prove every dependency from ADC value to encoded payload.

The transmit path read consecutive bytes starting at RAM 2360CC with last
fetch 18B0. AN2-only reads through 2360D3 were D0 7F E0 00 20 E0 00 00;
AN6-only reads through 2360D1 were E0 3E 5F E0 7E 7F. Each read occurred
five CPU cycles before the corresponding TDR1 write and matched its byte.
This identifies a transmit-buffer source for every observed outgoing byte,
not the complete producer/consumer or wraparound semantics of that buffer.
Next: observe writes to this buffer and trace payload producers before
introducing any behavioral PKS reply.

ROM hash stayed
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
Original MAME source remains locked at
706dbb73281320c54d7c2f16262c1c3786f2be01. No ROM contents are included.
VOP3 sample counters remain zero; full boot, MIDI and audio stay unaccepted.

## ADC read-site trace — merged in #249, 2026-11-08

Four read-only probes repeated the AN2-only/AN6-only high-scan cases with
initial single AN3=1023, the fixed 130-million-cycle horizon and both slice
sizes. Temporary observers logged ADC data reads and the last instruction
word fetch address before each read and TDR1 byte write. Every previous
case JSON remained byte-identical; same-input new logs also matched across
slices. These are last-fetch addresses, not asserted instruction-start PCs.

Reads of the shared result word FFFFE4 distinguish the two profiles:

| Last fetch | AN2-only value | AN6-only value | Reads per run |
| --- | --- | --- | --- |
| 1BF4 | FFC0 | 0000 | 10 |
| 1C9C | 0000 | FFC0 | 10 |
| 1D2E | FFC0 | 0000 | 10 |
| 1DC0 | 0000 | FFC0 | 9 |

FFC0 is the model's left-aligned ten-bit maximum (1023 shifted by six).
The same address is reused across lower/upper scan banks; its value alone
cannot identify a channel without bank/time context. Every observed TDR1
byte write had last fetch 18B2. This localizes read and output sites; it
does not yet establish the transformations or meanings of the D0/E0 data.
Next, follow RAM intermediates from these read sites into the common
transmit path. No PKS response should be inferred from outgoing traffic.
ROM hash remained
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe;
no ROM bytes or instruction contents are included. Original MAME source
remains locked at 706dbb73281320c54d7c2f16262c1c3786f2be01.
VOP3 sample counters remain zero; boot, MIDI and audio are unaccepted.

## Isolated scan-channel dependence — merged in #248, 2026-11-07

Twenty read-only local 1.04 probes isolated the ADC scan inputs at the
same 130-million-cycle horizon. Each case used 4,096/1,000,000-cycle slices.
The first eight cases held the initial single AN3 conversion at 1023,
then drove just one scan channel to 1023 and the other seven to zero.
AN2 produced D0 7F E0 00 20 E0 00 00. AN6 produced
E0 3E 5F E0 7E 7F. Selecting AN0, AN1, AN3, AN4, AN5 or AN7 produced
the zero-input prefix E0 00 20 E0 00 00 within this horizon.

Two control cases set the initial single AN3 conversion to zero: an AN2-only
high scan still produced D0 7F E0 00 20 E0 00 00; an all-zero scan produced
E0 00 20 E0 00 00. Thus the observed D0 addition survives removal of the
initial high AN3 sample. This isolates scan-channel dependence in the
experimental model, not physical knob wiring or protocol semantics.

All runs reached the cycle limit without an access fault. Same-input logs
matched byte for byte across slices; all reported JSON fields matched
except callback counts. Independent 8N1 decoding checked 512-cycle spacing,
start/stop levels and agreement with TDR writes. No RDR1 byte reads were
observed or PKS input supplied; VOP3 sample counters stayed zero. ROM hash
remained 4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
Original MAME source remains locked at
706dbb73281320c54d7c2f16262c1c3786f2be01. Next: trace firmware reads of
AN2 and AN6 through payload construction before implementing responses.
Full boot, hardware fidelity, MIDI, audio and REAPER remain unaccepted.

## Intermediate ADC inputs and PKS traffic — merged in #247, 2026-11-07

Eight additional read-only local probes used uniform synthetic ADC values
256, 511, 512 and 768, each with 4,096 and 1,000,000-cycle slices at the
same 130-million-cycle horizon. All reached the requested horizon without
an access fault. Same-input logs were byte-identical; reported JSON state
matched except completed_slice_callbacks. Independent 8N1 decoding checked
512-cycle bit spacing, start/stop levels and equality with TDR byte writes.

| Uniform ADC input | SCI1 transmitted bytes by the horizon |
| --- | --- |
| 256 | E0 00 30 E0 00 20 |
| 511 | D0 36 |
| 512 | D0 37 |
| 768 | D0 71 E0 1E 4F E0 3E 5F |

No RDR1 byte reads were observed, no PKS reply was injected, and VOP3
sample counters stayed zero. The one-count change 511 to 512 reproducibly
changes the second D0 byte 36 to 37. This narrows an input-dependent
behavior; it does not identify the protocol, the originating ADC channel,
or a firmware scaling formula. Uniform values vary all channels together;
shorter byte sequences at this fixed horizon do not imply a missing frame.
Next, isolate ADC channels and trace the source of the D0 payload before
implementing any response. Full boot, hardware calibration, MIDI and audio
remain unaccepted. Runtime source is unchanged; the reference remains
original MAME 706dbb73281320c54d7c2f16262c1c3786f2be01.

## PKS transmit frames — merged in #246, 2026-11-07

Four read-only local 1.04 probes repeated the 130-million-cycle profile
with synthetic ADC 0/1023 and slices 4,096/1,000,000. A temporary SCI1 TX
callback observer and RDR1 byte-read observer left every baseline JSON byte
unchanged. Same-input trace logs matched byte for byte across slices.
Each decoded frame had a low start, eight LSB-first data bits and a high
stop, spaced 512 CPU cycles per bit (31,250 bit/s under the experimental
16 MHz clock). Callback samples include repeated levels, not just edges.

Zero-input frame start cycles: 127846336, 127851456, 127856576,
128288512, 128293632, 128298752; bytes E0 00 20 E0 00 00.
High-input starts: 127803104, 127808224, 127846976, 127852096,
127857216, 128251072, 128256192, 128261312;
bytes D0 7F E0 3E 5F E0 7E 7F. All frames passed independent start,
stop, bit-spacing and LSB-first decoding checks. No observed RDR1 byte
reads occurred. No PKS input was supplied. This does not establish a
physical PKS response, full boot, or exhaustive receive-access coverage.

ROM SHA-256 before/after:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
Both VOP3 sample counters remained zero. Source reference remains original
MAME commit 706dbb73281320c54d7c2f16262c1c3786f2be01 and the checked-in
source lock; no new hardware comparison was performed. The earlier MAME
zero-input TDR prefix agrees, but is not a wire-timing reference.
Next: explain the ADC-dependent outgoing data from firmware execution
before designing any PKS response. MIDI, audio and REAPER remain open.

## First PKS enable and transmit-register writes — merged in #245, 2026-11-07

Four read-only 1.04 probes requested the same 130-million-cycle horizon.
The added write observer preserved each previous JSON byte for byte.
For each synthetic ADC value, both slice sizes produced identical SCI logs.
SCI1 SCR first became 70 at cycle 127,762,748 with zero ADC input, and
127,704,707 with 1023 input. SCI0 SCR remained 00 at the endpoint.

The zero-input SCI1 TDR prefix was E0 00 20 E0 00 00, with writes at cycles
127,846,279; 127,847,946; 127,851,151; 128,288,457; 128,290,124; 128,293,326.
It matches the byte prefix in the earlier MAME research observations.
The high-input prefix was D0 7F E0 3E 5F E0 7E 7F. These are CPU register
writes, not decoded wire frames or evidence of a PKS response. Both runs
had SCI1 receive/transmit enabled and SCI0 disabled at the endpoint.
The ROM hash was unchanged. VOP3 sample counters stayed zero.

Next, observe actual serial frames and receive-register access under the
same fixed-cycle profile. Establish the cause of the varying transmit data
before introducing a behavioral PKS response. The locked MAME source
remains 706dbb73281320c54d7c2f16262c1c3786f2be01.
Boot, MIDI reception and audio acceptance remain open.

## Fixed-cycle readiness and callback accounting — merged in #244, 2026-11-07

Four read-only probes requested 130 million cycles with the same synthetic
ADC inputs and two slice sizes. Each same-input pair matched every reported
JSON field except completed_slice_callbacks. Zero input ended at cycle
130,000,002, PC 0xE2A; 1023 input ended at 130,000,003, PC 0x15DC.
SCI1 SCR was 70, SCI0 SCR was 00 and ADCSR was B7 in all snapshots.
The ROM hash was unchanged and VOP3 sample counters remained zero.

H8ExecutionRunner counts update/full/partial calls, including update(start)
at every run invocation. Its fail-stop callback limit therefore includes
partition overhead. The 4,096-cycle low/high probes used 19,557,981 and
19,556,979 callbacks; the million-cycle probes used 19,494,892 and
19,493,891. Equal callback budgets are not equal execution horizons.
The earlier differing budget-limit endpoints alone do not establish a
machine-state divergence. These probes compare reported fields, not every
internal memory byte. Next, trace the first PKS-port enable and transmit
sequence at fixed cycle milestones. Firmware boot, MIDI and audio stay open.

## PKS serial port enabled after upper ADC integration — merged in #243, 2026-11-07

Four read-only 1.04 swap16 probes on main bf6cf09 passed the upper ADC
request with synthetic inputs 0 and 1023 and 4,096/1,000,000-cycle slices.
All stopped at the 20-million execution callback limit without an access
fault. End cycles were 132,771,393 and 133,175,867 for zero input, and
132,778,347 and 133,182,221 for 1023 input. These budget-limited endpoints
differ across slices and are recorded separately.

All four endpoint snapshots had SCI1 SCR=70, SCI0 SCR=00 and SSR=84 on
both ports. The locked H8 SCI model interprets 70 as receive/transmit and
receive-interrupt enabled on the PKS port. This does not establish a PKS
reply or MIDI reception. The ADC remained B3. Both VOP3 sample counters
stayed zero and the local ROM hash was unchanged. Next, observe the first
SCI1 enable and PKS transmit sequence at fixed cycle milestones, while
investigating callback-budget accounting across run partitions.
Firmware boot, hardware ADC calibration, MIDI and audio remain open.

## Upper ADC scan on the AN1x bus — merged in #242, 2026-11-07

The explicit ADC sink now receives the observed 0x37 restart at channel
4 after lower-bank ADST clear. The new start replaces the old deadline
with a 266-cycle deadline. Upper channels reuse ADDRA–ADDRD and commit
every 256 cycles. Same-bank control writes apply ADF write-zero behavior.
Bank changes without the supported ADST transition reject explicitly.

Independent bus tests verify lower-to-upper and upper-to-lower starts,
callback channel/time, retained results and four result lanes across
1,035 time partitions. Both focused ADC tests passed. The reference is
locked H8_ADC_3337 at MAME 706dbb73281320c54d7c2f16262c1c3786f2be01.
Next, repeat the extended local firmware probes with explicit inputs.
Physical ADC behavior, boot, MIDI and audio acceptance remain open.

## Standalone upper ADC scan restart — merged in #241, 2026-11-07

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

## Hosszabb boot-diagnosztika — 2026-11-03

## Historical development snapshots

Earlier candidate descriptions below record their preparation-time state;
merged status is tracked in development-ledger.json and the current summary.
