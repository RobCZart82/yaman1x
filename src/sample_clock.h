#ifndef YAMANIX_SAMPLE_CLOCK_H
#define YAMANIX_SAMPLE_CLOCK_H
#include <cstdint>
#include <stdexcept>
namespace yamanix {
// Absolute integer nanoseconds. A tick at exactly t precedes an access at t.
// Block partitioning does not change the number of samples; no rounded periods.
class SampleClock {
public:
    explicit SampleClock(uint32_t rate) : m_rate(rate) {
        if (!rate || rate > 192000) throw std::invalid_argument("sample rate");
    }
    uint64_t due(uint64_t ns) const {
        if (ns < m_time) throw std::invalid_argument("time moved backwards");
        const uint64_t target = (ns / 1000000000) * m_rate +
            ((ns % 1000000000) * m_rate) / 1000000000;
        return target - m_samples;
    }
    template<class Tick> void advance(uint64_t ns, uint64_t limit, Tick tick) {
        const auto count = due(ns);
        if (count > limit) throw std::invalid_argument("sample budget exceeded");
        // Caller must discard its engine if a tick throws after partial progress.
        for (uint64_t i = 0; i < count; ++i) { tick(); ++m_samples; }
        m_time = ns;
    }
    uint64_t samples() const { return m_samples; }
private:
    uint32_t m_rate;
    uint64_t m_time = 0, m_samples = 0;
};
}
#endif
