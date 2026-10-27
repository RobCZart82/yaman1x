# H83002 SCI register and IRQ adapter

Date: 2026-10-27. Baseline: main `276399870400bd81272c21889257984651c816ed`
(#37). This is a component integration milestone under M0. The full CPU,
machine scheduler, PKS/MIDI firmware acceptance and audio remain open.

`src/h83002_sci_mmio.h` owns two reviewed asynchronous SCI channels and connects
their interrupt requests to a machine-owned `H8hIrqController`. The original
BSD-3-Clause/Olivier Galibert notices are retained. The map and vectors come
from the pinned MAME H83002 source; identities and scope are recorded in
`research/h8-sci-mmio-provenance.json` and the license-coverage manifest.

## Register and interrupt wiring

| Channel | SMR | BRR | SCR | TDR | SSR | RDR | ERI / RXI / TXI / TEI |
|---|---|---|---|---|---|---|---|
| SCI0 | `0xFFFFB0` | `0xFFFFB1` | `0xFFFFB2` | `0xFFFFB3` | `0xFFFFB4` | `0xFFFFB5` | 52 / 53 / 54 / 55 |
| SCI1 | `0xFFFFB8` | `0xFFFFB9` | `0xFFFFBA` | `0xFFFFBB` | `0xFFFFBC` | `0xFFFFBD` | 56 / 57 / 58 / 59 |

Only the advanced 24-bit address view is supported. Holes, unrelated addresses,
20-bit aliases and unaligned words are rejected before dispatch. Byte accesses
forward to the existing owner. RDR writes are ignored, matching its read-only
mapping. Sync/MP framing and external clocks remain explicitly unsupported.

Aligned words select high/even and low/odd bytes in big-endian order. Only zero,
high-byte, low-byte and full-word masks are accepted. Partial-bit masks are
rejected. Unselected read lanes return ones without reading the register, as
this adapter's contract; this does not model a full machine's unmapped bus.
Zero-mask writes have no effects.

Reads separate debugger side-effect suppression and qualified DMA access.
Ordinary RDR reads retain RDRF; a qualified side-effectful DMA RDR read clears it.
An SSR/RDR word reads SSR first, then RDR. A CPU TDR write queues data and its
SSR acknowledgement starts TX. DMA-qualified TDR writes preserve TE gating
and the existing immediate-start rule.

## Owner and event contract

The owning IRQ controller must outlive the adapter. Copy and move are disabled
to prevent copying UART state while silently sharing that controller. Configure
the controller's CPU filter explicitly; the adapter does not invent filter state.

The sink provides `tx(channel, level)`, `clock(channel, level)` and
`bool dma_request(vector)`. Each interrupt first queries the DMA hook, then
publishes an unconsumed request through `internal_irq`. Consumption does not
remove a previously pending IRQ. This hook is not a DMA engine; actual transfers
and qualified bus accesses remain the machine owner's work. Callbacks must not
re-enter/reconfigure the channel or acknowledge IRQs.

The owner supplies total cycles for register-triggered TX and the now-as-cycles
view for RX transitions. `next_event(channel)` and the minimum nonzero
`next_event()` expose deadlines; `service_one` services a selected channel once.
A bounded scheduler must drain these and define tie ordering with CPU, lines
and other peripherals. That full scheduler is not implemented here. Tests use
wire-before-edge and channel-0-before-channel-1 ordering as fixture conventions.

`reset_channel` resets that UART and publishes idle TX/clock levels. It does
not reset the controller or clear pending IRQ latches. Full machine reset must
handle the controller and reapply filters separately. Physical RX and SSR
read-latch retention follow the existing channel. Callback/clock-overflow
failures can leave partial progress; discard/reinitialize the owner afterward.
No save-state or real-time safety is claimed.

## Validation

Local macOS arm64, AppleClang 21, CMake Release:

- 512 RX cases: all 256 8N1 bytes through each channel's MMIO/RDR and RXI.
- CPU SSR-driven TX waveform/cycles, TXI/TEI, framing/overrun ERI,
  DMA qualification/consumption and disabled-TE behavior on both channels.
- Byte lanes, read-only RDR, zero/invalid masks, invalid addresses, pre-mutation
  mode rejection, debugger/DMA semantics and clock pin routing.
- Shared IRQ arbitration, distinct deadlines and reset/machine isolation.
- Full ROM-free suite: 23 CTest groups PASS on the baseline plus this candidate.
  Existing 4,096-format TX/RX/duplex checks remain separate unchanged tests.

The new native regression also passes AddressSanitizer/UndefinedBehaviorSanitizer.
All data are synthetic. Final PR-head platform CI is separate evidence, visible
on that revision's checks; these tests do not execute Yamaha firmware.

## Next gate

Build the common CPU/peripheral scheduler and complete the CPU port's framework
and generated-source boundary. Exercise instructions, interrupt taking and bus
timing before claiming a CPU-connected port. M1 must prove firmware byte/note
handling separately. The MAME patch chain and DSP/EG profiles are unchanged.
