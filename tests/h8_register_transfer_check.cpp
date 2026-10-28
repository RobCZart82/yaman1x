#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static std::vector<uint16_t> init(std::array<uint16_t,16>& r){std::vector<uint16_t> p;for(unsigned n=0;n<8;++n){uint32_t v=0x12345678u+n*0x1020304u;r[n]=uint16_t(v);r[n+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|n),uint16_t(v>>16),uint16_t(v)});}return p;}
static void byte(std::array<uint16_t,16>& r,unsigned lane,unsigned v){if(lane&8)r[lane&7]=uint16_t((r[lane&7]&0xff00)|v);else r[lane&7]=uint16_t((r[lane&7]&255)|(v<<8));}
static unsigned flags(uint32_t v,unsigned bits,unsigned seed){return (seed&~14u)|(v==0?4u:((uint64_t(v)>>(bits-1))?8u:0u));}
int main(){
 for(unsigned bits:{8u,16u,32u})for(unsigned src=0;src<(bits==32?8u:16u);++src)for(unsigned dst=0;dst<(bits==32?8u:16u);++dst)for(unsigned n=0;n<256;++n){
  const unsigned seed=(n*37)&255;const uint32_t v=bits==8?n:bits==16?uint16_t(n*257):n==0?0:n==1?0x80000000u:n==2?0xffffffffu:n*0x01020305u;
  std::array<uint16_t,16> want{};auto p=init(want);
  if(bits==8){p.push_back(uint16_t(0xf000|(src<<8)|v));byte(want,src,v);}
  else if(bits==16){p.insert(p.end(),{uint16_t(0x7900|src),uint16_t(v)});want[src]=uint16_t(v);}
  else{p.insert(p.end(),{uint16_t(0x7a00|src),uint16_t(v>>16),uint16_t(v)});want[src]=uint16_t(v);want[src+8]=uint16_t(v>>16);}
  p.push_back(uint16_t(0x0700|seed));p.push_back(uint16_t((bits==8?0x0c00:bits==16?0x0d00:0x0f80)|(src<<4)|dst));
  if(bits==8)byte(want,dst,v);else{want[dst]=uint16_t(v);if(bits==32)want[dst+8]=uint16_t(v>>16);}
  Host h;h.program(p);Cpu c(h);c.run(0,bits==8?59:bits==16?61:63,1000);
  check(c.snapshot().registers==want && c.snapshot().ccr==flags(v,bits,seed),"Register transfer lane/width/alias/flag matrix");
 }
 for(unsigned seed=0;seed<1000;++seed){Host a,b;std::vector<uint16_t> p{0x7a00,uint16_t(seed),uint16_t(seed*997),0x0c89,0x0d12,0x0f92,0x0c98,0x0d21,0x0fa0,0x40e8};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,400,10000);uint64_t start=0;unsigned random=seed+1;while(start<400){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>400)n=int(400-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace,"Register transfer partition equivalence");}
 std::printf("Register transfers: 147456 lane/width/alias cases and 1000 partitions: %u failures\n",failures);return failures?1:0;
}
