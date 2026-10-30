#pragma once
#include <cstdint>
namespace yamanix {
// Project-owned mode equations from pinned H83002 source facts.
// Pure contract only: no CPU binding,MMIO,reset sequence or standby support.
// docs/h83002-syscr-modes.md records the source and integration gates.
struct H83002SyscrModes {
    static constexpr uint8_t cold_value=0x09;
    static constexpr unsigned irq_filter(uint8_t syscr,uint8_t ccr) {
        const bool i=(ccr&0x80)!=0;
        const bool ui=(ccr&0x40)!=0;
        if(!i)return 0;
        return (syscr&0x08)||ui?2:1;
    }
    static constexpr uint8_t entry_ccr(uint8_t syscr,uint8_t ccr) {
        return uint8_t(ccr|((syscr&0x08)?0x80:0xc0));
    }
    static constexpr unsigned trapa_base=8;
};
}
