# ROM-free audit follow-up

Baseline: `d242372929083fc8626c6cea0e346d555bc7e308`.

| Finding | Disposition |
|---|---|
| ctypes calls inside Python assertions disappear under `-O` | Both ABI scripts now use explicit checks that execute and fail under `-O`; CTest runs both normal and optimized variants. |
| AsmJit glob includes upstream `*_test.cpp` files | Excluded from the runtime source list, without editing vendored sources. |
| THIRD_PARTY patch count | Corrected from seven to ten, matching `research/*.patch`. |
| VOP3 initialization / `memcmp` | Reproduced under MemorySanitizer and locally fixed with value initialization; [evidence and provenance](vop3-initial-decode-fix.md). |
| EG target hold | Unchanged; no AN1x hardware evidence supplied. |

In the original `third_party/vop3/src/yss236_core.h`, the constructor calls `decode_all()`
before initializing `ops`; `decode(int)` compares the newly decoded operation
with the previous `ops[st]` using `memcmp`. The first comparison therefore reads
uninitialized object representation. The stale flag already starts true; an
audible failure or divergent execution has not been demonstrated. The focused
MemorySanitizer experiment now reproduces that read; the reviewed adaptation
initializes the array and retains an exact reversible patch and original hash.
This closes the first-decode read finding, not all possible engine/JIT risks.
ASan and ordinary successful builds alone would not have cleared the finding.

EG timing checks characterize upstream behavior and transport only. They do
not establish target-hold fidelity, the AN1x clock, or real firmware boot.
