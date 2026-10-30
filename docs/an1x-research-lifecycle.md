# Cold research machine lifecycle

Recorded:2026-10-30. Merged #143 as `301eec75c72028b8f377ee4e099fce0b1df4de5e`;tested head `d2da27c15c8ebf8c083f35bb4a2cef18eba4d8c4`.
All five final-head CI checks PASS:41127313949/41127314006/41127313987.
No recorded automated review result;none claimed.
 `An1xResearchMachine<Sink>` owns immutable
copied logical ROM and one stable heap epoch:bus first,CPU second,CPU bound to the
same bus IRQ owner. No internal reference escapes;objects are noncopyable/nonmovable.
Sink must outlive the machine. No threads or mutating callback reentry.

`run(cycles,limit)` advances an owned logical slice cursor,checking negative/
overflowing ranges before execution. CPU bus accesses can overrun a slice;existing
debt handling remains within that epoch. Callback-limit/CPU/bus/reentry failure
requires discard/reconstruction;failed epochs cannot resume. Snapshots and NVRAM
exports are values. No filesystem,ROM rewriting,GUI or DAW ownership is added.

## Cold restart contract

`cold_restart(preserve|clear,replacement_rx_edges)` constructs an entirely new
CPU/bus/SCI/IRQ/GPIO/SYSCR/internal-RAM epoch before swapping it in. Preserve loads
an exact copied256KiB NVRAM image;clear leaves new NVRAM zero. Unknown policy or
invalid replacement input/allocation failure leaves old epoch,clock and generation
unchanged. Even a failed epoch may be reconstructed,with explicit NVRAM policy.
The private immutable ROM is reused as input,not patched. New generation increments
and logical time starts0. Old partial CPU state/debt and pending RX schedule are
discarded. Replacement edges are copied and relative to new epoch0;default empty
means no RX input. Invalid unsorted/channel input cannot destroy the old epoch.

Sink history/external state is caller-owned and not cleared;timestamps repeat
from0 after restart. Consumers must separate generations (query generation()) or
clear their history. No callbacks during candidate construction/NVRAM loading,
so replacement validation cannot publish an invented reset waveform. This does
not reset real hardware,input devices,LCD,DSP or an audio endpoint. No save-state
or continuous-time warm reset semantics claimed.

## Pinned original boundary

Original MAME commit706dbb73281320c54d7c2f16262c1c3786f2be01 was inspected again:
`h8_port.cpp::device_reset` only republishes output and retains port registers;
`h8_sci.cpp::device_reset` resets registers/clock state and emits clock/TX high;
`h8h_intc_device::device_reset` calls base reset and clears ICR. Original identities
remain in research/h8-source-inventory.json and component provenance. These
methods are not replaced by a claim of warm-reset equivalence. Cold reconstruction
is a project lifecycle using existing tested constructors;physical warm reset,
retention rules and initial electrical publications remain separate open work.
No original body or ROM payload is copied into tracked code in this increment.

## Independent acceptance

401 cut points0..400 restart CPU in partial/debt/peripheral states. Check fresh CPU
snapshot,zero internal RAM,SYSCR09,IRQ disabled,SCI BRRFF,time0/generation increment,
NVRAM preservation,then CPU snapshot/TX waveform equality with an independent fresh
machine. Full256KiB deterministic NVRAM pattern survives8 restarts;explicit clear
zeros all bytes. Invalid RX replacement preserves an active old epoch;failed CPU
and suppressed callback reentry recover only by reconstructing. Caller ROM mutation
cannot alter later execution;machine owners remain isolated. Old RX consumption
is discarded;copied replacement input begins at epoch0 despite caller mutation.
Ignoring preserved-NVRAM reload in an ignored mutant causes410 expected failures.
Tracked code unchanged. No MAME whole-machine reset differential/hardware trace.

Full88 CTest/87 Python and targeted ASan/UBSan PASS. Linux UBSan includes the new
target. Final-head CI PASS;review status recorded above.
Source notices are not legal/binary release clearance. Warm hardware reset and
firmware/MIDI/PKS/DSP/audio/plugin/REAPER acceptance remain open.
