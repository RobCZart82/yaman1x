#include "sci_access_trace.h"
#include <array>
#include <cstdio>
#include <vector>

int main() {
    using Trace = yamanix::SciAccessTrace;
    using Event = std::array<uint64_t, 6>;
    std::vector<Event> events;
    auto emit = [&](auto seq, auto channel, auto op, auto reg, auto data, auto mask) {
        events.push_back({seq, channel, uint64_t(op), reg, data, mask});
    };
    int failures = 0;
    auto check = [&](bool ok, const char *message) {
        if (!ok) { std::fprintf(stderr, "%s\n", message); ++failures; }
    };
    Trace a, b;
    a.observe(0, 'W', 0xffffb2, 0x705a, 0xffff, emit);
    a.observe(1, 'W', 0xffffba, 0x1020, 0xff00, emit);
    a.observe(1, 'R', 0xffffbc, 0x843c, 0xffff, emit);
    check(events == std::vector<Event>{{0,0,'W',2,0x70,0xff}, {1,0,'W',3,0x5a,0xff},
                                      {2,1,'W',2,0x10,0xff}, {3,1,'R',5,0x3c,0xff}},
          "SCI channel, byte lane or ordering mismatch");
    a.observe(0, 'R', 0xffffb0, 0xffff, 0xffff, emit);
    a.observe(0, 'R', 0xffffb4, 0xffff, 0xff00, emit);
    a.observe(0, 'W', 0xffffb0, 0xffff, 0, emit);
    a.observe(2, 'W', 0xffffb0, 0xffff, 0xffff, emit);
    a.observe(0, 'W', 0xffffb1, 0xffff, 0xffff, emit);
    a.observe(0, 'W', 0xffffb8, 0xffff, 0xffff, emit);
    a.observe(0, 'X', 0xffffb0, 0xffff, 0xffff, emit);
    check(a.events() == 4, "Polling, empty or invalid access was recorded");
    a.observe(0, 'W', 0xffffb2, 0x1000, 0x1000, emit);
    check(events.back() == Event{4,0,'W',2,0x10,0x10}, "Partial mask lost");
    b.observe(0, 'W', 0xffffb0, 0x1234, 0xffff, emit);
    check(b.events() == 2 && a.events() == 5, "Observer instances share state");
    uint64_t emitted = 0;
    Trace bounded;
    for (uint64_t i = 0; i < Trace::capacity + 7; ++i)
        bounded.observe(0, 'W', 0xffffb2, 0, 0xff00,
            [&](auto...) { ++emitted; });
    check(emitted == Trace::capacity && bounded.events() == Trace::capacity + 7 &&
          bounded.dropped() == 7, "Overflow must be bounded and counted");
    std::printf("SCI passive bus observer: %s\n", failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
