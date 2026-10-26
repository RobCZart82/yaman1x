#include "upload_control_contract.h"
#include <cstdio>
#include <initializer_list>

int main() {
    yamanix::UploadControlContract control;
    auto require = [](bool condition) { if (!condition) std::puts("FAIL: experimental upload-control contract"); return condition; };
    if (!require(control.status() == 0)) return 1;
    for (unsigned cycle = 0; cycle < 100; ++cycle) {
        if (!require(control.write(1, 0xffff) && control.status() == 0x8000)) return 1;
        if (!require(control.write(1, 0xffff) && control.status() == 0x8000)) return 1;
        for (uint16_t invalid : {uint16_t(2), uint16_t(0x8000), uint16_t(0xffff)})
            if (!require(!control.write(invalid, 0xffff) && control.status() == 0x8000)) return 1;
        if (!require(!control.write(0, 0x00ff) && control.status() == 0x8000)) return 1;
        if (!require(!control.write(0, 0xff00) && control.status() == 0x8000)) return 1;
        if (!require(control.write(0, 0xffff) && control.status() == 0)) return 1;
    }
    std::puts("PASS: logical request/release, repeat commands and unsupported-write rejection (not hardware validation)");
}
