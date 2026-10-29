# H8H signed byte and word multiply

Candidate based on merged PR83, engine baseline PR82. Selects unchanged original
01c0 prefix dispatch and MULXS.B/MULXS.W full/partial bodies. Guarded continuation
state0xe0000 accepts only5000/ff00 and5200/ff08. Reserved word destination bit3
and every unrelated prefix extension fail-stop; there is no fallback decoding.

Signed low destination byte/word and source byte/word are read before write;
result occupies the destination16/32-bit register. Original set_nz16/set_nz32
helpers change N/Z only, preserving V,C,H and interrupt bits. Signed word products
lie within[-1073709056,1073741824], so their signed32 multiplication cannot overflow.
Fixed H8H m_has_mac=false selects original11/19 internal cycles plus host charge.
No DSP,EG,MAC/H8S profile, data accesses or Yamaha inputs are introduced.

## Evidence

Independent signed64 oracle uses mathematical sign conversion, not the candidate
casts:65536 byte operand pairs,327680 word sweeps,28800 register/alias/CCR boundary
cases,20000 deterministic random word pairs. Whole register images, all CCR bits,
absence of data access,241 split snapshots and ordered instruction/event traces
are checked. All65536 signed-prefix extension words are accepted/rejected by an
independent mask expectation, alongside the existing exhaustive first-word guard.

Original decoder/handlers/execute_run:200000 slices,692193 instruction reads,
37350 byte reads,24577 byte writes,54220 word reads,41309 word writes;zero differences.
Both signed forms must be reached and all prior family reachability guards remain.
The prior unsigned-word reference restriction remains: safe bounded original
products, with high products independently checked by its own arithmetic oracle.
A deliberate signed-word product+1 change in both ignored candidate bodies is
detected at seed9/slice14. No mutation remains in tracked source.

Candidate suite:59 CTest groups/87 Python cases;488 upstream-derived methods,
486 unchanged and the2 prior unsigned-word widening adaptations. Inventory:
235 selected/29 missing/135 excluded encoding rows, not CPU completion percentage.
Local Release build,58 runnable CTest groups and86 Python cases PASS (one known Windows symlink privilege
case excluded); remote platform/UBSan/MSan/MAME results are pending.
Linux UBSan now runs both signed and unsigned-word arithmetic tests, retaining
the required original unsigned-promotion negative control. Generated selections
match pinned outputs plus the two explicitly reviewed prior adaptations; source
and notice audits PASS. This is source/synthetic evidence, not physical timing,
firmware boot,MIDI,audio or plugin acceptance.

See [plan](FIRST_REAPER_SOUND_PLAN.md) and [status](STATUS.md).
