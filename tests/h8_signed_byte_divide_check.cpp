#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
static int64_t signed_value(unsigned raw,unsigned bits){return raw<(1u<<(bits-1))?int64_t(raw):int64_t(raw)-(int64_t(1)<<bits);}
static void one(unsigned src,unsigned dst,unsigned numerator,unsigned denominator,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)want[i]=uint16_t(0xa500u+i*0x321u);
 want[dst]=uint16_t(numerator);auto& source=want[src&7];source=uint16_t(src&8?((source&0xff00)|denominator):((source&255)|(denominator<<8)));
 int64_t n=signed_value(want[dst],16),d=signed_value(src&8?(want[src&7]&255):(want[src&7]>>8),8),q=0;
 if(d){q=n/d;int64_t r=n-q*d;check(n==q*d+r && (r==0||(r<0)==(n<0)),"Independent signed division identity/remainder sign");want[dst]=uint16_t((uint64_t(q)&255)|((uint64_t(r)&255)<<8));}
 // Build pre-instruction registers separately,including aliases,from the original setup.
 std::array<uint16_t,16> initial{};for(unsigned i=0;i<16;++i)initial[i]=uint16_t(0xa500u+i*0x321u);initial[dst]=uint16_t(numerator);auto& lane=initial[src&7];lane=uint16_t(src&8?((lane&0xff00)|denominator):((lane&255)|(denominator<<8)));
 for(unsigned i=0;i<8;++i)code.insert(code.end(),{uint16_t(0x7a00|i),initial[i+8],initial[i]});
 code.insert(code.end(),{uint16_t(0x0700|flags),0x01d0,uint16_t(0x5100|(src<<4)|dst),0x40fe});h.program(code);Cpu c(h);c.run(0,200,1000);
 check(c.snapshot().registers==want,"Signed64 truncating quotient/remainder packing,zero retention,whole registers/aliases");check(c.snapshot().ccr==uint8_t((flags&~12u)|(d==0?4:q<0?8:0)),"N from full quotient sign;Z from zero divisor;other CCR preserved");check(h.data.empty(),"No data bus access");
}
int main(){
 for(unsigned n=0;n<65536;++n)for(unsigned d:{0u,1u,127u,128u,253u,255u})one(0,1,n,d,n&255);
 for(unsigned d=0;d<256;++d)for(unsigned n:{0u,1u,20u,255u,32767u,32768u,65516u,65535u})one(0,1,n,d,d);
 for(unsigned s=0;s<16;++s)for(unsigned t=0;t<16;++t)for(unsigned n:{0u,1u,20u,255u,32767u,32768u,65516u,65535u})for(unsigned d:{0u,1u,127u,128u,253u,255u})for(unsigned f:{0u,0x55u,0xffu})one(s,t,n,d,f);
 uint32_t random=123;for(unsigned i=0;i<20000;++i){random=random*1664525u+1013904223u;unsigned n=random>>16;random=random*1664525u+1013904223u;one(i%16,(i/16)%16,n,random>>24,random&255);}
 for(unsigned cut=0;cut<=240;++cut){Host a,b;const std::vector<uint16_t> p{0x7900,0x8000,0xf1ff,0x01d0,0x5110,0xf100,0x01d0,0x5110,0xf101,0x01d0,0x5110,0x01d0,0x5100,0x40fe};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,1000);y.run(0,int(cut),1000);y.run(cut,int(240-cut),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"241 split snapshots/instruction-event histories");}
 for(unsigned ext=0;ext<65536;++ext){Host h;h.program({0x01d0,uint16_t(ext)});Cpu c(h);bool rejected=false;try{c.run(0,9,100);}catch(const std::runtime_error&){rejected=true;}check(rejected!=((ext&0xff00)==0x5100)&&c.failed()==rejected,"65536 guarded prefix continuations");}
 std::printf("Signed byte divide:393216 numerator sweeps,2048 divisor boundaries,36864 alias/CCR cases,20000 random,241 splits,65536 prefix guards:%u failures\n",failures);return failures?1:0;
}
