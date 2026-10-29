#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void one(unsigned src,unsigned dst,uint32_t numerator,unsigned denominator,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)want[i]=uint16_t(0xa500u+i*0x321u);
 want[dst]=uint16_t(numerator);want[dst+8]=uint16_t(numerator>>16);want[src]=uint16_t(denominator);
 const uint64_t n=uint64_t(want[dst])+65536ull*want[dst+8],d=want[src];
 for(unsigned r=0;r<8;++r)code.insert(code.end(),{uint16_t(0x7a00|r),want[r+8],want[r]});
 code.insert(code.end(),{uint16_t(0x0700|flags),uint16_t(0x5300|(src<<4)|dst),0x40fe});h.program(code);
 if(d){uint64_t quotient=n/d,remainder=n-quotient*d;uint64_t packed=quotient%65536+65536*remainder;want[dst]=uint16_t(packed);want[dst+8]=uint16_t(packed/65536);}
 Cpu c(h);c.run(0,200,1000);check(c.snapshot().registers==want,"Unsigned64 oracle/quotient truncation/high remainder/zero retention/aliases");
 check(c.snapshot().ccr==uint8_t((flags&~12u)|(d==0?4:(d&128)?8:0)),"Original N is divisor bit7,not bit15;Z is divisor zero;other bits preserved");check(h.data.empty(),"No data bus access");
}
int main(){
 for(unsigned d=0;d<65536;++d)for(uint32_t n:{0u,1u,65535u,65536u,0x7fffffffu,0x80000000u,0xfffeffffu,0xffffffffu})one(1,0,n,d,d&255);
 for(unsigned src=0;src<16;++src)for(unsigned dst=0;dst<8;++dst)for(uint32_t n:{0u,1u,65535u,65536u,0x7fffffffu,0x80000000u,0xfffeffffu,0xffffffffu})for(unsigned d:{0u,1u,127u,128u,255u,256u,32767u,32768u,32896u,65535u})for(unsigned f:{0u,0x55u,0xffu})one(src,dst,n,d,f);
 uint32_t random=123;for(unsigned i=0;i<20000;++i){random=random*1664525u+1013904223u;uint32_t n=random;random=random*1664525u+1013904223u;one(i%16,(i/16)%8,n,random>>16,random&255);}
 for(unsigned cut=0;cut<=240;++cut){Host a,b;const std::vector<uint16_t> p{0x7a00,0xfffe,0xffff,0x7901,0xffff,0x5310,0x7901,0,0x5310,0x7901,1,0x5310,0x5380,0x40fe};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,1000);y.run(0,int(cut),1000);y.run(cut,int(240-cut),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"241 split states and ordered instruction/event histories");}
 for(unsigned r=8;r<16;++r){Host h;h.program({uint16_t(0x5300|r)});Cpu c(h);bool rejected=false;try{c.run(0,30,100);}catch(const std::runtime_error&){rejected=true;}check(rejected&&c.failed(),"Reserved ER destination neighbors rejected");}
 std::printf("Word divide:524288 divisor sweeps,30720 alias/CCR cases,20000 random,241 splits,reserved ER guards:%u failures\n",failures);return failures?1:0;
}
