#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <functional>
static unsigned failures;
static void check(bool ok,const char* why) {
    if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);
}
struct ObservedHost:Host {
    std::function<void()> observe;
    uint16_t read_word(uint32_t a){observe();return Host::read_word(a);}
    void write_word(uint32_t a,uint16_t v){observe();Host::write_word(a,v);}
};
using Cpu=yamanix::H8FirstInstructions<ObservedHost>;
static void one(bool store,unsigned ptr,uint32_t base,unsigned word,unsigned flags) {
    ObservedHost h;std::array<uint16_t,16> want{};std::vector<uint16_t> p;
    for(unsigned r=0;r<8;++r) {
        uint32_t v=r==ptr?base:(r+1)*0x1234567u;
        want[r]=uint16_t(v);want[r+8]=uint16_t(v>>16);
        p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)});
    }
    const uint32_t next=store?uint32_t(uint64_t(base)+0xfffffffeu):uint32_t(uint64_t(base)+2);
    const uint32_t address=(store?next:base)&~1u,physical=address&0xffffff;
    want[ptr]=uint16_t(next);want[ptr+8]=uint16_t(next>>16);
    h.data[physical]=uint8_t(word>>8);h.data[(physical+1)&0xffffff]=uint8_t(word);
    p.insert(p.end(),{uint16_t(0x0700|flags),0x0140,uint16_t((store?0x6d80:0x6d00)|(ptr<<4))});
    h.program(p);Cpu c(h);unsigned calls=0;
    h.observe=[&]{++calls;check(c.snapshot().registers==want&&c.snapshot().ccr==flags,"Pointer changes before data callback;CCR changes after successful read");};
    c.run(0,110,1000);check(calls==1,"One observed data callback");
    check(c.snapshot().registers==want&&c.snapshot().ccr==(store?flags:word>>8),"Final complete register/CCR state");
    const unsigned memory=store?flags*257:word;
    check(h.data.size()==2&&h.data[physical]==(memory>>8)&&h.data[(physical+1)&0xffffff]==(memory&255),"Word lanes and exact memory footprint");
    std::vector<std::tuple<char,uint32_t,uint16_t>> data;
    for(auto e:h.trace)if(std::get<0>(e)!='I')data.push_back(e);
    check(data==std::vector<std::tuple<char,uint32_t,uint16_t>>{{store?'S':'Q',address,uint16_t(memory)}},"Exact old/new aligned full address,word value and transaction count");
}
struct IrqHost:Host {
    yamanix::H8hIrqController irq;bool nmi,raised=false;std::vector<int> exceptions;
    IrqHost(unsigned flags,bool use_nmi):nmi(use_nmi) {
        irq.set_filter(2,-1);const unsigned v=nmi?7:52;
        words[2*v]=0;words[2*v+1]=0x180;words[0xc0]=0x5670;
        program({0x7a07,0,0x300,0x7a00,0x12,0x3457,0x0780,0x0140,0x6d00,0,0x40fe});
        data[0x123456]=uint8_t(flags);data[0x123457]=uint8_t(~flags);
    }
    uint16_t read_word(uint32_t a) {
        auto value=Host::read_word(a);
        if(a==0x123456&&!raised){raised=true;if(nmi)irq.nmi_input(true,true);else irq.internal_irq(52);}
        return value;
    }
    void exception_hook(int v){exceptions.push_back(v);}
};
int main() {
    for(unsigned ptr=0;ptr<8;++ptr)for(unsigned word=0;word<65536;++word)
        one(false,ptr,0xab123457,word,(word*73)&255);
    for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr)
        for(uint32_t base:{0u,1u,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})
            for(unsigned flags=0;flags<256;++flags)one(store,ptr,base,((flags^0xa5)<<8)|(flags^0x5a),flags);
    const std::vector<uint16_t> p{0x7a00,0xab12,0x3459,0x0755,0x0140,0x6d80,0x0780,0x0140,0x6d00,0x40fe};
    for(unsigned split=0;split<=200;++split) {
        Host a,b;a.program(p);b.program(p);yamanix::H8FirstInstructions<Host> x(a),y(b);
        x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);
        check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"201 split snapshots,temporary state,RAM and ordered trace");
    }
    for(bool nmi:{false,true})for(unsigned flags=0;flags<256;++flags) {
        IrqHost h(flags,nmi);yamanix::H8FirstInstructions<IrqHost> c(h,0x80,&h.irq);c.run(0,160,10000);
        const bool take=nmi||!(flags&0x80);
        check(h.raised&&h.exceptions.size()==unsigned(take)&&c.snapshot().ccr==flags&&c.snapshot().registers[7]==0x300&&c.snapshot().registers[0]==0x3459,"Filter update,NMI bypass,postincrement and RTE restore");
        if(take)check(h.exceptions[0]==(nmi?7:52)&&h.data[0x2fc]==flags&&h.data[0x2fd]==0&&h.data[0x2fe]==1&&h.data[0x2ff]==0x14,"Following NOP completes before IRQ frame");
    }
    for(bool nmi:{false,true})for(unsigned split=0;split<=160;++split) {
        IrqHost a(0x55,nmi),b(0x55,nmi);yamanix::H8FirstInstructions<IrqHost> x(a,0x80,&a.irq),y(b,0x80,&b.irq);
        x.run(0,160,10000);y.run(0,int(split),10000);y.run(split,int(160-split),10000);
        check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data&&a.exceptions==b.exceptions&&a.irq.current().level==b.irq.current().level,"322 IRQ/NMI every-cycle split histories");
    }
    for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr) {
        Host h;h.program({uint16_t(0x7a00|ptr),0xffff,0xffff,0x0755,0x0140,uint16_t((store?0x6d80:0x6d00)|(ptr<<4))});
        h.bad_read=!store;h.bad_write=store;yamanix::H8FirstInstructions<Host> c(h);bool rejected=false;
        try{c.run(0,80,1000);}catch(const std::runtime_error&){rejected=true;}
        const uint32_t next=store?0xfffffffdu:1u;
        check(rejected&&c.failed()&&c.snapshot().registers[ptr]==uint16_t(next)&&c.snapshot().registers[ptr+8]==uint16_t(next>>16)&&c.snapshot().ccr==0x55,"Fault preserves preceding pointer change and previous CCR");
        if(store)check(h.data[0xfffffc]==0x55&&h.data[0xfffffd]==0x55,"Write mutation retained after fault");
        bool again=false;try{c.run(80,1,1000);}catch(const std::logic_error&){again=true;}check(again,"Poisoned CPU cannot resume");
    }
    std::printf("CCR autoindex:524288 loads,24576 boundary cases,201 splits,512 IRQ masks,322 IRQ splits,16 faults:%u failures\n",failures);
    return failures?1:0;
}
