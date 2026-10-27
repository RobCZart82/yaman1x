#include "h8_digital_port.h"
#include <cstdio>
#include <initializer_list>
int main() {
    using Port = yamanix::H8DigitalPort;
    int failures = 0;
    auto check = [&](bool ok) { if (!ok) ++failures; };
    for (unsigned mask : {0u, 0x80u, 0xa5u, 0xffu})
    for (unsigned forced : {0u, 0x40u, 0xffu})
    for (unsigned direction = 0; direction < 256; ++direction) {
        Port port{uint8_t(direction), uint8_t(mask), uint8_t(forced)};
        unsigned output = 0, output_ddr = 0;
        auto write = [&](uint8_t data, uint8_t ddr) { output = data; output_ddr = ddr; };
        for (unsigned data = 0; data < 256; ++data) {
            port.write_data(uint8_t(data), write);
            check(output == ((data | (~direction & 255)) & (~mask & 255)));
            check(output_ddr == (direction & (~mask & 255)));
            check(port.data() == (data | mask));
            for (unsigned pins : {0u, 0x55u, 0xaau, 0xffu}) {
                unsigned reads = 0, expected = 0;
                const auto actual = port.read_pins([&]() { ++reads; return uint8_t(pins); });
                for (unsigned bit = 1; bit < 256; bit <<= 1) {
                    const bool high = (mask & bit) ||
                        ((direction & bit) && !(forced & bit) && (data & bit)) ||
                        (((forced & bit) || !(direction & bit)) && (pins & bit));
                    if (high) expected |= bit;
                }
                check(actual == expected);
                check(reads == unsigned((direction & (~mask & 255)) != (~mask & 255) || forced));
            }
        }
    }
    Port a, b;
    unsigned calls = 0;
    auto write = [&](uint8_t, uint8_t) { ++calls; };
    a.reset(write); a.reset(write);
    check(calls == 1);
    a.write_ddr(0xff, write); a.write_data(0x42, write); a.write_data(0x42, write);
    check(calls == 3);
    a.write_pcr(0x12); a.write_odr(0x34); a.reset(write);
    check(a.ddr() == 0xff && a.data() == 0x42 && a.pcr() == 0x12 && calls == 3);
    check(b.ddr() == 0 && b.data() == 0 && b.pcr() == 0);
    Port masked(0, 0xa5);
    check(masked.odr() == 0x5a && masked.pcr() == 0xa5);
    std::printf("H8 digital port: %s (%d failures)\n", failures ? "FAIL" : "PASS", failures);
    return failures ? 1 : 0;
}
