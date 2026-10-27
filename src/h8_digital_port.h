// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Adapted from MAME h8_port.cpp; see docs/h8-digital-port.md.
#pragma once
#include <cstdint>
namespace yamanix {
class H8DigitalPort {
public:
    explicit H8DigitalPort(uint8_t default_ddr = 0, uint8_t mask = 0, uint8_t forced = 0)
        : m_ddr(default_ddr), m_mask(mask), m_forced(forced) {}
    uint8_t ddr() const { return m_ddr; }
    uint8_t data() const { return uint8_t(m_dr | m_mask); }
    uint8_t pcr() const { return uint8_t(m_pcr | m_mask); }
    uint8_t odr() const { return uint8_t(m_odr | uint8_t(~m_mask)); }
    void write_pcr(uint8_t value) { m_pcr = value; }
    void write_odr(uint8_t value) { m_odr = value; }
    template<class Read> uint8_t read_pins(Read read) const {
        uint8_t result = uint8_t(m_mask | (m_dr & uint8_t(~m_forced) & m_ddr));
        if ((m_ddr & uint8_t(~m_mask)) != uint8_t(~m_mask) || m_forced)
            result |= uint8_t(read()) & uint8_t(m_forced | uint8_t(~m_ddr));
        return result;
    }
    template<class Write> void write_ddr(uint8_t value, Write write) { m_ddr = value; update(write); }
    template<class Write> void write_data(uint8_t value, Write write) { m_dr = value; update(write); }
    // Mirrors MAME device_reset: retain registers, publish changed output only.
    // Construct a new instance for device_start-style initialization.
    template<class Write> void reset(Write write) { update(write); }
private:
    template<class Write> void update(Write write) {
        const uint8_t data = uint8_t((m_dr | uint8_t(~m_ddr)) & uint8_t(~m_mask));
        const uint8_t direction = uint8_t(m_ddr & uint8_t(~m_mask));
        const uint16_t output = uint16_t(uint16_t(direction) << 8 | data);
        if (m_first || output != m_last) {
            m_first = false;
            m_last = output;
            write(data, direction);
        }
    }
    uint8_t m_ddr, m_mask, m_forced;
    uint8_t m_dr = 0, m_pcr = 0, m_odr = 0;
    uint16_t m_last = 0;
    bool m_first = true;
};
}
