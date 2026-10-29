#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <cstring>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<8)std::fprintf(stderr,"%s\n",why);}
static void one(unsigned src,unsigned dst,unsigned a,unsigned b,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)want[i]=uint16_t(0xa500u+i*0x321u);
 want[dst]=uint16_t(a);want[src]=uint16_t(b);
 const uint64_t product=uint64_t(want[dst])*uint64_t(want[src]);
 for(unsigned r=0;r<8;++r)code.insert(code.end(),{uint16_t(0x7a00|r),want[r+8],want[r]});
 code.insert(code.end(),{uint16_t(0x0700|flags),uint16_t(0x5200|(src<<4)|dst),0x40fe});h.program(code);
 want[dst]=uint16_t(product);want[dst+8]=uint16_t(product>>16);Cpu c(h);c.run(0,200,1000);
 check(c.snapshot().registers==want,"Unsigned64 oracle/full register image/source aliases");check(c.snapshot().ccr==flags,"All CCR bits unchanged");check(h.data.empty(),"No data bus access");
}
int main(int argc,char** argv){
 if(argc==2 && std::strcmp(argv[1],"--promotion-control")==0){
  // Deliberately reproduce the original u16 * u16 promotion under UBSan only.
  volatile uint16_t left=65535,right=65535;volatile int result=left*right;
  std::printf("Original promoted product:%d\n",result);return 0;
 }
 for(unsigned a=0;a<65536;++a)for(unsigned b:{0u,1u,32767u,32768u,65535u})one(1,0,a,b,a&255);
 for(unsigned s=0;s<16;++s)for(unsigned d=0;d<8;++d)for(unsigned a:{0u,1u,32767u,32768u,65535u})for(unsigned b:{0u,1u,32767u,32768u,65535u})for(unsigned f:{0u,0x55u,0xffu})one(s,d,a,b,f);
 uint32_t random=123;for(unsigned i=0;i<20000;++i){random=random*1664525u+1013904223u;unsigned a=random>>16;random=random*1664525u+1013904223u;one(i%16,(i/16)%8,a,random>>16,random&255);}
 for(unsigned cut=0;cut<=240;++cut){Host a,b;const std::vector<uint16_t> p{0x7a00,0xffff,0xffff,0x7901,0xffff,0x07ff,0x5210,0x5280,0x40fe};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,1000);y.run(0,int(cut),1000);y.run(cut,int(240-cut),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"241 split execution snapshots and ordered instruction/event history");}
 std::printf("Word multiply:327680 operand sweeps,9600 alias/CCR boundaries,20000 random cases,241 splits:%u failures\n",failures);return failures?1:0;
}
