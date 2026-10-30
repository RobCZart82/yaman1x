# H83002 SYSCR mode contract — standalone candidate

Recorded:2026-10-30;engine baseline #136,documentation baseline #138.
`H83002SyscrModes` is a project-owned pure contract,not CPU/host integration.
FFFFF2 remains unmapped;the CPU still uses its fixed profile. No reset,standby,
MMIO storage,firmware or audio acceptance is added.

Pinned MAME commit706dbb73281320c54d7c2f16262c1c3786f2be01:
`src/devices/cpu/h8/h83002.cpp` SHA256
`2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92`.
Read directly from the locked original archive. `device_reset` assigns09;
`syscr_w` stores all8 bits and immediately calls `update_irq_filter`.
This differs from the existing research CPU's stated fixed0B profile;09 and0B
have equal bit3 modes,but the bytes must not be misreported as identical.

## Exact mode equations

Filter publication always uses IPR argument -1. Table gives ICR threshold:

| SYSCR bit3 | I=0,UI=0 | I=0,UI=1 | I=1,UI=0 | I=1,UI=1 |
|---|---|---|---|---|
| 0 | 0 | 0 | 1 | 2 |
| 1 | 0 | 0 | 2 | 2 |

IRQ and TRAPA entry set I. If bit3 is0,they also set UI. They preserve all
other CCR bits,including an already set UI in bit3=1 mode. TRAPA returns base8.
The component computes values without publishing to or mutating an IRQ owner.
Other SYSCR bits do not affect these three original methods;this does not mean
those bits have no hardware role or that standby is supported.

## Evidence

All65536 SYSCR/CCR pairs checked against an independent threshold table and
per-bit entry oracle,including entry idempotence and unrelated SYSCR-bit changes.
Full86 CTest/87 Python PASS;targeted ASan/UBSan and strict compiler warnings PASS.
Linux CI includes this target under UBSan. An ignored fixture compiles the three verbatim original
methods (`update_irq_filter`,`irq_setup`,`trapa_setup`) against a minimal host:
all65536 pairs,zero mismatches. Reset/storage/logging/MAME device framework are
not executed. No original body is copied into tracked code;the project equations
are not evidence of whole-device equivalence.
Ignoring UI/bit3 and always masking at threshold2 when I=1 produces8192 expected
failures in an ignored mutant. Tracked source unchanged.

## Next integration acceptance gates

One shared SYSCR owner must serve MMIO readback and CPU policy. After a write,
filter publication must occur before the next eligible IRQ decision,also after
CCR writes,RTE and wake checks. IRQ/TRAPA entry must use the same current mode;
no duplicate stale CPU byte. Reset must publish the intended cold profile in an
explicit lifecycle order;legacy generic hosts retain their documented profile.
Reject or explicitly scope unsupported standby/other mode semantics rather than
silently accepting a fully writable byte. Test both priority thresholds,NMI,
mode changes while pending/masked/asleep,entry stack/CCR,RTE and every-cycle
partitions using independent oracles and pinned original references.
This candidate deliberately does not choose a callback lifetime or bind FFFFF2
before these requirements are implemented. CPU generated-handler coverage remains
562 methods/264 selected rows;full CPU/machine acceptance remains open.
