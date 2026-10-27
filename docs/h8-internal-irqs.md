# H8H internal interrupt arbitration

Implementation: [PR #26](https://github.com/RobCZart82/yaman1x/pull/26).
Date: 2026-10-27. Status: standalone native tests; CPU integration NOT RUN.

`H8hInternalIrqs` adapts the internal-vector selection subset of the pinned
MAME H8H controller. It supports internal vectors 20..63 and the already
qualified NMI latch at vector 7. The exact source hashes and authorship are in
`research/h8-internal-irqs-provenance.json`; BSD-3-Clause terms remain in
`licenses/MAME-BSD-3-Clause.txt`. It is not the complete interrupt controller.

## Ownership and semantics

All pending bits, priority bits and filters belong to the instance. The owner
invokes its DMA trigger before `raise()` and passes whether that request was
consumed. Consumption prevents a new IRQ from being added and does not clear
an earlier pending request. The arbiter does not emulate DMA.

Selection follows upstream `update_irq_state()` and H8H `get_priority()`:
ICR slots use the original `slot ^ 7` bit mapping, NMI has ICR priority two,
and H8H IPR priority is zero. Eligibility uses `icr >= icr_filter` and
`ipr > ipr_filter`. An IPR filter of -1 selects ICR arbitration levels;
other accepted negative filters retain upstream IPR-level behavior. Equal
levels choose the lowest vector. Selection does not acknowledge a request;
`taken()` clears the specified pending bit. Duplicate requests coalesce.

`reset()` clears pending/priority bits, preserving the CPU-supplied filters
as the upstream reset does. A fresh object starts with zero filters. The owner
must update filters from its CPU state; a default IPR filter of zero excludes
all requests under this H8H priority rule, including NMI. This is upstream
eligibility behavior, not a claim about physical hardware.

## Boundary deliberately left to the future controller

- No IER/ISR, ISCR or byte-mapped ICR access. `set_priority_bits()` accepts an
  already assembled priority value; it is not a replacement MMIO handler.
- No external vectors 12..19, pin levels or edge latching. Unsupported vectors
  are rejected before shifting or mutating pending state.
- `latch_nmi()` means the owner has already qualified the edge and nonzero
  machine-time condition. There is no invented NMI timing or pin polarity.
- `select()` is a pull interface. A future owner must publish the returned
  selection to the CPU at the correct scheduling point. No callback frequency
  or cycle equivalence is claimed by this component.
- No CPU execution, peripheral timing, save-state/NVRAM, reset line handling
  or firmware behavior is implemented or changed.

The subset avoids importing the full MAME framework and avoids copying the
upstream ICR byte-write implementation into a new MMIO contract before that
behavior is reviewed. There is no change to the separate research MAME target.

## Validation

Native tests check all eleven internal ICR slot groups against all 44 internal
vectors; all 990 unordered vector pairs for tie-breaking/acknowledgement/NMI;
higher priority beating a smaller vector; DMA consumption; instance isolation;
filter transitions; vector 63 unsigned-bit safety; reset semantics; invalid
vector rejection without pending-state mutation. Local Windows MSVC Release
build/test PASS. Final platform checks are recorded in the implementation PR.

These are synthetic contract tests, not a full upstream-device differential
or hardware validation. Next: connect a complete interrupt-controller adapter
to CPU filters, external pins and scheduling; then attach a timed SCI model
and prove actual byte receipt separately from IRQ selection.
