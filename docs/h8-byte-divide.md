# H8H unsigned byte divide

Candidate based on merged PR85, engine baseline PR84. Selects unchanged pinned
DIVXU.B full/partial bodies with flat5100/ff00 decoding. A16-bit destination word
is divided by the selected8-bit register; operands are read before writing.
For nonzero divisors, low8 quotient bits occupy the low byte and the remainder
the high byte. A zero divisor preserves the destination word. N follows divisor
bit7 and Z means zero divisor; all other CCR bits remain unchanged. These are
original-source semantics, not independently measured AN1x hardware facts.
Original11 internal cycles plus existing host charge are retained. No word or
signed division, exception model, DSP,EG or Yamaha inputs are enabled.

## Evidence

Independent unsigned64 oracle computes quotient and derives remainder by
subtraction:393216 numerator sweeps (every16-bit value against six divisors),
2048 all-divisor boundaries,30720 register/alias/CCR cases,20000 random pairs.
Whole register images, zero-divisor retention, quotient truncation, all CCR bits,
no data accesses and241 split snapshots/ordered instruction-event traces checked.
Existing65536 first-word acceptance and signed-prefix rejection guards remain.

Original decoder/handlers/execute_run:200000 slices,689545 instruction reads,
37192 byte reads,24419 byte writes,53747 word reads,40837 word writes;zero differences.
Address-bound reachability guards require high-bit divisor,zero divisor and1
sequences; all earlier family guards remain. A deliberate quotient+1 mutation
in both ignored candidate bodies is detected at seed9/slice18; tracked bodies
are unchanged. During preparation an incorrect insertion under signed-prefix
continuation was detected by native/reference execution and corrected to flat
dispatch before the final successful run. No failed-run evidence is called PASS.

Candidate suite:60 CTest groups/87 Python cases;490 upstream-derived methods,
488 unchanged plus the2 prior unsigned-word widening adaptations. Inventory:
236 selected/28 missing/135 excluded encoding rows, not CPU completion percentage.
Local Release build,59 runnable CTest groups and86 Python cases PASS; one known Windows symlink privilege
case excluded. Remote platform/UBSan/MSan/MAME results pending.
Linux UBSan now also runs byte divide, retaining signed/unsigned multiply tests
and the required original unsigned-promotion negative control. Generated selection,
source and notice checks pass. This is synthetic/source-reference evidence;
physical timing,firmware boot,MIDI,audio and plugin acceptance remain open.

See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).
