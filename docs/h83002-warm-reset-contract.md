# Pinned reset-method contract — research candidate

Recorded:2026-10-30. Integrated engine baseline #143;no warm reset API added.
This preparation establishes local source identities and method-local effects,
not global reset ordering or physical AN1x behavior.

`research/h83002-reset-contract.json` pins five files and six device_reset method
bodies at original MAME706dbb73281320c54d7c2f16262c1c3786f2be01. Whole archive SHA256
is checked before ZIP content is read,using research/mame-source-lock.json.
No original body or ROM payload is copied into tracked code.

Reproduce:

```
python3 tools/h8_reset_contract.py --archive /absolute/path/to/mame-master.zip --check
```

Without --check,the tool prints the report;--output writes a new file exclusively.
It reads only fixed members into memory,never extracts or modifies the archive.
The method matcher is formatting-sensitive lexical evidence,not a C++ parser.
Full file hashes detect surrounding changes;method hashes isolate observed bodies.
`h8h.h` and h8h.cpp have no device_reset override in the inspected source;this is
an inspected inheritance boundary,not a new whole-framework closure claim.

## Method-local matrix

| Original method | Explicit effects | Not established by that body alone |
|---|---|---|
| h8_device::device_reset | Sets cycles_base from machine time;STATE_RESET/substate0;clears requested/taken IRQ,DMA/DTC and standby pending;publishes standby0 | Generated reset instruction effects/register values,global time continuity/framework counters |
| h83002_device::device_reset | Calls parent reset;sets SYSCR09 | When the new SYSCR filter becomes published relative to child resets |
| h8_intc_device::device_reset | Sets level modes/falling NMI;clears pending,ISCR,IER,ISR;then check_level_irqs(false) | Final ISR need not stay zero under held physical levels;whole CPU publication ordering |
| h8h_intc_device::device_reset | Calls base reset;clears ICR | Does not itself establish filter or last-publication reset policy |
| h8_sci_device::device_reset | RDR/SMR/SCR0,TDR/BRR/TSR FF,SSR84;clears shifter/phase/clock state;clock_update;emits clock high then TX high | External RX/read-latch retention across every owner reset;absolute event rebase/global pin order |
| h8_port_device::device_reset | Calls update_output only | It does not clear DDR/latch/PCR/ODR in this method;output notification may be suppressed if unchanged |

RAM/NVRAM/ROM reset retention is not proved by absence of assignments in these
methods. Bus maps,driver/framework memory lifecycle and battery semantics require
separate evidence. Cold reconstruction in #143 deliberately has a different
contract:zero internal state,new event epoch,explicit retain/clear NVRAM.

## Implementation gates before in-place reset

1. Verify generated CPU reset and runner/debt behavior,then define supported
   reset boundary:stopped at a valid bus/event point versus mid-instruction.
2. Review owner/device reset traversal order and original timestamp/frame-counter
   interactions;choose explicit event-time/input policy without calling it a
   measured hardware order. Never implicitly rewind an active event timeline.
3. Test SCI transmission/reception interrupted by reset,including high-output
   publication and held RX;test GPIO retained registers and notification suppression.
4. Test IRQ/NMI physical levels,relatched ISR,ICR/filter republishing,CPU CCR/SYSCR
   and pending requests. Source-method identity alone does not validate these.
5. Validate RAM/NVRAM persistence,instance isolation,repeated reset and callback
   reentry/failed-owner behavior. Keep cold recovery API distinct.

The existing component reset tests remain evidence for those components;this
candidate adds no replacement implementation. A whole-machine MAME reset trace
and physical hardware trace are NOT RUN. No new firmware/audio acceptance.

## Tool validation

Four independent Python controls check changed body identity,nested braces under
original indentation,missing/duplicate methods,non-reset substitution and rejection
of wrong archive hash before ZIP parsing. Real locked archive report matches all
six methods. Full Python suite91 tests PASS;engine evidence stays #143 (88 CTest/87 Python),
not rerun for this research-only increment. Final-head CI/review pending.
