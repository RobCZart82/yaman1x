#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<8)std::fprintf(stderr,"%s\n",why);}
static void one(unsigned src,unsigned dst,unsigned a,unsigned b,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)want[i]=uint16_t(0xa500u+i*0x321u);
 want[dst]=uint16_t((want[dst]&0xff00)|b);
 auto& source=want[src&7];source=uint16_t(src&8?((source&0xff00)|a):((source&255)|(a<<8)));
 // Aliases use the final pre-instruction image, with destination low byte as operand.
 unsigned lhs=want[dst]&255,rhs=src&8?(want[src&7]&255):(want[src&7]>>8);
 for(unsigned r=0;r<8;++r)code.insert(code.end(),{uint16_t(0x7a00|r),want[r+8],want[r]});
 code.insert(code.end(),{uint16_t(0x0700|flags),uint16_t(0x5000|(src<<4)|dst),0x40fe});h.program(code);
 want[dst]=uint16_t(lhs*rhs);Cpu c(h);c.run(0,160,1000);
 check(c.snapshot().registers==want,"Unsigned product/full register image/aliases");check(c.snapshot().ccr==flags,"All CCR bits unchanged");check(h.data.empty(),"No data bus access");
}
int main(){
 for(unsigned a=0;a<256;++a)for(unsigned b=0;b<256;++b)one(0,1,a,b,(a+b)&255);
 for(unsigned s=0;s<16;++s)for(unsigned d=0;d<16;++d)for(unsigned a:{0u,1u,127u,128u,255u})for(unsigned b:{0u,1u,127u,128u,255u})for(unsigned f:{0u,0x55u,0xffu})one(s,d,a,b,f);
 for(unsigned cut=0;cut<=160;++cut){Host a,b;a.program({0x7900,0x80ff,0x070f,0x5000,0x5088,0x40fe});b.program({0x7900,0x80ff,0x070f,0x5000,0x5088,0x40fe});Cpu x(a),y(b);x.run(0,160,1000);y.run(0,int(cut),1000);y.run(cut,int(160-cut),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"Split execution state and ordered instruction/event history");}
 std::printf("Byte multiply:65536 operand pairs,19200 alias/CCR boundaries,161 splits:%u failures\n",failures);return failures?1:0;
}
