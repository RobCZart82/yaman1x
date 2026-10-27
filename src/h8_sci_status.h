// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Adapted from MAME h8_sci_device's SSR access rules; see research/h8-sci-status-provenance.json.
#pragma once
#include <cstdint>

namespace yamanix {
// Status latch only. The owner supplies serial events, DMA qualification and
// performs tx_start/sync_rx_start after writes; this class cannot emit bytes.
class H8SciStatus {
public:
    static constexpr uint8_t tdre=0x80, rdrf=0x40, orer=0x20, fer=0x10,
                             per=0x08, tend=0x04, mpb=0x02, mpbt=0x01;
    void reset() { value_=tdre|tend; } // Upstream reset deliberately retains ssr_read.
    uint8_t read(bool side_effects=true) {
        if (side_effects) read_=value_;
        return value_;
    }
    uint8_t value() const { return value_; }
    uint8_t read_latch() const { return read_; }
    bool receive_error() const { return (value_ & (orer|fer|per)) != 0; }
    void write(uint8_t data, bool transmit_enabled) {
        if (transmit_enabled && (value_ & read_ & tdre) && !(data & tdre))
            value_ &= uint8_t(~(tdre|tend));
        value_ = uint8_t((value_ & (~read_ | data | tdre | tend | mpb) & ~mpbt) | (data & mpbt));
        read_ &= value_;
    }
    // Hardware may raise status flags, but MPBT is software-controlled.
    void raise_flags(uint8_t flags) { value_ |= flags & uint8_t(~mpbt); }
    void dma_transmit_write(bool transmit_enabled) {
        if (transmit_enabled) {
            value_ &= uint8_t(~(tdre|tend));
            read_ &= value_;
        }
    }
    void dma_receive_read(bool side_effects=true) {
        if (side_effects) {
            value_ &= uint8_t(~rdrf);
            read_ &= value_;
        }
    }
private:
    uint8_t value_=tdre|tend;
    uint8_t read_=0;
};
} // namespace yamanix
