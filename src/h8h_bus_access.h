// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// H8H-only bus helpers adapted from pinned MAME h8.cpp (m_has_exr=false).
// Identities and scope: research/h8h-bus-provenance.json.
#pragma once
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace yamanix {
// Bus: read_instruction_word, read_byte, write_byte, read_word, write_word.
// Bus and CPU-owned signed remaining-cycle counter must outlive this adapter.
// No callback reentry or counter mutation. Failure requires discarding owner.
template<class Bus> class H8hBusAccess {
public:
    H8hBusAccess(Bus& bus, int& remaining) : bus_(bus), remaining_(remaining) {}
    H8hBusAccess(const H8hBusAccess&)=delete;
    H8hBusAccess& operator=(const H8hBusAccess&)=delete;
    bool failed() const { return failed_; }
    uint16_t read16i(uint32_t address) {
        return access([&]{return bus_.read_instruction_word(address & ~uint32_t{1});});
    }
    uint8_t read8(uint32_t address) {
        return access([&]{return bus_.read_byte(address);});
    }
    void write8(uint32_t address,uint8_t data) {
        access([&]{bus_.write_byte(address,data);});
    }
    uint16_t read16(uint32_t address) {
        return access([&]{return bus_.read_word(address & ~uint32_t{1});});
    }
    void write16(uint32_t address,uint16_t data) {
        access([&]{bus_.write_word(address & ~uint32_t{1},data);});
    }
private:
    template<class F> decltype(auto) access(F f) {
        if(busy_) throw std::logic_error("Reentrant H8H bus access");
        if(failed_) throw std::logic_error("Failed H8H bus owner");
        if(remaining_<std::numeric_limits<int>::min()+2)
            throw std::overflow_error("H8H cycle counter underflow");
        remaining_-=2; // Charge before calling the bus, including across zero.
        busy_=true;
        try {
            // A guard also handles void operations without duplicating charge.
            struct Guard { bool& busy; ~Guard(){busy=false;} } guard{busy_};
            return f();
        } catch(...) { failed_=true; throw; }
    }
    Bus& bus_;
    int& remaining_;
    bool busy_=false, failed_=false;
};
} // namespace yamanix
