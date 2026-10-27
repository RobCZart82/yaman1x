# H8 SCI status-latch extraction

Date: 2026-10-27. Baseline: main `019070d95f82ac4efd011c4b1402da792ad99088`.

`src/h8_sci_status.h` extracts the SSR flag and read-latch rules from the
pinned MAME SCI source identified in `research/h8-sci-status-provenance.json`.
It uses the existing BSD-3-Clause license and Olivier Galibert attribution.
This is a prerequisite for SCI byte-state integration, not a functioning UART.

## Contract

- A side-effecting SSR read snapshots the flags. A debugger read leaves that
  snapshot alone. A write can clear observed receive/error flags, while newly
  raised, unobserved flags survive. Software writes cannot set those flags.
- TDRE acknowledgement requires TE enabled, an observed TDRE, and writing its
  bit as zero. This clears both TDRE and TEND. Software cannot otherwise clear
  TEND. MPB is preserved; MPBT is directly software writable.
- Hardware raises flags through `raise_flags`; it cannot set MPBT. The future
  serial owner must supply the actual state-machine events and flag timing.
- A DMA transmit write clears TDRE/TEND only with TE enabled. A side-effecting
  DMA receive read clears RDRF. These qualified methods do not model DMA bus
  arbitration, TDR/RDR storage or transfers.
- Upstream `device_reset` sets SSR to 0x84 but does not reset `ssr_read`.
  `reset` preserves that behavior. Fresh construction initializes the latch
  to zero, matching the upstream constructor. This is preserved reference
  behavior, not newly verified AN1x hardware behavior.

After an SSR write the owner must perform upstream `tx_start` when idle and
TDRE is clear, then `sync_rx_start`; a successful `tx_start` raises TDRE again.
After a DMA transmit write the owner must similarly start idle transmission.
This component intentionally exposes the intermediate flag state; it must not
be installed as complete SCI MMIO without those owner actions. IRQ delivery,
register address decoding, byte framing, receive error generation and the
cycle-clock integration remain open. No fabricated received bytes are used.

## Verification

The native Release test covers **33,554,432 transitions**: every initial SSR
byte, newly raised flag mask, software write byte, and TE state. A per-bit
oracle independently checks the resulting SSR, read latch and error predicate.
Explicit sequences check read-before-clear, debugger access, TE gating, DMA,
reset and instance isolation. These tests execute without C++ assertions and
without Yamaha ROMs. Local Windows MSVC Release: PASS, zero failures.

The final tested revision and Linux/macOS/Windows results are recorded on the
associated PR checks. This is a register-rule test; complete byte-state,
CPU, firmware/MIDI and hardware acceptance have not run for this component.

## Next acceptance gate

Implement TX/RX framing and the data registers with actual line-level events,
connect this status latch and the internal cycle clock, and verify byte and
interrupt sequences before connecting the H8 CPU. Preserve upstream semantics
unless a separately documented hardware experiment warrants a change.
