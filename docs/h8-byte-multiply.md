# H8H unsigned byte multiply

Candidate based on merged PR79, engine baseline PR78. Pinned original
`mulxu_b_r8h_r16l` full/partial bodies are unchanged except class qualifiers.
Flat decoder accepts all 256 words5000..50ff. Destination is a 16-bit register;
its old low byte and the selected byte source are multiplied before the write.
CCR is unchanged. Fixed H8H `m_has_mac=false` selects original11 internal cycles;
existing host `internal` charging adds one. No MAC/H8S profile is enabled.

Independent checks:65536 operand pairs,19200 register/alias/CCR boundaries,
161 split execution histories, complete register images and no data bus accesses.
Original decoder/execute_run comparison:200000 slices,700069 instruction reads,
38006 byte reads,25233 byte writes,56188 word reads,43277 word writes; zero differences.
The new opcode is required to be reached; all prior family reachability guards remain.
A deliberate product+1 mutation in both ignored candidate bodies is detected at
seed9/slice1. Tracked handlers remain unchanged by this control.

Local Release build,56 runnable CTest groups and86 Python cases PASS; one known
Windows symlink privilege case excluded. Full candidate suite:57 CTest/87 Python.
Source audit465 files,201 upstream identities,one reviewed local edit PASS.
Final-head remote CI pending. This is synthetic/source-reference evidence, not
physical timing, firmware boot, MIDI, audio or plugin acceptance.

## Word multiply followup risk

The pinned generated `mulxu_w_r16h_r32l` expression multiplies a `u16` cast by
`r16_r` (u16). With32-bit int both operands undergo signed-int promotion;
65535*65535 exceeds INT_MAX. This is a source-level C++ overflow risk, not a
measured hardware defect. Word multiply remains unselected. Before enabling it,
add independent high-product cases and UBSan evidence, choose a minimal reviewed
unsigned-widening adaptation with explicit provenance, and keep the original
reference comparison separate from the arithmetic oracle. No unverified upstream
word handler is enabled by this PR.

See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).
