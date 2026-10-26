# H8 MIDI input route

Date: 2026-10-26. This records the first MIDI input connection in the separate
MAME research target. It is not part of the plugin engine.

## Hardware route

The AN1x service manual's H8 pin table identifies CPU RXD0 as MIDI IN and
RXD1 as keyboard receive. The MAME H83002 model exposes these receive inputs as
SCI channel 0 and channel 1. The ninth research patch connects MAME's `midiin`
image port to `h83002_device::sci_rx_w<0>`, leaving the keyboard serial input
separate. It counts RXD0 line transitions for diagnostics; the counter is not a
UART-byte or firmware-event counter.

## File input and observation

MAME's emulated MIDI input accepts `.mid` and `.syx` images. File playback is
scheduled no earlier than ten seconds after machine start, so the probe's
`--midi-file` option requires `--seconds` greater than ten. The run manifest
records the supplied file's name, size and SHA-256, not its contents.

A locally generated MIDI file with one note-on and one note-off was played in
12- and 30-second runs. Each showed 21 line transitions on H8 RXD0 and MAME
exited successfully. The tap also observed one SCI0 SCR write (`0x00`) and no
reads from the SCI0 receive-data register (RDR). In the observed driver state,
the H8 SCI receiver is disabled; the run therefore verifies the serial line
connection only, not UART byte reception. It does not prove that firmware
accepts or acts on a note, or that either VOP3 produces audio. The PKS, synth
output routing, table-ROM access and MEG paths remain incomplete.

## Reproduction

Apply the ninth patch after `mame-timed-vop3-experiment.patch`, rebuild the
headless MAME target, and provide a local `.mid` or `.syx` file:

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --midi-file /absolute/path/to/test.mid --work /absolute/path/to/new-midi-run --seconds 12
```

The file remains external and is not copied into the work directory. `NO_USE_MIDI=1`
disables MAME's host MIDI backend in the documented build; the emulated MIDI
image port remains available for file playback. The research target and its
MAME licensing requirements remain separate from the product engine.
