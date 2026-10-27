#ifndef YAMANIX_SCI_ACCESS_TRACE_H
#define YAMANIX_SCI_ACCESS_TRACE_H
#include <cstdint>

namespace yamanix {
// Passive research observer. Input is an aligned H8 16-bit bus access.
// Emit writes to SCI byte registers and reads of RDR, never polling reads.
class SciAccessTrace {
public:
    static constexpr uint64_t capacity = 4096;
    uint64_t events() const { return m_events; }
    uint64_t dropped() const { return m_dropped; }

    template<class Emit>
    void observe(unsigned channel, char op, uint32_t address, uint16_t data,
                 uint16_t mask, Emit emit) {
        if (channel > 1 || (op != 'R' && op != 'W')) return;
        const uint32_t base = channel ? 0xffffb8 : 0xffffb0;
        if (address < base || address > base + 4 || (address & 1)) return;
        for (unsigned lane = 0; lane < 2; ++lane) {
            const unsigned shift = lane ? 0 : 8;
            const auto byte_mask = uint8_t(mask >> shift);
            const auto reg = unsigned(address - base + lane);
            if (!byte_mask || (op == 'R' && reg != 5)) continue;
            const auto seq = m_events++;
            if (seq >= capacity) { ++m_dropped; continue; }
            emit(seq, channel, op, reg, uint8_t(data >> shift), byte_mask);
        }
    }
private:
    uint64_t m_events = 0, m_dropped = 0;
};
}
#endif
