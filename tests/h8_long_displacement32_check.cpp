#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu = yamanix::H8FirstInstructions<Host>;
static unsigned failures;
static void check(bool ok, const char* why) {
    if (!ok && ++failures < 12) std::fprintf(stderr, "%s\n", why);
}
static void one(bool store, unsigned ptr, unsigned reg, uint32_t base, uint32_t disp,
                uint32_t value, unsigned flags) {
    Host h;
    std::array<uint16_t,16> want{};
    std::vector<uint16_t> p;
    for (unsigned r=0; r<8; ++r) {
        uint32_t v = r == ptr ? base : (r+1)*0x1234567u;
        if (store && r == reg) v=value; // Pointer/data alias uses the actual initial ER.
        want[r]=uint16_t(v); want[r+8]=uint16_t(v>>16);
        p.insert(p.end(), {uint16_t(0x7a00|r), uint16_t(v>>16), uint16_t(v)});
    }
    const uint32_t actual_base=uint32_t(want[ptr]) | (uint32_t(want[ptr+8])<<16);
    const uint32_t full=uint32_t(uint64_t(actual_base)+uint64_t(disp));
    const uint32_t first=full&~1u, second=(full+2u)&~1u;
    if (!store) for (unsigned b=0; b<4; ++b)
        h.data[((b<2?first:second)+(b%2))&0xffffff]=uint8_t(value>>(24-8*b));
    p.insert(p.end(), {uint16_t(0x0700|flags), 0x0100, uint16_t(0x7800|(ptr<<4)),
                     uint16_t((store?0x6ba0:0x6b20)|reg), uint16_t(disp>>16), uint16_t(disp)});
    h.program(p); Cpu c(h); c.run(0,110,1000);
    if (store) {
        for (unsigned b=0; b<4; ++b)
            check(h.data[((b<2?first:second)+(b%2))&0xffffff]==uint8_t(value>>(24-8*b)), "Stored high/low word bytes and wrapping");
    } else { want[reg]=uint16_t(value); want[reg+8]=uint16_t(value>>16); }
    check(c.snapshot().registers==want, "Complete register image including pointer/data alias");
    check(c.snapshot().ccr==uint8_t((flags&~14u)|(value==0?4u:((value>>31)?8u:0u))), "NZV and unrelated CCR");
    std::vector<std::pair<uint32_t,uint16_t>> got;
    for (auto [kind,a,v]:h.trace) if (kind==(store?'S':'Q')) got.emplace_back(a,v);
    check(got==std::vector<std::pair<uint32_t,uint16_t>>{{first,uint16_t(value>>16)},{second,uint16_t(value)}}, "Exact two aligned word accesses, high first, full32-bit bus arguments");
}
int main() {
    // Independent half-offset sweeps, not an exhaustive 2^32 cross product.
    for (bool store:{false,true}) for (unsigned high:{0u,0x8000u,0xffffu})
        for (unsigned low=0; low<65536; ++low)
            one(store,0,1,0xab123457,(high<<16)|low,low*0x1020305u,low&255);
    for (bool store:{false,true}) for (unsigned high=0; high<65536; ++high)
        one(store,0,1,0xffffffff,(high<<16)|0xa55au,high*0x1020305u,high&255);
    for (bool store:{false,true}) for (unsigned ptr=0; ptr<8; ++ptr)
        for (unsigned reg=0; reg<8; ++reg) for (uint32_t base:{0u,1u,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})
            for (uint32_t disp:{0u,1u,0x7fffffffu,0x80000000u,0xffff0000u,0xffffffffu})
                for (uint32_t value:{0u,0x80000000u,0xffffffffu,0x12345678u})
                    for (unsigned flags:{0u,0x55u,0xffu}) one(store,ptr,reg,base,disp,value,flags);
    const std::vector<uint16_t> program{0x7a00,0xffff,0xffff,0x7a01,0x89ab,0xcdef,
        0x0100,0x7800,0x6ba1,0,1,0x0100,0x7800,0x6b22,0,1,0x40fe};
    for (unsigned cut=0; cut<=240; ++cut) {
        Host a,b; a.program(program); b.program(program); Cpu x(a),y(b);
        x.run(0,240,10000); y.run(0,int(cut),10000); y.run(cut,int(240-cut),10000);
        check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data, "241 every-cycle splits, five-word IR, bus order and RAM");
    }
    for (unsigned ext=0; ext<65536; ++ext) {
        Host h; h.program({0x0100,0x7800,uint16_t(ext),0,0}); Cpu c(h); bool rejected=false;
        try { c.run(0,15,100); } catch (const std::runtime_error&) { rejected=true; }
        const unsigned op=ext&0xfff8;
        check(rejected!=(op==0x6b20 || op==0x6ba0) && c.failed()==rejected, "All65536 second extension guards, invalid ER8..15 rejected");
    }
    // Original prefix accepts bit7 of the pointer nibble as an ER alias.
    for (unsigned ptr=0; ptr<16; ++ptr) for (bool store:{false,true}) {
        Host h; h.program({uint16_t(0x7a00|(ptr&7)),0x12,0x3457,0x0100,uint16_t(0x7800|(ptr<<4)),uint16_t((store?0x6ba0:0x6b20)|1),0,0});
        Cpu c(h); c.run(0,40,100); check(!c.failed(), "Original first extension pointer aliases accepted");
        std::vector<uint32_t> addresses;
        for (auto [kind,a,v]:h.trace) if(kind==(store?'S':'Q')) addresses.push_back(a);
        check(addresses==std::vector<uint32_t>{0x123456,0x123458}, "Pointer bit7 aliases the same ER and word addresses");
    }
    for (bool store:{false,true}) for (unsigned fail_at:{1u,2u}) {
        struct FaultHost:Host {
            unsigned reads=0,fail_read=0;
            uint16_t read_word(uint32_t a) { if (++reads==fail_read) throw std::runtime_error("word read fault"); return Host::read_word(a); }
        } h;
        h.program(program); if(store)h.fail_word_write=fail_at;else h.fail_read=fail_at;
        yamanix::H8FirstInstructions<FaultHost> c(h); bool rejected=false;
        try { c.run(0,100,1000); } catch(const std::runtime_error&) { rejected=true; }
        check(rejected && c.failed(), "First/second data transfer fault poisons CPU");
        check(h.word_writes==(store?fail_at:2u), "No extra stores after failure");
        if(store) {
            check(h.data[0]==0x89 && h.data[1]==0xab, "Committed high word survives failure");
            if(fail_at==1) check(!h.data.contains(2), "First write fault prevents low-word mutation");
            else check(h.data[2]==0xcd && h.data[3]==0xef, "Low-word mutation survives second write fault");
        } else {
            check(h.reads==fail_at, "Read fault performs no subsequent read");
            check(c.snapshot().registers[2]==0 && c.snapshot().registers[10]==0, "Incomplete load leaves destination ER intact");
        }
        bool again=false; try { c.run(100,1,100); } catch(const std::logic_error&) { again=true; }
        check(again, "Failed owner cannot resume");
    }
    std::printf("Long displacement32:524288 half-offset sweeps,55296 boundary cases,241 splits,65536 second extension guards,aliases/faults:%u failures\n",failures);
    return failures?1:0;
}
