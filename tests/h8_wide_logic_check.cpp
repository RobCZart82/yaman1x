#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static uint32_t value(unsigned op,uint32_t a,uint32_t b){return op==0?a|b:op==1?a^b:a&b;}
static uint8_t flags(unsigned bits,uint32_t v,unsigned seed){return uint8_t((seed&~14)|(v==0?4:((uint64_t(v)>>(bits-1))?8:0)));}
static std::vector<uint16_t> init(std::array<uint16_t,16>& r,unsigned seed){std::vector<uint16_t> p;for(unsigned n=0;n<8;++n){const uint32_t v=(0x1234567u*(n+1))^(seed*0x11111111u);r[n]=uint16_t(v);r[n+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|n),uint16_t(v>>16),uint16_t(v)});}return p;}
int main(){
 for(unsigned op=0;op<3;++op)for(unsigned a=0;a<65536;++a){unsigned b=(a*997+0x5a5a)&65535,seed=(a*37)&255;Host h;h.program({0x7900,uint16_t(a),0x7901,uint16_t(b),uint16_t(0x0700|seed),uint16_t(0x6410|(op<<8))});Cpu c(h);c.run(0,17,1000);auto s=c.snapshot();std::array<uint16_t,16> want{};want[0]=uint16_t(value(op,a,b));want[1]=uint16_t(b);check(s.registers==want && s.ccr==flags(16,want[0],seed),"196608 exhaustive word operands and flags");}
 for(unsigned bits:{16u,32u})for(unsigned op=0;op<3;++op)for(unsigned src=0;src<(bits==16?16u:8u);++src)for(unsigned dst=0;dst<(bits==16?16u:8u);++dst)for(unsigned seed=0;seed<256;++seed){
  std::array<uint16_t,16> want{};auto p=init(want,seed);const uint32_t a=bits==16?want[dst]:uint32_t(want[dst])|(uint32_t(want[dst+8])<<16),b=bits==16?want[src]:uint32_t(want[src])|(uint32_t(want[src+8])<<16),v=value(op,a,b);want[dst]=uint16_t(v);if(bits==32)want[dst+8]=uint16_t(v>>16);p.push_back(uint16_t(0x0700|seed));if(bits==32)p.push_back(0x01f0);p.push_back(uint16_t(0x6400|(op<<8)|(src<<4)|dst));Host h;h.program(p);Cpu c(h);c.run(0,bits==16?57:59,1000);check(c.snapshot().registers==want && c.snapshot().ccr==flags(bits,v,seed),"245760 register/lane/alias/CCR combinations");
 }
 for(unsigned ext=0;ext<65536;++ext){const bool valid=(ext&0xff88)==0x6400 || (ext&0xff88)==0x6500 || (ext&0xff88)==0x6600;Host h;h.program({0x01f0,uint16_t(ext)});Cpu c(h);bool threw=false;try{c.run(0,11,100);}catch(const std::runtime_error&){threw=true;}check(threw!=valid && c.failed()==!valid,"65536 extension encodings and explicit rejection");if(!valid)check(c.snapshot().registers==std::array<uint16_t,16>{},"Invalid extension cannot mutate registers");}
 {Host h;h.program({0x01f0,0x6500});Cpu c(h);c.run(0,7,100);check(c.snapshot().state==0x01f0 && c.snapshot().substate==2,"Prefix fetch suspended");auto t=h.trace;c.run(7,1,100);check(h.trace==t,"Debt repayment does not refetch extension");c.run(8,1,100);check(c.snapshot().state==0x116500 && c.snapshot().substate==2,"Long logic executes after prefix continuation");t=h.trace;c.run(9,1,100);check(h.trace==t,"Completed logic prefetch not repeated");}
 for(unsigned seed=0;seed<1000;++seed){std::array<uint16_t,16> r{};auto p=init(r,seed);p.insert(p.end(),{uint16_t(0x0700|(seed&255)),0x6412,0x6523,0x6634,0x01f0,0x6412,0x01f0,0x6523,0x01f0,0x6634,0x40ea});Host a,b;a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,512,10000);uint64_t start=0;unsigned rand=seed+1;while(start<512){rand=rand*1664525u+1013904223u;int n=int(1+rand%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data,"1000 mixed word/long partition histories");}
 std::printf("H8 wide register logic: 196608 words, 245760 aliases/flags, 65536 extensions, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
