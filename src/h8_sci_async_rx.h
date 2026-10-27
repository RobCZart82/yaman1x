// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Adapted from pinned MAME h8_sci.cpp; see research/h8-sci-rx-provenance.json.
#pragma once
#include "h8_sci_cycle_clock.h"
#include "h8_sci_status.h"

namespace yamanix {
// RX-only owner. Sink: rx_irq(), error_irq(), clock(bool); no reentrant calls.
// Shared TX/RX registers, CPU/IRQ hookup and standby scheduling are not modeled.
class H8SciAsyncRx {
public:
    enum class State { idle, start, bit, parity, stop };
    State state() const { return state_; }
    uint8_t smr() const { return smr_; }
    uint8_t scr() const { return scr_; }
    uint8_t brr() const { return brr_; }
    uint8_t phase() const { return phase_; }
    bool input() const { return input_; }
    uint64_t next_event() const { return clock_.next_event(); }
    uint8_t read_ssr(bool effects=true) { return status_.read(effects); }
    void write_ssr(uint8_t data) { status_.write(data,false); }
    uint8_t read_rdr(bool dma=false, bool effects=true) {
        if (dma) status_.dma_receive_read(effects);
        return rdr_;
    }
    template<class Sink> void reset(Sink& sink) {
        status_.reset(); clock_.reset();
        smr_=scr_=phase_=bits_=rdr_=shift_=parity_=0; brr_=0xff;
        state_=State::idle;
        // Upstream reset retains the physical RX input and SSR read latch.
        sink.clock(true);
    }
    void write_smr(uint8_t data) {
        if (data & 0x84) throw std::invalid_argument("SCI RX sync/MP mode not implemented");
        clock_.configure(data,brr_,scr_); smr_=data;
    }
    void write_brr(uint8_t data) { clock_.configure(smr_,data,scr_); brr_=data; }
    template<class Sink> void write_scr(uint8_t data, Sink& sink) {
        if (data & 0xa4) throw std::invalid_argument("SCI RX-only owner cannot enable TX controls");
        clock_.configure(smr_,brr_,data);
        const auto delta=uint8_t(scr_^data); scr_=data;
        if ((delta & 0x10) && !(scr_ & 0x10)) {
            state_=State::idle; clock_.stop(H8SciCycleClock::Side::rx);
        }
        if ((delta & 0x40) && (scr_ & 0x40)) {
            if (status_.value() & H8SciStatus::rdrf) sink.rx_irq();
            if (status_.receive_error()) sink.error_irq();
        }
    }
    // Input transitions are supplied at machine-cycle times, not injected bytes.
    // now_as_cycles is the receiver's clock view, matching upstream clock_start.
    void write_rx(bool value, uint64_t now_as_cycles) {
        if (value!=input_ && clock_.active(H8SciCycleClock::Side::rx))
            if (phase_==1 || phase_==15) phase_=0;
        if (state_==State::idle && (scr_ & 0x10) && input_ && !value) {
            parity_=(smr_ & 0x10)?0:1; shift_=0; state_=State::start; bits_=1;
            if (clock_.start(H8SciCycleClock::Side::rx,now_as_cycles,now_as_cycles)) phase_=15;
        }
        input_=value;
    }
    // Exactly one due edge; drain with a bounded scheduler at individual deadlines.
    template<class Sink> uint64_t service_one(uint64_t cycles, Sink& sink) {
        return clock_.service_one(cycles,[](auto){},[&](auto) {
            phase_=uint8_t((phase_+1)&15);
            if (phase_==8) step(sink);
        },[&]{sink.clock(true);});
    }
private:
    template<class Sink> void done(Sink& sink) {
        if (!(status_.value() & H8SciStatus::fer)) {
            if ((smr_ & 0x20) && parity_) status_.raise_flags(H8SciStatus::per);
            else if (status_.value() & H8SciStatus::rdrf) status_.raise_flags(H8SciStatus::orer);
            else { status_.raise_flags(H8SciStatus::rdrf); rdr_=shift_; }
        }
        if (scr_ & 0x40) {
            if (status_.receive_error()) sink.error_irq();
            else sink.rx_irq();
        }
        state_=State::idle; clock_.stop(H8SciCycleClock::Side::rx);
    }
    template<class Sink> void step(Sink& sink) {
        switch (state_) {
        case State::start:
            if (input_) { state_=State::idle; clock_.stop(H8SciCycleClock::Side::rx); break; }
            state_=State::bit; bits_=(smr_ & 0x40)?7:8; break;
        case State::bit:
            shift_>>=1;
            if (input_) { parity_=uint8_t(!parity_); shift_|=(smr_ & 0x40)?0x40:0x80; }
            if (!--bits_) { state_=(smr_ & 0x20)?State::parity:State::stop; bits_=1; }
            break;
        case State::parity:
            parity_^=uint8_t(input_); state_=State::stop; bits_=1; break;
        case State::stop:
            if (!input_) status_.raise_flags(H8SciStatus::fer);
            else if ((smr_ & 0x20) && parity_) status_.raise_flags(H8SciStatus::per);
            done(sink); break;
        default: throw std::logic_error("SCI RX clock tick while idle");
        }
    }
    H8SciStatus status_;
    H8SciCycleClock clock_;
    uint8_t smr_=0, scr_=0, brr_=0xff, phase_=0, bits_=0, rdr_=0, shift_=0, parity_=0;
    bool input_=true;
    State state_=State::idle;
};
} // namespace yamanix
