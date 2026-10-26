#include "sample_clock.h"
#include <cstdio>
#include <limits>
#include <initializer_list>
#include <stdexcept>
static void require(bool v) { if (!v) throw std::runtime_error("clock assertion"); }
int main() {
    for (uint32_t rate : {44100u, 48000u, 192000u}) {
        yamanix::SampleClock whole(rate), split(rate);
        uint64_t a = 0, b = 0;
        whole.advance(1000000000, rate, [&] { ++a; });
        for (uint64_t t = 0; t < 1000000000; t += 997003)
            split.advance(t, 256, [&] { ++b; });
        split.advance(1000000000, 256, [&] { ++b; });
        require(a == rate && b == a && whole.samples() == split.samples());
        const uint64_t boundary = (1000000000ull + rate - 1) / rate;
        yamanix::SampleClock edge(rate);
        require(edge.due(boundary - 1) == 0 && edge.due(boundary) == 1);
        edge.advance(boundary, 1, []{});
        edge.advance(boundary, 0, []{});
        require(edge.samples() == 1);
        bool rejected = false;
        try { edge.advance(boundary - 1, 1, []{}); } catch (const std::invalid_argument&) { rejected = true; }
        require(rejected && edge.samples() == 1);
        rejected = false;
        try { edge.advance(1000000000, 1, []{}); } catch (const std::invalid_argument&) { rejected = true; }
        require(rejected && edge.samples() == 1);
        yamanix::SampleClock huge(rate);
        const uint64_t maximum = std::numeric_limits<uint64_t>::max();
        // Independently computed with arbitrary-precision multiplication before division.
        const uint64_t expected = rate == 44100 ? 813501413650591ull :
            rate == 48000 ? 885443715538058ull : 3541774862152233ull;
        require(huge.due(maximum) == expected);
    }
    std::puts("PASS: rational sample count, 44.1/48/192 kHz, partition independence, boundaries and atomic time/budget rejection");
}
