# ROM-free audit follow-up

Baseline: `d242372929083fc8626c6cea0e346d555bc7e308`.

| Finding | Disposition |
|---|---|
| ctypes calls inside Python assertions disappear under `-O` | Both ABI scripts now use explicit checks that execute and fail under `-O`; CTest runs both normal and optimized variants. |
| AsmJit glob includes upstream `*_test.cpp` files | Excluded from the runtime source list, without editing vendored sources. |
| THIRD_PARTY patch count | Corrected from seven to ten, matching `research/*.patch`. |
| VOP3 initialization / `memcmp` | Open upstream risk; not claimed fixed. |
| EG target hold | Unchanged; no AN1x hardware evidence supplied. |

In `third_party/vop3/src/yss236_core.h`, the constructor calls `decode_all()`
before initializing `ops`; `decode(int)` compares the newly decoded operation
with the previous `ops[st]` using `memcmp`. The first comparison therefore reads
uninitialized object representation. The stale flag already starts true; an
audible failure or divergent execution has not been demonstrated. A focused
MemorySanitizer experiment and an explicit initialization/field comparison fix
should precede any reviewed vendor patch. ASan and ordinary successful builds
alone do not clear this risk. Keep the recorded upstream snapshot unchanged
until that follow-up has its own provenance and regression evidence.

EG timing checks characterize upstream behavior and transport only. They do
not establish target-hold fidelity, the AN1x clock, or real firmware boot.
