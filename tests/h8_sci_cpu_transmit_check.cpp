#include "h8_first_instructions.h"
#include "h83002_sci_mmio.h"
#include "h8_memory_fixture.h"
#include <cstdio>
struct TxHost:Host {
 yamanix::H8hIrqController irq;yamanix::H83002SciMmio sci{irq};
 unsigned channel;uint64_t now=0,launch=0;std::vector<std::tuple<unsigned,uint64_t,bool>> wire;
 uint32_t base()const{return channel?0xffffb8:0xffffb0;}
 void tx(unsigned ch,bool level){wire.emplace_back(ch,now,level);}
 void clock(unsigned,bool){} bool dma_request(unsigned){return false;}
 uint64_t next_bus_event()const{return sci.next_event();}
 uint64_t update(uint64_t cycle){unsigned guard=0;while(sci.next_event() && sci.next_event()<=cycle){if(++guard>4096)throw std::runtime_error("UART progress");now=sci.next_event();for(unsigned c=0;c<2;++c)if(sci.next_event(c)==now)sci.service_one(c,now,*this);}return sci.next_event();}
 void bus_cycle(uint64_t cycle){update(cycle);now=cycle;}
 TxHost(unsigned ch,uint8_t byte):channel(ch){
 // CPU configures BRR=0, SCR=TE, writes TDR, reads SSR and clears TDRE.
 program({0x7a00,0x00ff,uint16_t(base()+1),0xfa00,0x688a,0x0b00,
 0xfa20,0x688a,0x0b00,uint16_t(0xfa00|byte),0x688a,0x0b00,
 0x680b,0xeb7f,0x688b,0x40fe});
 }
 uint8_t read_byte(uint32_t a){if(yamanix::H83002SciMmio::mapped(a)){auto v=sci.read8(a);trace.emplace_back('R',a,v);return v;}return Host::read_byte(a);}
 void write_byte(uint32_t a,uint8_t v){if(yamanix::H83002SciMmio::mapped(a)){sci.write8(a,v,now,*this);trace.emplace_back('W',a,v);if(a==base()+4)launch=now;return;}Host::write_byte(a,v);}
};
using Cpu=yamanix::H8FirstInstructions<TxHost>;
static unsigned failures=0;static void check(bool ok,const char* w){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",w);}
int main(){
 for(unsigned ch=0;ch<2;++ch)for(unsigned byte=0;byte<256;++byte){TxHost h(ch,uint8_t(byte));Cpu c(h);c.run(0,800,10000);check(h.wire.size()==11,"Start/eight data/stop and final idle sequence");if(h.wire.size()==11)for(unsigned bit=0;bit<11;++bit){auto [channel,time,level]=h.wire[bit];check(channel==ch && level==(bit==0?false:bit>=9?true:((byte>>(bit-1))&1)!=0),"Independent UART wire levels");if(bit)check(time==std::get<1>(h.wire[0])+32*bit,"Independent 32-cycle bit spacing");}check(h.launch && (h.sci.read8(h.base()+4,false)&0x84)==0x84,"CPU launch and completed TDRE/TEND");}
 for(unsigned ch=0;ch<2;++ch)for(unsigned split=0;split<=800;++split){TxHost a(ch,0xa5),b(ch,0xa5);Cpu x(a),y(b);x.run(0,800,10000);y.run(0,int(split),10000);y.run(split,int(800-split),10000);check(a.wire==b.wire && a.trace==b.trace && a.launch==b.launch && x.snapshot()==y.snapshot(),"MMIO-created deadlines invariant over every split");}
 std::printf("CPU SCI TX: 512 byte/channel frames, 1602 splits: %u failures\n",failures);return failures?1:0;
}
