#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static uint8_t flags(unsigned seed,unsigned value,unsigned bits){return uint8_t((seed&~14u)|(value==0?4:((value>>(bits-1))&1)?8:0));}
static void one(bool word,bool store,unsigned reg,unsigned addr,unsigned value,unsigned seed){
 Host h;const uint32_t full=addr;const uint32_t physical=full&0xffffff,aligned=word?physical&~1u:physical;
 std::vector<uint16_t> code;for(unsigned r=0;r<8;++r)code.insert(code.end(),{uint16_t(0x7a00|r),0xa55a,0x5aa5});
 if(store){if(word)code.insert(code.end(),{uint16_t(0x7900|reg),uint16_t(value)});else code.push_back(uint16_t(0xf000|(reg<<8)|value));}
 code.push_back(uint16_t(0x0700|seed));const auto op=uint16_t((word?(store?0x6ba0:0x6b20):(store?0x6aa0:0x6a20))|reg);code.insert(code.end(),{op,uint16_t(addr>>16),uint16_t(addr)});h.program(code);
 if(!store){h.data[aligned]=uint8_t(word?value>>8:value);if(word)h.data[(aligned+1)&0xffffff]=uint8_t(value);}
 Cpu c(h);c.run(0,90,1000);auto s=c.snapshot();
 if(store){check(h.data[aligned]==uint8_t(word?value>>8:value) && (!word || h.data[(aligned+1)&0xffffff]==uint8_t(value)),"Absolute store data and aligned big endian");}
 else {auto expected=word?uint16_t(value):uint16_t(reg&8?(0x5a00|value):((value<<8)|0xa5));if(!word)check(s.registers[reg&7]==expected,"Byte load preserves other lane");if(word)check(s.registers[reg]==value,"Word load selects correct low/high bank");}
 check(s.ccr==flags(seed,value,word?16:8),"Absolute move NZV preserves remaining CCR");
 bool saw=false;for(auto [kind,a,v]:h.trace)if(kind==(word?(store?'S':'Q'):(store?'W':'R'))){saw=true;check(a==(word?full&~1u:full),"Full abs32 bus argument and word alignment");}check(saw,"Expected absolute bus access reached");
}
int main(){
 const uint32_t boundaries[]={0,1,0x7fff,0x8000,0xffff,0x10000,0x7fffff,0x800000,0xfffffe,0xffffff,0x1000000,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};
 for(bool word:{false,true})for(bool store:{false,true})for(unsigned reg=0;reg<16;++reg)for(auto address:boundaries)for(unsigned v:{0u,1u,0x7fu,0x80u,0xffu,0x8000u,0xffffu})for(unsigned seed:{0u,0x55u,0xffu})one(word,store,reg,address,word?v:v&255,seed);
 for(bool word:{false,true})for(bool store:{false,true})for(unsigned a=0;a<65536;++a){const uint32_t address=(uint32_t((a*73)&65535)<<16)|a;one(word,store,a&15,address,word?((a*997)&65535):((a*73)&255),a&255);}
 for(unsigned split=0;split<=200;++split){Host a,b;std::vector<uint16_t> p{0xf95a,0x6aa9,0xab12,0x3457,0x6a2a,0xab12,0x3457,0x7903,0x8000,0x6ba3,0xffff,0xffff,0x6b24,0xffff,0xffff,0x40e0};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);check(x.snapshot()==y.snapshot() && a.data==b.data && a.trace==b.trace,"Abs32 mixed every-split invariance");}
 for(auto op:{0x6a29,0x6aa9,0x6b23,0x6ba3}){Host h;h.program({uint16_t(op),0xffff,0xffff});const bool store=op==0x6aa9 || op==0x6ba3;h.bad_read=!store;h.bad_write=store;Cpu c(h);bool threw=false;try{c.run(0,30,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed(),"Abs32 data fault poisons CPU");}
 std::printf("Abs32 memory: boundaries/lanes/flags,65536 address samples per form,201 splits: %u failures\n",failures);return failures?1:0;
}
