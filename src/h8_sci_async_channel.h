// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Adapted from pinned MAME h8_sci.cpp; see research/h8-sci-channel-provenance.json.
#pragma once
#include "h8_sci_cycle_clock.h"
#include "h8_sci_status.h"

namespace yamanix {
// One asynchronous SCI channel: shared registers, status latch and cycle clock.
// Sink: tx(bool), clock(bool), tx_irq(), end_irq(), rx_irq(), error_irq().
// Sink callbacks must not re-enter or reconfigure this owner.
class H8SciAsyncChannel {
public:
    enum class TxState { idle, start, bit, parity, stop, last_tick };
    enum class RxState { idle, start, bit, parity, stop };
    TxState tx_state() const { return tx_state_; }
    RxState rx_state() const { return rx_state_; }
    uint8_t smr() const { return smr_; }
    uint8_t scr() const { return scr_; }
    uint8_t brr() const { return brr_; }
    uint8_t tdr() const { return tdr_; }
    uint8_t tx_phase() const { return tx_phase_; }
    uint8_t rx_phase() const { return rx_phase_; }
    bool input() const { return input_; }
    uint64_t next_event() const { return clock_.next_event(); }
    uint8_t read_ssr(bool effects=true) { return status_.read(effects); }
    uint8_t read_rdr(bool dma=false, bool effects=true) {
        if (dma) status_.dma_receive_read(effects);
        return rdr_;
    }
    template<class Sink> void reset(Sink& sink) {
        status_.reset(); clock_.reset();
        smr_=scr_=tx_phase_=rx_phase_=tx_bits_=rx_bits_=rdr_=rx_shift_=tx_parity_=rx_parity_=0;
        brr_=tdr_=tx_shift_=0xff; tx_state_=TxState::idle; rx_state_=RxState::idle;
        // Preserve physical RX input and SSR read latch, as upstream reset does.
        sink.clock(true); sink.tx(true);
    }
    void write_smr(uint8_t data) {
        if (data & 0x84) throw std::invalid_argument("SCI sync/MP mode not implemented");
        clock_.configure(data,brr_,scr_); smr_=data;
    }
    void write_brr(uint8_t data) { clock_.configure(smr_,data,scr_); brr_=data; }
    template<class Sink> void write_scr(uint8_t data,Sink& sink) {
        clock_.configure(smr_,brr_,data);
        const auto delta=uint8_t(scr_^data); scr_=data;
        if ((delta & 0x10) && !(scr_ & 0x10)) {
            rx_state_=RxState::idle; clock_.stop(H8SciCycleClock::Side::rx);
        }
        if ((delta & 0x80) && (scr_ & 0x80) && (status_.value() & H8SciStatus::tdre)) sink.tx_irq();
        if ((delta & 0x04) && (scr_ & 0x04) && (status_.value() & H8SciStatus::tend)) sink.end_irq();
        if ((delta & 0x40) && (scr_ & 0x40) && (status_.value() & H8SciStatus::rdrf)) sink.rx_irq();
        if ((delta & 0x40) && (scr_ & 0x40) && status_.receive_error()) sink.error_irq();
    }
    template<class Sink> void write_tdr(uint8_t data,bool dma,uint64_t cycles,Sink& sink) {
        tdr_=data;
        if (dma && (scr_ & 0x20)) {
            status_.dma_transmit_write(true);
            if (tx_state_==TxState::idle) tx_start(cycles,sink);
        }
    }
    template<class Sink> void write_ssr(uint8_t data,uint64_t cycles,Sink& sink) {
        status_.write(data,(scr_ & 0x20)!=0);
        if (tx_state_==TxState::idle && !(status_.value() & H8SciStatus::tdre)) tx_start(cycles,sink);
    }
    void write_rx(bool value,uint64_t now_as_cycles) {
        if (value!=input_ && clock_.active(H8SciCycleClock::Side::rx))
            if (rx_phase_==1 || rx_phase_==15) rx_phase_=0;
        if (rx_state_==RxState::idle && (scr_ & 0x10) && input_ && !value) {
            rx_parity_=(smr_ & 0x10)?0:1; rx_shift_=0; rx_state_=RxState::start; rx_bits_=1;
            if (clock_.start(H8SciCycleClock::Side::rx,now_as_cycles,now_as_cycles)) rx_phase_=15;
        }
        input_=value;
    }
    // One due shared edge, TX before RX; RX eligibility is rechecked after TX.
    template<class Sink> uint64_t service_one(uint64_t cycles,Sink& sink) {
        return clock_.service_one(cycles,[&](auto mode) {
            tx_phase_=uint8_t((tx_phase_+1)&15);
            if (!tx_phase_) {
                tx_step(cycles,sink);
                if (mode==H8SciCycleClock::Mode::async_out) sink.clock(false);
            } else if (tx_phase_==8 && mode==H8SciCycleClock::Mode::async_out) sink.clock(true);
        },[&](auto) {
            rx_phase_=uint8_t((rx_phase_+1)&15);
            if (rx_phase_==8) rx_step(sink);
        },[&]{sink.clock(true);});
    }
private:
    template<class Sink> void tx_start(uint64_t cycles, Sink& sink) {
        status_.raise_flags(H8SciStatus::tdre);
        tx_shift_=tdr_; tx_parity_=(smr_ & 0x10)?0:1;
        if (scr_ & 0x80) sink.tx_irq();
        tx_state_=TxState::start; tx_bits_=1;
        if (clock_.start(H8SciCycleClock::Side::tx,cycles,cycles)) tx_phase_=15;
    }
    template<class Sink> void tx_step(uint64_t cycles, Sink& sink) {
        switch (tx_state_) {
        case TxState::start:
            sink.tx(false); tx_state_=TxState::bit; tx_bits_=(smr_ & 0x40)?7:8; break;
        case TxState::bit:
            tx_parity_^=tx_shift_ & 1; sink.tx((tx_shift_ & 1)!=0); tx_shift_>>=1;
            if (!--tx_bits_) {
                tx_state_=(smr_ & 0x20)?TxState::parity:TxState::stop;
                tx_bits_=(tx_state_==TxState::parity)?1:((smr_ & 0x08)?2:1);
            }
            break;
        case TxState::parity:
            sink.tx(tx_parity_!=0); tx_state_=TxState::stop; tx_bits_=(smr_ & 0x08)?2:1; break;
        case TxState::stop:
            sink.tx(true);
            if (!--tx_bits_) {
                if (!(status_.value() & H8SciStatus::tdre)) tx_start(cycles,sink);
                else tx_state_=TxState::last_tick;
            }
            break;
        case TxState::last_tick:
            tx_state_=TxState::idle; tx_bits_=0;
            clock_.stop(H8SciCycleClock::Side::tx);
            sink.tx(true);
            if (scr_ & 0x04) sink.end_irq();
            if ((scr_ & 0x20) && !(status_.value() & H8SciStatus::tdre)) tx_start(cycles,sink);
            else status_.raise_flags(H8SciStatus::tend);
            break;
        default: throw std::logic_error("SCI TX clock tick while idle");
        }
    }
    template<class Sink> void rx_done(Sink& sink) {
        if (!(status_.value() & H8SciStatus::fer)) {
            if ((smr_ & 0x20) && rx_parity_) status_.raise_flags(H8SciStatus::per);
            else if (status_.value() & H8SciStatus::rdrf) status_.raise_flags(H8SciStatus::orer);
            else { status_.raise_flags(H8SciStatus::rdrf); rdr_=rx_shift_; }
        }
        if (scr_ & 0x40) {
            if (status_.receive_error()) sink.error_irq();
            else sink.rx_irq();
        }
        rx_state_=RxState::idle; clock_.stop(H8SciCycleClock::Side::rx);
    }
    template<class Sink> void rx_step(Sink& sink) {
        switch (rx_state_) {
        case RxState::start:
            if (input_) { rx_state_=RxState::idle; clock_.stop(H8SciCycleClock::Side::rx); break; }
            rx_state_=RxState::bit; rx_bits_=(smr_ & 0x40)?7:8; break;
        case RxState::bit:
            rx_shift_>>=1;
            if (input_) { rx_parity_=uint8_t(!rx_parity_); rx_shift_|=(smr_ & 0x40)?0x40:0x80; }
            if (!--rx_bits_) { rx_state_=(smr_ & 0x20)?RxState::parity:RxState::stop; rx_bits_=1; }
            break;
        case RxState::parity:
            rx_parity_^=uint8_t(input_); rx_state_=RxState::stop; rx_bits_=1; break;
        case RxState::stop:
            if (!input_) status_.raise_flags(H8SciStatus::fer);
            else if ((smr_ & 0x20) && rx_parity_) status_.raise_flags(H8SciStatus::per);
            rx_done(sink); break;
        default: throw std::logic_error("SCI RX clock tick while idle");
        }
    }
    H8SciStatus status_;
    H8SciCycleClock clock_;
    uint8_t smr_=0,scr_=0,brr_=0xff,tdr_=0xff,rdr_=0;
    uint8_t tx_phase_=0,rx_phase_=0,tx_bits_=0,rx_bits_=0;
    uint8_t tx_shift_=0xff,rx_shift_=0,tx_parity_=0,rx_parity_=0;
    bool input_=true;
    TxState tx_state_=TxState::idle;
    RxState rx_state_=RxState::idle;
};
} // namespace yamanix
