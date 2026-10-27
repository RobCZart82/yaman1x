// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Compatibility TX facade over the shared SCI channel; see docs/h8-sci-async-channel.md.
#pragma once
#include "h8_sci_async_channel.h"

namespace yamanix {
class H8SciAsyncTx {
public:
    using State=H8SciAsyncChannel::TxState;
    State state() const { return channel_.tx_state(); }
    uint8_t scr() const { return channel_.scr(); }
    uint8_t smr() const { return channel_.smr(); }
    uint8_t brr() const { return channel_.brr(); }
    uint8_t tdr() const { return channel_.tdr(); }
    uint8_t phase() const { return channel_.tx_phase(); }
    uint64_t next_event() const { return channel_.next_event(); }
    uint8_t read_ssr(bool effects=true) { return channel_.read_ssr(effects); }
    void write_smr(uint8_t data) { channel_.write_smr(data); }
    void write_brr(uint8_t data) { channel_.write_brr(data); }
    template<class Sink> void reset(Sink& sink) { Adapter<Sink> a{sink}; channel_.reset(a); }
    template<class Sink> void write_scr(uint8_t data,Sink& sink) { Adapter<Sink> a{sink}; channel_.write_scr(data,a); }
    template<class Sink> void write_tdr(uint8_t data,bool dma,uint64_t cycles,Sink& sink) {
        Adapter<Sink> a{sink}; channel_.write_tdr(data,dma,cycles,a);
    }
    template<class Sink> void write_ssr(uint8_t data,uint64_t cycles,Sink& sink) {
        Adapter<Sink> a{sink}; channel_.write_ssr(data,cycles,a);
    }
    template<class Sink> uint64_t service_one(uint64_t cycles,Sink& sink) {
        Adapter<Sink> a{sink}; return channel_.service_one(cycles,a);
    }
private:
    template<class Sink> struct Adapter {
        Sink& sink;
        void tx(bool b) { sink.tx(b); }
        void clock(bool b) { sink.clock(b); }
        void tx_irq() { sink.tx_irq(); }
        void end_irq() { sink.end_irq(); }
        void rx_irq() {} void error_irq() {}
    };
    H8SciAsyncChannel channel_;
};
} // namespace yamanix
