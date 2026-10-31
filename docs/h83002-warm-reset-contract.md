# Pinned reset-method contract — merged research evidence

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

## CPU reset and runner boundary review — 2026-10-31 candidate

This is a review of the current local implementation, not an executed upstream
warm-reset comparison. PR #145 merged at 2026-10-30T22:25:47Z with all five CI
jobs passing. The engine implementation baseline remains #143.

`src/h8_first_handlers.h` contains full and partial generated reset handlers.
For the selected advanced H8H profile the full handler ORs I into CCR (it does
not clear the other CCR bits), sets EXR to I|NC, reads vector words at 0 and 2,
then publishes the IRQ filter, prefetches at the vector PC, advances PC by two
and completes prefetch without IRQ selection. These are local source observations;
physical reset register values and global reset ordering remain unverified.
The partial handler has distinct continuation states for completed versus retried
accesses. Setting only the CPU state number would leave old partial state and
runner bookkeeping unless an explicit transition handles them.

`H8ExecutionBudget` requires contiguous slices after its first invocation.
A slice can end with debt from an already-completed synchronous bus access;
that debt is carried into the next slice. `H8ExecutionRunner` updates the host
at the slice start before applying debt and resuming a partial instruction.
Neither class currently exposes an in-place reset operation. Reconstructing the
runner would discard its continuity/debt checks; retaining it while changing CPU
state would require a documented policy for pre-reset completed work.

Before implementing reset, tests must independently observe vector-read order,
CCR preservation, filter publication, first prefetch, every partial cut, debt and
nonzero absolute time. Peripheral reset order, retained RX/IRQ levels and pending
events must be specified separately. No instruction-boundary or arbitrary-cycle
reset API is accepted by this review. Existing cold epoch reconstruction remains
the supported recovery operation and explicitly starts a new time epoch.

## Independent cold CPU execution tests — candidate 2026-10-31

`tests/h8_reset_execution_check.cpp` checks all 256 initial CCR bytes at absolute
origins 0,1000 and 2^40,each cut from zero through six cycles (5376 split runs).
A literal expected trace requires vector word reads at origin+2 and origin+4,
filter publication after the second vector read and before the first prefetch,
and the prefetch at origin+6. The filter callback has no timestamp argument:
its recorded coordinate is the preceding bus completion,not an independently
measured filter event time. Zero continuations must not duplicate effects.

Independent assertions require CCR=input|0x80,EXR=0x7F,the vector PC and partial
state8 after six cycles,one cycle of debt at odd cuts,and exactly one next NOP
fetch at origin+8 after continuation. Snapshot/trace partition equality provides
an additional check;it is not the sole expected-value oracle.

Strict warnings and targeted ASan/UBSan pass. The pinned original generated
H8H decoder/handlers plus execute_run differential also passes 200000 slices
with zero differences. This broader existing reference comparison is distinct
from a new original whole-machine warm-reset trace;the test starts newly
constructed CPU owners. Original ROMs and generated engine bodies are unchanged.

A temporary negative-control build replacing reset CCR OR with assignment produces
6858 assertion failures;the test therefore rejects clobbering preserved flags.

## GPIO retention extension — candidate 2026-10-31

The digital-port test covers three masks (00,A5,FF),all256 DDR values and
all256 data values:196608 configurations,each reset three times. It requires
unchanged DDR and masked DR/PCR/ODR readback,zero additional output callbacks,
and a per-bit pin-read oracle with forced input40 and external pins96.
The locked original device_reset identity still matches:it calls update_output
only. This test validates the local component contract,not framework traversal,
physical hardware retention or global reset timing. Engine code is unchanged.
