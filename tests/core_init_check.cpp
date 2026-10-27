#include "yss236_core.h"
#include <cstdio>
#include <cstdlib>

#ifdef YAMANIX_INIT_STANDALONE
// Constructor/decode-only sanitizer probe: no JIT allocation or execution.
void yss236_core::jit_free::operator()(jit_code *code) const {
    if (code) std::abort();
}
#endif

int main() {
    for (unsigned repeat = 0; repeat < 4; ++repeat) {
        auto first = std::make_unique<yss236_core>();
        auto second = std::make_unique<yss236_core>();
        for (int step = 0; step < yss236_core::STEPS; ++step) {
            yss236_core::op expected;
            yss236_core::decode(first->prog[step], expected);
            if (std::memcmp(&expected, &first->ops[step], sizeof expected)) return 1;
            first->prog[step][0] = 1;
            first->prog[step][1] = 0xffff;
            first->prog[step][2] = 0xabcd;
            first->prog[step][3] = 0x1234;
            first->prog[step][4] = 0x5678;
            first->decode(step);
            yss236_core::decode(first->prog[step], expected);
            if (std::memcmp(&expected, &first->ops[step], sizeof expected)) return 2;
            first->decode(step); // Unchanged uploads are safe to decode again.
            if (std::memcmp(&expected, &first->ops[step], sizeof expected)) return 3;
            if (second->prog[step][0] || second->ops[step].memop) return 4;
        }
    }
    std::puts("PASS: repeated core construction, all 512 decoded steps, re-decode and instance isolation");
    return 0;
}
