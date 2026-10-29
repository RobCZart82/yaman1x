#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static void one(bool store,unsigned ptr,uint32_t base,unsigned word,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> p;
 for(unsigned r=0;r<8;++r){uint32_t v=r==ptr?base:0x12345678u+r*0x1020304u;want[r]=uint16_t(v);want[r+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)});}
 const uint32_t address=base&~1u,physical=address&0xffffff;
 h.data[physical]=uint8_t(word>>8);h.data[(physical+1)&0xffffff]=uint8_t(word);
 p.insert(p.end(),{uint16_t(0x0700|flags),0x0140,uint16_t((store?0x6980:0x6900)|(ptr<<4))});h.program(p);Cpu c(h);c.run(0,100,1000);
 const unsigned expected=store?flags:word>>8;
 check(c.snapshot().ccr==expected&&c.snapshot().registers==want,"Complete CCR replacement and all registers preserved");
 const unsigned stored=store?(flags*0x101):word;
 check(h.data[physical]==(stored>>8)&&h.data[(physical+1)&0xffffff]==(stored&255)&&h.data.size()==2,"Word memory lanes and neighboring RAM unchanged");
 std::vector<std::tuple<char,uint32_t,uint16_t>> data;for(auto entry:h.trace)if(std::get<0>(entry)!='I')data.push_back(entry);
 check(data==std::vector<std::tuple<char,uint32_t,uint16_t>>{{store?'S':'Q',address,uint16_t(store?stored:word)}},"Exactly one aligned word access retains full bus address");
}
struct IrqHost:Host {
 yamanix::H8hIrqController irq;bool nmi=false,raised=false;std::vector<int> exceptions;
 IrqHost(unsigned flags,bool use_nmi):nmi(use_nmi){irq.set_filter(2,-1);unsigned v=nmi?7:52;words[2*v]=0;words[2*v+1]=0x180;words[0xc0]=0x5670;program({0x7a07,0,0x300,0x7a00,0x12,0x3457,0x0780,0x0140,0x6900,0,0x40fe});data[0x123456]=uint8_t(flags);data[0x123457]=uint8_t(~flags);}
 uint16_t read_word(uint32_t a){auto value=Host::read_word(a);if(a==0x123456&&!raised){raised=true;if(nmi)irq.nmi_input(true,true);else irq.internal_irq(52);}return value;}
 void exception_hook(int v){exceptions.push_back(v);}
};
int main(){
 for(unsigned ptr=0;ptr<8;++ptr)for(unsigned word=0;word<65536;++word)one(false,ptr,0xab123457,word,(word*73)&255);
 for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr)for(uint32_t base:{0u,1u,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})for(unsigned flags=0;flags<256;++flags)one(store,ptr,base,((flags^0xa5)<<8)|(flags^0x5a),flags);
 for(unsigned ext=0;ext<65536;++ext){Host h;h.program({0x0140,uint16_t(ext)});Cpu c(h);bool rejected=false;try{c.run(0,11,100);}catch(const std::runtime_error&){rejected=true;}bool valid=(ext&0xff8f)==0x6900||(ext&0xff8f)==0x6980;check(rejected!=valid&&c.failed()==rejected,"65536 CCR prefix extensions reject unsupported forms");}
 const std::vector<uint16_t> p{0x7a00,0xab12,0x3457,0x0755,0x0140,0x6980,0x0780,0x0140,0x6900,0x40ec};
 for(unsigned split=0;split<=200;++split){Host a,b;a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"201 split snapshots, RAM and ordered bus traces");}
 for(bool nmi:{false,true})for(unsigned flags=0;flags<256;++flags){IrqHost h(flags,nmi);yamanix::H8FirstInstructions<IrqHost> c(h,0x80,&h.irq);c.run(0,160,10000);bool take=nmi||!(flags&0x80);check(h.raised&&h.exceptions.size()==unsigned(take)&&c.snapshot().ccr==flags&&c.snapshot().registers[7]==0x300,"LDC updates IRQ mask, NMI bypass and RTE restore");if(take)check(h.exceptions[0]==(nmi?7:52)&&h.data[0x2fc]==flags&&h.data[0x2fd]==0&&h.data[0x2fe]==1&&h.data[0x2ff]==0x14,"No-IRQ LDC completion permits following NOP before IRQ frame");}
 for(bool nmi:{false,true})for(unsigned split=0;split<=160;++split){IrqHost a(0x55,nmi),b(0x55,nmi);yamanix::H8FirstInstructions<IrqHost> x(a,0x80,&a.irq),y(b,0x80,&b.irq);x.run(0,160,10000);y.run(0,int(split),10000);y.run(split,int(160-split),10000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data&&a.exceptions==b.exceptions&&a.irq.current().level==b.irq.current().level,"322 IRQ/NMI split histories");}
 for(bool store:{false,true}){Host h;h.program({0x0140,uint16_t(store?0x6980:0x6900)});h.bad_read=!store;h.bad_write=store;Cpu c(h);bool rejected=false;try{c.run(0,80,1000);}catch(const std::runtime_error&){rejected=true;}check(rejected&&c.failed(),"Word bus fault poisons CPU");bool again=false;try{c.run(80,1,1000);}catch(const std::logic_error&){again=true;}check(again,"Failed CPU cannot resume");}
 std::printf("CCR indirect:524288 word cases,24576 address/CCR cases,65536 prefix guards,201 splits,512 IRQ/NMI masks,322 IRQ/NMI splits,faults:%u failures\n",failures);return failures?1:0;
}
