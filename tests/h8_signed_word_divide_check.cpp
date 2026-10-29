#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
static int64_t sign_value(uint64_t raw,unsigned bits){return raw<(uint64_t(1)<<(bits-1))?int64_t(raw):int64_t(raw)-int64_t(uint64_t(1)<<bits);}
static void one(unsigned src,unsigned dst,uint32_t numerator,unsigned denominator,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)want[i]=uint16_t(0xa500u+i*0x321u);
 want[dst]=uint16_t(numerator);want[dst+8]=uint16_t(numerator>>16);want[src]=uint16_t(denominator);
 auto initial=want;int64_t n=sign_value(uint64_t(want[dst])+65536ull*want[dst+8],32),d=sign_value(want[src],16),q=0;
 if(d){q=n/d;int64_t r=n-q*d;check(n==q*d+r && (r==0||(r<0)==(n<0)),"Independent signed64 division/remainder identity");uint64_t packed=(uint64_t(q)&65535)+65536*(uint64_t(r)&65535);want[dst]=uint16_t(packed);want[dst+8]=uint16_t(packed>>16);}
 // N follows original32-bit narrowed quotient,including mathematical +2147483648.
 bool negative=(uint64_t(q)&0xffffffffu)>=0x80000000ull;
 for(unsigned i=0;i<8;++i)code.insert(code.end(),{uint16_t(0x7a00|i),initial[i+8],initial[i]});
 code.insert(code.end(),{uint16_t(0x0700|flags),0x01d0,uint16_t(0x5300|(src<<4)|dst),0x40fe});h.program(code);Cpu c(h);c.run(0,200,1000);
 check(c.snapshot().registers==want,"Signed64 oracle/low quotient/high remainder/zero retention/full registers/aliases");check(c.snapshot().ccr==uint8_t((flags&~12u)|(d==0?4:negative?8:0)),"Original narrowed32-bit quotient N/zero divisor Z/all other bits");check(h.data.empty(),"No data bus access");
}
int main(){
 for(unsigned d=0;d<65536;++d)for(uint32_t n:{0u,1u,20u,65535u,0x7fffffffu,0x80000000u,0xffffffecu,0xffffffffu})one(1,0,n,d,d&255);
 for(unsigned src=0;src<16;++src)for(unsigned dst=0;dst<8;++dst)for(uint32_t n:{0u,1u,20u,65535u,0x7fffffffu,0x80000000u,0xffffffecu,0xffffffffu})for(unsigned d:{0u,1u,3u,32767u,32768u,65533u,65535u})for(unsigned f:{0u,0x55u,0xffu})one(src,dst,n,d,f);
 uint32_t random=123;for(unsigned i=0;i<20000;++i){random=random*1664525u+1013904223u;uint32_t n=random;random=random*1664525u+1013904223u;one(i%16,(i/16)%8,n,random>>16,random&255);}
 // Explicit witnesses separate N of narrowed quotient from sign of stored16-bit value.
 for(unsigned f=0;f<256;++f){one(1,0,0x80000000u,65535,f);one(1,0,32768,1,f);one(1,0,0xffff0000u,1,f);one(1,0,0x80000000u,0,f);}
 for(unsigned cut=0;cut<=320;++cut){Host a,b;const std::vector<uint16_t> p{0x7a00,0x8000,0,0x7901,0xffff,0x01d0,0x5310,0x7901,0,0x01d0,0x5310,0x7901,1,0x01d0,0x5310,0x01d0,0x5380,0x40fe};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,320,1000);y.run(0,int(cut),1000);y.run(cut,int(320-cut),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"321 split snapshots and ordered instruction/event histories");}
 for(unsigned src=0;src<16;++src)for(unsigned dst=8;dst<16;++dst){Host h;h.program({0x01d0,uint16_t(0x5300|(src<<4)|dst)});Cpu c(h);bool rejected=false;try{c.run(0,30,100);}catch(const std::runtime_error&){rejected=true;}check(rejected&&c.failed(),"128 reserved ER extension neighbors rejected");}
 std::printf("Signed word divide:524288 divisor sweeps,21504 alias/CCR cases,20000 random,1024 extreme/CCR witnesses,321 splits,128 reserved guards:%u failures\n",failures);return failures?1:0;
}
