#include "h8_adc_slow_scan.h"
#include <array>

using yamanix::H8AdcSlowScan;

static void check(bool condition) {
    if (!condition)
        throw std::runtime_error("Slow ADC scan contract failed");
}

int main() {
    constexpr std::array<uint64_t, 8> edges{306, 562, 818, 1074, 1330, 1586, 1842, 2098};
    constexpr std::array<unsigned, 8> channels{0, 1, 2, 3, 0, 1, 2, 3};
    constexpr std::array<uint16_t, 9> samples{1, 257, 513, 1023, 5, 21, 42, 128, 0};
    for (uint64_t cut = 40; cut <= edges.back(); ++cut) {
        H8AdcSlowScan adc;
        adc.start(40, samples[0]);
        adc.write_adf(true);
        check(adc.control() == 0x33);
        std::array<uint16_t, 4> expected{};
        unsigned event = 0;
        for (uint64_t horizon : {cut, edges.back()}) {
            while (adc.deadline() && *adc.deadline() <= horizon) {
                check(event < edges.size() && adc.deadline() == edges[event]);
                check(adc.next_channel() == (event + 1) % 4);
                adc.service(edges[event], samples[event + 1]);
                expected[channels[event]] = uint16_t(samples[event] * 64);
                for (unsigned channel = 0; channel < expected.size(); ++channel)
                    check(adc.result_word(channel) == expected[channel]);
                check(adc.control() == (event == 3 || event == 7 ? 0xb3 : 0x33));
                if (event == 3 || event == 7) {
                    adc.write_adf(true);
                    check(adc.control() == 0xb3);
                    adc.write_adf(false);
                    check(adc.control() == 0x33);
                }
                ++event;
            }
        }
        check(event == edges.size() && adc.deadline() == 2354);
        adc.reset();
        check(!adc.deadline() && adc.control() == 0 && adc.result_word(3) == 0);
    }
    for (uint64_t cut = 0; cut <= 1034; ++cut) {
        H8AdcSlowScan scan;
        scan.start(0, 42);
        scan.service(266, 43);
        scan.clear_start(false);
        scan.restart(300, 4, 100, false);
        check(scan.control() == 0x37 && scan.deadline() == 566 &&
              scan.result_word(0) == 42 * 64 && scan.next_channel() == 5);
        unsigned event = 0;
        constexpr std::array<uint64_t,4> upper_edges{566,822,1078,1334};
        for (uint64_t horizon : {300 + cut, uint64_t(1334)}) {
            while (*scan.deadline() <= horizon) {
                check(event < 4 && scan.deadline() == upper_edges[event]);
                check(scan.next_channel() == (event == 3 ? 4 : event + 5));
                scan.service(upper_edges[event], uint16_t(101 + event));
                check(scan.result_word(event) == (100 + event) * 64);
                check(scan.control() == (event == 3 ? 0xb7 : 0x37));
                ++event;
            }
        }
        check(event == 4 && scan.deadline() == 1590);
        scan.clear_start(true);
        scan.restart(1400, 0, 7, true);
        check(scan.control() == 0xb3 && scan.deadline() == 1666 &&
              scan.result_word(3) == 103 * 64 && scan.next_channel() == 1);
    }
    H8AdcSlowScan adc;
    adc.start(40, 1);
    for (auto cycle : {305, 307}) {
        bool rejected = false;
        try { adc.service(cycle, 2); } catch (const std::invalid_argument&) { rejected = true; }
        check(rejected && adc.deadline() == 306 && adc.result_word(0) == 0);
    }
    bool rejected = false;
    try { adc.service(306, 1024); } catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && adc.deadline() == 306 && adc.result_word(0) == 0);
}
