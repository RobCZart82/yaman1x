# H8H decimal adjustment (DAA/DAS)

M0 increment based on merged PR89,engine baseline PR88. Selects four unchanged
original DAA/DAS full/partial handlers and guarded0f00/fff0,1f00/fff0 decoding.
Fixed m_has_hc=true and existing reviewed do_add8 arithmetic helper are retained;
no new semantic helper,body adaptation,DSP,EG or Yamaha input is introduced.
The original algorithms also define behavior for non-BCD inputs; no hardware
claim is made for those combinations.

## Independent decimal oracle

All100*100 decimal operand pairs are tested for addition/subtraction with two
unrelated-CCR patterns (40000 cases). Inputs are encoded in packed BCD, then
binary add/subtract produces the raw byte and C/H input flags. The expected
adjusted result comes from integer decimal arithmetic modulo100, not the original
correction branches. The correction byte is derived from expected-result minus
raw-byte modulo256; expected arithmetic H/N/Z/V/C comes from the independent
8-bit addition of that correction. I/UI/unused bit and full register image are
checked. All16 byte-register lanes are exercised across2048 boundary cases;
no data transfers are expected. These cases represent valid decimal operations,
not an exhaustive independent oracle for arbitrary invalid BCD/flag inputs.
241 split histories also include non-BCD values and compare snapshots/ordered
instruction-event traces. Since adjustment precedes prefetch, this checks that
suspension/resume does not repeat the register/flag update. Existing exhaustive
65536 first-word acceptance/rejection and prefix guards remain active.

Original decoder/handlers/execute_run:200000 slices,693532 instruction reads,
36494 byte reads,23720 byte writes,51657 word reads,38747 word writes;zero differences.
Both DAA/DAS must be fetched at their instruction addresses; all prior family
reachability requirements remain. A deliberate DAA correction+1 mutation in
both ignored candidate bodies is detected at seed7/slice0. No tracked mutation.

Candidate:62 CTest/87 Python;496 upstream-derived methods (492 unchanged,4 prior
adaptations),239 selected/25 missing/135 excluded encoding rows. This is not CPU
completion percentage. Local Release build,61 runnable CTest groups,decimal oracle and86 Python PASS;
one known Windows symlink case excluded. Remote CI pending.
Linux UBSan includes decimal adjustment and previous multiply/divide controls.
Generated selection,source/notice and local documentation checks are separate
from physical timing,firmware/MIDI/audio/plugin acceptance; M0 remains partial.
See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).

Closure: merged [PR90](https://github.com/RobCZart82/yaman1x/pull/90), engine `a4f52f600d2cfd85f14a5e8822eb5e025d28825d`, final head `f24ca47f3511764dfec4971e84c3c1487aaed443`. All five jobs PASS including decimal/prior arithmetic Linux UBSan and original-overflow control; [run links](STATUS.md). Automated code review completed with no findings on this head. Preparation-time candidate/pending notes above are historical.
Final source inventory:477 files,201 locked upstream identities,one reviewed manifest edit;four prior generated-body adaptations remain separately recorded. Source/notice checks PASS.
