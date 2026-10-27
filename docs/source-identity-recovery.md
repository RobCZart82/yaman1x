# Source identity recovery and full research-build validation

Date: 2026-10-27. Baseline `291e04554c59d172a1ac6cc73b3d8398d89c5e0c`.
This advances the M0 source lock and the M1 research-build prerequisite.

## Recovered source identities

The original archive SHA-256 values in `source-lock.json` were rechecked.
ZIP comments supplied candidate upstream commits. Those comments alone are
untrusted metadata; the following independent content checks establish scope:

| Source | Recovered Git commit | Verification |
|---|---|---|
| VOP3 | `a3c8182811c398efd41daadf78d6fabcf50af56a` | Recomputed Git blob/tree objects for the complete ZIP, including file modes, yielded `89a324b2602b9f91af013483b7c6bdc09c80ee60`, matching the official commit's root tree. No Yamaha files were extracted, published or added to the project. |
| MAME | `706dbb73281320c54d7c2f16262c1c3786f2be01` | All four original patch input files were compared byte-for-byte with official files at that commit. SHA-256 locks are in `research/mame-source-lock.json`. |

Official commit evidence: [VOP3](https://github.com/musicastudio/VOP3/commit/a3c8182811c398efd41daadf78d6fabcf50af56a),
[MAME](https://github.com/mamedev/mame/commit/706dbb73281320c54d7c2f16262c1c3786f2be01).

MAME's raw ZIP tree differs from the commit tree. At least some differences
are archive line-ending conversions (`uismall.bdf` becomes identical after
CRLF-to-LF normalization); full archive equality has **not** been established.
The new build uses a clean checkout of the official commit, verifies all four
patch inputs, and does not assume the ZIP comment proves the whole source tree.
Archive hashes, commit identities and Git trees remain separately recorded.
The vendored VOP3/AsmJit file contents and all 201 provenance hashes are unchanged.

## Repeatable patch preparation

Check the original local archive without changing it:

```sh
python3 tools/prepare_mame_research.py --archive /absolute/path/to/mame-master.zip
```

Only the four reviewed source-text inputs are extracted temporarily. All eleven
patches are applied in order, output paths are checked against the reviewed
set, and all four injected helper headers must equal the project sources.
No firmware, ROM or bank member is extracted by this command.

For a full source build, create a separate checkout of the pinned official
commit, then prepare it:

```sh
git clone https://github.com/mamedev/mame.git /absolute/path/to/research-mame
git -C /absolute/path/to/research-mame checkout 706dbb73281320c54d7c2f16262c1c3786f2be01
python3 tools/prepare_mame_research.py --source /absolute/path/to/research-mame
```

The command requires that checkout root, exact revision and a clean worktree.
A complete temporary preflight precedes source writes, so a late patch failure
does not partially apply the chain to the checkout. A repeated invocation on
an already prepared checkout is rejected as dirty; use a separate fresh
checkout rather than resetting a working research tree.

## Separate ROM-free MAME CI

`research-build.yml` checks out the exact MAME revision on macOS arm64, verifies
and prepares the patch chain, builds the complete research executable and runs
`-listfull an1x`. This is separate from the three-platform core/helper CI.
The research build uses `OPTIMIZE=0` to limit validation build cost; it is not
a performance benchmark or the product toolchain. The job records compiler,
Xcode, Python and Make versions. It neither acquires Yamaha inputs nor runs
firmware nor uploads the executable as an artifact.

Validation status and the implementation PR's CI checks must distinguish:

- source/patch preparation and synthetic regression tests;
- complete research executable compilation and ROM-free registration smoke;
- actual local firmware execution and SCI/MIDI observations.

The third item remains **NOT RUN** for the new SCI access patch. M1 is still
in progress, M0's H8 port/isolation work remains open, and EG target hold is
unchanged. Follow [the M1 experiment](m1-sci-access-milestone.md) after a
successful full build; successful machine registration does not prove boot.
