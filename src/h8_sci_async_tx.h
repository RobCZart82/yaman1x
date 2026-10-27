// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Adapted from pinned MAME h8_sci.cpp; see research/h8-sci-tx-provenance.json.
#pragma once
#include "h8_sci_cycle_clock.h"
#include "h8_sci_status.h"

namespace yamanix {
// TX-only owner. Sink methods: tx(bool), clock(bool), tx_irq(), end_irq().
// Sinks must not re-enter this owner. No receive/DMA controller/CPU is modeled.
class H8SciAsyncTx {
public:
    enum class State { idle, start, bit, parity, stop, last_tick };
    State state() const { return state_; }
    uint8_t scr() const { return scr_; }
    uint8_t smr() const { return smr_; }
    uint8_t brr() const { return brr_; }
    uint8_t tdr() const { return tdr_; }
    uint8_t phase() const { return phase_; }
    uint64_t next_event() const { return clock_.next_event(); }
    uint8_t read_ssr(bool effects=true) { return status_.read(effects); }

    template<class Sink> void reset(Sink& sink) {
        status_.reset(); clock_.reset();
        smr_=scr_=phase_=bits_=0; brr_=tdr_=shift_=0xff;
        parity_=0; state_=State::idle;
        sink.clock(true); sink.tx(true);
    }
    void write_smr(uint8_t data) {
        // Synchronous and multiprocessor framing are outside this extraction.
        if (data & 0x84) throw std::invalid_argument("SCI TX sync/MP mode not implemented");
        clock_.configure(data,brr_,scr_); smr_=data;
    }
    void write_brr(uint8_t data) { clock_.configure(smr_,data,scr_); brr_=data; }
    template<class Sink> void write_scr(uint8_t data, Sink& sink) {
        clock_.configure(smr_,brr_,data); // Reject external clock before changing SCR.
        const auto delta=uint8_t(scr_^data); scr_=data;
        if ((delta & 0x80) && (scr_ & 0x80) && (status_.value() & H8SciStatus::tdre)) sink.tx_irq();
        if ((delta & 0x04) && (scr_ & 0x04) && (status_.value() & H8SciStatus::tend)) sink.end_irq();
        // RE/RIE concern the future RX owner; no receive interrupts are emitted.
    }
    template<class Sink> void write_tdr(uint8_t data, bool dma, uint64_t cycles, Sink& sink) {
        tdr_=data;
        if (dma && (scr_ & 0x20)) {
            status_.dma_transmit_write(true);
            if (state_==State::idle) start(cycles,sink);
        }
    }
    template<class Sink> void write_ssr(uint8_t data, uint64_t cycles, Sink& sink) {
        status_.write(data,(scr_ & 0x20)!=0);
        if (state_==State::idle && !(status_.value() & H8SciStatus::tdre)) start(cycles,sink);
    }
    // One due peripheral edge. Owner drains overdue events with a bounded budget.
    // cycles is the CPU's current total-cycle view, including for a late restart.
    template<class Sink> uint64_t service_one(uint64_t cycles, Sink& sink) {
        return clock_.service_one(cycles,[&](auto mode) {
            phase_=uint8_t((phase_+1)&15);
            if (!phase_) {
                step(cycles,sink);
                if (mode==H8SciCycleClock::Mode::async_out) sink.clock(false);
            } else if (phase_==8 && mode==H8SciCycleClock::Mode::async_out) sink.clock(true);
        },[](auto){},[&]{sink.clock(true);});
    }
private:
    template<class Sink> void start(uint64_t cycles, Sink& sink) {
        status_.raise_flags(H8SciStatus::tdre);
        shift_=tdr_; parity_=(smr_ & 0x10)?0:1;
        if (scr_ & 0x80) sink.tx_irq();
        state_=State::start; bits_=1;
        if (clock_.start(H8SciCycleClock::Side::tx,cycles,cycles)) phase_=15;
    }
    template<class Sink> void step(uint64_t cycles, Sink& sink) {
        switch (state_) {
        case State::start:
            sink.tx(false); state_=State::bit; bits_=(smr_ & 0x40)?7:8; break;
        case State::bit:
            parity_^=shift_ & 1; sink.tx((shift_ & 1)!=0); shift_>>=1;
            if (!--bits_) {
                state_=(smr_ & 0x20)?State::parity:State::stop;
                bits_=(state_==State::parity)?1:((smr_ & 0x08)?2:1);
            }
            break;
        case State::parity:
            sink.tx(parity_!=0); state_=State::stop; bits_=(smr_ & 0x08)?2:1; break;
        case State::stop:
            sink.tx(true);
            if (!--bits_) {
                if (!(status_.value() & H8SciStatus::tdre)) start(cycles,sink);
                else state_=State::last_tick;
            }
            break;
        case State::last_tick:
            state_=State::idle; bits_=0;
            clock_.stop(H8SciCycleClock::Side::tx);
            sink.tx(true);
            if (scr_ & 0x04) sink.end_irq();
            if ((scr_ & 0x20) && !(status_.value() & H8SciStatus::tdre)) start(cycles,sink);
            else status_.raise_flags(H8SciStatus::tend);
            break;
        default: throw std::logic_error("SCI TX clock tick while idle");
        }
    }
    H8SciStatus status_;
    H8SciCycleClock clock_;
    uint8_t smr_=0, scr_=0, brr_=0xff, tdr_=0xff, shift_=0xff;
    uint8_t parity_=0, bits_=0, phase_=0;
    State state_=State::idle;
};
} // namespace yamanix
