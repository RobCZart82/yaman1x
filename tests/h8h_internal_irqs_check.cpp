#include "h8h_internal_irqs.h"
#include <cstdio>
#include <initializer_list>

int main() {
    using Irqs = yamanix::H8hInternalIrqs;
    unsigned failures = 0;
    auto check = [&](bool ok, const char *name) {
        if (!ok && ++failures < 8) std::fprintf(stderr, "%s\n", name);
    };
    // Each shared ICR slot independently routes the following internal groups.
    constexpr unsigned groups[][2] = {{20,23},{24,27},{28,31},{32,35},{36,39},
        {40,43},{44,47},{48,51},{52,55},{56,59},{60,63}};
    for (unsigned group = 0; group < 11; ++group) {
        Irqs irq;
        irq.set_filter(1, -1);
        irq.set_priority_bits(uint32_t{1} << ((group + 4) ^ 7));
        for (unsigned vector = 20; vector < 64; ++vector) {
            irq.raise(vector);
            const bool eligible = vector >= groups[group][0] && vector <= groups[group][1];
            check(irq.select().vector == (eligible ? int(vector) : 0), "ICR slot mapping");
            irq.taken(vector);
        }
    }
    for (unsigned first = 20; first < 64; ++first)
    for (unsigned second = first; second < 64; ++second) {
        Irqs irq;
        irq.set_filter(0, -1);
        irq.raise(second); irq.raise(first);
        check(irq.select().vector == int(first), "Equal priority chooses lower vector");
        irq.taken(first);
        check(irq.select().vector == (first == second ? 0 : int(second)), "Taken clears one pending bit");
        irq.latch_nmi();
        check(irq.select().vector == 7 && irq.select().level == 2 && irq.select().nmi,
              "Qualified NMI priority");
        irq.taken(7);
        check(!irq.select().nmi, "NMI acknowledgement");
    }
    Irqs a, b;
    a.set_filter(0, -1);
    a.raise(20); a.raise(63);
    a.set_priority_bits(1u << (14 ^ 7));
    check(a.select().vector == 63 && a.select().level == 1, "Higher priority wins over lower vector");
    a.reset();
    a.raise(63, true);
    check(!a.pending(63), "DMA consumes fresh request");
    a.raise(63); a.raise(63, true);
    check(a.pending(63) && !b.pending(63), "DMA does not clear old pending / isolated instances");
    check(b.select().level == -1, "Fresh controller has no selected request");
    a.set_filter(0, 0);
    check(a.select().level == -1, "IPR zero filter blocks requests");
    a.set_filter(0, -1);
    check(a.select().vector == 63, "Highest bit shift is unsigned and safe");
    a.set_filter(1, -1);
    a.set_priority_bits(1u << (14 ^ 7));
    check(a.select().vector == 63, "Priority change visible at next selection");
    a.set_filter(2, -1);
    check(a.select().level == -1, "ICR threshold blocks maskable request");
    a.latch_nmi();
    check(a.select().vector == 7, "NMI passes ICR threshold two");
    a.reset(); a.raise(63);
    check(a.select().level == -1 && !a.pending(7), "Reset clears ICR/pending and retains filters");
    a.set_filter(0, -2);
    check(a.select().level == 0, "Non-minus-one negative IPR filter uses IPR priority");
    for (unsigned invalid : {0u, 6u, 7u, 12u, 19u, 64u, 255u}) {
        bool rejected = false;
        try { a.raise(invalid); } catch (const std::out_of_range &) { rejected = true; }
        check(rejected && a.pending(63), "Reject unsupported vector without mutation");
    }
    std::printf("H8H internal IRQ arbitration: %s (%u failures)\n", failures ? "FAIL" : "PASS", failures);
    return failures ? 1 : 0;
}
