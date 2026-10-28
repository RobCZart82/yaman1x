#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <algorithm>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
#include "h8_flags_oracle.h"
static uint8_t byte(const std::array<uint16_t,16>& r,unsigned reg){return uint8_t(reg&8?r[reg&7]:r[reg&7]/256);}
static void set_byte(std::array<uint16_t,16>& r,unsigned reg,uint8_t v){r[reg&7]=uint16_t(reg&8?((r[reg&7]&0xff00)|v):((r[reg&7]&0xff)|(uint16_t(v)<<8)));}
static void byte_pair(unsigned form,unsigned a,unsigned b,unsigned seed){
 Host host;std::vector<uint16_t> code{uint16_t(0xf800|a)};const bool reg=form>=2,sub=form==1||form>=3,cmp=form==1||form==4;
 if(reg)code.push_back(uint16_t(0xf900|b));
 code.push_back(uint16_t(form==0?(0x8800|b):form==1?(0xa800|b):form==2?0x0898:form==3?0x1898:0x1c98));host.program(code);Cpu cpu(host,uint8_t(seed));cpu.run(0,reg?11:9,1000);
 const auto e=expected(8,a,b,sub,seed);std::array<uint16_t,16> wanted{};wanted[0]=uint16_t(cmp?a:e.value);if(reg)wanted[1]=uint16_t(b);
 check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==e.ccr,"Exhaustive byte arithmetic result, flags and CMP no-write");
}
static void wide_case(unsigned bits,unsigned op,unsigned reg,uint32_t a,uint32_t b,unsigned seed){
 const uint64_t mask=(uint64_t{1}<<bits)-1;a=uint32_t(a&mask);b=uint32_t(b&mask);
 Host host;std::vector<uint16_t> code;
 if(bits==16)code={uint16_t(0x7900|reg),uint16_t(a),uint16_t(0x7900|((op==0?1:op==1?3:2)<<4)|reg),uint16_t(b)};
 else code={uint16_t(0x7a00|reg),uint16_t(a>>16),uint16_t(a),uint16_t(0x7a00|((op==0?1:op==1?3:2)<<4)|reg),uint16_t(b>>16),uint16_t(b)};
 host.program(code);Cpu cpu(host,uint8_t(seed));cpu.run(0,bits==16?13:17,1000);const auto e=expected(bits,a,b,op!=0,seed);const auto value=op==2?a:e.value;
 std::array<uint16_t,16> wanted{};wanted[reg]=uint16_t(value);if(bits==32)wanted[reg+8]=uint16_t(value>>16);
 check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==e.ccr,"Wide arithmetic signed-range oracle, flags and CMP no-write");
}
int main(){
 for(unsigned form=0;form<5;++form)for(unsigned a=0;a<256;++a)for(unsigned b=0;b<256;++b)byte_pair(form,a,b,(a*37+b*13)&255);
 for(unsigned op=0;op<3;++op)for(unsigned src=0;src<16;++src)for(unsigned dst=0;dst<16;++dst)for(unsigned n=0;n<16;++n){
  Host host;std::vector<uint16_t> code;std::array<uint16_t,16> wanted{};
  for(unsigned r=0;r<8;++r){const uint32_t value=(0x12345678u*(r+1))^(n*0x11111111u);code.insert(code.end(),{uint16_t(0x7a00|r),uint16_t(value>>16),uint16_t(value)});wanted[r]=uint16_t(value);wanted[r+8]=uint16_t(value>>16);}
  const auto e=expected(8,byte(wanted,dst),byte(wanted,src),op!=0,n*17);if(op!=2)set_byte(wanted,dst,uint8_t(e.value));
  code.push_back(uint16_t((op==0?0x0800:op==1?0x1800:0x1c00)|(src<<4)|dst));host.program(code);Cpu cpu(host,uint8_t(n*17));cpu.run(0,55,1000);
  check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==e.ccr,"12288 register/byte-lane/alias arithmetic cases");}
 const uint32_t boundaries[]={0,1,7,8,15,16,0xff,0x100,0x7ff,0xfff,0x1000,0x7fff,0x8000,0xffff,0x10000,0x0fffffff,0x10000000,0x7fffffff,0x80000000,0xffffffff};
 for(unsigned bits:{16u,32u})for(unsigned op=0;op<3;++op)for(unsigned reg=0;reg<(bits==16?16u:8u);++reg){
  for(auto a:boundaries)for(auto b:boundaries)wide_case(bits,op,reg,a,b,(a^b)&255);
  unsigned random=reg+op*37+bits;for(unsigned n=0;n<1000;++n){random=random*1664525u+1013904223u;const auto a=random;random=random*1664525u+1013904223u;wide_case(bits,op,reg,a,random,n&255);}}
 {Host host;host.program({0xf87f,0x8801});Cpu cpu(host);cpu.run(0,9,100);auto once=cpu.snapshot();check(once.registers[0]==0x80 && once.substate==2,"Arithmetic committed before suspended prefetch");cpu.run(9,1,100);check(cpu.snapshot().registers[0]==0x80,"Debt repayment does not repeat arithmetic");cpu.run(10,1,100);check(cpu.snapshot().registers[0]==0x80 && cpu.snapshot().ccr==0xaa,"Partial continuation never applies ADD twice");}
 for(unsigned seed=0;seed<1000;++seed){Host a,b;const std::vector<uint16_t> code{0x7a00,0x12,0x3457,uint16_t(0xf900|(seed&255)),0x8901,0xa980,0x4702,0x18a9,0x089a,0x1c9a,0x7903,0x7fff,0x7913,1,0x7923,0x8000,0x7933,0xffff,0x7a04,0x7fff,0xffff,0x7a14,0,1,0x7a24,0x8000,0,0x7a34,0xffff,0xffff,0x6889,0x680a,0x40be};a.program(code);b.program(code);Cpu x(a),y(b);x.run(0,512,10000);unsigned random=seed+1;uint64_t start=0;while(start<512){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data,"1000 arithmetic/condition/memory partition histories");}
 std::printf("H8 arithmetic: 327680 byte pairs, 12288 register/alias cases, 100800 wide cases, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
