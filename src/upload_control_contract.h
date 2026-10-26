#pragma once
#include <cstdint>

namespace yamanix {
// Experimental logical contract inferred from AN1x v1.04 host code, NOT measured YSS236 silicon.
// command=1 requests a stopped/uploadable state and the host waits for status bit 15.
// command=0 releases it and the host waits for bit 15 to clear.
// Other status bits and acknowledgement timing are NOT established; zero bits are assumptions.
class UploadControlContract {
public:
    bool write(uint16_t value, uint16_t mask) {
        if (mask != 0xffff || value > 1) return false;
        m_stopped = value != 0;
        return true;
    }
    uint16_t status() const { return m_stopped ? 0x8000 : 0; }
private:
    bool m_stopped = false;
};
}
