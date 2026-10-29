#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static void one(unsigned bits,bool store,unsigned ptr,unsigned reg,uint32_t base,unsigned disp,unsigned seed){
 Host h;std::array<uint16_t,16> want{};std::vector<uint16_t> p;
 for(unsigned r=0;r<8;++r){uint32_t v=r==ptr?base:((r+1)*0x1234567u)^(seed*0x11111111u);want[r]=uint16_t(v);want[r+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)});}
 uint32_t full=uint32_t(uint64_t(base)+uint64_t(disp));uint32_t physical=(bits==8?full:full&~1u)&0xffffff;
 uint32_t value=store?(bits==8?uint8_t(reg&8?want[reg&7]:want[reg&7]/256):bits==16?want[reg]:uint32_t(want[reg])|(uint32_t(want[reg+8])<<16)):uint32_t(disp*0x1020305u)^seed;
 if(bits==8)value&=255;if(bits==16)value&=65535;
 if(!store)for(unsigned b=0;b<bits/8;++b)h.data[(physical+b)&0xffffff]=uint8_t(value>>(bits-8-8*b));
 p.push_back(uint16_t(0x0700|seed));p.push_back(uint16_t(0x7800|(ptr<<4)));p.push_back(uint16_t((bits==8?0x6a20:0x6b20)|(store?0x80:0)|reg));p.push_back(uint16_t(disp>>16));p.push_back(uint16_t(disp));h.program(p);Cpu c(h);c.run(0,100,1000);
 if(store){for(unsigned b=0;b<bits/8;++b)check(h.data[(physical+b)&0xffffff]==uint8_t(value>>(bits-8-8*b)),"Store value/address/lane order");}
 else if(bits==8){auto& w=want[reg&7];w=uint16_t(reg&8?((w&0xff00)|value):((w&255)|(value<<8)));}else {want[reg]=uint16_t(value);if(bits==32)want[reg+8]=uint16_t(value>>16);}
 check(c.snapshot().registers==want,"Full register image and address/data alias");check(c.snapshot().ccr==uint8_t((seed&~14u)|(value==0?4:((uint64_t(value)>>(bits-1))?8:0))),"NZV and unrelated CCR bits");
 std::vector<uint32_t> got;for(auto [kind,a,v]:h.trace)if(kind==(store?(bits==8?'W':'S'):(bits==8?'R':'Q')))got.push_back(a);std::vector<uint32_t> addresses{bits==8?full:full&~1u};if(bits==32)addresses.push_back((full+2)&~1u);check(got==addresses,"Exact data addresses/count and long high/low order");
}
int main(){
 for(unsigned bits:{8u,16u})for(bool store:{false,true})for(unsigned high:{0u,0x8000u,0xffffu})for(unsigned low=0;low<65536;++low)one(bits,store,0,bits==8?9:1,0xab123457,(high<<16)|low,low&255);
 for(unsigned bits:{8u,16u})for(bool store:{false,true})for(unsigned high=0;high<65536;++high)one(bits,store,0,bits==8?9:1,0xffffffff,(high<<16)|0xa55au,high&255);
 for(unsigned bits:{8u,16u})for(bool store:{false,true})for(unsigned ptr=0;ptr<8;++ptr)for(unsigned reg=0;reg<16;++reg)for(uint32_t offset:{0u,1u,0x7fffffffu,0x80000000u,0xffff0000u,0xffffffffu})for(uint32_t base:{0u,1u,0xffffffu,0x1000000u,0x80000000u,0xffffffffu})for(unsigned flags:{0u,0x55u,0xffu})one(bits,store,ptr,reg,base,offset,flags);
 for(unsigned cut=0;cut<=240;++cut){Host a,b;const std::vector<uint16_t> p{0x7a00,0xab12,0x3457,0x7a01,0x89ab,0xcdef,0x7800,0x6aa9,0xffff,0xffff,0x7800,0x6a2a,0xffff,0xffff,0x7800,0x6ba1,0x8000,0,0x7800,0x6b22,0x8000,0,0x40d2};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,10000);y.run(0,int(cut),10000);y.run(cut,int(240-cut),10000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"241 split snapshots/ordered bus histories/RAM");}
 for(unsigned ext=0;ext<65536;++ext){Host h;h.program({0x7800,uint16_t(ext),0,0});Cpu c(h);bool rejected=false;try{c.run(0,9,100);}catch(const std::runtime_error&){rejected=true;}unsigned op=ext&0xfff0;bool valid=op==0x6a20||op==0x6aa0||op==0x6b20||op==0x6ba0;check(rejected!=valid&&c.failed()==rejected,"65536 displacement32 extension masks");}
 for(unsigned bits:{8u,16u})for(bool store:{false,true}){Host h;h.program({0x7a00,0xffff,0xffff,0x7800,uint16_t((bits==8?0x6a20:0x6b20)|(store?0x80:0)|1),0,1});h.bad_read=!store;h.bad_write=store;Cpu c(h);bool rejected=false;try{c.run(0,80,1000);}catch(const std::runtime_error&){rejected=true;}check(rejected&&c.failed(),"Read/write faults poison CPU");bool again=false;try{c.run(80,1,1000);}catch(const std::logic_error&){again=true;}check(again,"Failed CPU cannot resume");}
 std::printf("Displacement32 byte/word:1048576 half-offset sweeps,55296 alias/address/CCR boundaries,241 splits,65536 prefix guards,faults:%u failures\n",failures);return failures?1:0;
}
