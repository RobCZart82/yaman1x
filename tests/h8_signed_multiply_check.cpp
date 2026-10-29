#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static int64_t signed_value(unsigned raw,unsigned bits){return raw<(1u<<(bits-1))?int64_t(raw):int64_t(raw)-(int64_t(1)<<bits);}
static void one(unsigned bits,unsigned src,unsigned dst,unsigned a,unsigned b,unsigned flags){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)want[i]=uint16_t(0xa500u+i*0x321u);
 if(bits==8){want[dst]=uint16_t((want[dst]&0xff00)|a);auto& w=want[src&7];w=uint16_t(src&8?((w&0xff00)|b):((w&255)|(b<<8)));}
 else {want[dst]=uint16_t(a);want[src]=uint16_t(b);}
 unsigned lhs=bits==8?(want[dst]&255):want[dst],rhs=bits==8?(src&8?(want[src&7]&255):(want[src&7]>>8)):want[src];
 int64_t product=signed_value(lhs,bits)*signed_value(rhs,bits);uint32_t raw=uint32_t(product);
 for(unsigned r=0;r<8;++r)code.insert(code.end(),{uint16_t(0x7a00|r),want[r+8],want[r]});
 code.insert(code.end(),{uint16_t(0x0700|flags),0x01c0,uint16_t((bits==8?0x5000:0x5200)|(src<<4)|dst),0x40fe});h.program(code);
 want[dst]=uint16_t(raw);if(bits==16)want[dst+8]=uint16_t(raw>>16);Cpu c(h);c.run(0,200,1000);
 check(c.snapshot().registers==want,"Signed64 independent product/full register image/aliases");check(c.snapshot().ccr==uint8_t((flags&~12u)|(product==0?4:product<0?8:0)),"Only N/Z changed;V,C,H,I/UI preserved");check(h.data.empty(),"No data bus access");
}
int main(){
 for(unsigned a=0;a<256;++a)for(unsigned b=0;b<256;++b)one(8,0,1,a,b,(a+b)&255);
 for(unsigned a=0;a<65536;++a)for(unsigned b:{0u,1u,32767u,32768u,65535u})one(16,1,0,a,b,a&255);
 for(unsigned bits:{8u,16u})for(unsigned s=0;s<16;++s)for(unsigned d=0;d<(bits==8?16u:8u);++d)for(unsigned a:{0u,1u,(1u<<(bits-1))-1,1u<<(bits-1),(1u<<bits)-1})for(unsigned b:{0u,1u,(1u<<(bits-1))-1,1u<<(bits-1),(1u<<bits)-1})for(unsigned f:{0u,0x55u,0xffu})one(bits,s,d,a,b,f);
 uint32_t random=123;for(unsigned i=0;i<20000;++i){random=random*1664525u+1013904223u;unsigned a=random>>16;random=random*1664525u+1013904223u;one(16,i%16,(i/16)%8,a,random>>16,random&255);}
 for(unsigned cut=0;cut<=240;++cut){Host a,b;const std::vector<uint16_t> p{0x7a00,0xffff,0x8000,0x7901,0xffff,0x07ff,0x01c0,0x5210,0x01c0,0x5000,0x40fe};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,1000);y.run(0,int(cut),1000);y.run(cut,int(240-cut),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"241 split execution states/ordered instruction/event histories");}
 for(unsigned ext=0;ext<65536;++ext){Host h;h.program({0x01c0,uint16_t(ext)});Cpu c(h);bool rejected=false;try{c.run(0,9,100);}catch(const std::runtime_error&){rejected=true;}bool supported=(ext&0xff00)==0x5000||(ext&0xff08)==0x5200;check(rejected!=supported&&c.failed()==rejected,"All65536 signed prefix extensions guarded");}
 std::printf("Signed multiply:65536 byte pairs,327680 word sweeps,28800 alias/CCR cases,20000 random,241 splits,65536 prefix extensions:%u failures\n",failures);return failures?1:0;
}
