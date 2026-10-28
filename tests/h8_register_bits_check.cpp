#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <algorithm>
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
#include "h8_bit_oracle.h"
static unsigned get(const std::array<uint16_t,16>& r,unsigned n){return n&8?r[n&7]&255:r[n&7]/256;}
static void put(std::array<uint16_t,16>& r,unsigned n,unsigned v){auto& x=r[n&7];x=uint16_t(n&8?((x&0xff00)|v):((x&255)|(v<<8)));}
int main(){
 for(unsigned op=0;op<14;++op)for(unsigned v=0;v<256;++v)for(unsigned bit=0;bit<8;++bit)for(unsigned seed=0;seed<256;++seed){Host h;h.program({uint16_t(0xf800|v),uint16_t(0x0700|seed),uint16_t(codes[op]|(bit<<4)|8)});Cpu c(h);c.run(0,11,1000);auto e=oracle(op,v,bit,seed);std::array<uint16_t,16> want{};want[0]=e.value;check(c.snapshot().registers==want && c.snapshot().ccr==e.ccr,"7340032 immediate bit/operand/CCR cases");}
 for(unsigned op=0;op<4;++op)for(unsigned source=0;source<256;++source)for(unsigned v=0;v<256;++v)for(unsigned carry=0;carry<2;++carry){unsigned seed=((source*17+v*37)&254)|carry;Host h;h.program({uint16_t(0xf900|source),uint16_t(0xf800|v),uint16_t(0x0700|seed),uint16_t(0x6098|(op<<8))});Cpu c(h);c.run(0,13,1000);auto e=oracle(op,v,source%8,seed);std::array<uint16_t,16> want{};want[0]=e.value;want[1]=uint16_t(source);check(c.snapshot().registers==want && c.snapshot().ccr==e.ccr,"524288 register index high-bit cases");}
 for(unsigned op=0;op<4;++op)for(unsigned src=0;src<16;++src)for(unsigned dst=0;dst<16;++dst)for(unsigned seed=0;seed<256;++seed){std::array<uint16_t,16> want{};std::vector<uint16_t> p;for(unsigned i=0;i<8;++i){uint32_t v=(0x1234567u*(i+1))^(seed*0x11111111u);want[i]=uint16_t(v);want[i+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|i),uint16_t(v>>16),uint16_t(v)});}auto e=oracle(op,get(want,dst),get(want,src)%8,seed);put(want,dst,e.value);p.push_back(uint16_t(0x0700|seed));p.push_back(uint16_t(0x6000|(op<<8)|(src<<4)|dst));Host h;h.program(p);Cpu c(h);c.run(0,57,1000);check(c.snapshot().registers==want && c.snapshot().ccr==e.ccr,"262144 source/destination alias and CCR cases");}
 {Host h;h.program({0xf801,0x7108});Cpu c(h);c.run(0,9,100);check(c.snapshot().registers[0]==0,"Bit toggle committed before suspended prefetch");auto trace=h.trace;c.run(9,1,100);check(h.trace==trace,"Debt repayment has no fetch");c.run(10,1,100);check(c.snapshot().registers[0]==0 && std::count_if(h.trace.begin(),h.trace.end(),[](auto e){return std::get<1>(e)==0x104;})==1,"Bit toggle and completed fetch never repeat");}
 for(unsigned seed=0;seed<1000;++seed){std::vector<uint16_t> p{uint16_t(0xf800|(seed&255)),uint16_t(0xf900|((seed*73)&255)),uint16_t(0x0700|(seed&255)),0x6098,0x6198,0x6298,0x6398};for(auto code:codes)p.push_back(uint16_t(code|0x28));p.push_back(0x40da);Host a,b;a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,512,10000);unsigned r=seed+1;uint64_t start=0;while(start<512){r=r*1664525u+1013904223u;int n=int(1+r%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data,"1000 mixed bit partition histories");}
 std::printf("H8 register bits: 7340032 immediate, 524288 register, 262144 aliases, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
