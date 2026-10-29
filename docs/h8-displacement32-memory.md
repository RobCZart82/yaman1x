# H8H displacement32 byte/word memory moves

Candidate based on merged PR95,engine baseline PR94. Selects unchanged7800 prefix
and four MOV byte/word load/store groups (ten full/partial methods). Existing
four-word IR is sufficient. First-word mask includes high internal-state bits,
so a continuation containing7800 cannot redispatch as a new prefix. State160000
accepts exactly6a20/6aa0/6b20/6ba0 with low register nibble; other extensions fail-stop.
Long displacement32 remains excluded. No new semantic helper or body adaptation.

Full ER base plus32-bit offset wraps modulo2^32. Word accesses align down using
existing bus helpers; host24-bit physical mapping is separate from full bus
addresses. Source/address aliases use pre-write register values. Byte register
lanes,all16 word halves,N/Z/V updates and unrelated CCR bits are preserved.
No Yamaha inputs,DSP,EG or full-machine MMIO behavior is introduced.

## Independent controls

1048576 half-offset sweeps: every low16 offset with high0/8000/ffff plus every
high16 offset with fixed lowa55a,across all four forms. This is not an exhaustive
2^32 offset Cartesian product.55296 base/register/alias/CCR boundary cases cover
zero,odd,24-bit boundary,high-bit and full32 wrap. Oracle forms the address using
unsigned64 then narrows to32,checks complete register images,RAM lanes,full bus
addresses and transfer counts.241 split states/RAM/ordered bus histories PASS.
All65536 extension words independently accepted/rejected; first-word exhaustive
guard remains. Four read/write fault cases fail-stop; subsequent run must reject
with the established logic_error contract.

During preparation the prefix test detected eight invalid continuation words
redispatching as first words. Widening the first-word mask to include internal
state bits fixed this before final testing. A fixture initially expected the
wrong exception type on failed-CPU reuse; it was corrected,not CPU behavior.

Original decoder/handlers/execute_run:200000 slices,672156 instruction reads,
37865 byte reads,25092 byte writes,46618 word reads,36910 word writes;zero differences.
Address-bound guards require all four forms,including negative/high offsets;
all prior family reachability guards remain. Deliberately dropping the high16
offset in both ignored byte-load bodies is detected at seed6/slice5. No tracked
mutation. Generated methods match locked originals plus four prior adaptations.

Candidate:65 CTest/87 Python;512 upstream-derived methods (508 unchanged,4 prior
adaptations);245 selected/19 missing/135 excluded encoding rows,not CPU percentage.
Local Release build,64 runnable CTest groups,new target and86 Python PASS;one known Windows symlink case
excluded. Remote platform/UBSan/MSan/MAME pending. Linux
UBSan includes this target and previous arithmetic/original-overflow controls.
Source/notice checks pass separately from physical timing,firmware/MIDI/audio or
plugin acceptance. M0 remains partial. See [plan](FIRST_REAPER_SOUND_PLAN.md).

Closure: merged [PR96](https://github.com/RobCZart82/yaman1x/pull/96),engine `8b1c37eb569202e394ff1caeb3f64618bae78a69`,final head `fd5d1293a21cf63c4b39293b30ca7e650702efe4`. All five jobs PASS including Linux UBSan displacement32/prior arithmetic controls;[run links](STATUS.md). Automated code review could not complete due to service usage limits;no successful review claimed. Preparation-time candidate/pending notes above are historical.
Final source inventory:483 files,201 locked upstream identities,one reviewed manifest edit;four prior generated-body adaptations separately recorded. Source/notice checks PASS. Physical timing/firmware/MIDI/audio/plugin acceptance remains open.
