# H8H long-data absolute memory moves

Four original MOV.L abs16/abs32 load/store groups, eight unchanged full/partial
handlers, bring the selected total458->466. The existing0100 prefix state
selects each form with exact ER0..7 guards. Abs32 long instructions need four
instruction words, so CPU/snapshot/reference IR storage is enlarged3->4.
Encoding inventory225 selected/39 missing/135 excluded (not CPU percentage).

Abs16 addresses retain signed extension. Abs32 retains the full32-bit bus
argument; physical24-bit mapping remains host-owned. The original transfer is
two big-endian word accesses, high first then low, each aligned independently.
Address+2 wraps in uint32 before the second access. A fault is fail-stop and
already committed word effects remain. Original selected bodies are unchanged.

Independent native tests exercise all65536 abs16 addresses in both directions,
each ER register,32-bit sign/zero boundaries at abs32 boundary addresses and
three CCR patterns. Non-target ER registers and non-NZV CCR bits are preserved.
Every201 split compares full state/RAM/bus histories for a mixed program.
First/second store faults verify committed write counts. Invalid prefix ER
neighbors are rejected. A dedicated reachable original-reference loop requires
all four new forms and preserves previous family reachability guards.

Limits: native abs32 addresses are boundary samples, not all2^32 addresses;
word store-fault tests do not prove arbitrary hardware bus-error recovery.
Displacement addressing and other missing instruction groups remain open.
No original ROM modification, full machine map, firmware boot or audio claim.
Own final-head cross-platform CI/review required.
