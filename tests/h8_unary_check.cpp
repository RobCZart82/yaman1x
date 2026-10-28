#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 for(unsigned bits:{8u,16u,32u})for(unsigned op=0;op<(bits==8?2u:4u);++op){
  const unsigned count=bits==32?65536:bits==16?65536:256;
  for(unsigned n=0;n<count;++n){uint32_t value=bits==32?n<4?std::array<uint32_t,4>{0,0x7fffffff,0x80000000,0xffffffff}[n]:(n*0x1020305u):n;unsigned seed=(n*37)&255;uint64_t mask=(uint64_t{1}<<bits)-1,sign=uint64_t{1}<<(bits-1);uint32_t result=0;
   if(op==0)result=uint32_t((~uint64_t(value))&mask);else if(op==1)result=uint32_t((0-uint64_t(value))&mask);else if(op==2)result=value&((uint32_t{1}<<(bits/2))-1);else{unsigned half=bits/2;uint32_t low=value&((uint32_t{1}<<half)-1);result=uint32_t((low&(uint32_t{1}<<(half-1)))?uint64_t(low)| (mask^((uint64_t{1}<<half)-1)):low);}
   unsigned expected=(seed&~14u)|(result==0?4u:((uint64_t(result)&sign)?8u:0u));if(op==1){const uint64_t halfmask=(uint64_t{1}<<(bits-4))-1;expected&=~(1u|32u);if(value)expected|=1;if(value&halfmask)expected|=32;if(value==sign)expected|=2;}
   std::vector<uint16_t> p;if(bits==8)p={uint16_t(0xf800|value)};else if(bits==16)p={0x7900,uint16_t(value)};else p={0x7a00,uint16_t(value>>16),uint16_t(value)};
   const unsigned opcode=op==0?(bits==8?0x1708:bits==16?0x1710:0x1730):op==1?(bits==8?0x1788:bits==16?0x1790:0x17b0):op==2?(bits==16?0x1750:0x1770):(bits==16?0x17d0:0x17f0);
   p.insert(p.end(),{uint16_t(0x0700|seed),uint16_t(opcode)});Host h;h.program(p);Cpu c(h);c.run(0,bits==8?11:bits==16?13:15,1000);auto s=c.snapshot();check((bits==32?(uint32_t(s.registers[0])|(uint32_t(s.registers[8])<<16)):s.registers[0])==result && s.ccr==expected,"Unary value and independent flags/sign-extension");
  }
 }
 for(unsigned seed=0;seed<1000;++seed){Host a,b;std::vector<uint16_t> p{0x7a00,uint16_t(seed*73),uint16_t(seed*997),0x1708,0x1788,0x1710,0x1790,0x1730,0x17b0,0x1750,0x17d0,0x1770,0x17f0,0x40e4};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,400,10000);uint64_t start=0;unsigned random=seed+1;while(start<400){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>400)n=int(400-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace,"Unary partition equivalence");}
 std::printf("NOT/NEG/EXTU/EXTS exhaustive narrow inputs, long boundaries and 1000 partitions: %u failures\n",failures);return failures?1:0;
}
