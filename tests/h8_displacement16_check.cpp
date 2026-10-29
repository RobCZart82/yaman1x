#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static void one(unsigned bits,bool store,unsigned ptr,unsigned reg,uint32_t base,unsigned disp,unsigned seed){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> p;
 for(unsigned r=0;r<8;++r){uint32_t v=r==ptr?base:((r+1)*0x1234567u)^(seed*0x11111111u);want[r]=uint16_t(v);want[r+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)});}
 int64_t offset=disp<32768?disp:int64_t(disp)-65536;uint32_t full=uint32_t((int64_t(base)+offset+0x100000000LL)%0x100000000LL);uint32_t physical=(bits==8?full:full&~1u)&0xffffff;
 uint32_t value=store?(bits==8?uint8_t(reg&8?want[reg&7]:want[reg&7]/256):bits==16?want[reg]:uint32_t(want[reg])|(uint32_t(want[reg+8])<<16)):uint32_t(disp*0x1020305u)^seed;
 if(bits==8)value&=255;if(bits==16)value&=65535;
 if(!store)for(unsigned b=0;b<bits/8;++b)h.data[(physical+b)&0xffffff]=uint8_t(value>>(bits-8-8*b));
 p.push_back(uint16_t(0x0700|seed));if(bits==32)p.push_back(0x0100);p.push_back(uint16_t((bits==8?0x6e00:0x6f00)|(store?0x80:0)|(ptr<<4)|reg));p.push_back(uint16_t(disp));h.program(p);Cpu c(h);c.run(0,100,1000);
 if(store){for(unsigned b=0;b<bits/8;++b)check(h.data[(physical+b)&0xffffff]==uint8_t(value>>(bits-8-8*b)),"Store value/address/lane order");}
 else if(bits==8){auto& w=want[reg&7];w=uint16_t(reg&8?((w&0xff00)|value):((w&255)|(value<<8)));}else {want[reg]=uint16_t(value);if(bits==32)want[reg+8]=uint16_t(value>>16);}
 check(c.snapshot().registers==want,"Full register image and address/data alias");check(c.snapshot().ccr==uint8_t((seed&~14u)|(value==0?4:((uint64_t(value)>>(bits-1))?8:0))),"NZV and unrelated CCR bits");
 std::vector<uint32_t> got;for(auto [kind,a,v]:h.trace)if(kind==(store?(bits==8?'W':'S'):(bits==8?'R':'Q')))got.push_back(a);std::vector<uint32_t> addresses{bits==8?full:full&~1u};if(bits==32)addresses.push_back((full+2)&~1u);check(got==addresses,"Exact data addresses/count and long high/low order");
}
int main(){
 for(unsigned bits:{8u,16u,32u})for(bool store:{false,true})for(unsigned d=0;d<65536;++d)one(bits,store,0,bits==8?9:1,0xab123457,d,d&255);
 for(unsigned bits:{8u,16u,32u})for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr)for(unsigned r=0;r<(bits==32?8u:16u);++r)for(unsigned d:{0u,1u,0x7fffu,0x8000u,0xffffu})for(uint32_t base:{0u,1u,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})for(unsigned seed:{0u,0x55u,0xffu})one(bits,store,ptr,r,base,d,seed);
 for(unsigned split=0;split<=200;++split){Host a,b;std::vector<uint16_t> p{0x7a00,0xab12,0x3457,0x7a01,0x89ab,0xcdef,0x6e89,0xffff,0x6e0a,0xffff,0x6f81,0x8000,0x6f02,0x8000,0x0100,0x6f81,0x7fff,0x0100,0x6f03,0x7fff,0x40d6};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);check(x.snapshot()==y.snapshot() && a.data==b.data && a.trace==b.trace,"201 split state/memory/ordered bus histories");}
 for(unsigned fault:{1u,2u}){Host h;h.program({0x7a00,0,0,0x7a01,0x89ab,0xcdef,0x0100,0x6f81,0xffff});h.fail_word_write=fault;Cpu c(h);bool threw=false;try{c.run(0,60,1000);}catch(const std::runtime_error&){threw=true;}check(threw&&c.failed()&&h.word_writes==fault,"Long store fault retains completed high/low writes");}
 for(unsigned r=8;r<16;++r){Host h;h.program({0x0100,uint16_t(0x6f00|r),0});Cpu c(h);bool threw=false;try{c.run(0,40,1000);}catch(const std::runtime_error&){threw=true;}check(threw&&c.failed(),"Reserved long ER neighbor rejected");}
 std::printf("Displacement16: all offsets,register/address/CCR boundaries,201 splits,faults: %u failures\n",failures);return failures?1:0;
}
