# M1: timestamped serial control-flow observation

Date: 2026-10-27. Baseline `064ddbe56dd7931839bd2a45e73c2093240098b1`.
Implementation: [PR #19](https://github.com/RobCZart82/yaman1x/pull/19).
Status: observation tooling implemented; firmware/MIDI acceptance NOT RUN.

Follow-up: [PR #20](https://github.com/RobCZart82/yaman1x/pull/20) verified the
complete patched MAME build and arm64/ROM-free registration smoke in CI at
`31664c74f2726091eb9f403c1621ef949479ddbb`. See
[source/build evidence](source-identity-recovery.md#verified-result).
The original Windows-local NOT RUN statements below are historical validation
boundaries; fresh firmware/SCI/MIDI acceptance still has not been executed.

The eleventh research patch adds passive SCI0/SCI1 bus-access events to the
existing MAME harness. It records all writes to SMR, BRR, SCR, TDR, SSR and RDR,
and reads of RDR. Other register polling reads are omitted. Each selected byte
has a global sequence, absolute emulated nanoseconds, channel, operation,
register offset, data, byte mask and observed CPU PC. Two bytes in one word
access are emitted high lane first at the same time. This is bus observation,
not a full instruction trace; the PC is the value reported by MAME at the tap,
not a proven call-site or instruction-start address.

The observer does not supply serial replies, change registers, inject MIDI
bytes into RAM or modify firmware. Side-effect-suppressed debugger/snapshot
accesses are excluded by the driver. The stream is bounded to 4096 byte events
per fresh process; later events increment a dropped counter. A versioned start
record and completion footer delimit the stream. The parser rejects any loss,
truncation, sequence gap, reversed timestamp, invalid register/mask or extra
record after completion. Equal timestamps are valid and sequence-ordered.
An empty complete stream is distinct from a legacy capture without this patch.

This closes an observation gap: SCR writes enabling and disabling receive
between the older one-second snapshots can now be located in time and PC.
The summary lists configuration writes and writes selecting the receiver-enable
bit. It does not infer unobserved initial state, successful received bytes,
firmware note handling, a PKS handshake, or PKS causality from an RDR read.
RDR data can be stale. The full event list remains in local run output only.

## Validation and boundaries

- Native ROM-free test: channels and lane order, read filtering, zero/partial
  masks, invalid addresses, observer instance isolation and overflow accounting.
- Python synthetic tests: short SCR enable windows, channel summaries, legacy
  and empty streams, corruption/truncation/loss rejection, 4096-event boundary,
  and integration into `summarize_trace`.
- All eleven patches were applied sequentially to source files read from the
  locked local MAME archive. The injected helper matches `src/sci_access_trace.h`.
- This exposed a ninth-patch context error: the MIDI patch expected a blank
  line between MEG setup and the closing brace absent from the locked source.
  The hunk now adds that blank line itself; MIDI behavior is unchanged.
- Full patched MAME compilation and fresh firmware runs: **NOT RUN** on this
  Windows development machine. Native helper tests do not validate MAME taps,
  time/PC semantics, cold boot or actual serial reception. Existing research
  bridge loading remains macOS-specific.

## Next local experiment

Apply `research/mame-sci-access-observation.patch` after the ten documented
patches, rebuild the separate macOS research target, and use a fresh output
directory for each run. Keep the original firmware and generated MIDI input
identical. First reproduce the passive baseline; then change only one
experimental profile per paired run. Audit with:

```sh
python3 tools/sci_trace.py /absolute/path/to/new-run
```

`run_h8_probe.py` also stores the validated access observation in its local
manifest. Legacy captures remain supported. A capture with any access loss is
rejected rather than summarized as a complete control-flow observation.

Locate SCI1 SMR/BRR/SCR setup, its TDR writers and RDR readers, and every SCI0
SCR write. Follow those PCs with private firmware disassembly to establish the
branch/call path to MIDI enable. Correlate RDR reads with SSR receive-ready and
the expected test byte sequence before claiming reception; the present stream
does not record SSR polling reads. Do not derive a PKS reply from the observed
`E0`/`FE` TDR prefix alone. A verified protocol/reference capture is still
required before adding PKS HLE. M1 acceptance remains fresh-boot firmware
consumption of note-on/note-off and corresponding voice/EG/register changes.
