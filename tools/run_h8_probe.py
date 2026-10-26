"""Run the instrumented MAME research target against an unchanged user ROM.

This is a boot observation harness, not a complete AN1x emulator or plugin.
Use with the documented research patches applied to the locked MAME source.
"""
import argparse
import hashlib
import json
import re
import os
from pathlib import Path
import subprocess
import zipfile
from rom_inspect import read_input, identify


def summarize_trace(lines):
    pcs = [int(match.group(1), 16) for line in lines
           if (match := re.search(r"YAMANIX PC tick=\d+ pc=([0-9a-fA-F]+)", line))]
    result = {"pc_samples": len(pcs), "unique_sampled_pcs": sorted(set(pcs)),
            "h8_cpu_progress_observed": len(set(pcs) - {0}) > 1,
            "read_addresses_observed": sum("YAMANIX READ" in line for line in lines),
            "write_addresses_observed": sum("YAMANIX WRITE" in line for line in lines)}
    dsp = set()
    for line in lines:
        match = re.search(r"YAMANIX (?:READ|WRITE).*address=([0-9a-fA-F]+)", line)
        if match:
            address = int(match.group(1), 16)
            if 0x440000 <= address < 0x440060 or 0x480000 <= address < 0x480060:
                dsp.add(address)
    result["dsp_mmio_addresses_observed"] = sorted(dsp)
    summaries = [match for line in lines if (match := re.search(
        r"LCD_SUMMARY tick=(\d+) busy_reads=(\d+) ready_reads=(\d+) commands=(\d+) data=(\d+)", line))]
    if summaries:
        summary = summaries[-1]
        result["lcd_counters"] = dict(zip(
            ["tick", "busy_reads", "ready_reads", "commands", "data_writes"], map(int, summary.groups())))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mame", type=Path, required=True)
    parser.add_argument("--rom", type=Path, required=True)
    parser.add_argument("--member")
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--seconds", type=int, default=1, choices=range(1, 11))
    parser.add_argument("--dsp-profile", choices=("passive", "firmware-contract"), default="passive",
                        help="firmware-contract is experimental, not hardware-validated")
    parser.add_argument("--dms-board", choices=("absent", "installed"), default="absent",
                        help="installed grounds P92 per service schematic; absent preserves the earlier input")
    parser.add_argument("--bridge", type=Path,
                        help="Explicit local VOP3 register bridge library; requires firmware-contract")
    parser.add_argument("--eg-readback", choices=("zero", "an1x-level-experiment"), default="zero",
                        help="Experimental selected-channel EG level readback; requires --bridge")
    args = parser.parse_args()
    try:
        binary = args.mame.resolve(strict=True)
        bridge = args.bridge.resolve(strict=True) if args.bridge else None
        if bridge and args.dsp_profile != "firmware-contract":
            raise ValueError("The live bridge requires --dsp-profile firmware-contract")
        if args.eg_readback != "zero" and not bridge:
            raise ValueError("Experimental EG readback requires --bridge")
        raw = read_input(args.rom, args.member)
        identity = identify(raw)
        if not identity["matches_known_an1x_104"]:
            raise ValueError("Expected the known AN1x 1.04 ROM; refusing unknown image")
        work = args.work.resolve()
        # Require a fresh directory so previous logs/NVRAM cannot silently affect results.
        work.mkdir(parents=True, exist_ok=False)
        roms = work / "roms"
        roms.mkdir()
        with zipfile.ZipFile(roms / "an1x.zip", "w", zipfile.ZIP_DEFLATED) as archive:
            archive.writestr("yamaha an1x 1.04.ic2", raw)
        command = [str(binary), "an1x", "-noreadconfig", "-video", "none", "-sound", "none",
                   "-nothrottle", "-skip_gameinfo", "-seconds_to_run", str(args.seconds),
                   "-rompath", str(roms), "-nonvram_save", "-log", "-noplugins",
                   "-keyboardprovider", "none", "-mouseprovider", "none",
                   "-lightgunprovider", "none", "-joystickprovider", "none"]
        environment = os.environ.copy()
        environment["YAMANIX_DSP_PROFILE"] = args.dsp_profile
        environment["YAMANIX_DMS_BOARD"] = args.dms_board
        environment["YAMANIX_EG_READBACK"] = args.eg_readback
        environment.pop("YAMANIX_VOP3_BRIDGE", None)
        if bridge:
            environment["YAMANIX_VOP3_BRIDGE"] = str(bridge)
        with (work / "console.log").open("w") as console:
            result = subprocess.run(command, cwd=work, stdout=console, stderr=subprocess.STDOUT,
                                    timeout=120, check=False, env=environment)
        log = work / "error.log"
        lines = log.read_text(errors="replace").splitlines() if log.exists() else []
        trace = [line for line in lines if "YAMANIX" in line]
        (work / "trace.log").write_text("\n".join(trace) + "\n")
        bridge_completed = (bool(bridge) and (work / "live-vop3-state.bin").is_file()
                            and sum("YAMANIX BRIDGE_END" in line for line in trace) == 2)
        manifest = {"rom_identity": identity, "command": command, "returncode": result.returncode,
                    "dsp_profile": args.dsp_profile, "dsp_hardware_validated": False,
                    "dms_board": args.dms_board, "vop3_bridge": str(bridge) if bridge else None,
                    "dsp_sample_execution": False, "eg_readback": args.eg_readback,
                    "live_bridge_completed": bridge_completed,
                    "mame_sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
                    "bridge_sha256": hashlib.sha256(bridge.read_bytes()).hexdigest() if bridge else None,
                    "trace_lines": len(trace), "complete_an1x_boot": False,
                    "note": "Trace proves observation only; unmapped devices remain unimplemented."}
        manifest.update(summarize_trace(trace))
        (work / "run.json").write_text(json.dumps(manifest, indent=2) + "\n")
        if result.returncode != 0 or not manifest["h8_cpu_progress_observed"]:
            raise ValueError(f"Probe did not produce a valid run; inspect {work / 'console.log'}")
        if bridge and not bridge_completed:
            raise ValueError(f"Bridge did not complete; inspect {work / 'trace.log'}")
        print(json.dumps(manifest, indent=2))
    except (OSError, ValueError, subprocess.TimeoutExpired, zipfile.BadZipFile) as error:
        parser.exit(2, f"H8 probe error: {error}\n")


if __name__ == "__main__":
    main()
