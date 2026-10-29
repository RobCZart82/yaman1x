#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include "h8h_irq_controller.h"
#include <cstdio>
static unsigned failures;
static void check(bool ok,const char* why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
struct SleepHost:Host {
 yamanix::H8hIrqController irq;bool nmi=false,raised=false,no_events=false;uint64_t trigger=55;
 std::vector<uint64_t> ticks;std::vector<int> exceptions;
 SleepHost(unsigned flags,bool use_nmi,bool none=false):nmi(use_nmi),no_events(none){irq.set_filter((flags&0x80)?2:0,-1);unsigned v=nmi?7:52;words[2*v]=0;words[2*v+1]=0x180;words[0xc0]=0x5670;program({0x7a07,0,0x300,uint16_t(0x0700|flags),0x0180,0xf85a,0x40fe});}
 uint64_t update(uint64_t cycle){if(no_events)return 0;while(event<=cycle){ticks.push_back(event);if(!raised&&event>=trigger){raised=true;if(nmi)irq.nmi_input(true,true);else irq.internal_irq(52);}event+=11;}return event;}
 void exception_hook(int vector){exceptions.push_back(vector);}
};
using Cpu=yamanix::H8FirstInstructions<SleepHost>;
int main(){
 for(bool nmi:{false,true})for(unsigned flags=0;flags<256;++flags){SleepHost h(flags,nmi);Cpu c(h,0,&h.irq);c.run(0,160,10000);bool wake=nmi||!(flags&0x80);check(h.raised&&!h.ticks.empty(),"Peripheral events advance while sleeping");check(c.snapshot().registers[0]==(wake?0x5a:0)&&c.snapshot().registers[7]==0x300,"Wake executes prefetched following instruction,RTE restores SP");check(h.exceptions.size()==unsigned(wake),"Masked IRQ stays asleep,NMI bypasses I");if(wake)check(h.exceptions[0]==(nmi?7:52)&&h.data[0x2fc]==flags&&h.data[0x2fd]==0&&h.data[0x2fe]==1&&h.data[0x2ff]==0x0a,"Sleep wake stacks next PC and loaded CCR");else check(c.snapshot().state==0x0180&&c.snapshot().substate==3&&c.snapshot().ccr==flags,"Masked sleep stays at continuation3 without changing CCR");}
 for(bool nmi:{false,true})for(unsigned flags:{0u,0x80u})for(unsigned cut=0;cut<=160;++cut){SleepHost a(flags,nmi),b(flags,nmi);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,160,10000);y.run(0,int(cut),10000);y.run(cut,int(160-cut),10000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data&&a.ticks==b.ticks&&a.exceptions==b.exceptions&&a.irq.current().vector==b.irq.current().vector,"644 every-cycle sleep/wake snapshots,deadline ticks,bus/RAM/IRQ histories");}
 for(bool no_events:{false,true}){SleepHost h(0x80,false,no_events);h.trigger=10000;Cpu c(h,0,&h.irq);c.run(0,160,10000);auto before=c.snapshot();auto trace=h.trace;c.run(160,160,10000);auto after=c.snapshot();check(after.state==0x0180&&after.substate==3&&before.registers==after.registers&&before.pc==after.pc&&before.ccr==after.ccr&&trace==h.trace,"Sleeping slices do not refetch or execute following instruction");check(no_events?h.ticks.empty():h.ticks.size()>14,"No-deadline sleep consumes budget;periodic peripherals continue");}
 {SleepHost h(0,false,true);yamanix::H8FirstInstructions<SleepHost>c(h);c.run(0,160,10000);check(c.snapshot().state==0x0180&&c.snapshot().substate==3&&h.exceptions.empty(),"Optional absent IRQ owner sleeps indefinitely");}
 // Wake with an already pending unmasked IRQ at SLEEP entry.
 {SleepHost h(0,false,true);Cpu c(h,0,&h.irq);c.run(0,14,1000);h.irq.internal_irq(52);c.run(14,146,10000);check(h.exceptions.size()==1&&c.snapshot().registers[0]==0x5a,"Pending IRQ wakes without future peripheral deadline");}
 {struct Bad:SleepHost{Bad():SleepHost(0,false,true){}uint16_t read_instruction_word(uint32_t a){if(a==0x10a)throw std::runtime_error("sleep prefetch fault");return Host::read_instruction_word(a);}}h;yamanix::H8FirstInstructions<Bad>c(h);bool bad=false;try{c.run(0,160,1000);}catch(const std::runtime_error&){bad=true;}check(bad&&c.failed(),"Sleep prefetch failure poisons CPU");bool again=false;try{c.run(160,1,100);}catch(const std::logic_error&){again=true;}check(again,"Failed sleep owner cannot resume");}
 std::printf("SLEEP:512 IRQ/NMI masks,644 every-cycle partitions,no-event/peripheral-progress/pending-IRQ/fault controls:%u failures\n",failures);return failures?1:0;
}
