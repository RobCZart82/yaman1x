#include "h8_first_instructions.h"
#include <cstdio>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>
using namespace yamanix;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
#include "h8_memory_fixture.h"
using Cpu=H8FirstInstructions<Host>;
static bool condition(unsigned op,unsigned flags){
 bool c=flags&1,v=flags&2,z=flags&4,n=flags&8;
 switch(op){case 0:return true;case 1:return false;case 2:return !c&&!z;case 3:return c||z;
 case 4:return !c;case 5:return c;case 6:return !z;case 7:return z;case 8:return !v;case 9:return v;
 case 10:return !n;case 11:return n;case 12:return n==v;case 13:return n!=v;case 14:return !z&&(n==v);default:return z||(n!=v);}
}
static unsigned count(const Host& h,char kind){return unsigned(std::count_if(h.trace.begin(),h.trace.end(),[&](auto e){return std::get<0>(e)==kind;}));}
static std::vector<uint16_t> load32(unsigned reg,uint32_t value){return {uint16_t(0x7a00|reg),uint16_t(value>>16),uint16_t(value)};}
template<class F>void fails(F f){try{f();check(false,"Expected explicit failure");}catch(const std::runtime_error&){} }
int main(){
 for(unsigned op=0;op<16;++op)for(unsigned flags=0;flags<16;++flags)for(unsigned displacement=0;displacement<256;++displacement){
  Host host;host.program({uint16_t(0x4000|(op<<8)|displacement)});Cpu cpu(host,uint8_t(flags));cpu.run(0,11,100);
  const uint32_t target=uint32_t(0x102+int(int8_t(displacement)));
  check(cpu.snapshot().pc==(condition(op,flags)?target+2:0x104),"65536 rel8 condition/flag/displacement outcomes");
  check(host.trace.size()>=5 && std::get<1>(host.trace[4])==(target&~uint32_t{1}),"Taken and untaken branch preserve target-read bus transaction");
 }
 const uint32_t values[]={0,1,0x7fffffff,0x80000000,0xffffffff,0xdeadbeef,0x123456,0xffff0000};
 for(unsigned reg=0;reg<8;++reg)for(auto value:values){
  Host host;host.program(load32(reg,value));Cpu cpu(host);cpu.run(0,13,100);auto s=cpu.snapshot();
  check(s.registers[reg]==uint16_t(value) && s.registers[reg+8]==uint16_t(value>>16),"Full 32-bit register halves");
  check(s.ccr==(0x80|(value==0?4:(value&0x80000000?8:0))),"32-bit NZV flags");}
 for(unsigned p=0;p<8;++p)for(unsigned dst=0;dst<16;++dst)for(unsigned value=0;value<256;++value){
  Host host;const uint32_t address=0xab123457u+(p<<16);auto code=load32(p,address);code.push_back(uint16_t(0x6800|(p<<4)|dst));host.program(code);host.data[address&0xffffff]=uint8_t(value);
  const auto seed=uint8_t(p*31+dst*7);Cpu cpu(host,seed);cpu.run(0,17,100);auto s=cpu.snapshot();
  std::array<uint16_t,16> expected{};expected[p]=uint16_t(address);expected[p+8]=uint16_t(address>>16);
  auto& lane=expected[dst&7];lane=uint16_t(dst&8?((lane&0xff00)|value):((lane&0xff)|(value<<8)));
  check(s.registers==expected,"32768 byte-load lane/address/alias/value cases");
  check(s.ccr==(((seed|0x80)&~0x0e)|(value==0?4:(value&0x80?8:0))),"Byte-load flags preserve unrelated bits");
  check(count(host,'R')==1 && count(host,'W')==0,"Completed load is never repeated");
  auto it=std::find_if(host.trace.begin(),host.trace.end(),[](auto e){return std::get<0>(e)=='R';});
  check(it!=host.trace.end() && it!=host.trace.begin() && std::get<1>(*it)==address && std::get<1>(*(it-1))==0x108,"Full ER address and prefetch-before-load ordering");
 }
 for(unsigned p=0;p<8;++p)for(unsigned src=0;src<16;++src)for(unsigned value=0;value<256;++value){
  Host host;const uint32_t address=0xab123457u+(p<<16);auto code=load32(p,address);code.push_back(uint16_t(0xf000|(src<<8)|value));code.push_back(uint16_t(0x6880|(p<<4)|src));host.program(code);Cpu cpu(host);cpu.run(0,19,100);
  uint32_t destination=address;
  if((src&7)==p)destination=src&8?((address&0xffffff00)|value):((address&0xffff00ff)|(value<<8));
  check(host.data[destination&0xffffff]==value && count(host,'W')==1 && count(host,'R')==0,"32768 byte-store register/address/alias/value cases");
  check(cpu.snapshot().ccr==(0x80|(value==0?4:(value&0x80?8:0))),"Byte-store NZV flags");
  auto it=std::find_if(host.trace.begin(),host.trace.end(),[](auto e){return std::get<0>(e)=='W';});
  check(it!=host.trace.end() && it!=host.trace.begin() && std::get<1>(*it)==destination && std::get<1>(*(it-1))==0x10a,"Prefetch precedes completed data write");
 }
 for(unsigned seed=0;seed<1000;++seed){
  Host a,b;const std::vector<uint16_t> code{0x7a00,0x12,0x3457,uint16_t(0xf900|(seed&255)),0x6889,0x680a,0x4702,0,0xfb00,0x40ec};a.program(code);b.program(code);Cpu x(a),y(b);x.run(0,512,10000);unsigned random=seed+1;uint64_t start=0;
  while(start<512){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}
  check(x.snapshot()==y.snapshot() && a.data==b.data && a.trace==b.trace,"1000 mixed memory/branch partition histories");}
 for(unsigned opcode=0;opcode<65536;++opcode){
  const bool shift=(opcode&0xfc00)==0x1000 && ((opcode&0x70)==0 || (opcode&0x70)==0x10 || (opcode&0x78)==0x30);
  const bool bit=((opcode&0xfc00)==0x6000 || (opcode&0xff00)==0x6700 || ((opcode&0xfc80)==0x7000) || (opcode&0xfc00)==0x7400);
  const bool control=opcode==0x5470 || opcode==0x5670 || (opcode&0xff00)==0x5500 || opcode==0x5c00 || (opcode&0xff8f)==0x5900 || (opcode&0xff8f)==0x5d00 || (opcode&0xfe00)==0x5a00 || (opcode&0xfe00)==0x5e00 || (opcode&0xff0f)==0x5800 || (opcode&0xfe00)==0x6c00;
  const bool ccr=(opcode&0xfff0)==0x0200 || (opcode&0xfff0)==0x0300 || (opcode&0xff00)==0x0400 || (opcode&0xff00)==0x0500 || (opcode&0xff00)==0x0600;
  const bool transfer=(opcode&0xff00)==0x0c00 || (opcode&0xff00)==0x0d00 || (opcode&0xff88)==0x0f80;
  const bool small_arithmetic=(opcode&0xfff0)==0x0a00 || (opcode&0xfff0)==0x1a00 || (opcode&0xfff0)==0x0b50 || (opcode&0xfff8)==0x0b70 || (opcode&0xfff0)==0x0bd0 || (opcode&0xfff8)==0x0bf0 || (opcode&0xfff0)==0x1b50 || (opcode&0xfff8)==0x1b70 || (opcode&0xfff0)==0x1bd0 || (opcode&0xfff8)==0x1bf0 || (opcode&0xfff8)==0x0b00 || (opcode&0xfff8)==0x0b80 || (opcode&0xfff8)==0x0b90 || (opcode&0xfff8)==0x1b00 || (opcode&0xfff8)==0x1b80 || (opcode&0xfff8)==0x1b90;
  const bool unary=(opcode&0xfff0)==0x1700 || (opcode&0xfff0)==0x1710 || (opcode&0xfff8)==0x1730 || (opcode&0xfff0)==0x1750 || (opcode&0xfff8)==0x1770 || (opcode&0xfff0)==0x1780 || (opcode&0xfff0)==0x1790 || (opcode&0xfff8)==0x17b0 || (opcode&0xfff0)==0x17d0 || (opcode&0xfff8)==0x17f0;
  const bool trap=(opcode&0xffcf)==0x5700;
  const bool absolute=(opcode&0xf000)==0x2000 || (opcode&0xf000)==0x3000 || (opcode&0xfff0)==0x6a00 || (opcode&0xfff0)==0x6a80 || (opcode&0xfff0)==0x6b00 || (opcode&0xfff0)==0x6b80;
  const bool absolute32=(opcode&0xfff0)==0x6a20 || (opcode&0xfff0)==0x6aa0 || (opcode&0xfff0)==0x6b20 || (opcode&0xfff0)==0x6ba0;
  const bool supported=opcode==0x7b5c || opcode==0x7bd4 || (opcode&0xfff0)==0x6a40 || (opcode&0xfff0)==0x6ac0 || opcode==0x0140 || (opcode&0xff8f)==0x7800 || opcode==0x01d0 || (opcode&0xfff0)==0x0f00 || (opcode&0xfff0)==0x1f00 || (opcode&0xff08)==0x5300 || (opcode&0xff00)==0x5100 || opcode==0x01c0 || (opcode&0xff08)==0x5200 || (opcode&0xff00)==0x5000 || (opcode&0xfe00)==0x6e00 || absolute32 || absolute || trap || unary || small_arithmetic || transfer || ccr || control || ((opcode&0xfe00)==0x7e00) || ((opcode&0xfe8f)==0x7c00) || shift || bit || ((opcode&0xff00)==0x0700 || (opcode&0xff00)==0x0900 || (opcode&0xff00)==0x1900 || (opcode&0xff00)==0x1d00 || (opcode&0xff88)==0x0a80 || (opcode&0xff88)==0x1a80 || (opcode&0xff88)==0x1f80 || (opcode&0xf000)==0x9000 || (opcode&0xf000)==0xb000 || (opcode&0xff00)==0x0e00 || (opcode&0xff00)==0x1e00 || (opcode&0xf000)==0xc000 || (opcode&0xf000)==0xd000 || (opcode&0xf000)==0xe000 || (opcode&0xff00)==0x1400 || (opcode&0xff00)==0x1500 || (opcode&0xff00)==0x1600) || opcode==0 || opcode==0x0100 || opcode==0x01f0 || (opcode&0xff00)==0x6400 || (opcode&0xff00)==0x6500 || (opcode&0xff00)==0x6600 || (opcode&0xf000)==0x8000 || (opcode&0xf000)==0xa000 || (opcode&0xff00)==0x0800 || (opcode&0xff00)==0x1800 || (opcode&0xff00)==0x1c00 || ((opcode&0xff00)==0x7900 && (opcode&0xf0)<=0x60) || (opcode&0xff00)==0x6900 || (opcode&0xf000)==0xf000 || (opcode&0xf000)==0x4000 || ((opcode&0xff08)==0x7a00 && (opcode&0xf0)<=0x60) || (opcode&0xff00)==0x6800;
  Host host;host.program({uint16_t(opcode)});Cpu cpu(host);bool rejected=false;
  try{cpu.run(0,7,100);}catch(const std::runtime_error&){rejected=true;}
  check(rejected!=supported && cpu.failed()==rejected,"Exhaustive 65536-word reduced-decoder acceptance/rejection");}
 for(auto opcode:{0x7a08,0x7a70,0x6a10}){Host host;host.program({uint16_t(opcode)});Cpu cpu(host);fails([&]{cpu.run(0,20,100);});check(cpu.failed(),"Unsupported opcode neighbors rejected");}
 {Host host;auto code=load32(0,0x123457);code.push_back(0x6809);host.program(code);host.bad_read=true;Cpu cpu(host);fails([&]{cpu.run(0,20,100);});check(cpu.failed(),"Data read failure poisons CPU");}
 {Host host;auto code=load32(0,0x123457);code.push_back(0xf980);code.push_back(0x6889);host.program(code);host.bad_write=true;Cpu cpu(host);fails([&]{cpu.run(0,20,100);});check(cpu.failed() && host.data[0x123457]==0x80,"Failed data store has no rollback; discard owner");}
 std::printf("H8 memory/branches: 65536 branch, 32768 load, 32768 store cases, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
