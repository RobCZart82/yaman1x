# H8H absolute byte/word memory moves

Six original groups: MOV.B abs8/register and register/abs8, plus MOV.B/W
abs16/register and register/abs16. Twelve original full/partial handlers are
selected unchanged from the locked h8.hxx (438 ->450 total). The flat CPU
dispatch adds guarded first-match exits; unsupported neighbors stay rejected.
Coverage encoding inventory:217 selected /47 missing /135 excluded. This is
not a CPU completion percentage. Abs32, displacement and long absolute forms
remain missing.

Abs8 uses the original top-page address expression, then the host's24-bit map.
Abs16 uses signed16 extension; word accesses align down in the existing bus
adapter and preserve big-endian order. NZV reflects the transferred value;
other CCR bits survive. Byte loads preserve the opposite register lane, word
loads support both16-bit banks. Store/load fault propagation remains fail-stop.

Independent native evidence covers abs8 all16 byte lanes/all256 values with
three CCR patterns, and all65536 abs16 encodings at byte/word widths in both
directions, including sign boundary and odd-address alignment. The odd997
word-value multiplier also traverses every16-bit value. A mixed program is
compared at every201 split position; all six groups have injected data faults.

A dedicated reachable original-reference loop at seed%16==13 exercises all six
forms. A fetch guard requires all forms before the comparison can pass. The
original generated decoder, selected methods and execute_run are used; bus,
register, flag, partial state and memory histories are compared. Existing
CCR/unary/small/TRAPA reachability guards remain intact.

No original ROM mutation or new hardware behavior claim. Full CPU, remaining
address forms, machine mapping, firmware boot and audio acceptance remain open.
Own final-head cross-platform CI/review required; no multi-milestone hold.
