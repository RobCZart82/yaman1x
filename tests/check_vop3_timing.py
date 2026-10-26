"""Check actual VOP3 EG evolution through ABI v2 using synthetic host writes only."""
import ctypes as c
import sys
import math
import json
lib = c.CDLL(sys.argv[1])
lib.yamanix_vop3_create.restype = c.c_void_p
lib.yamanix_vop3_destroy.argtypes = [c.c_void_p]
lib.yamanix_vop3_set_eg_readback.argtypes = [c.c_void_p, c.c_uint32]
lib.yamanix_vop3_advance.argtypes = [c.c_void_p, c.c_uint64]
lib.yamanix_vop3_write.argtypes = [c.c_void_p, c.c_uint32, c.c_uint32, c.c_uint16, c.c_uint16]
lib.yamanix_vop3_read.argtypes = [c.c_void_p, c.c_uint32, c.c_uint32, c.c_uint16, c.POINTER(c.c_uint16)]
class Metrics(c.Structure):
    _fields_ = [(key, c.c_uint64*2) for key in ('writes', 'reads', 'unsupported_reads', 'sample_passes')]
lib.yamanix_vop3_get_metrics.argtypes = [c.c_void_p, c.POINTER(Metrics)]
assert lib.yamanix_vop3_abi_version() == 2
handles = [lib.yamanix_vop3_create(), lib.yamanix_vop3_create()]
assert all(handles)
def write(h, chip, reg, val): assert lib.yamanix_vop3_write(h, chip, reg, val & 65535, 65535) == 0
def read(h, chip, reg=0x22):
    value = c.c_uint16()
    assert lib.yamanix_vop3_read(h, chip, reg, 65535, c.byref(value)) == 0
    return value.value
try:
    a, b = handles
    for h in handles:
        assert lib.yamanix_vop3_set_eg_readback(h, 1) == 0
        # Select channel 3, initialize to -256 at rate 0. 0x21 remains a distinct status port.
        write(h, 0, 0, 3); write(h, 0, 0x2c, -256)
        assert read(h, 0) == 0xc000 and read(h, 0, 0x21) == 0
        write(h, 1, 0, 3); write(h, 1, 0x2c, 256)
        assert read(h, 1) == 0x4000
        write(h, 1, 0, 4); write(h, 1, 0x2c, -512); assert read(h, 1) == 0x8000
        write(h, 1, 0x2c, 512); assert read(h, 1) == 0x7fff
        write(h, 1, 0, 3)
        write(h, 0, 0, 4); assert read(h, 0) == 0
        write(h, 0, 0, 16)
        value = c.c_uint16(123)
        assert lib.yamanix_vop3_read(h, 0, 0x22, 65535, c.byref(value)) == -4 and value.value == 123
        write(h, 0, 0, 3)
        # Freeze, arm a target on the opposite side, then start a finite-rate segment.
        write(h, 0, 0x2b, 255); write(h, 0, 0x2d, 511); write(h, 0, 0x2c, 128); write(h, 0, 0x2b, 64)
    # Same elapsed time in different partitions: core.run(), EG evolution and host reads agree.
    assert lib.yamanix_vop3_advance(a, 1000000) == 0
    for ns in (200000, 400000, 600000, 800000, 1000000): assert lib.yamanix_vop3_advance(b, ns) == 0
    # Before crossing, compare with the independent closed-form exponential.
    duration = 1.33e-4 * 2**(64/15.5)
    expected = int(64 * (511 + (-256 - 511) * math.exp(-math.log(3) * .001 / duration))) & 65535
    assert read(a, 0) == read(b, 0) == expected
    assert lib.yamanix_vop3_advance(a, 50000000) == 0
    for ns in range(2000000, 50000001, 1000000): assert lib.yamanix_vop3_advance(b, ns) == 0
    actual = read(a, 0)
    assert actual == read(b, 0) and actual != 0
    # Record the upstream discrepancy; equality here checks transport, not EG fidelity.
    print(json.dumps(dict(model_eg_target_word=0x2000, observed_word_after_50ms=actual,
                          upstream_holds_target=actual == 0x2000, hardware_validated=False)))
    assert read(a, 1) == read(b, 1) == 0x4000
    for h in handles:
        metrics = Metrics(); assert lib.yamanix_vop3_get_metrics(h, c.byref(metrics)) == 0
        assert list(metrics.sample_passes) == [2400, 2400]
        assert lib.yamanix_vop3_advance(h, 50000000) == 0
        assert lib.yamanix_vop3_advance(h, 49999999) == -4
        assert lib.yamanix_vop3_advance(h, 1000000000) == -4
        assert lib.yamanix_vop3_get_metrics(h, c.byref(metrics)) == 0
        assert list(metrics.sample_passes) == [2400, 2400]
        assert lib.yamanix_vop3_set_eg_readback(h, 2) == -1
        assert lib.yamanix_vop3_set_eg_readback(h, 0) == 0 and read(h, 0) == 0
finally:
    for h in handles: lib.yamanix_vop3_destroy(h)
print('PASS: synthetic EG levels, independent chips/channels, distinct status/level ports, 2400 real samples/chip, split-call equality and time guards')
