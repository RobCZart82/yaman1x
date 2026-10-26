"""Exercise the shared-library ABI and independent two-chip upload-state reconstruction."""
import ctypes as c
from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from dsp_trace import reconstruct

lib = c.CDLL(sys.argv[1])
lib.yamanix_vop3_create.restype = c.c_void_p
lib.yamanix_vop3_destroy.argtypes = [c.c_void_p]
lib.yamanix_vop3_write.argtypes = [c.c_void_p, c.c_uint32, c.c_uint32, c.c_uint16, c.c_uint16]
lib.yamanix_vop3_read.argtypes = [c.c_void_p, c.c_uint32, c.c_uint32, c.c_uint16, c.POINTER(c.c_uint16)]
lib.yamanix_vop3_snapshot.argtypes = [c.c_void_p, c.POINTER(c.c_uint8), c.c_size_t]
class Metrics(c.Structure):
    _fields_ = [(name, c.c_uint64*2) for name in ("writes", "reads", "unsupported_reads", "sample_passes")]
lib.yamanix_vop3_get_metrics.argtypes = [c.c_void_p, c.POINTER(Metrics)]
assert lib.yamanix_vop3_abi_version() == 2
handle = lib.yamanix_vop3_create()
assert handle
try:
    events = []
    for chip in (0, 1):
        for reg, value in [(0, 0x81ff), (10, 0x1234+chip), (11, 0x5678), (12, 0x00ab), (13, 0x100), (14, 0x123)]:
            assert lib.yamanix_vop3_write(handle, chip, reg, value, 65535) == 0
            events.append(dict(chip=chip, reg=reg, data=value, op="W"))
    blob = (c.c_uint8 * 14340)()
    assert lib.yamanix_vop3_snapshot(handle, blob, len(blob)) == 0
    expected = reconstruct(events)[0]
    assert bytes(blob) == expected
    assert lib.yamanix_vop3_write(handle, 0, 10, 0, 255) == -2
    assert lib.yamanix_vop3_write(handle, 2, 10, 0, 65535) == -1
    assert lib.yamanix_vop3_write(None, 0, 10, 0, 65535) == -1
    assert lib.yamanix_vop3_snapshot(handle, blob, 1) == -1
    value = c.c_uint16(0xffff)
    assert lib.yamanix_vop3_read(handle, 0, 0x21, 65535, c.byref(value)) == 0 and value.value == 0
    assert lib.yamanix_vop3_read(handle, 1, 0x22, 65535, c.byref(value)) == 0
    metrics = Metrics()
    assert lib.yamanix_vop3_get_metrics(handle, c.byref(metrics)) == 0
    assert list(metrics.writes) == [6, 6] and list(metrics.reads) == [1, 1]
    assert list(metrics.unsupported_reads) == [0, 1] and list(metrics.sample_passes) == [0, 0]
    assert lib.yamanix_vop3_snapshot(handle, blob, len(blob)) == 0 and bytes(blob) == expected
finally:
    lib.yamanix_vop3_destroy(handle)
lib.yamanix_vop3_destroy(None)
print("PASS: C ABI, independent chip state, argument guards and honest unsupported-read/sample counters")
