#include "h8_sci_duplex_fixture.h"
using Cpu=yamanix::H8FirstInstructions<DuplexHost>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void accepted(DuplexHost& h,Cpu& c,uint8_t sent){check(h.data[0x400]==h.received && h.exceptions==std::vector<int>{int(53+4*h.rxch)} && c.snapshot().registers[7]==0x300,"Concurrent receive IRQ RAM and RTE");check(!(h.sci.read8(h.base(h.rxch)+4,false)&0x40) && h.irq.current().level==-1,"Receive acknowledged without pending IRQ");check(h.wire.size()==11,"One complete TX frame during RX");if(h.wire.size()==11){check(std::get<1>(h.wire.front())<160 && std::get<1>(h.wire.back())>160,"TX and external RX overlap");for(unsigned b=0;b<11;++b){auto [ch,t,level]=h.wire[b];check(ch==h.txch && t==std::get<1>(h.wire[0])+32*b && level==(b==0?false:b>=9?true:((sent>>(b-1))&1)!=0),"Independent TX levels and timing during RX");}}}
int main(){
 for(unsigned tx=0;tx<2;++tx)for(unsigned rx=0;rx<2;++rx)for(unsigned b=0;b<256;++b){DuplexHost h(tx,rx,uint8_t(b),uint8_t(b^0xa5));Cpu c(h,0,&h.irq);c.run(0,900,20000);accepted(h,c,uint8_t(b));}
 for(unsigned tx=0;tx<2;++tx)for(unsigned rx=0;rx<2;++rx)for(unsigned split=0;split<=900;++split){DuplexHost a(tx,rx,0x96,0x69),b(tx,rx,0x96,0x69);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,900,20000);y.run(0,int(split),20000);y.run(split,int(900-split),20000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.wire==b.wire && a.data==b.data && a.exceptions==b.exceptions,"Duplex every-split invariance");}
 std::printf("CPU duplex: 1024 channel/byte cases, 3604 splits: %u failures\n",failures);return failures?1:0;
}
