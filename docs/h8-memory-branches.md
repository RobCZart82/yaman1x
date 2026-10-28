# H8H byte memory operations and all short branch conditions

Historical #48 increment, 2026-10-28, based on #47. The current
[word/long extension](h8-word-long-memory.md) expands this profile further. M0 remains partial.

The limited `H8FirstInstructions` profile at #48 compiled 44 original generated
full/partial methods from the locked base-H8 and H8H outputs. In addition to the
first reset/NOP/immediate-byte/BRA selection, it supports all 16 rel8 branch
conditions, MOV.L immediate32 to ER0–ER7, and indirect MOV.B loads/stores through
those ER registers. Other instructions still fail explicitly.

| Operation | First instruction word |
|---|---|
| MOV.L #imm32, ERn | `0x7a00 | n`, followed by high/low immediate words; n=0..7 |
| MOV.B @ERs, Rd | `0x6800 | (s << 4) | d`; d=0..15 byte-register selector |
| MOV.B Rs, @ERd | `0x6880 | (d << 4) | s`; s=0..15 byte-register selector |
| Bcc rel8 | `0x4000 | (condition << 8) | displacement8`; condition=0..15 |

The 16 condition selectors are always, never, HI, LS, CC, CS, NE, EQ, VC, VS,
PL, MI, GE, LT, GT and LE. Register halves and all CCR conditions are preserved
from the source. The IR diagnostic array grows to three words for immediate32;
it remains a diagnostic snapshot, not a serialized state format.

## Bus and ordering contract

Host now supplies `read_instruction_word`, `read_byte`, `write_byte` and the
existing `update(cycle)` deadline callback. Read/write callbacks complete
synchronously; unsupported retries, wait states, IRQ/DMA/DTC/trace, standby and
warm reset remain outside the fixed profile. No enabling API is provided.
Callbacks must not reenter the CPU or mutate its cycle counter.

Each completed byte data transaction charges two cycles before the callback,
using the existing H8H bus adapter. Generated handlers preserve instruction
prefetch before the data access. Store flags are assigned before the write;
load flags/register assignment occurs after the completed data read, possibly
in a later partial continuation. A completed transaction must not repeat merely
because its instruction crosses a slice or peripheral boundary.

ER registers retain all 32 bits. Host owns actual address-space mapping, including
advanced-mode 24-bit wrapping. Synthetic tests check raw 32-bit callback addresses
and a separately masked data store; this is not an AN1x RAM/MMIO map. Code storage
remains immutable in the supported research profile. Self-modifying code, real
MMIO side effects and CPU/SCI tie ordering are not accepted by these fixtures.

The pinned branch handlers read both the fallthrough and target even for untaken
branches. This behavior is retained and independently tested; it is source
compatibility, not a new claim about physical AN1x bus timing.

Exceptions may follow a completed store. CPU and host must then be discarded;
there is no rollback. Instance state, data storage and callback logs are tested
for isolation and whole/partitioned equality.

## ROM-free evidence

- Native condition control: 65,536 condition/CCR/displacement combinations.
  A separate exhaustive 65,536-word decoder check verifies the exact reduced
  acceptance set and explicit rejection of every unsupported opcode.
- 32-bit immediates check all ER registers with zero, sign-boundary and mixed
  bit patterns. Byte-memory controls cover 32,768 loads and 32,768 stores,
  including all byte-register lanes, pointer/data aliases, odd addresses,
  nonzero upper register halves and full raw address bits.
- 1,000 mixed memory/branch partition histories agree in complete diagnostic
  state, ordered instruction/data bus trace and data memory. Failure controls
  include read exceptions and writes that mutate memory before throwing.
- Original H8H decoder/handlers and original bus/register/flag/prefetch/runner
  helpers: 100 synthetic programs over 200,000 slices; 844,255 instruction reads,
  27,571 byte reads and 27,574 byte writes; zero differences after each slice.
  The reference disables unsupported IRQ/transfer/debug services and is not a
  complete MAME machine, firmware run or hardware measurement.
- Negative control: one deliberately corrupted store bit in an ignored candidate
  copy is detected by the unchanged original reference at seed 0, slice 1.
- The expanded Python suite checks both separate generated selections/hashes,
  nesting, wrong-output rejection, corruption, newline normalization and archive
  rejection before generator/compiler execution. Full source outputs regenerate
  twice from the locked archive before the optional reference is compiled.

```sh
python tools/check_h8_first_instructions.py --generated /absolute/verified/h8.hxx
python tools/check_h8_first_instructions.py --generated /absolute/verified/h8h.hxx --h8h
python tools/check_h8_first_instructions.py --archive /absolute/mame-master.zip --work work/memory-reference
```

Provenance and retained BSD notices cover both generated headers and adapted
helpers. No new upstream revision or Yamaha input is introduced. Earlier #47
measurements remain historical in the first-instruction note and ledger.

Next: word/long data operations, arithmetic/comparison flag behavior and broader
instruction coverage, then IRQ entry and explicit CPU/SCI scheduling integration.
Complete CPU, faithful reset/save-state, firmware PKS/MIDI, audio and REAPER
acceptance remain open. M0/M1 acceptance criteria have not been relaxed.

Candidate local validation: Windows MSVC Release build, all 29 runnable CTest groups and 83 Python cases PASS. The one established symlink-privilege test is explicitly excluded locally; the complete candidate suite has 30 CTest groups / 84 Python cases. Source/notice audits PASS. Final platform, MSan and MAME evidence remains tied to the PR head.
