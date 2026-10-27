// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Adapted from pinned MAME h8_intc.cpp; see docs/h8h-irq-controller.md.
#pragma once
#include "h8h_internal_irqs.h"

namespace yamanix {
// H8H register/pin adapter, with the owning CPU's IRQ publication represented
// by current(). It has no clock, CPU callback, DMA engine or address map.
class H8hIrqController {
public:
    using Selection = H8hInternalIrqs::Selection;
    Selection current() const { return m_current; }
    uint8_t ier() const { return m_ier; }
    uint8_t isr() const { return m_isr; }
    uint8_t iscr() const { return m_iscr; }
    uint8_t icr(unsigned offset) const {
        require_offset(offset);
        return uint8_t(m_icr >> (8 * offset));
    }
    void write_icr(unsigned offset, uint8_t value) {
        require_offset(offset);
        // Preserve the pinned upstream expression, including its mask (not
        // complemented) and deferred IRQ publication. Hardware rule unverified.
        const auto mask = uint32_t{0xff} << (8 * offset);
        m_icr = (m_icr & mask) | (uint32_t(value) << (8 * offset));
        m_internal.set_priority_bits(m_icr);
    }
    void set_filter(int icr, int ipr) {
        m_icr_filter = icr; m_ipr_filter = ipr;
        m_internal.set_filter(icr, ipr);
        update();
    }
    void write_ier(uint8_t value) { m_ier = value; update(); }
    void write_isr(uint8_t value) { m_isr &= value; check_levels(false); update(); }
    void write_iscr(uint8_t value) { m_iscr = value; check_levels(true); }
    void set_nmi_rising(bool rising) { m_nmi_rising = rising; }
    void nmi_input(bool asserted, bool machine_time_after_zero) {
        const bool edge = m_nmi_rising ? !asserted && m_nmi_input : asserted && !m_nmi_input;
        m_nmi_input = asserted;
        if (edge && machine_time_after_zero) { m_internal.latch_nmi(); update(); }
    }
    void irq_input(unsigned line, bool asserted) {
        if (line >= 8) throw std::out_of_range("H8H IRQ line");
        const uint8_t bit = uint8_t(1u << line);
        const bool previously_asserted = (m_irq_input & bit) != 0;
        const bool latch = (m_iscr & bit) ? asserted && !previously_asserted : asserted;
        if (asserted) m_irq_input |= bit;
        else m_irq_input &= uint8_t(~bit);
        // H8H ISR latches level requests too; deassert does not clear ISR.
        if (latch) { m_isr |= bit; update(); }
    }
    void internal_irq(unsigned vector, bool dma_consumed = false) {
        m_internal.raise(vector, dma_consumed);
        if (!dma_consumed) update();
    }
    void taken(unsigned vector) {
        if (vector >= 12 && vector < 20) {
            const uint8_t bit = uint8_t(1u << (vector - 12));
            if ((m_iscr & bit) || !(m_irq_input & bit)) m_isr &= uint8_t(~bit);
        } else {
            m_internal.taken(vector); // validates before mutating
        }
        update();
    }
    void reset() {
        m_internal.reset();
        m_iscr = m_ier = m_isr = 0;
        m_nmi_rising = false;
        check_levels(false);
        m_icr = 0;
        // Like upstream device_reset: physical inputs, filters and last CPU
        // publication survive. Owner must reapply CPU filters after reset.
    }
private:
    static void require_offset(unsigned offset) {
        if (offset >= 3) throw std::out_of_range("H8H ICR byte offset");
    }
    void check_levels(bool publish) {
        const uint8_t newly_latched = uint8_t(m_irq_input & uint8_t(~m_iscr) & uint8_t(~m_isr));
        m_isr |= newly_latched;
        if (newly_latched && publish) update();
    }
    void update() {
        auto selected = m_internal.select();
        constexpr unsigned slots[8] = {0,1,2,2,3,3,3,3};
        if (m_ipr_filter < 0) {
            for (unsigned line = 0; line < 8; ++line) {
                if (!(m_isr & m_ier & (1u << line))) continue;
                const int priority = int((m_icr >> (slots[line] ^ 7)) & 1);
                if (priority < m_icr_filter) continue;
                const int level = m_ipr_filter == -1 ? priority : 0;
                const int vector = int(line + 12);
                if (level > selected.level || (level == selected.level && vector < selected.vector))
                    selected = {vector, level, false};
            }
        }
        m_current = selected;
    }
    H8hInternalIrqs m_internal;
    Selection m_current;
    uint32_t m_icr = 0;
    uint8_t m_irq_input = 0, m_ier = 0, m_isr = 0, m_iscr = 0;
    int m_icr_filter = 0, m_ipr_filter = 0;
    bool m_nmi_input = false, m_nmi_rising = false;
};
}
