# H8H signed byte divide

Candidate based on merged PR91,engine baseline PR90. Selects unchanged01d0
prefix dispatch and DIVXS.B full/partial bodies. Guarded continuation0xf0000
accepts5100/ff00 only; signed-word division and all other continuations fail-stop.
Fixed11 internal cycles plus existing host charge are retained. No new helper,
body adaptation,DSP,EG or Yamaha input is introduced.

Signed16 destination and signed8 source are read before write. For nonzero divisor,
quotient truncates toward zero; remainder is numerator minus quotient*divisor,
with numerator sign or zero. Low8 quotient/remainder bits occupy low/high bytes.
N follows the full quotient sign, not the stored byte's bit7; Z means zero divisor,
which preserves the whole destination. Other CCR bits remain unchanged.
Original comments include examples inconsistent with executable truncation
(e.g.20/-3); tests use independent mathematics,not those comments. Source behavior
is preserved without claiming independent AN1x hardware evidence.

## Evidence and limits

Signed64 oracle performs mathematical sign conversion before division and checks
n=q*d+r and remainder sign.393216 numerator sweeps cover all16-bit patterns against
six divisor patterns;2048 all-divisor boundaries,36864 register/alias/CCR cases,
20000 random pairs,241 split snapshots/ordered instruction-event traces PASS.
The byte-domain quotient lies in[-32768,32768]; INT_MIN/-1 of32-bit division cannot
arise here. All65536 prefix extension words are independently accepted/rejected;
existing first-word and other-prefix guards remain. No data access is expected.

Original decoder/handlers/execute_run:200000 slices,668016 instruction reads,
36494 byte reads,23720 byte writes,51657 word reads,38747 word writes;zero differences.
Address-bound reachability guards require negative,zero and one-divisor paths;
all prior-family guards remain. A deliberate quotient+1 mutation in both ignored
candidate bodies is detected at seed7/slice3. No tracked mutation remains.

Candidate:63 CTest/87 Python,500 upstream-derived methods (496 unchanged,4 prior
adaptations),240 selected/24 missing/135 excluded encoding rows,not CPU percentage.
Local Release build,62 runnable CTest groups,arithmetic and86 Python PASS;one known Windows symlink case
excluded. Remote platform,UBSan,MSan and MAME results pending.
Linux UBSan includes this target and prior arithmetic/original-overflow controls.
Generated selection/source/notice checks pass separately from physical timing,
firmware/MIDI/audio/plugin acceptance. M0 remains partial.

Signed-word DIVXS remains unselected: before enabling it,independently examine
INT32_MIN/-1,full quotient sign and packing,source narrowing and exact flag
behavior; use widened arithmetic/provenance review if needed and distinguish
source agreement from hardware evidence. This candidate does not settle that case.
See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).

Closure: merged [PR92](https://github.com/RobCZart82/yaman1x/pull/92), engine `1adeb1030871083bb510f4e691da4467cd96a816`, final head `b719cf25b0d0d8b929c5b639d5971ddc424a1a49`. All five jobs PASS including Linux UBSan signed-byte divide and prior arithmetic controls; [run links](STATUS.md). Final-head automated code review completed with no findings. Preparation-time candidate/pending notes above are historical.
Final source inventory:479 files,201 locked upstream identities,one reviewed manifest edit;four prior generated-body adaptations separately recorded. Source/notice checks PASS.
