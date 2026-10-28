#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static uint8_t flags(unsigned seed,unsigned value,unsigned bits){return uint8_t((seed&~14u)|(value==0?4:((value>>(bits-1))&1)?8:0));}
static void one(bool word,bool store,bool short8,unsigned reg,unsigned addr,unsigned value,unsigned seed){
 Host h;const uint32_t full=short8?0xffffff00u|addr:uint32_t(addr<0x8000?addr:0xffff0000u|addr);const uint32_t physical=full&0xffffff,aligned=word?physical&~1u:physical;
 std::vector<uint16_t> code;for(unsigned r=0;r<8;++r)code.insert(code.end(),{uint16_t(0x7a00|r),0xa55a,0x5aa5});
 if(store){if(word)code.insert(code.end(),{uint16_t(0x7900|reg),uint16_t(value)});else code.push_back(uint16_t(0xf000|(reg<<8)|value));}
 code.push_back(uint16_t(0x0700|seed));const auto op=uint16_t(short8?(store?0x3000:0x2000)|(reg<<8)|addr:(word?(store?0x6b80:0x6b00):(store?0x6a80:0x6a00))|reg);code.push_back(op);if(!short8)code.push_back(uint16_t(addr));h.program(code);
 if(!store){h.data[aligned]=uint8_t(word?value>>8:value);if(word)h.data[(aligned+1)&0xffffff]=uint8_t(value);}
 Cpu c(h);c.run(0,90,1000);auto s=c.snapshot();
 if(store){check(h.data[aligned]==uint8_t(word?value>>8:value) && (!word || h.data[(aligned+1)&0xffffff]==uint8_t(value)),"Absolute store data and aligned big endian");}
 else {auto expected=word?uint16_t(value):uint16_t(reg&8?(0x5a00|value):((value<<8)|0xa5));if(!word)check(s.registers[reg&7]==expected,"Byte load preserves other lane");if(word)check(s.registers[reg]==value,"Word load selects correct low/high bank");}
 check(s.ccr==flags(seed,value,word?16:8),"Absolute move NZV preserves remaining CCR");
 bool saw=false;for(auto [kind,a,v]:h.trace)if(kind==(word?(store?'S':'Q'):(store?'W':'R'))){saw=true;check(a==(word?full&~1u:full),"Raw signed16/top-page8 address and word alignment");}check(saw,"Expected absolute bus access reached");
}
int main(){
 for(bool store:{false,true})for(unsigned reg=0;reg<16;++reg)for(unsigned value=0;value<256;++value)for(unsigned seed:{0u,0x55u,0xffu})one(false,store,true,reg,(value*73)&255,value,seed);
 for(bool word:{false,true})for(bool store:{false,true})for(unsigned a=0;a<65536;++a)one(word,store,false,a&15,a,word?((a*997)&65535):((a*73)&255),a&255);
 for(unsigned split=0;split<=200;++split){Host a,b;std::vector<uint16_t> p{0xf95a,0x39ab,0x2aab,0x6a89,0x8001,0x6a0a,0x8001,0x7903,0x8000,0x6b83,0xffff,0x6b04,0xffff,0x40e4};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);check(x.snapshot()==y.snapshot() && a.data==b.data && a.trace==b.trace,"Absolute mixed program every-split invariance");}
 for(auto op:{0x2901,0x3901,0x6a09,0x6a89,0x6b03,0x6b83}){Host h;h.program({uint16_t(op),0x8001});const bool store=op==0x3901 || op==0x6a89 || op==0x6b83;h.bad_read=!store;h.bad_write=store;Cpu c(h);bool threw=false;try{c.run(0,30,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed(),"Absolute access failure poisons CPU");}
 std::printf("Absolute memory: abs8 lane/value/flag matrix, all65536 abs16 addresses and201 splits: %u failures\n",failures);return failures?1:0;
}
