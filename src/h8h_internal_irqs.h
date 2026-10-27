// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// H8H internal arbitration adapted from MAME h8_intc.cpp.
// Source identity and scope: docs/h8-internal-irqs.md.
#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>

namespace yamanix {
// Internal vectors 20..63 and an already-qualified NMI latch (vector 7).
// External IRQ pins, edge detection, IER/ISR and MMIO registers are absent.
class H8hInternalIrqs {
public:
    struct Selection {
        int vector = 0;
        int level = -1;
        bool nmi = false;
    };

    void reset() {
        m_pending = 0;
        m_priority = 0;
        // Upstream reset clears pending/ICR but preserves CPU-supplied filters.
    }
    void set_priority_bits(uint32_t bits) { m_priority = bits; }
    void set_filter(int icr, int ipr) { m_icr_filter = icr; m_ipr_filter = ipr; }

    // The owner must call its DMA trigger first, as MAME internal_interrupt
    // does. A consumed request does not add/clear any pending IRQ.
    void raise(unsigned vector, bool dma_consumed = false) {
        require_internal(vector);
        if (!dma_consumed) m_pending |= uint64_t{1} << vector;
    }
    // The owner already established the NMI edge/time condition. No pin or
    // time assumption is hidden here; this method only latches its result.
    void latch_nmi() { m_pending |= uint64_t{1} << 7; }
    void taken(unsigned vector) {
        if (vector != 7) require_internal(vector);
        m_pending &= ~(uint64_t{1} << vector);
    }
    bool pending(unsigned vector) const {
        if (vector != 7) require_internal(vector);
        return (m_pending & (uint64_t{1} << vector)) != 0;
    }
    Selection select() const {
        Selection selected;
        for (unsigned vector = 0; vector < 64; ++vector) {
            if (!(m_pending & (uint64_t{1} << vector))) continue;
            const int icr = vector == 7 ? 2 : int((m_priority >> (slots[vector] ^ 7)) & 1);
            // H8H get_priority supplies IPR=0. The CPU's -1 IPR filter
            // selects ICR level mode; preserve the strict IPR comparison.
            if (icr >= m_icr_filter && 0 > m_ipr_filter) {
                const int level = m_ipr_filter == -1 ? icr : 0;
                if (level > selected.level)
                    selected = {int(vector), level, vector == 7};
            }
        }
        return selected;
    }

private:
    static void require_internal(unsigned vector) {
        if (vector < 20 || vector >= 64)
            throw std::out_of_range("Expected H8H internal vector 20..63");
    }
    inline static constexpr std::array<int, 64> slots = {
        -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,0,1,2,2,
        3,3,3,3,4,4,4,4, 5,5,5,5,6,6,6,6,
        7,7,7,7,8,8,8,8, 9,9,9,9,10,10,10,10,
        11,11,11,11,12,12,12,12, 13,13,13,13,14,14,14,14
    };
    uint64_t m_pending = 0;
    uint32_t m_priority = 0;
    int m_icr_filter = 0, m_ipr_filter = 0;
};
}
