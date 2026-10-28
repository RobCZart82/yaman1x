# H8H byte/word MOV with absolute32 address

Four original H8H groups add MOV.B/W abs32 load/store. Eight unchanged original
full/partial handlers bring selected methods450->458. Guarded flat dispatch
adds64 first-word encodings. Encoding inventory221 selected/43 missing/135
excluded; no CPU completion percentage is inferred.

Both extension words are fetched separately and partial execution resumes at
the original states. The complete32-bit operand is passed to the host bus;
the synthetic host maps it to24-bit physical memory. Word accesses align down
before mapping and use big-endian order. No new mask is inserted into original
handlers. Byte register lanes, word banks and NZV/remaining CCR preservation
follow the existing contracts. Long data width and displacement forms remain
missing even though byte/word abs32 addressing is now supported.

Independent evidence:15 boundary addresses from0 through0xffffffff across
all16 register lanes/banks, seven value patterns and three CCR seeds. Each of
four width/direction forms also samples65536 distinct addresses: low16 bits
traverse their full range while high16=(low16*73) modulo65536 also traverses
its full range. These are65536 correlated32-bit samples, not all2^32 addresses.
Every16-bit word value is traversed by the odd997 multiplier. A mixed six-byte
instruction program compares every201 split point; all four groups inject
bus faults. Data, flags, raw bus addresses and physical mapping are checked.

A dedicated reachable original-reference loop (seed%16==12) exercises all four
forms, with a fetch guard required for success. Existing abs8/abs16 and earlier
family reachability guards remain. Original decoder/execute_run comparison
provides separate evidence for extension fetch and bus ordering.

No ROM edits, production machine map, full CPU, firmware boot or audio claim.
Own final-head cross-platform CI/review required; no multi-milestone hold.
