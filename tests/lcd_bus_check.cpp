#include "lcd_bus.h"
#include <cstdio>

int main() {
    using Bus = yamanix::LcdBus;
    using E = Bus::Event;
    int failures = 0;
    auto check = [&](bool ok, const char* what) {
        if (!ok) { std::fprintf(stderr, "%s\n", what); ++failures; }
    };
    for (unsigned value = 0; value < 256; ++value) {
        Bus bus;
        bus.pa = (value & 0x3d) | 0xc2;
        bus.pb = (value & 0xc2) | 0x39; // RS low, unrelated outputs high.
        check(bus.output_data() == value, "Split write bus mismatch");
        bus.p9 = 0;
        check(bus.control(0x20) == E::none, "Writes must not commit on E rising");
        check(bus.control(0) == E::instruction_write, "Command missing on E falling");
        check(bus.control(0) == E::none, "Duplicate E level produced a write");
        bus.pb |= 4;
        bus.control(0x20);
        check(bus.control(0) == E::data_write, "RS data selection mismatch");
        bus.pb &= ~4;
        check(bus.control(1) == E::none, "RW change without E edge produced transfer");
        check(bus.control(0x21) == E::status_read, "Status missing on E rising");
        bus.read_latch = value;
        check(((bus.input_pa() & 0x3d) | (bus.input_pb() & 0xc2)) == value, "Split read bus mismatch");
        check((bus.input_pa() & 0xc2) == 0xc2 && (bus.input_pb() & 0x3d) == 0x3d,
              "Unrelated inputs lost pull-up levels");
        check(bus.control(1) == E::none, "Read E falling produced another read");
        check(bus.input_pa() == 0xff && bus.input_pb() == 0xff, "LCD drives idle bus");
        bus.pb |= 4;
        check(bus.control(0x21) == E::data_read, "RS read-data selection mismatch");
    }
    std::printf("LCD wiring: 256 byte values, edge direction, RS and idle levels: %s\n", failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
