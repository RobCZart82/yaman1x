#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures;
static void check(bool ok,const char* why) {
    if(!ok && ++failures<12) std::fprintf(stderr,"%s\n",why);
}
static void one(bool wide,bool store,uint32_t encoded,unsigned word,unsigned flags) {
    Host h; std::array<uint16_t,16> want{}; std::vector<uint16_t> p;
    for(unsigned r=0;r<8;++r) {
        uint32_t value=(r+1)*0x1234567u;
        want[r]=uint16_t(value);want[r+8]=uint16_t(value>>16);
        p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(value>>16),uint16_t(value)});
    }
    uint32_t full=wide?encoded:((encoded&0x8000)?(0xffff0000u|(encoded&65535)):(encoded&65535));
    full&=~1u; const uint32_t physical=full&0xffffff;
    h.data[physical]=uint8_t(word>>8);h.data[(physical+1)&0xffffff]=uint8_t(word);
    p.insert(p.end(),{uint16_t(0x0700|flags),0x0140,uint16_t((wide?0x6b20:0x6b00)|(store?0x80:0))});
    if(wide)p.push_back(uint16_t(encoded>>16));p.push_back(uint16_t(encoded));
    h.program(p);Cpu c(h);c.run(0,110,1000);
    const unsigned expected=store?flags:word>>8;
    check(c.snapshot().ccr==expected && c.snapshot().registers==want,"Complete CCR replacement and unchanged register image");
    const unsigned memory=store?flags*257:word;
    check(h.data.size()==2 && h.data[physical]==(memory>>8) && h.data[(physical+1)&0xffffff]==(memory&255),"Word lanes and exact RAM footprint");
    std::vector<std::tuple<char,uint32_t,uint16_t>> data;
    for(auto e:h.trace)if(std::get<0>(e)!='I')data.push_back(e);
    check(data==std::vector<std::tuple<char,uint32_t,uint16_t>>{{store?'S':'Q',full,uint16_t(memory)}},"Exactly one aligned word transaction with full signed16/unsigned32 address");
}
struct IrqHost:Host {
    yamanix::H8hIrqController irq; bool nmi,raised=false; uint32_t target; std::vector<int> exceptions;
    IrqHost(bool wide,unsigned flags,bool use_nmi):nmi(use_nmi),target(wide?0xab123456u:0x3456u) {
        irq.set_filter(2,-1);const unsigned v=nmi?7:52;
        words[2*v]=0;words[2*v+1]=0x180;words[0xc0]=0x5670;
        std::vector<uint16_t> p{0x7a07,0,0x300,0x0780,0x0140,uint16_t(wide?0x6b20:0x6b00)};
        if(wide)p.push_back(0xab12);p.insert(p.end(),{0x3457,0,0x40fe});program(p);
        data[target&0xffffff]=uint8_t(flags);data[(target+1)&0xffffff]=uint8_t(~flags);
    }
    uint16_t read_word(uint32_t a) {
        auto value=Host::read_word(a);
        if(a==target && !raised){raised=true;if(nmi)irq.nmi_input(true,true);else irq.internal_irq(52);}
        return value;
    }
    void exception_hook(int v){exceptions.push_back(v);}
};
int main() {
    for(bool wide:{false,true})for(unsigned word=0;word<65536;++word)
        one(wide,false,wide?0xab123457u:0x8001u,word,(word*73)&255);
    for(bool store:{false,true})for(unsigned address=0;address<65536;++address)
        one(false,store,address,((address^0xa5)&255)*256+((address^0x5a)&255),address&255);
    for(bool store:{false,true})for(unsigned high:{0u,0x8000u,0xffffu})for(unsigned low=0;low<65536;++low)
        one(true,store,(high<<16)|low,low^0xa55a,low&255);
    for(bool store:{false,true})for(unsigned high=0;high<65536;++high)
        one(true,store,(high<<16)|0xa55a,high^0xa55a,high&255);
    for(bool wide:{false,true})for(bool store:{false,true})for(unsigned flags=0;flags<256;++flags)
        for(uint32_t address:{0u,1u,0x7fffu,0x8000u,0xffffu,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})
            one(wide,store,address,((flags^0xa5)<<8)|(flags^0x5a),flags);
    const std::vector<uint16_t> p{0x0755,0x0140,0x6b80,0x8001,0x0140,0x6b00,0x8001,
        0x07aa,0x0140,0x6ba0,0xffff,0xffff,0x0140,0x6b20,0xffff,0xffff,0x40fe};
    for(unsigned split=0;split<=240;++split) {
        Host a,b;a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,10000);
        y.run(0,int(split),10000);y.run(split,int(240-split),10000);
        check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"241 full/partial split snapshots,RAM and ordered traces");
    }
    for(bool wide:{false,true})for(bool nmi:{false,true})for(unsigned flags=0;flags<256;++flags) {
        IrqHost h(wide,flags,nmi);yamanix::H8FirstInstructions<IrqHost> c(h,0x80,&h.irq);c.run(0,160,10000);
        const bool take=nmi||!(flags&0x80);
        check(h.raised&&h.exceptions.size()==unsigned(take)&&c.snapshot().ccr==flags&&c.snapshot().registers[7]==0x300,"LDC filter update,NMI bypass,RTE restoration");
        if(take)check(h.exceptions[0]==(nmi?7:52)&&h.data[0x2fc]==flags&&h.data[0x2fd]==0&&h.data[0x2fe]==1&&h.data[0x2ff]==(wide?0x12:0x10),"Following NOP completes before interrupt frame");
    }
    for(bool wide:{false,true})for(bool nmi:{false,true})for(unsigned split=0;split<=160;++split) {
        IrqHost a(wide,0x55,nmi),b(wide,0x55,nmi);
        yamanix::H8FirstInstructions<IrqHost> x(a,0x80,&a.irq),y(b,0x80,&b.irq);
        x.run(0,160,10000);y.run(0,int(split),10000);y.run(split,int(160-split),10000);
        check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data&&a.exceptions==b.exceptions&&a.irq.current().level==b.irq.current().level,"644 IRQ/NMI split histories");
    }
    for(bool wide:{false,true})for(bool store:{false,true}) {
        Host h;std::vector<uint16_t> code{0x0755,0x0140,uint16_t((wide?0x6b20:0x6b00)|(store?0x80:0))};
        if(wide)code.push_back(0xffff);code.push_back(0xffff);h.program(code);h.bad_read=!store;h.bad_write=store;
        Cpu c(h);bool rejected=false;try{c.run(0,80,1000);}catch(const std::runtime_error&){rejected=true;}
        check(rejected&&c.failed(),"Word bus fault poisons CPU");
        if(store)check(h.data[0xfffffe]==0x55&&h.data[0xffffff]==0x55,"Host mutation is retained after write fault");
        else check(c.snapshot().ccr==0x55,"Read fault leaves previous CCR intact");
        bool again=false;try{c.run(80,1,1000);}catch(const std::logic_error&){again=true;}
        check(again,"Failed CPU cannot resume");
    }
    std::printf("CCR absolute:131072 word loads,131072 abs16 addresses,524288 abs32 half sweeps,9216 CCR boundaries,241 splits,1024 IRQ masks,644 IRQ splits,faults:%u failures\n",failures);
    return failures?1:0;
}
