#include "h8h_irq_controller.h"
#include <cstdio>

int main() {
    using Irq = yamanix::H8hIrqController;
    unsigned failures = 0;
    auto check = [&](bool ok, const char *message) {
        if (!ok && ++failures < 8) std::fprintf(stderr, "%s\n", message);
    };
    for (unsigned line = 0; line < 8; ++line) {
        const uint8_t bit = uint8_t(1u << line);
        Irq level;
        level.set_filter(0,-1);
        level.irq_input(line,true);
        check(level.isr() == bit && level.current().level == -1,"Disabled external latch");
        level.write_ier(bit);
        check(level.current().vector == int(12+line),"Enable latched level");
        level.taken(12+line);
        check(level.isr() == bit,"Asserted level remains latched after take");
        level.write_isr(0);
        check(level.isr() == bit,"Asserted level re-latches after ISR clear");
        level.irq_input(line,false);
        check(level.isr() == bit,"H8H deassert retains ISR");
        level.write_isr(0);
        check(level.isr() == 0 && level.current().level == -1,"Clear deasserted level");
        Irq edge;
        edge.set_filter(0,-1); edge.write_ier(bit); edge.write_iscr(bit);
        edge.irq_input(line,true); edge.write_isr(0); edge.irq_input(line,true);
        check(edge.isr() == 0,"No repeated edge on held input");
        edge.irq_input(line,false); edge.irq_input(line,true);
        check(edge.isr() == bit,"New assertion edge");
        edge.taken(12+line);
        check(edge.isr() == 0,"Edge take clears ISR");
    }
    Irq a,b;
    a.set_filter(0,-1); a.write_ier(1); a.internal_irq(52); a.irq_input(0,true);
    check(a.current().vector == 12,"External lower vector wins equal priority");
    a.nmi_input(true,false);
    check(a.current().vector == 12,"Time-zero NMI ignored");
    a.nmi_input(false,true); a.nmi_input(true,true);
    check(a.current().vector == 7 && a.current().nmi,"Qualified NMI edge");
    a.taken(7); a.nmi_input(true,true);
    check(a.current().vector == 12,"Held NMI does not re-latch");
    a.set_nmi_rising(true); a.nmi_input(false,true);
    check(a.current().vector == 7,"Rising NMI deassert edge");
    a.taken(7); a.irq_input(0,false); a.write_isr(0);
    check(a.current().vector == 52 && b.current().level == -1,"Internal request survives / isolation");
    a.write_icr(0,0x80);
    a.write_icr(1,0x12);
    check(a.icr(0) == 0 && a.icr(1) == 0x12,"Preserve pinned upstream ICR write mask");
    a.write_icr(1,0x01);
    check(a.icr(1) == 0x13,"Pinned ICR same-byte OR behavior");
    a.irq_input(0,true); a.reset();
    check(a.ier() == 0 && a.iscr() == 0 && a.isr() == 1,"Reset retains input and rechecks levels");
    a.set_filter(0,-1);
    check(a.current().level == -1,"Reset pending internal requests cleared");
    bool rejected = false;
    try { a.irq_input(8,true); } catch(const std::out_of_range&) { rejected = true; }
    check(rejected && a.isr() == 1,"Invalid line rejected before mutation");
    rejected = false;
    try { a.write_icr(3,0xff); } catch(const std::out_of_range&) { rejected = true; }
    check(rejected && a.icr(0) == 0,"Invalid ICR offset rejected");
    Irq deferred;
    deferred.set_filter(1,-1); deferred.write_ier(1); deferred.irq_input(0,true);
    deferred.write_icr(0,0x80);
    check(deferred.current().level == -1,"ICR write does not publish immediately");
    deferred.set_filter(1,-1);
    check(deferred.current().vector == 12 && deferred.current().level == 1,
          "Reapplying filter publishes new ICR priority");
    deferred.reset();
    check(deferred.current().vector == 12,"Reset retains last CPU publication until owner refresh");
    deferred.set_filter(0,-1);
    check(deferred.current().level == -1,"Reset refresh uses disabled IER");
    std::printf("H8H IRQ register/pin adapter: %s (%u failures)\n",failures?"FAIL":"PASS",failures);
    return failures?1:0;
}
