#include "vop3_bridge.h"
#include "vop3.h"
#include "sample_clock.h"
#include <algorithm>
#include <cmath>
#include <array>
#include <cstring>
#include <memory>

struct yamanix_vop3 {
    std::array<std::unique_ptr<Vop3>, 2> chips{std::make_unique<Vop3>(), std::make_unique<Vop3>()};
    yamanix_vop3_metrics metrics{};
    uint32_t eg_readback = 0;
    yamanix::SampleClock clock{Vop3::SR};
};

extern "C" uint32_t yamanix_vop3_abi_version(void) { return 2; }
extern "C" yamanix_vop3 *yamanix_vop3_create(void) {
    try { return new yamanix_vop3; } catch (...) { return nullptr; }
}
extern "C" void yamanix_vop3_destroy(yamanix_vop3 *handle) { delete handle; }
extern "C" int yamanix_vop3_write(yamanix_vop3 *h, uint32_t chip, uint32_t reg, uint16_t value, uint16_t mask) {
    if (!h || chip > 1 || reg > 127) return -1;
    if (mask != 0xffff) return -2;
    try { h->chips[chip]->write(reg, value); ++h->metrics.writes[chip]; return 0; }
    catch (...) { return -3; }
}
extern "C" int yamanix_vop3_read(yamanix_vop3 *h, uint32_t chip, uint32_t reg, uint16_t mask, uint16_t *value) {
    if (!h || !value || chip > 1 || reg > 127) return -1;
    if (mask != 0xffff) return -2;
    try {
        if (reg == 0x22 && h->eg_readback == 1) {
            const auto index = h->chips[chip]->select & 511;
            if (index >= 16) return -4;
            const double scaled = h->chips[chip]->eg[index].level * 64.0;
            if (!std::isfinite(scaled)) return -3;
            *value = static_cast<uint16_t>(static_cast<int16_t>(
                std::clamp(std::trunc(scaled), -32768.0, 32767.0)));
        } else {
            *value = h->chips[chip]->read(reg);
        }
        ++h->metrics.reads[chip];
        if (reg != vop3::EG_STATUS && !(reg == 0x22 && h->eg_readback == 1))
            ++h->metrics.unsupported_reads[chip];
        return 0;
    } catch (...) { return -3; }
}
extern "C" int yamanix_vop3_snapshot(yamanix_vop3 *h, uint8_t *output, size_t length) {
    if (!h || !output || length != 14340) return -1;
    // Fixed byte format, independent of C++ object layout and host byte order.
    auto word = [&](uint16_t value) { *output++ = uint8_t(value >> 8); *output++ = uint8_t(value); };
    for (const auto& chip : h->chips) {
        word(chip->select);
        for (const auto& row : chip->core.prog) for (auto value : row) word(value);
        for (auto value : chip->core.coef) word(value);
        for (auto value : chip->core.tag) *output++ = value;
        for (const auto& row : chip->core.slot) for (auto value : row) word(value);
    }
    return 0;
}
extern "C" int yamanix_vop3_get_metrics(yamanix_vop3 *h, yamanix_vop3_metrics *output) {
    if (!h || !output) return -1;
    *output = h->metrics;
    return 0;
}

extern "C" int yamanix_vop3_set_eg_readback(yamanix_vop3 *h, uint32_t profile) {
    if (!h || profile > 1) return -1;
    h->eg_readback = profile;
    return 0;
}
extern "C" int yamanix_vop3_advance(yamanix_vop3 *h, uint64_t absolute_ns) {
    if (!h) return -1;
    try {
        if (h->clock.due(absolute_ns) > 4096) return -4;
    } catch (const std::invalid_argument &) { return -4; }
    try {
        h->clock.advance(absolute_ns, 4096, [h]() {
            for (unsigned chip = 0; chip < 2; ++chip) {
                h->chips[chip]->run();
                ++h->metrics.sample_passes[chip];
            }
        });
        return 0;
    } catch (...) { return -3; }
}
