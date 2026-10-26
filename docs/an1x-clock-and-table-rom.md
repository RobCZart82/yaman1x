# AN1x VOP3 table-ROM and clock findings

Date: 2026-10-26. Inputs: the local Yamaha AN1x 1.04 firmware image, the
downloaded VOP3 source archive, and the AN1x service manual. This page records
read-only analysis; it does not include Yamaha firmware, table-ROM data,
patches or audio captures.

## Table image found in the VOP3 archive

The downloaded VOP3 archive contains
`docs/vop3_an1x/maskrom.bin`, a 131,072-byte (64 Ki-word) AN1x VOP3 table-ROM
image. Its SHA-256 is
`97eff8e560cb5e644b8567601a72fbe353fd0eb1770c368b717aa51f76055944`.
The archive's research note attributes it to an AN1x VOP3 ROM dump made by
rgwan on 2026-10-15. This is provenance recorded by the upstream researcher,
not an independent physical read by this project.

The image's shipped-voice tables were compared byte for byte against the
table copy at offset `0xE496C` in the local 1.04 CPU firmware:

| Table | Table-ROM word range | Firmware-copy word range | Result |
|---|---:|---:|---|
| Exponential | `0x0000..0x0FFF` | `0x0000..0x0FFF` | Exact match |
| Powers of two | `0x1000..0x101F` | `0x1000..0x100F` | Exact match after duplicating each firmware word |
| Decay | `0x1020..0x301F` | `0x1010..0x300F` | Exact match |
| Sine | `0x3020..0x401F` | `0x3010..0x400F` | Exact match |
| Clipper | `0x4020..0x5020` | `0x4010..0x5010` | Exact match |

`tools/an1x_table_rom_audit.py` repeats these comparisons using external file
or ZIP-member inputs. It does not extract or modify either input. The output
includes the image digests and explicitly marks address-index scaling and
hardware validation as unresolved.

The image is not copied into this repository. The VOP3 project notes that its
extracted Yamaha program images are not covered by that project's BSD license.
The MAME driver still marks both board ROM regions `NO_DUMP`; this one external
image does not establish that the main-board and DMS-board ROM contents are
identical.

## What the comparison does not establish

The byte comparison validates the table values used by the shipped AN1x voice
against the copy in the 1.04 CPU firmware. It does not prove the physical
image's origin, recover the DMS ROM, determine how the DSP's index input is
scaled, or establish a complete waveform/output path. The upstream VOP3
research says its table-index probes stayed at indices 0 or 1, so table-index
scaling remains an open question. The existing VOP3 core also marks the
`idxw` scale as open.

## Clock evidence and remaining uncertainty

The service manual's block diagram, parts list and DM/DMS circuit schematic
show a 16 MHz CPU clock source (`X1`) and an 11.2896 MHz crystal (`X2`) in the
MEG oscillator circuit. The MAME configuration uses 16 MHz for the H8 and
11.2896 MHz for the MEG. These are component clock references, not a measured
AN1x VOP3 sample rate. The available sources do not establish the VOP3
`CLKIN` frequency/divider or the number of VOP3 passes per output sample.
Therefore the core's 48 kHz model remains an assumption for AN1x operation.

To close the clock question, trace the `MEG MCLK` to VOP3 `CLKIN` path and
establish the VOP3 pass/output cadence from primary timing evidence or an
AN1x hardware capture. Keep this separate from the FS1R VOP3's documented
24.576 MHz / 512 = 48 kHz model.
