// YamANIx headless DSP integration check. No Yamaha firmware or microcode embedded.
#include "vop3.h"
#include <cmath>
#include <cstdio>
#include <memory>

static void upload(Vop3& chip) {
    // Synthetic serial loopback with one pass of pipeline latency.
    const uint16_t program[2][5] = {{0, 0, 1 << 7, 0, 0}, {0, 0, 0, 0, 1 | 1 << 6}};
    for (int step = 0; step < 2; ++step) {
        chip.write(vop3::SELECT, step);
        for (int word = 0; word < 5; ++word)
            chip.write(vop3::PROGRAM - word, program[step][word]);
    }
}

int main() {
    auto reference = std::make_unique<Vop3>();
    auto compiled = std::make_unique<Vop3>();
    reference->core.interpret = true;
    upload(*reference);
    upload(*compiled);
    double previous = 0;
    bool jit_seen = false;
    constexpr int samples = 4096;
    for (int sample = 0; sample < samples; ++sample) {
        const double input = ((sample % 257) - 128) / 256.0;
        for (Vop3* chip : {reference.get(), compiled.get()}) {
            chip->core.in[0][0] = input * Vop3::WORD;
            chip->run();
        }
        const double actual = reference->core.out[0] / Vop3::WORD;
        const double native = compiled->core.out[0] / Vop3::WORD;
        if (!std::isfinite(actual) || actual != previous || native != actual) {
            std::fprintf(stderr, "Serial I/O mismatch at sample %d: expected %.17g, interpreter %.17g, default %.17g\n",
                         sample, previous, actual, native);
            return 1;
        }
        jit_seen |= compiled->core.m_jit != nullptr;
        previous = input;
    }
    if (yss236_core::jit_target && !jit_seen) {
        std::fprintf(stderr, "JIT supported but never activated\n");
        return 1;
    }
    std::printf("YamANIx: %d serial samples verified; pipeline latency 1 pass; mode %s.\n",
                samples, jit_seen ? "interpreter + JIT" : "interpreter only (JIT unavailable)");
    std::puts("Synthetic DSP integration only: AN1x CPU firmware is not executing.");
    return 0;
}
