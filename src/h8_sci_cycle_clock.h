// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Internal-clock scheduling adapted from MAME h8_sci.cpp.
// No serial register/state machine or external-rate approximation.
#pragma once
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace yamanix {
class H8SciCycleClock {
public:
    enum class Mode { async, async_out, sync_out };
    enum class Side : uint8_t { tx = 1, rx = 2 };

    static uint64_t divider_for(uint8_t smr, uint8_t brr) {
        return (uint64_t{2} << (2 * (smr & 3))) * (uint64_t(brr) + 1);
    }
    void configure(uint8_t smr, uint8_t brr, uint8_t scr) {
        if (scr & 2) throw std::invalid_argument("External SCI clock not implemented");
        const auto mode = (smr & 0x80) ? Mode::sync_out : (scr & 1) ? Mode::async_out : Mode::async;
        m_divider = divider_for(smr, brr);
        m_mode = mode;
        // Upstream clock_update changes mode/divider, but an active internal
        // clock keeps its existing step/event until restarted.
    }
    void reset() {
        m_active = 0; m_event = m_step = 0;
        m_divider = 512; m_mode = Mode::async;
    }
    uint64_t next_event() const { return m_event; }
    uint64_t step() const { return m_step; }
    uint64_t divider() const { return m_divider; }
    Mode mode() const { return m_mode; }
    bool active(Side side) const { return (m_active & bit(side)) != 0; }

    // Return true only for a newly started side: the serial-state owner must
    // then initialize that side's 16-phase counter to 15, as upstream does.
    bool start(Side side, uint64_t total_cycles, uint64_t now_as_cycles) {
        const auto flag = bit(side);
        if (m_active & flag) return false;
        if (m_active) { m_active |= flag; return true; }
        const auto now = side == Side::tx ? total_cycles : now_as_cycles;
        const auto event = add(now - now % m_divider, m_divider);
        m_active = flag; m_step = m_divider; m_event = event;
        return true;
    }
    void stop(Side side) {
        m_active &= uint8_t(~bit(side));
        if (!m_active) m_event = m_step = 0;
    }
    void standby_exit(uint64_t total_cycles, uint64_t standby_time) {
        if (!m_event) return;
        if (total_cycles < standby_time) throw std::invalid_argument("SCI standby cycle order");
        m_event = add(m_event, total_cycles - standby_time);
    }
    // Exactly one due edge, even if current_time is far ahead, like upstream
    // internal_update. The owner drains overdue events with its own budget.
    // Callbacks may stop/start sides; RX eligibility is checked after TX.
    // Callback/overflow exceptions after a tick require discarding the owner;
    // side effects cannot be rolled back. No callback is fired when not due.
    template<class Tx, class Rx, class IdleClock>
    uint64_t service_one(uint64_t current_time, Tx tx, Rx rx, IdleClock idle_clock) {
        if (!m_event || current_time < m_event) return m_event;
        const auto dispatch_mode = m_mode;
        if (m_active & 1) tx(dispatch_mode);
        if (m_active & 2) rx(dispatch_mode);
        if (m_active) m_event = add(m_event, m_step);
        else {
            m_event = 0;
            if (m_mode != Mode::async) idle_clock();
        }
        return m_event;
    }
private:
    static uint8_t bit(Side side) {
        const auto value = uint8_t(side);
        if (value != 1 && value != 2) throw std::invalid_argument("SCI clock side");
        return value;
    }
    static uint64_t add(uint64_t a, uint64_t b) {
        if (a > std::numeric_limits<uint64_t>::max() - b)
            throw std::overflow_error("SCI cycle clock overflow");
        return a + b;
    }
    uint64_t m_event = 0, m_step = 0, m_divider = 512;
    uint8_t m_active = 0;
    Mode m_mode = Mode::async;
};
}
