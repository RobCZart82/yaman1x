#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures;
static void check(bool ok,const char* why) {
    if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);
}
static void one(bool store,unsigned ptr,uint32_t base,unsigned disp,unsigned word,unsigned flags) {
    Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> p;
    for(unsigned r=0;r<8;++r) {
        const uint32_t value=r==ptr?base:(r+1)*0x1234567u;
        want[r]=uint16_t(value);want[r+8]=uint16_t(value>>16);
        p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(value>>16),uint16_t(value)});
    }
    // Independent widened arithmetic: wrap the full unsigned32 sum explicitly.
    const uint64_t offset=disp;
    const uint32_t address=uint32_t(uint64_t(base)+offset)&~1u,physical=address&0xffffff;
    h.data[physical]=uint8_t(word>>8);h.data[(physical+1)&0xffffff]=uint8_t(word);
    p.insert(p.end(),{uint16_t(0x0700|flags),0x0140,uint16_t((store?0x7880:0x7800)|(ptr<<4)),uint16_t(store?0x6ba0:0x6b20),uint16_t(disp>>16),uint16_t(disp)});
    h.program(p);Cpu c(h);c.run(0,110,1000);
    check(c.snapshot().registers==want&&c.snapshot().ccr==(store?flags:word>>8),"All registers preserved;complete CCR replacement/high lane");
    const unsigned memory=store?flags*257:word;
    check(h.data.size()==2&&h.data[physical]==(memory>>8)&&h.data[(physical+1)&0xffffff]==(memory&255),"Exact RAM footprint and duplicated store bytes");
    std::vector<std::tuple<char,uint32_t,uint16_t>> data;
    for(auto e:h.trace)if(std::get<0>(e)!='I')data.push_back(e);
    check(data==std::vector<std::tuple<char,uint32_t,uint16_t>>{{store?'S':'Q',address,uint16_t(memory)}},"One transaction,independent unsigned32 addition/alignment/full32 address");
}
struct IrqHost:Host {
    yamanix::H8hIrqController irq;bool nmi,raised=false;std::vector<int> exceptions;
    IrqHost(unsigned flags,bool use_nmi):nmi(use_nmi) {
        irq.set_filter(2,-1);const unsigned v=nmi?7:52;
        words[2*v]=0;words[2*v+1]=0x180;words[0xc0]=0x5670;
        program({0x7a07,0,0x300,0x7a00,0x12,0x3466,0x0780,0x0140,0x7800,0x6b20,0xffff,0xfff1,0,0x40fe});
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
        one(false,ptr,0xab123458,0xffffffff,word,(word*73)&255);
    for(bool store:{false,true})for(unsigned high:{0u,0x8000u,0xffffu})for(unsigned low=0;low<65536;++low)
        one(store,0,0xffffffff,(high<<16)|low,low^0xa55a,low&255);
    for(bool store:{false,true})for(unsigned high=0;high<65536;++high)
        one(store,0,0xffffffff,(high<<16)|0xa55a,high^0xa55a,high&255);
    for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr)
        for(uint32_t base:{0u,1u,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})
            for(unsigned disp:{0u,1u,0x7fffffffu,0x80000000u,0xffff0000u,0xffffffffu})
                for(unsigned flags=0;flags<256;++flags)one(store,ptr,base,disp,((flags^0xa5)<<8)|(flags^0x5a),flags);
    for(bool store:{false,true})for(unsigned ext=0;ext<65536;++ext) {
        Host h;h.program({0x0140,uint16_t(store?0x7880:0x7800),uint16_t(ext),0,0});Cpu c(h);bool rejected=false;
        try{c.run(0,17,100);}catch(const std::runtime_error&){rejected=true;}
        check(rejected!=(ext==(store?0x6ba0u:0x6b20u))&&c.failed()==rejected,"131072 exact third-word guards including cross-direction rejection");
    }
    const std::vector<uint16_t> p{0x7a00,0xab12,0x3458,0x0755,0x0140,0x7880,0x6ba0,0xffff,0xffff,0x0780,0x0140,0x7800,0x6b20,0xffff,0xffff,0x40fe};
    for(unsigned split=0;split<=260;++split) {
        Host a,b;a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,260,10000);
        y.run(0,int(split),10000);y.run(split,int(260-split),10000);
        check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"261 every-cycle split snapshots,RAM and ordered traces");
    }
    for(bool nmi:{false,true})for(unsigned flags=0;flags<256;++flags) {
        IrqHost h(flags,nmi);yamanix::H8FirstInstructions<IrqHost> c(h,0x80,&h.irq);c.run(0,160,10000);
        const bool take=nmi||!(flags&0x80);
        check(h.raised&&h.exceptions.size()==unsigned(take)&&c.snapshot().ccr==flags&&c.snapshot().registers[7]==0x300&&c.snapshot().registers[0]==0x3466,"Filter update,NMI bypass,unchanged pointer and RTE restore");
        if(take)check(h.exceptions[0]==(nmi?7:52)&&h.data[0x2fc]==flags&&h.data[0x2fd]==0&&h.data[0x2fe]==1&&h.data[0x2ff]==0x1a,"Following NOP completes before IRQ frame");
    }
    for(bool nmi:{false,true})for(unsigned split=0;split<=160;++split) {
        IrqHost a(0x55,nmi),b(0x55,nmi);yamanix::H8FirstInstructions<IrqHost> x(a,0x80,&a.irq),y(b,0x80,&b.irq);
        x.run(0,160,10000);y.run(0,int(split),10000);y.run(split,int(160-split),10000);
        check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data&&a.exceptions==b.exceptions&&a.irq.current().level==b.irq.current().level,"322 IRQ/NMI split histories");
    }
    for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr) {
        Host h;h.program({uint16_t(0x7a00|ptr),0,0,0x0755,0x0140,uint16_t((store?0x7880:0x7800)|(ptr<<4)),uint16_t(store?0x6ba0:0x6b20),0xffff,0xffff});
        h.bad_read=!store;h.bad_write=store;Cpu c(h);bool rejected=false;
        try{c.run(0,80,1000);}catch(const std::runtime_error&){rejected=true;}
        check(rejected&&c.failed()&&c.snapshot().registers[ptr]==0&&c.snapshot().registers[ptr+8]==0&&c.snapshot().ccr==0x55,"Fault preserves pointer and previous CCR;poisons owner");
        if(store)check(h.data[0xfffffe]==0x55&&h.data[0xffffff]==0x55,"Write mutation retained after fault");
        bool again=false;try{c.run(80,1,1000);}catch(const std::logic_error&){again=true;}check(again,"Poisoned owner cannot resume");
    }
    std::printf("CCR displacement32:524288 word loads,524288 half-offset sweeps,147456 boundaries,261 splits,131072 third-word guards,512 IRQ masks,322 IRQ splits,16 faults:%u failures\n",failures);
    return failures?1:0;
}
