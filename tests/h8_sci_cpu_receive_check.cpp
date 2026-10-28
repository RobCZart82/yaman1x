#include "h8_first_instructions.h"
#include "h83002_sci_mmio.h"
#include "h8_memory_fixture.h"
#include <algorithm>
#include <cstdio>
// Composition fixture only: UART configured before execution. CPU writes only
// SSR receive acknowledgement; no timed TX/register reconfiguration is claimed.
struct SerialHost:Host {
 yamanix::H8hIrqController irq;
 yamanix::H83002SciMmio sci{irq};
 unsigned channel,bit=0;uint8_t payload;uint64_t now=0;
 std::vector<int> exceptions;std::vector<unsigned> requests;
 uint32_t base()const{return channel?0xffffb8:0xffffb0;}
 void tx(unsigned,bool){} void clock(unsigned,bool){}
 bool dma_request(unsigned v){requests.push_back(v);return false;}
 void exception_hook(int v){exceptions.push_back(v);}
 SerialHost(unsigned ch,uint8_t byte):channel(ch),payload(byte){
  sci.write8(base()+1,0,0,*this);sci.write8(base()+2,0x50,0,*this);
  const auto vector=53+4*channel;words[2*vector]=0;words[2*vector+1]=0x180;
  program({0x7a07,0,0x300,0x0700,0,0x40fe});
  const uint16_t handler[]={0x7a00,0x00ff,uint16_t(base()+4),0x7a01,0,0x400,
   0x680a,0x0b00,0x680b,0x689b,0x1b00,0xeabf,0x688a,0x5670};
  for(unsigned i=0;i<std::size(handler);++i)words[0xc0+i]=handler[i];
 }
 uint64_t wire_event()const{return bit<10?200+32*bit:0;}
 uint64_t next()const{const auto a=wire_event(),b=sci.next_event();return !a?b:!b?a:std::min(a,b);}
 uint64_t update(uint64_t cycle){
  unsigned guard=0;
  while(next() && next()<=cycle){if(++guard>4096)throw std::runtime_error("SCI scheduler progress");now=next();
   // Explicit fixture tie rule: external edge before SCI sample, channel0 before1.
   if(wire_event()==now){const bool level=bit==0?false:bit==9?true:((payload>>(bit-1))&1)!=0;sci.write_rx(channel,level,now);++bit;}
   for(unsigned ch=0;ch<2;++ch)if(sci.next_event(ch)==now)sci.service_one(ch,now,*this);
  }
  now=cycle;return next();
 }
 uint8_t read_byte(uint32_t a){if(yamanix::H83002SciMmio::mapped(a)){const auto v=sci.read8(a);trace.emplace_back('R',a,v);return v;}return Host::read_byte(a);}
 void write_byte(uint32_t a,uint8_t v){if(yamanix::H83002SciMmio::mapped(a)){if(a!=base()+4)throw std::runtime_error("Untimed UART reconfiguration unsupported");sci.write8(a,v,now,*this);trace.emplace_back('W',a,v);return;}Host::write_byte(a,v);}
};
using Cpu=yamanix::H8FirstInstructions<SerialHost>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void accepted(SerialHost& h,Cpu& c){check(h.data[0x400]==h.payload && h.exceptions==std::vector<int>{int(53+4*h.channel)} && h.requests==std::vector<unsigned>{53+4*h.channel},"Wire byte -> RXI -> CPU ISR -> RAM");check(!(h.sci.read8(h.base()+4,false)&0x40) && h.irq.current().level==-1 && c.snapshot().registers[7]==0x300,"ISR clears RDRF and RTE restores stack");}
int main(){
 for(unsigned ch=0;ch<2;++ch)for(unsigned b=0;b<256;++b){SerialHost h(ch,uint8_t(b));Cpu c(h,0,&h.irq);c.run(0,800,10000);accepted(h,c);}
 for(unsigned ch=0;ch<2;++ch)for(unsigned split=0;split<=800;++split){SerialHost a(ch,0xa5),b(ch,0xa5);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,800,10000);y.run(0,int(split),10000);y.run(split,int(800-split),10000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data && a.exceptions==b.exceptions && a.requests==b.requests && a.sci.read8(a.base()+4,false)==b.sci.read8(b.base()+4,false),"CPU/SCI shared-deadline split invariance");}
 std::printf("CPU/SCI: 512 byte/channel cases, 1602 splits: %u failures\n",failures);return failures?1:0;
}
