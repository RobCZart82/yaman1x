#pragma once
#include <cstdint>

namespace yamanix {
// AN1x schematic: PA0/PA2..5 = DB0/DB2..5; PB1/PB6/PB7 = DB1/DB6/DB7.
// PB2 = RS, P90 = R/W, P95 = E. Unconnected inputs retain pull-up levels.
struct LcdBus {
    enum class Event { none, status_read, data_read, instruction_write, data_write };
    uint8_t pa = 0xff, pb = 0xff, p9 = 0xff, read_latch = 0xff;

    uint8_t output_data() const { return (pa & 0x3d) | (pb & 0xc2); }
    bool reading() const { return (p9 & 0x21) == 0x21; }
    uint8_t input_pa() const { return reading() ? (read_latch & 0x3d) | 0xc2 : 0xff; }
    uint8_t input_pb() const { return reading() ? (read_latch & 0xc2) | 0x3d : 0xff; }

    Event control(uint8_t value) {
        const bool edge = ((p9 ^ value) & 0x20) != 0;
        p9 = value;
        if (!edge) return Event::none;
        if (p9 & 0x20) {
            if (!(p9 & 1)) return Event::none;
            return (pb & 4) ? Event::data_read : Event::status_read;
        }
        if (p9 & 1) return Event::none;
        return (pb & 4) ? Event::data_write : Event::instruction_write;
    }
};
}
