#pragma once
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>

namespace yamanix {

// Standalone H8_ADC_3337 subset for ADCSR 0x33/0x37. The caller supplies
// a sample at each scheduled edge and owns the event budget.
class H8AdcSlowScan {
public:
    std::optional<uint64_t> deadline() const { return deadline_; }
    unsigned next_channel() const { return first_channel_ + (channel_ + 1) % results_.size(); }
    uint16_t result_word(unsigned channel) const { return uint16_t(results_.at(channel) << 6); }
    uint8_t control() const { return uint8_t((deadline_ ? (start_bit_ ? (0x33 | first_channel_) : (0x13 | first_channel_)) : 0) | (finished_ ? 0x80 : 0)); }

    void start(uint64_t cycle, uint16_t an0, uint16_t previous_an3 = 0, bool previous_adf = false) {
        if (deadline_)
            throw std::logic_error("ADC scan already active");
        if (previous_an3 > 1023)
            throw std::invalid_argument("Previous AN3 sample exceeds ten bits");
        deadline_ = sample_deadline(cycle, 266, an0);
        pending_sample_ = an0;
        results_[3] = previous_an3;
        finished_ = previous_adf;
    }

    void restart(uint64_t cycle, unsigned first_channel, uint16_t sample, bool keep_adf) {
        if (!deadline_ || start_bit_)
            throw std::logic_error("ADC restart requires cleared ADST");
        if (first_channel != 0 && first_channel != 4)
            throw std::invalid_argument("Unsupported ADC scan bank");
        const auto next = sample_deadline(cycle, 266, sample);
        first_channel_ = first_channel;
        channel_ = 0;
        start_bit_ = true;
        write_adf(keep_adf);
        pending_sample_ = sample;
        deadline_ = next;
    }

    void service(uint64_t cycle, uint16_t next_sample) {
        if (!deadline_ || cycle != *deadline_)
            throw std::invalid_argument("ADC scan completion outside deadline");
        const auto next = sample_deadline(cycle, 256, next_sample);
        results_[channel_] = pending_sample_;
        finished_ |= channel_ == 3;
        channel_ = (channel_ + 1) % results_.size();
        pending_sample_ = next_sample;
        deadline_ = next;
    }

    // Original adcsr_w clears ADST without cancelling the existing REPEAT mode.
    void clear_start(bool keep_adf) {
        start_bit_ = false;
        write_adf(keep_adf);
    }
    void write_adf(bool keep) { finished_ &= keep; }
    void reset() { *this = {}; }

private:
    static uint64_t sample_deadline(uint64_t cycle, unsigned delay, uint16_t sample) {
        if (sample > 1023)
            throw std::invalid_argument("ADC scan sample exceeds ten bits");
        if (cycle > std::numeric_limits<uint64_t>::max() - delay)
            throw std::overflow_error("ADC scan deadline exceeds timeline");
        return cycle + delay;
    }

    std::array<uint16_t, 4> results_{};
    std::optional<uint64_t> deadline_;
    uint16_t pending_sample_ = 0;
    unsigned channel_ = 0;
    unsigned first_channel_ = 0;
    bool finished_ = false;
    bool start_bit_ = true;
};

}
