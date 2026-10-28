#include "h8_sci_duplex_fixture.h"
struct OverrunHost:DuplexHost {
 uint8_t second;
 OverrunHost(unsigned ch,uint8_t first,uint8_t next):DuplexHost(ch,ch,0x96,first),second(next){
  words[0x83]=0x0780; // Keep IRQ masked while two frames fill an unread RDR.
  const uint16_t delay[]={0xfe00,0x0a0e,0x46fc,0x0700,0x40fe};for(unsigned i=0;i<std::size(delay);++i)words[0x9d+i]=delay[i];
  const auto error=52+4*ch,receive=error+1;words[2*error]=0;words[2*error+1]=0x180;words[2*receive]=0;words[2*receive+1]=0x1c0;words[0xe0]=0x5670;
  const uint16_t handler[]={0x7a04,0x00ff,uint16_t(base(ch)+4),0x7a05,0,0x400,0x684e,0x68de,0x0b05,0x0b04,0x684b,0x68db,0x1b04,0xee9f,0x68ce,0x5670};for(unsigned i=0;i<std::size(handler);++i)words[0xc0+i]=handler[i];
 }
 uint64_t pin_event()const{return bit<10?160+32*bit:bit<20?600+32*(bit-10):0;}
 uint64_t next_bus_event()const{const auto a=pin_event(),b=sci.next_event();return !a?b:!b?a:std::min(a,b);}
 uint64_t update(uint64_t cycle){unsigned guard=0;while(next_bus_event() && next_bus_event()<=cycle){if(++guard>4096)throw std::runtime_error("Overrun progress");now=next_bus_event();if(pin_event()==now){const auto n=bit%10;const auto value=bit<10?received:second;sci.write_rx(rxch,n==0?false:n==9?true:((value>>(n-1))&1)!=0,now);++bit;}for(unsigned c=0;c<2;++c)if(sci.next_event(c)==now)sci.service_one(c,now,*this);}return next_bus_event();}
 void bus_cycle(uint64_t cycle){update(cycle);now=cycle;}
};
using Cpu=yamanix::H8FirstInstructions<OverrunHost>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void accepted(OverrunHost& h,Cpu& c){check((h.data[0x400]&0x78)==0x60 && h.data[0x401]==h.received,"ORER plus unread RDRF preserves first byte");check(h.exceptions==std::vector<int>{int(52+4*h.rxch),int(53+4*h.rxch)},"ERI precedes previously latched RXI");check(!(h.sci.read8(h.base(h.rxch)+4,false)&0x78) && h.irq.current().level==-1 && c.snapshot().registers[7]==0x300,"CPU clears ORER RDRF and returns through both IRQs");}
int main(){
 for(unsigned ch=0;ch<2;++ch)for(unsigned b=0;b<256;++b){OverrunHost h(ch,uint8_t(b),uint8_t(b^0xff));Cpu c(h,0,&h.irq);c.run(0,1000,30000);check(h.exceptions.empty() && (h.sci.read8(h.base(ch)+4,false)&0x78)==0x60 && h.sci.read8(h.base(ch)+5,false)==b,"Two frames overrun while CPU I mask prevents ISR");c.run(1000,1400,30000);accepted(h,c);}
 for(unsigned ch=0;ch<2;++ch)for(unsigned split=0;split<=2400;++split){OverrunHost a(ch,0x96,0x69),b(ch,0x96,0x69);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,2400,30000);y.run(0,int(split),30000);y.run(split,int(2400-split),30000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data && a.wire==b.wire && a.exceptions==b.exceptions,"Overrun mask/unmask every-split invariance");}
 std::printf("CPU overrun: 512 byte/channel cases,4802 splits: %u failures\n",failures);return failures?1:0;
}
