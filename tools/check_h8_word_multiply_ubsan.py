"""Require UBSan to detect the original word-multiply promotion in a separate process."""
import argparse
from pathlib import Path
import subprocess


def check(executable):
    result = subprocess.run([str(executable.resolve()), '--promotion-control'],
                            capture_output=True, text=True, timeout=20)
    if result.returncode == 0 or 'signed integer overflow' not in result.stderr:
        raise RuntimeError('Expected a failing signed-overflow UBSan control: '+result.stdout+result.stderr)
    print('Original u16 multiplication signed-int overflow detected; negative control PASS')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('executable', type=Path)
    check(parser.parse_args().executable)
