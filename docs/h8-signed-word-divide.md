# H8H signed word divide and extreme quotient boundary

Candidate based on merged PR93,engine baseline PR92. Selects two unchanged original
DIVXS.W full/partial bodies. Existing01d0 continuation accepts5300/ff08 alongside
byte5100/ff00; reserved ER bit3 and every unrelated continuation fail-stop.
Fixed19 internal cycles plus existing host charge are retained. No body adaptation,
new helper,DSP,EG or Yamaha input. Host requires31 value bits in int explicitly.

## Source semantics at the extreme

The original uses unsigned32 temporary magnitudes,not a direct signed32
INT_MIN/-1 division. C++20 narrowing of the resulting quotient into32-bit int
precedes N selection. Mathematical -2147483648/-1 gives +2147483648; the original
narrows it to -2147483648,sets N,stores quotient low16 zero/remainder zero,and
leaves Z clear because divisor is nonzero. V,C,H and interrupt bits are preserved;
no new overflow exception or hardware policy is invented. This is pinned-source
compatibility under the required C++20/32-bit-int profile,not measured AN1x behavior.

Independent signed64 math calculates q truncated toward zero,r=n-q*d,and checks
identity/remainder sign. Packing uses low16 q/r; expected N is the sign after
modulo2^32 narrowing,not the mathematical64-bit sign or stored16-bit sign.
The independent oracle deliberately models that source flag boundary separately.
The misleading original signed-division comment examples do not define the oracle.

## Evidence

524288 divisor sweeps (all16-bit patterns against eight32-bit boundary numerators),
21504 register/alias/CCR cases,20000 random pairs and1024 explicit extreme/CCR
witnesses PASS. Witnesses include INT_MIN/-1,positive32768/1,negative65536/1,and
zero divisor across all256 CCR inputs. Whole register images and absence of data
transfers checked.321 split states/ordered instruction-event histories PASS;
128 reserved ER neighbors rejected. The existing65536 prefix acceptance oracle
now accepts exactly byte and word forms; earlier first-word/prefix guards remain.

Original decoder/handlers/execute_run:200000 slices,662384 instruction reads,
36494 byte reads,23720 byte writes,51657 word reads,38747 word writes;zero differences.
Address-bound guards require actual INT_MIN/-1,zero-divisor and one-divisor paths;
all prior-family guards remain. Deliberately reversing N selection in both ignored
candidate bodies is detected at seed7/slice7. Tracked handlers remain unchanged.

Candidate:64 CTest/87 Python,502 upstream-derived methods (498 unchanged,4 prior
adaptations),241 selected/23 missing/135 excluded encoding rows,not CPU percentage.
Local Release build,63 runnable CTest groups,arithmetic and86 Python PASS;one known Windows symlink case
excluded. Remote platform/UBSan/MSan/MAME pending. Linux
UBSan now includes the extreme signed-word target and prior arithmetic/control.
Generated selections match originals plus four earlier adaptations. Source/notice
checks PASS. This closes source-level extreme-case research only after remote
validation; physical timing and firmware/MIDI/audio/plugin acceptance stay open.
M0 remains partial. See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).

Closure: merged [PR94](https://github.com/RobCZart82/yaman1x/pull/94),engine `442949856a92392986f3bbc5927f36b7884a505b`,final head `9f3cda7fcbe545ee2b0da9cebc23d8dd5ab6b42a`. All five jobs PASS including Linux UBSan extreme signed-word/prior arithmetic controls;[run links](STATUS.md). Automated code review was still running at merge;no completed review claimed. Preparation-time candidate/pending notes above are historical.
Final source inventory:481 files,201 locked upstream identities,one reviewed manifest edit;four prior generated-body adaptations separately recorded. Source/notice checks PASS. Source/profile extreme-case investigation closed;hardware evidence remains open.
