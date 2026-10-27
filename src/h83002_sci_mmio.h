// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Register/vector wiring adapted from pinned MAME h83002.cpp/h8_intc.cpp.
// Scope and identities: research/h8-sci-mmio-provenance.json.
#pragma once
#include "h8_sci_async_channel.h"
#include "h8h_irq_controller.h"
#include <array>
#include <cstdint>
#include <stdexcept>

namespace yamanix {
// Two asynchronous channels on the H83002 advanced 24-bit program bus.
// The machine owns the IRQ controller, which must outlive this adapter.
// Sink: tx(channel,bool), clock(channel,bool), bool dma_request(vector).
// Callbacks must not re-enter/reconfigure the peripheral or acknowledge IRQs.
class H83002SciMmio {
public:
    explicit H83002SciMmio(H8hIrqController& irq) : irq_(irq) {}
    H83002SciMmio(const H83002SciMmio&) = delete;
    H83002SciMmio& operator=(const H83002SciMmio&) = delete;
    H83002SciMmio(H83002SciMmio&&) = delete;
    H83002SciMmio& operator=(H83002SciMmio&&) = delete;

    static bool mapped(uint32_t address) {
        return (address >= 0xffffb0 && address <= 0xffffb5) ||
               (address >= 0xffffb8 && address <= 0xffffbd);
    }
    uint8_t read8(uint32_t address, bool effects=true, bool dma=false) {
        const auto location = decode(address);
        auto& c = channels_[location.channel];
        switch (location.reg) {
        case 0: return c.smr();
        case 1: return c.brr();
        case 2: return c.scr();
        case 3: return c.tdr();
        case 4: return c.read_ssr(effects);
        case 5: return c.read_rdr(dma,effects);
        }
        throw std::logic_error("Invalid decoded SCI register");
    }
    template<class Sink>
    void write8(uint32_t address, uint8_t data, uint64_t cycles, Sink& sink, bool dma=false) {
        const auto location = decode(address);
        auto& c = channels_[location.channel];
        BoundSink<Sink> bound{sink,irq_,location.channel};
        switch (location.reg) {
        case 0: c.write_smr(data); break;
        case 1: c.write_brr(data); break;
        case 2: c.write_scr(data,bound); break;
        case 3: c.write_tdr(data,dma,cycles,bound); break;
        case 4: c.write_ssr(data,cycles,bound); break;
        case 5: break; // RDR is read-only; writes leave it unchanged.
        }
    }
    // Aligned, big-endian words. Masked-out lanes return ones and have no
    // side effects. Only full-byte lanes are supported, including mask=0.
    uint16_t read16(uint32_t address, uint16_t mask=0xffff, bool effects=true, bool dma=false) {
        require_word(address,mask);
        uint16_t value=0xffff;
        if (mask & 0xff00) value=uint16_t((value & 0xff) | (uint16_t(read8(address,effects,dma)) << 8));
        if (mask & 0x00ff) value=uint16_t((value & 0xff00) | read8(address+1,effects,dma));
        return value;
    }
    template<class Sink>
    void write16(uint32_t address, uint16_t data, uint16_t mask, uint64_t cycles, Sink& sink, bool dma=false) {
        require_word(address,mask);
        if (mask & 0xff00) write8(address,uint8_t(data >> 8),cycles,sink,dma);
        if (mask & 0x00ff) write8(address+1,uint8_t(data),cycles,sink,dma);
    }
    void write_rx(unsigned channel, bool level, uint64_t now_as_cycles) {
        at(channel).write_rx(level,now_as_cycles);
    }
    uint64_t next_event(unsigned channel) const { return at(channel).next_event(); }
    uint64_t next_event() const {
        const auto a=next_event(0), b=next_event(1);
        return !a ? b : !b ? a : a < b ? a : b;
    }
    template<class Sink> uint64_t service_one(unsigned channel, uint64_t cycles, Sink& sink) {
        auto& c=at(channel);
        BoundSink<Sink> bound{sink,irq_,channel};
        return c.service_one(cycles,bound);
    }
    // IRQ latches belong to the machine/controller, not the UART reset.
    template<class Sink> void reset_channel(unsigned channel, Sink& sink) {
        auto& c=at(channel);
        BoundSink<Sink> bound{sink,irq_,channel};
        c.reset(bound);
    }
    template<class Sink> void reset(Sink& sink) {
        reset_channel(0,sink); reset_channel(1,sink);
    }
private:
    struct Location { unsigned channel, reg; };
    static Location decode(uint32_t address) {
        if (!mapped(address)) throw std::out_of_range("Unmapped H83002 SCI address");
        const unsigned channel=address >= 0xffffb8;
        return {channel,unsigned(address-(channel ? 0xffffb8 : 0xffffb0))};
    }
    static void require_word(uint32_t address, uint16_t mask) {
        if ((address & 1) || !mapped(address) || !mapped(address+1))
            throw std::out_of_range("Unaligned or unmapped H83002 SCI word");
        if (mask!=0 && mask!=0xff && mask!=0xff00 && mask!=0xffff)
            throw std::invalid_argument("Partial SCI byte mask not implemented");
    }
    H8SciAsyncChannel& at(unsigned channel) {
        if (channel>=2) throw std::out_of_range("SCI channel");
        return channels_[channel];
    }
    const H8SciAsyncChannel& at(unsigned channel) const {
        if (channel>=2) throw std::out_of_range("SCI channel");
        return channels_[channel];
    }
    template<class Sink> struct BoundSink {
        Sink& sink; H8hIrqController& irq; unsigned channel;
        void tx(bool level) { sink.tx(channel,level); }
        void clock(bool level) { sink.clock(channel,level); }
        void error_irq() { request(0); }
        void rx_irq() { request(1); }
        void tx_irq() { request(2); }
        void end_irq() { request(3); }
        void request(unsigned kind) {
            const unsigned vector=52+4*channel+kind;
            const bool consumed=sink.dma_request(vector);
            irq.internal_irq(vector,consumed);
        }
    };
    H8hIrqController& irq_;
    std::array<H8SciAsyncChannel,2> channels_;
};
} // namespace yamanix
