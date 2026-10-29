# H8H signed16 displacement memory moves

Six original MOV byte/word/long load/store groups add twelve unchanged full/partial
handlers:466 ->478;231 selected/33 missing/135 excluded encoding rows (not a CPU
percentage). Long forms use guarded0100/6f state; byte/word use6e/6f. No new host
helper or upstream revision. Notices/source/generator locks are retained.

Effective address is full ER plus signed16 displacement with uint32 wrapping.
The host retains24-bit physical mapping; the bus carries the full32-bit address.
Words align down; long transfers preserve high/low word order, each independently
aligned, with address+2 wrapping. Flags update NZV only. Address/data aliases use
the old address before any destination write. No IRQ/SCI/ROM/DSP behavior change.

Independent controls cover all65536 displacements in both directions at all three
widths (393216 cases), plus every address/data register pairing across five offset,
six base-address and three CCR boundary patterns (57600 cases). Complete register
images, data lanes, full bus addresses, flags and access counts are compared.
201 split histories compare state/RAM/ordered trace; first/second long-store faults
preserve completed effects and fail-stop; invalid ER neighbors are rejected.

Original decoder/handlers/execute_run:200000 slices,704568 instruction reads,
38431 byte reads,25658 byte writes,57460 word reads,44552 word writes; zero differences.
The dedicated loop requires fetches of all six forms at their instruction addresses;
all prior family reachability guards remain active. A long negative-offset case was
added after the first deliberate unsigned-offset mutation escaped the positive-only
mixed fixture. Final negative-control result is recorded after the strengthened run.

Full candidate suite:56 CTest groups/87 Python cases. Local Windows excludes the
single known symlink privilege case (WinError1314); final-head CI is separate evidence.
No full AN1x memory map, physical bus-error recovery, firmware boot, MIDI/audio or
plugin acceptance. Remaining displacement32 and other H8H groups are still open.
See [development plan](FIRST_REAPER_SOUND_PLAN.md).

Final control: treating long-load signed16 as unsigned in both ignored candidate
bodies is detected at seed9/slice6. Local55 CTest and86 Python cases PASS; the
one known symlink case is excluded. Source audit464 files/201 upstream/one reviewed
edit PASS; notices229 files/five texts PASS. No Yamaha data introduced.

Closure: merged [#78](https://github.com/RobCZart82/yaman1x/pull/78), engine `d4ef0e45bc416f06216d43559e1f73df1d39caa8`, final head `ef6adefadb5b390f6822ae34c4db41edab5b1348`. All three platform jobs, MSan and full MAME passed; see [current status](STATUS.md) for run links. Preparation-time candidate wording above is historical.
