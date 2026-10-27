# VOP3 first-decode initialization

Date: 2026-10-27. [PR #22](https://github.com/RobCZart82/yaman1x/pull/22).
Scope: a constructor-time uninitialized read; no AN1x hardware/EG change.

The reviewed upstream core called `decode_all()` before initializing its
`ops` array. The first `decode(int)` passed that array to `memcmp`.
The standalone Clang 18 MemorySanitizer control reproduced an uninitialized
read at `yss236_core.h:144`, called by `decode_all()` and the constructor:
[original/candidate run 40461084899](https://github.com/RobCZart82/yaman1x/actions/runs/40461084899),
revision `44d5e8c8d2708e6736ce88e8cbe48be67a7b314c`.
The otherwise identical candidate initialized `ops` and passed.

The source change is `op ops[STEPS];` to `op ops[STEPS]{};`. Each element is
initialized before the constructor body. The existing decoded representation,
comparison, stale flag, DSP arithmetic and EG implementation are retained.
No audible divergence, hardware discrepancy or full-engine MSan cleanliness is
claimed. The flag already began true, so an ordinary passing output test alone
was insufficient evidence about the invalid read.

## Reversible provenance

The original archive and base upstream commit identities remain recorded.
`third_party/patches/vop3-initial-decode.patch` is the exact unified diff.
`third_party/provenance.json` records the reviewed file hash/size, the original
hash/size, the single reversible replacement and the patch hash.

The offline repository auditor checks current file hashes, the patch hash and
reverse applicability, reconstructs the original bytes, verifies the original
hash/size and checks that the diff exactly describes that reconstruction.
There are now **200 unchanged upstream files and one reviewed local adaptation**
in the 201-file manifest. The legacy `upstream_files_verified` count refers to
manifest file checks; `reviewed_local_edits_verified=1` identifies the adaptation.
This exception is explicit rather than relabeling patched bytes as upstream.

## Regression and limits

The standard native test uses the actual linked VOP3 library: repeated
construction/destruction, all 512 decoded steps, synthetic uploads, repeated
decode and two independent cores. The separate MSan job reconstructs the exact
hash-locked original and requires its expected `memcmp` diagnostic, then tests
the actual reviewed header and requires a clean result. Only explicit MSan
shadow-mapping startup failures may be retried; findings/crashes are not ignored.

The standalone MSan probe does not allocate/execute JIT code and supplies a
null-only JIT deleter stub; it cannot certify the JIT or audio path. Standard
platform tests link the real implementation. The final PR checks record those
Linux/macOS/Windows and sanitizer results separately.

M0 still needs the H8 port and instance/stepping acceptance; M1 still needs a
fresh local firmware/SCI/MIDI experiment. EG target hold is unchanged and
continues to require AN1x hardware evidence before a behavioral fix.
