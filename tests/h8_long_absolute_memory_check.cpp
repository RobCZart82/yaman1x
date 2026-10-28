#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static void one(bool wide,bool store,unsigned reg,uint32_t addr,uint32_t value,unsigned seed){
 Host h;uint32_t full=wide?addr:addr<0x8000?addr:0xffff0000u|addr;const uint32_t physical=(full&~1u)&0xffffff;
 std::vector<uint16_t> p;for(unsigned r=0;r<8;++r)p.insert(p.end(),{uint16_t(0x7a00|r),0xa55a,0x5aa5});
 if(store)p.insert(p.end(),{uint16_t(0x7a00|reg),uint16_t(value>>16),uint16_t(value)});
 p.insert(p.end(),{uint16_t(0x0700|seed),0x0100,uint16_t(0x6b00|(store?0x80:0)|(wide?0x20:0)|reg)});if(wide)p.push_back(uint16_t(addr>>16));p.push_back(uint16_t(addr));h.program(p);
 if(!store)for(unsigned b=0;b<4;++b)h.data[(physical+b)&0xffffff]=uint8_t(value>>(24-8*b));
 Cpu c(h);c.run(0,100,1000);auto s=c.snapshot();
 if(store){for(unsigned b=0;b<4;++b)check(h.data[(physical+b)&0xffffff]==uint8_t(value>>(24-8*b)),"Long store big endian and24-bit wrap");}
 else{check((uint32_t(s.registers[reg+8])<<16|s.registers[reg])==value,"Long load complete ER value");for(unsigned r=0;r<8;++r)if(r!=reg)check(s.registers[r]==0x5aa5 && s.registers[r+8]==0xa55a,"Unrelated ER preserved");}
 check(s.ccr==uint8_t((seed&~14u)|(value==0?4:(value>>31)?8:0)),"Long move NZV and unrelated CCR");
 std::vector<uint32_t> addresses;for(auto [kind,a,v]:h.trace)if(kind==(store?'S':'Q'))addresses.push_back(a);check(addresses==std::vector<uint32_t>{full&~1u,(full+2)&~1u},"Exactly two high-word then low-word accesses with uint32 wrap");
}
int main(){
 for(bool store:{false,true})for(unsigned a=0;a<65536;++a)one(false,store,a&7,a,uint32_t(a*0x1020305u),a&255);
 const uint32_t bounds[]={0,1,0x7fff,0x8000,0xffff,0x10000,0xffffff,0x1000000,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
 for(bool store:{false,true})for(unsigned r=0;r<8;++r)for(auto a:bounds)for(auto v:{0u,1u,0x7fffffffu,0x80000000u,0xffffffffu})for(auto seed:{0u,0x55u,0xffu})one(true,store,r,a,v,seed);
 for(unsigned split=0;split<=200;++split){Host a,b;std::vector<uint16_t> p{0x7a03,0x89ab,0xcdef,0x0100,0x6b83,0xfffd,0x0100,0x6b04,0xfffd,0x0100,0x6ba3,0xab12,0x3457,0x0100,0x6b24,0xab12,0x3457,0x40dc};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);check(x.snapshot()==y.snapshot() && a.data==b.data && a.trace==b.trace,"Four-word prefixed partial-state every-split invariance");}
 for(bool wide:{false,true})for(unsigned fault:{1u,2u}){Host h;std::vector<uint16_t> p{0x7a03,0x89ab,0xcdef,0x0100,uint16_t(wide?0x6ba3:0x6b83)};if(wide)p.push_back(0xffff);p.push_back(0xffff);h.program(p);h.fail_word_write=fault;Cpu c(h);bool threw=false;try{c.run(0,60,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed() && h.word_writes==fault,"First/second long store fault preserves committed count");}
 for(bool wide:{false,true})for(unsigned reg=8;reg<16;++reg){Host h;h.program({0x0100,uint16_t((wide?0x6b20:0x6b00)|reg),0,0});Cpu c(h);bool threw=false;try{c.run(0,40,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed(),"Invalid ER neighbor rejected after prefix");}
 std::printf("Long absolute: all65536 abs16 addresses,boundary ER/CCR,201 splits and faults: %u failures\n",failures);return failures?1:0;
}
