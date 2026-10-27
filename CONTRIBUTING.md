# Development and review records

Keep changes reviewable in a pull request. Run the checks appropriate to the
changed code and report what they actually establish.

Every development/fix PR must update the [development ledger](docs/DEVELOPMENT_LOG.md)
and its [structured records](docs/development-ledger.json). Add a dated entry
with the problem, resulting behavior, affected milestone, validation and
remaining limitations. Documentation-only PRs should identify that scope.
Use Europe/Budapest dates in narrative records; preserve GitHub UTC timestamps
in structured merge metadata.

Before merge, record the PR and reviewed head when known and leave unknown
merge/result fields explicitly pending. At the next ledger reconciliation,
record the actual merge SHA and final check links. Never predict a squash SHA
or reuse another revision's CI result. Preserve old evidence as dated history;
append corrections and update current status without rewriting an old outcome
as if it had been observed then. Historical imported metadata is distinguished
from newly executed checks in the audit report.

If readiness or dependencies change, update `docs/STATUS.md`, the applicable
milestone in `docs/FIRST_REAPER_SOUND_PLAN.md` and relevant component notes in
the same PR. The plan retains detailed milestone acceptance and historical
CI closures; the ledger provides the cross-project index. Avoid duplicating
long raw logs across those documents.

Run the ROM-free build/CTest suite for runtime changes, focused regressions for
fixes, and the source/notice audits for candidate uploads. Research patch changes
also require locked-source preparation and the separate MAME build where
applicable. The three base CI jobs are required by the ruleset; the separate
MSan and path-filtered MAME workflows provide additional scoped evidence.
A green source build is not proof of firmware/MIDI/audio/REAPER acceptance.

Keep Yamaha inputs, private traces, audio, personal paths and generated binaries
outside tracked files. Review imported source identities and notices before
changing provenance/coverage manifests. Refer to `THIRD_PARTY.md` and `LICENSE`;
the project-wide distribution license remains unselected.

The PR template assists this process. Ledger completeness still requires human
review; no automatic guarantee of per-PR log updates is claimed.
