#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <limits>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static uint8_t flags(uint32_t value,unsigned bits,unsigned amount,bool sub,uint8_t old){
 const uint64_t modulus=uint64_t{1}<<bits;const uint32_t result=uint32_t((sub?uint64_t(value)-amount:uint64_t(value)+amount)&(modulus-1));
 const int64_t signed_value=(uint64_t(value)&(modulus/2))?int64_t(value)-int64_t(modulus):int64_t(value);
 const int64_t mathematical=sub?signed_value-amount:signed_value+amount;
 return uint8_t((old&~14u)|(result==0?4u:((uint64_t(result)&(modulus/2))?8u:0u))|((mathematical< -int64_t(modulus/2) || mathematical>=int64_t(modulus/2))?2u:0u));
}
int main(){
 for(unsigned bits:{8u,16u,32u})for(bool sub:{false,true})for(unsigned amount:{1u,2u}){
  if(bits==8 && amount==2)continue;
  const unsigned count=bits==32?4096:bits==16?65536:256;
  for(unsigned n=0;n<count;++n){uint32_t value=bits==32?(n<4?std::array<uint32_t,4>{0,0x7fffffff,0x80000000,0xffffffff}[n]:n*0x1020305u):n;const uint8_t seed=uint8_t(n*37);
   std::vector<uint16_t> p;if(bits==8)p={uint16_t(0xf800|value)};else if(bits==16)p={0x7900,uint16_t(value)};else p={0x7a00,uint16_t(value>>16),uint16_t(value)};
   p.push_back(uint16_t(0x0700|seed));p.push_back(uint16_t(bits==8?(sub?0x1a08:0x0a08):(sub?0x1b00:0x0b00)+(bits==16?0x50:0x70)+(amount==2?0x80:0)));
   Host h;h.program(p);Cpu c(h);c.run(0,bits==8?11:bits==16?13:15,1000);auto s=c.snapshot();const uint64_t mask=(uint64_t{1}<<bits)-1;const uint32_t result=uint32_t((sub?uint64_t(value)-amount:uint64_t(value)+amount)&mask);
   check((bits==32?(uint32_t(s.registers[0])|(uint32_t(s.registers[8])<<16)):s.registers[0])==result && s.ccr==flags(value,bits,amount,sub,seed),"INC/DEC independent signed overflow and preserved H/C");
  }
 }
 for(bool sub:{false,true})for(unsigned amount:{1u,2u,4u})for(unsigned reg=0;reg<8;++reg)for(unsigned seed=0;seed<256;++seed){uint32_t v=seed*0x1020305u;Host h;h.program({uint16_t(0x7a00|reg),uint16_t(v>>16),uint16_t(v),uint16_t(0x0700|seed),uint16_t((sub?0x1b00:0x0b00)+(amount==1?0:amount==2?0x80:0x90)+reg)});Cpu c(h);c.run(0,15,1000);auto s=c.snapshot();check((uint32_t(s.registers[reg])|(uint32_t(s.registers[reg+8])<<16))==(sub?v-amount:v+amount) && s.ccr==seed,"ADDS/SUBS wraps and preserves all flags");}
 for(unsigned seed=0;seed<1000;++seed){Host a,b;std::vector<uint16_t> p{0x7a00,0x7fff,0xffff,0x0b70,0x1bf0,0x0b90,0x1b80,0x0b50,0x1bd0,0x0a08,0x1a08,0x40e8};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,400,10000);uint64_t start=0;unsigned random=seed+1;while(start<400){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>400)n=int(400-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace,"Small arithmetic partition equivalence");}
 std::printf("INC/DEC/ADDS/SUBS independent flag/value matrices and 1000 partitions: %u failures\n",failures);return failures?1:0;
}
