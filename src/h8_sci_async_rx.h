// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Compatibility RX facade over the shared SCI channel; see docs/h8-sci-async-channel.md.
#pragma once
#include "h8_sci_async_channel.h"

namespace yamanix {
class H8SciAsyncRx {
public:
    using State=H8SciAsyncChannel::RxState;
    State state() const { return channel_.rx_state(); }
    uint8_t smr() const { return channel_.smr(); }
    uint8_t scr() const { return channel_.scr(); }
    uint8_t brr() const { return channel_.brr(); }
    uint8_t phase() const { return channel_.rx_phase(); }
    bool input() const { return channel_.input(); }
    uint64_t next_event() const { return channel_.next_event(); }
    uint8_t read_ssr(bool effects=true) { return channel_.read_ssr(effects); }
    void write_ssr(uint8_t data) { NullSink sink; channel_.write_ssr(data,0,sink); }
    uint8_t read_rdr(bool dma=false,bool effects=true) { return channel_.read_rdr(dma,effects); }
    void write_smr(uint8_t data) { channel_.write_smr(data); }
    void write_brr(uint8_t data) { channel_.write_brr(data); }
    void write_rx(bool value,uint64_t cycles) { channel_.write_rx(value,cycles); }
    template<class Sink> void reset(Sink& sink) { Adapter<Sink> a{sink}; channel_.reset(a); }
    template<class Sink> void write_scr(uint8_t data,Sink& sink) {
        if (data & 0xa4) throw std::invalid_argument("SCI RX-only facade cannot enable TX controls");
        Adapter<Sink> a{sink}; channel_.write_scr(data,a);
    }
    template<class Sink> uint64_t service_one(uint64_t cycles,Sink& sink) {
        Adapter<Sink> a{sink}; return channel_.service_one(cycles,a);
    }
private:
    struct NullSink {
        void tx(bool) {} void clock(bool) {} void tx_irq() {} void end_irq() {}
        void rx_irq() {} void error_irq() {}
    };
    template<class Sink> struct Adapter {
        Sink& sink;
        void tx(bool) {} void tx_irq() {} void end_irq() {}
        void clock(bool b) { sink.clock(b); }
        void rx_irq() { sink.rx_irq(); }
        void error_irq() { sink.error_irq(); }
    };
    H8SciAsyncChannel channel_;
};
} // namespace yamanix
