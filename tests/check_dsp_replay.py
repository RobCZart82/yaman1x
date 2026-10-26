"""Cross-check actual VOP3 register uploads against an independent state reconstruction."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from dsp_trace import reconstruct, export_events

events = []
def event(chip, reg, data, op="W"):
    events.append(dict(seq=len(events), time_ns=len(events)*100, chip=chip, op=op,
                       reg=reg, data=data, mask=65535, pc=256))

for chip in range(2):
    event(chip, 0, 0x81ff)
    for reg in range(10, 5, -1):
        event(chip, reg, 100*chip+reg)  # Auto-increment and wrap at step 511.
    event(chip, 11, 0x1234)
    event(chip, 12, 0x3456)  # Tags retain the low byte.
    event(chip, 13, 0x1ab)
    event(chip, 14, 0x123)
    event(chip, 1, 0)       # Unknown registers still advance the select latch upstream.
    event(chip, 0, 127)
    event(chip, 13, 12)
    event(chip, 14, 34)
    event(chip, 0x21, 0, "R")

with tempfile.TemporaryDirectory() as temporary:
    root = Path(temporary)
    source, output = root / "events.tsv", root / "state.bin"
    source.write_text(export_events(events))
    result = subprocess.run([sys.argv[1], str(source), str(output)], capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stderr)
    if output.read_bytes() != reconstruct(events)[0]:
        raise RuntimeError("C++ VOP3 upload state differs from independent reconstruction")
    if json.loads(result.stdout)["dsp_samples_executed"] != 0:
        raise RuntimeError("Upload audit must not run DSP samples")
    for corruption in (source.read_text().replace("65535", "255", 1),
                       source.read_text().replace("0 0 W", "-1 0 W", 1),
                       source.read_text().rsplit("# END", 1)[0],
                       source.read_text() + "unexpected\n"):
        source.write_text(corruption)
        bad = subprocess.run([sys.argv[1], str(source), str(output)], capture_output=True, text=True)
        if bad.returncode == 0:
            raise RuntimeError("Corrupt replay input was accepted")
print("PASS: two-chip VOP3 upload state, auto-increment/wrap, masks and damaged-input rejection")
