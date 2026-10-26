# H8 serial state and keyboard-port observations

The tenth MAME research patch adds side-effect-suppressed snapshots of both
SCI devices once per simulated second. Reading the device's SMR, BRR, SCR and
SSR directly provides an independent check of the bus-tap observations.
The observer also counts SCI1 transmit-data-register (TDR) writes and
receive-data-register (RDR) reads, preserving the first 128 TDR writes.
The observations do not change UART registers or supply PKS replies.

## Paired local runs

Both 12-second runs used the unchanged AN1x 1.04 firmware and the same generated
note-on/note-off MIDI file (SHA-256
`b1e0a725ebe12cfff815716b82562d81370a8a5fd13c042c5788bc6da74d9e52`).

| Observation | Passive / DMS absent | Firmware-contract / DMS installed / timed model / experimental EG |
|---|---|---|
| Exit status | 0 | 0 |
| SCI0 SCR at snapshots 0..12 seconds | 0x00 | 0x00 |
| First sampled SCI1 SCR=0x70 | 6 seconds | 6 seconds |
| SCI1 TDR writes | 30 | 30 |
| SCI1 RDR reads | 0 | 0 |
| MIDI RXD0 transitions | 21 | 21 |

The SCI1 TDR prefix is `E0 00 20 E0 00 00`, followed by 24 writes of `FE`.
These are observed host-register writes; they do not establish how many bytes
went over the serial wire or what PKS commands they represent. No incoming
SCI1/PKS traffic is supplied in this harness. SCR `0x70` has receiver,
transmitter and receive-interrupt-enable bits set in the MAME H8 SCI model.
SCI0 remains disabled at the recorded sample instants. The snapshots are
one second apart and cannot rule out shorter state changes between them.

The comparison changes several DSP/board settings together. It shows that the
observed serial state persists under both complete configurations; it does not
isolate the effect of each setting or prove a causal PKS gate on MIDI startup.
Next research work should trace firmware control flow around keyboard setup
and MIDI receiver enabling, with the PKS protocol established before any HLE
reply is implemented. Complete boot, MIDI note handling and audio remain
unverified. The VOP3 clock profile is still an assumed 48 kHz software model.

## Reproduce and audit

Apply `research/mame-sci-state-observation.patch` after the corrected ninth
MIDI-input patch and rebuild the separate headless MAME target as documented
in `research/README.md`. Run the H8 probe for 12 seconds with a local MIDI file,
then audit its run directory:

```sh
python3 tools/sci_trace.py /absolute/path/to/new-run
```

The probe also stores the parsed observation under `sci_observation` in its
run manifest. The parser requires paired, ordered channel snapshots, a complete
SCI1 footer and contiguous bounded TDR-prefix records. Synthetic tests reject
missing/duplicate snapshots, truncated transmit records and malformed fields.
The parser accepts older captures without the tenth-patch records, returning
no new observation for those runs. Raw logs and Yamaha data stay external.
