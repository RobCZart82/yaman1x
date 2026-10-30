#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <cstdio>
struct Sink{std::vector<int> exceptions;void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}void exception_hook(int v){exceptions.push_back(v);}};
using Bus=yamanix::An1xSciBus<Sink>;
unsigned failures=0;void check(bool b,const char*s){if(!b&&++failures<12)std::fprintf(stderr,"%s\n",s);}
template<class F>void rejects(F f,const char*s){bool b=false;try{f();}catch(const std::exception&){b=true;}check(b,s);}
void words(std::vector<uint8_t>&r,unsigned a,const std::vector<uint16_t>&p){for(unsigned i=0;i<p.size();++i){r[a+i*2]=uint8_t(p[i]>>8);r[a+i*2+1]=uint8_t(p[i]);}}
std::vector<uint8_t> image(unsigned mode,unsigned ccr,bool trap){std::vector<uint8_t>r(0x100000);r[2]=1;for(unsigned v:{7u,8u,12u}){r[v*4+2]=1;r[v*4+3]=0x80;}
 words(r,0x100,{0x7a07,0xff,0xfefe,0x7a00,0xff,0xfff2,uint16_t(0xfa00|mode),0x688a,uint16_t(0x0700|ccr),uint16_t(trap?0x5700:0x0180),0x40fe});
 // Capture entry CCR before any MOV changes flags;then store it in NVRAM.
 words(r,0x180,{0x020a,0x7a03,0x20,0x400,0x68ba,uint16_t(trap?0x40fe:0x5670)});return r;}
struct Host:Bus{bool raised=false,switched=false;unsigned priority;bool nmi,change;Host(std::span<const uint8_t>r,Sink&s,unsigned p,bool n,bool ch):Bus(r,{},s),priority(p),nmi(n),change(ch){write_byte(0xfffff4,1);write_byte(0xfffff5,1);if(p)write_byte(0xfffff8,0x80);}
 uint64_t next_bus_event()const{auto next=Bus::next_bus_event();auto e=!raised?100:change&&!switched?150:0;return e&&(!next||uint64_t(e)<next)?uint64_t(e):next;}
 void events(){if(!raised&&cycle()>=100){raised=true;if(nmi)irq().nmi_input(true,true);else irq().irq_input(0,true);}if(change&&!switched&&cycle()>=150){switched=true;write_byte(0xfffff2,1);}}
 uint64_t update(uint64_t t){Bus::update(t);events();return next_bus_event();}void bus_cycle(uint64_t t){Bus::bus_cycle(t);events();}
};
int main(){
 for(unsigned mode:{1u,9u})for(unsigned mask:{0u,0x40u,0x80u,0xc0u})for(unsigned priority:{0u,1u})for(bool nmi:{false,true}){auto r=image(mode,mask,false);bool take=nmi||!(mask&0x80)||(mode==1&&!(mask&0x40)&&priority==1);unsigned expected=mask|(mode==1?0xc0:0x80);
 for(unsigned cut=0;cut<=240;++cut){Sink sa,sb;Host a(r,sa,priority,nmi,false),b(r,sb,priority,nmi,false);yamanix::H8FirstInstructions<Host>x(a,0,&a.irq()),y(b,0,&b.irq());x.run(0,240,10000);y.run(0,int(cut),10000);y.run(cut,int(240-cut),10000);check(sa.exceptions.size()==unsigned(take)&&(!take||sa.exceptions[0]==(nmi?7:12)),"Independent mode/I/UI/priority/NMI selection");check(a.read_byte(0x200400)==(take?expected:0),"IRQ entry CCR uses current SYSCR mode");check(x.snapshot().ccr==mask&&x.snapshot().registers[7]==0xfefe,"RTE restores CCR and internal RAM stack");check(x.snapshot()==y.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.exceptions==sb.exceptions&&a.irq().current().vector==b.irq().current().vector,"7712 every-cycle IRQ/NMI/mode partitions");}}
 for(unsigned mode:{1u,9u})for(unsigned ccr=0;ccr<256;++ccr){auto r=image(mode,ccr,true);Sink s;Bus b(r,{},s);yamanix::H8FirstInstructions<Bus>cpu(b,0,&b.irq());cpu.run(0,160,10000);check(s.exceptions==std::vector<int>{8}&&b.read_byte(0x200400)==uint8_t(ccr|(mode==1?0xc0:0x80)),"512 TRAPA entry CCR captures all incoming bits");check(b.read_byte(0xfffefa)==ccr&&cpu.snapshot().registers[7]==0xfefa,"TRAPA stack preserves incoming CCR before mode entry");}
 for(unsigned cut=0;cut<=240;++cut){auto r=image(9,0x80,false);Sink sa,sb;Host a(r,sa,1,false,true),b(r,sb,1,false,true);yamanix::H8FirstInstructions<Host>x(a,0,&a.irq()),y(b,0,&b.irq());x.run(0,240,10000);y.run(0,int(cut),10000);y.run(cut,int(240-cut),10000);check(b.switched&&b.syscr_value()==1&&sb.exceptions==std::vector<int>{12}&&b.read_byte(0x200400)==0xc0,"Pending priority1 IRQ wakes SLEEP immediately after host changes09 to01 with I=1/UI=0");check(x.snapshot()==y.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.exceptions==sb.exceptions&&a.syscr_value()==b.syscr_value(),"241 mode-write-during-SLEEP partitions");}
 {auto r=image(9,0,false);Sink s;Bus a(r,{},s),b(r,{},s);check(a.read_byte(0xabfffff2)==9&&b.syscr_value()==9,"Cold09 readback and24-bit alias");a.sync_cpu_ccr(0x80);a.write_byte(0xfffff8,0x80);a.write_byte(0xfffff5,1);a.irq().irq_input(0,true);check(a.irq().current().vector==0,"Mode09 masks priority1 with I");a.write_byte(0xfffff2,1);check(a.irq().current().vector==12&&b.syscr_value()==9,"Write01 republishes filter immediately and owner isolated");a.write_byte(0xfffff2,9);check(a.irq().current().vector==0,"Write09 masks pending priority1 immediately");}
 for(unsigned v=0;v<256;++v)if(v!=1&&v!=9){auto r=image(9,0,false);Sink s;Bus b(r,{},s);rejects([&]{b.write_byte(0xfffff2,uint8_t(v));},"Unsupported SYSCR bits fail closed");check(b.failed()&&b.syscr_value()==9,"Rejected mode retains byte and poisons host");}
 for(bool read:{false,true}){auto r=image(9,0,false);Sink s;Bus b(r,{},s);rejects([&]{if(read)b.read_word(0xfffff2);else b.write_word(0xfffff2,0x0100);},"F2/F3 incomplete word cannot partly change SYSCR");check(b.syscr_value()==9,"Rejected word preserves SYSCR");}
 std::printf("Shared SYSCR:7712 IRQ/NMI partitions,512 TRAPA values,pending SLEEP mode switch,readback/alias/isolation and unsupported-bit controls:%u failures\n",failures);return failures?1:0;
}
