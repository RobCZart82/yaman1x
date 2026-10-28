#include "h8_sci_duplex_fixture.h"
#include <bit>
struct ParityHost:DuplexHost {
 uint8_t format;
 ParityHost(unsigned channel,uint8_t mode,uint8_t value,bool wrong):DuplexHost(channel,channel,0x96,value,true,mode),format(mode){
  // Independent bit-count wire oracle for pinned source's SMR parity polarity.
  parity_bit=bool((std::popcount(unsigned(value))&1)^((mode&0x10)?0:1)^unsigned(wrong));
  if(wrong){const auto v=52+4*channel;words[2*v]=0;words[2*v+1]=0x180;
   const uint16_t handler[]={0x7a04,0x00ff,uint16_t(base(channel)+4),0x7a05,0,0x400,0x684e,0x68de,0xeec7,0x68ce,0x5670};for(unsigned i=0;i<std::size(handler);++i)words[0xc0+i]=handler[i];}
 }
};
using Cpu=yamanix::H8FirstInstructions<ParityHost>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void accepted(ParityHost& h,Cpu& c,bool wrong){check(h.exceptions==std::vector<int>{int((wrong?52:53)+4*h.rxch)} && c.snapshot().registers[7]==0x300,"PER selects ERI, valid parity selects RXI and RTE");check(wrong?(h.data[0x400]&0x78)==0x08:h.data[0x400]==h.received,"CPU captures PER only or valid data");check(!(h.sci.read8(h.base(h.rxch)+4,false)&0x78) && h.irq.current().level==-1,"Receive status acknowledged");check(h.wire.size()==12,"Parity-enabled TX completes");if(h.wire.size()==12)for(unsigned b=0;b<12;++b){auto [ch,t,level]=h.wire[b];const bool expected=b==0?false:b>=10?true:b==9?!(h.format&0x10):((0x96u>>(b-1))&1)!=0;check(ch==h.txch && t==std::get<1>(h.wire[0])+32*b && level==expected,"Independent transmitted parity and bit timing");}}
int main(){
 for(unsigned ch=0;ch<2;++ch)for(uint8_t mode:{uint8_t(0x20),uint8_t(0x30)})for(bool wrong:{false,true})for(unsigned v=0;v<256;++v){ParityHost h(ch,mode,uint8_t(v),wrong);Cpu c(h,0,&h.irq);c.run(0,1100,20000);accepted(h,c,wrong);}
 for(unsigned ch=0;ch<2;++ch)for(uint8_t mode:{uint8_t(0x20),uint8_t(0x30)})for(bool wrong:{false,true})for(unsigned split=0;split<=1100;++split){ParityHost a(ch,mode,0x69,wrong),b(ch,mode,0x69,wrong);Cpu x(a,0,&a.irq),y(b,0,&b.irq);x.run(0,1100,20000);y.run(0,int(split),20000);y.run(split,int(1100-split),20000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.wire==b.wire && a.data==b.data && a.exceptions==b.exceptions,"Parity every-split invariance");}
 std::printf("CPU parity: 2048 byte/mode cases, 8808 splits: %u failures\n",failures);return failures?1:0;
}
