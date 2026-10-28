#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
struct IrqHost:Host {
 yamanix::H8hIrqController irq;
 bool nmi=false,raised=false,pin=false;std::vector<int> exceptions;
 IrqHost(uint8_t ccr,bool use_nmi=false,bool use_pin=false):nmi(use_nmi),pin(use_pin){irq.set_filter(0,-1);if(pin){irq.write_ier(1);irq.write_iscr(1);}const unsigned vector=nmi?7:(pin?12:52);words[2*vector]=0;words[2*vector+1]=0x180;program({0x7a07,0,0x300,uint16_t(0x0700|ccr),0,0x40fe});words[0xc0]=0x5670;}
 uint64_t update(uint64_t cycle){if(cycle>=12 && !raised){raised=true;if(nmi)irq.nmi_input(true,true);else if(pin)irq.irq_input(0,true);else irq.internal_irq(52);}while(event<=cycle)event+=11;return !raised && event>12?12:event;}
 void exception_hook(int vector){exceptions.push_back(vector);}
};
using Cpu=yamanix::H8FirstInstructions<IrqHost>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 for(bool nmi:{false,true})for(unsigned ccr=0;ccr<256;++ccr){IrqHost h(uint8_t(ccr),nmi);Cpu c(h,0,&h.irq);c.run(0,160,10000);auto s=c.snapshot();const bool take=nmi || !(ccr&0x80);check(h.exceptions.size()==unsigned(take) && s.registers[7]==0x300 && s.ccr==ccr,"IRQ mask/NMI bypass and RTE SP/CCR restore");if(take){check(h.exceptions[0]==(nmi?7:52) && s.taken_vector==h.exceptions[0] && h.irq.current().level==-1,"Taken vector/controller acknowledgement");check(h.data[0x2fc]==(nmi?0x80:ccr) && h.data[0x2fd]==0 && h.data[0x2fe]==1 && h.data[0x2ff]==(nmi?6:0x0a),"IRQ frame after no-IRQ LDC prefetch and NOP boundary");}}
 for(bool nmi:{false,true})for(unsigned split=0;split<=160;++split){IrqHost a(0x55,nmi),b(0x55,nmi);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,160,10000);y.run(0,int(split),10000);y.run(split,int(160-split),10000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data && a.exceptions==b.exceptions && a.irq.current().level==b.irq.current().level,"IRQ full/partial event split invariance");}
 {IrqHost h(0,false,true);Cpu c(h,0,&h.irq);c.run(0,160,10000);check(h.exceptions==std::vector<int>{12} && h.irq.isr()==0 && h.irq.current().level==-1 && c.snapshot().registers[7]==0x300,"External edge IRQ pin vector and acknowledgement");}
 for(unsigned fault=1;fault<=2;++fault){IrqHost h(0);h.fail_word_write=fault;Cpu c(h,0,&h.irq);bool threw=false;try{c.run(0,80,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed() && h.irq.current().vector==52 && h.exceptions.empty(),"Stack failure does not acknowledge pending IRQ");}
 {IrqHost a(0),b(0);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,160,10000);check(b.exceptions.empty() && !b.raised && y.snapshot().state==0x10000,"Controller/CPU instance isolation");}
 std::printf("IRQ/NMI fixed SYSCR: 512 mask/return cases, 322 IRQ/NMI splits, faults and isolation: %u failures\n",failures);return failures?1:0;
}
