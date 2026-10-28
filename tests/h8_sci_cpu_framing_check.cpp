#include "h8_sci_duplex_fixture.h"
struct ErrorHost:DuplexHost {
 ErrorHost(unsigned tx,unsigned rx,uint8_t sent,uint8_t received):DuplexHost(tx,rx,sent,received,false){
  const auto vector=52+4*rx;words[2*vector]=0;words[2*vector+1]=0x180;
  // Preserve SSR in RAM, clear only receive-error flags, then RTE.
  const uint16_t handler[]={0x7a04,0x00ff,uint16_t(base(rx)+4),0x7a05,0,0x400,0x684e,0x68de,0xeec7,0x68ce,0x5670};
  for(unsigned i=0;i<std::size(handler);++i)words[0xc0+i]=handler[i];
 }
};
using Cpu=yamanix::H8FirstInstructions<ErrorHost>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void accepted(ErrorHost& h,Cpu& c){check((h.data[0x400]&0x78)==0x10,"CPU records FER without RDRF PER ORER");check(h.exceptions==std::vector<int>{int(52+4*h.rxch)} && c.snapshot().registers[7]==0x300,"ERI vector and RTE restore");check(!(h.sci.read8(h.base(h.rxch)+4,false)&0x78) && h.irq.current().level==-1,"CPU clears FER and acknowledges error request");check(h.wire.size()==11,"TX frame completes despite RX framing error");}
int main(){
 for(unsigned tx=0;tx<2;++tx)for(unsigned rx=0;rx<2;++rx)for(unsigned b=0;b<256;++b){ErrorHost h(tx,rx,uint8_t(b),uint8_t(b^0xa5));Cpu c(h,0,&h.irq);c.run(0,900,20000);accepted(h,c);}
 for(unsigned tx=0;tx<2;++tx)for(unsigned rx=0;rx<2;++rx)for(unsigned split=0;split<=900;++split){ErrorHost a(tx,rx,0x96,0x69),b(tx,rx,0x96,0x69);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,900,20000);y.run(0,int(split),20000);y.run(split,int(900-split),20000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.wire==b.wire && a.data==b.data && a.exceptions==b.exceptions,"Framing error every-split invariance");}
 // After FER acknowledgement, a fresh good frame must take RXI and reach RAM.
 for(unsigned rx=0;rx<2;++rx)for(unsigned b=0;b<256;++b){ErrorHost h(rx,rx,0x96,0x69);Cpu c(h,0,&h.irq);c.run(0,900,20000);accepted(h,c);h.sci.write_rx(rx,true,900);h.stop=true;h.bit=0;h.pin_start=1000;h.received=uint8_t(b);
  const uint16_t handler[]={0x7a04,0x00ff,uint16_t(h.base(rx)+4),0x7a05,0,0x401,0x684e,0x0b04,0x684b,0x68db,0x1b04,0xeebf,0x68ce,0x5670};for(unsigned i=0;i<std::size(handler);++i)h.words[0xc0+i]=handler[i];
  c.run(900,900,20000);check(h.data[0x401]==b && h.exceptions==std::vector<int>{int(52+4*rx),int(53+4*rx)} && !(h.sci.read8(h.base(rx)+4,false)&0x78) && h.irq.current().level==-1 && c.snapshot().registers[7]==0x300,"Good frame recovers after CPU FER clear");}
 std::printf("CPU FER: 1024 channel/byte cases, 3604 splits, 512 recovery cases: %u failures\n",failures);return failures?1:0;
}
