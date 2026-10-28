#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include "h8_flags_oracle.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static std::vector<uint16_t> init_registers(std::array<uint16_t,16>& r,unsigned n){
 std::vector<uint16_t> code;for(unsigned i=0;i<8;++i){const uint32_t v=(0x12345678u*(i+1))^(n*0x11111111u);r[i]=uint16_t(v);r[i+8]=uint16_t(v>>16);code.insert(code.end(),{uint16_t(0x7a00|i),uint16_t(v>>16),uint16_t(v)});}return code;
}
static uint32_t get(const std::array<uint16_t,16>& r,unsigned reg,unsigned bits){return bits==8?uint8_t(reg&8?r[reg&7]:r[reg&7]/256):bits==16?r[reg]:uint32_t(r[reg])|(uint32_t(r[reg+8])<<16);}
static void put(std::array<uint16_t,16>& r,unsigned reg,unsigned bits,uint32_t v){if(bits==8)r[reg&7]=uint16_t(reg&8?((r[reg&7]&0xff00)|uint8_t(v)):((r[reg&7]&255)|(uint16_t(uint8_t(v))<<8)));else{r[reg]=uint16_t(v);if(bits==32)r[reg+8]=uint16_t(v>>16);}}
static uint32_t logic(unsigned op,uint32_t a,uint32_t b){return op==0?a|b:op==1?a^b:a&b;}
static uint8_t logic_flags(unsigned bits,uint32_t value,unsigned seed){return uint8_t((seed&~0x0e)|(value==0?4:(uint64_t(value)>=(uint64_t{1}<<(bits-1))?8:0)));}
int main(){
 for(unsigned form=0;form<4;++form)for(unsigned c=0;c<2;++c)for(unsigned z=0;z<2;++z)for(unsigned a=0;a<256;++a)for(unsigned b=0;b<256;++b){
  const unsigned seed=(((a*37+b*13)&255)&~5)|c|(z<<2);const bool reg=form>=2,sub=form&1;
  Host host;std::vector<uint16_t> code{uint16_t(0xf800|a)};if(reg)code.push_back(uint16_t(0xf900|b));code.push_back(uint16_t(0x0700|seed));code.push_back(uint16_t(reg?(sub?0x1e98:0x0e98):((sub?0xb800:0x9800)|b)));host.program(code);Cpu cpu(host);cpu.run(0,reg?13:11,1000);
  const auto e=expected(8,a,b,sub,seed,true,false);std::array<uint16_t,16> wanted{};wanted[0]=uint16_t(e.value);if(reg)wanted[1]=uint16_t(b);
  check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==e.ccr,"1048576 carry-input/sticky-Z byte cases");}
 for(unsigned bits:{16u,32u})for(unsigned op=0;op<3;++op)for(unsigned src=0;src<(bits==16?16u:8u);++src)for(unsigned dst=0;dst<(bits==16?16u:8u);++dst)for(unsigned n=0;n<32;++n){
  std::array<uint16_t,16> wanted{};auto code=init_registers(wanted,n);const unsigned seed=(n*17+src*31+dst*7)&255;
  const auto e=expected(bits,get(wanted,dst,bits),get(wanted,src,bits),op!=0,seed,false,false);if(op!=2)put(wanted,dst,bits,e.value);
  const unsigned prefix=bits==16?(op==0?0x0900:op==1?0x1900:0x1d00):(op==0?0x0a80:op==1?0x1a80:0x1f80);
  code.push_back(uint16_t(0x0700|seed));code.push_back(uint16_t(prefix|(src<<4)|dst));Host host;host.program(code);Cpu cpu(host);cpu.run(0,57,1000);
  check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==e.ccr,"30720 wide register/lane/alias arithmetic cases");}
 for(unsigned op=0;op<3;++op)for(unsigned reg=0;reg<2;++reg)for(unsigned a=0;a<256;++a)for(unsigned b=0;b<256;++b){
  const unsigned seed=(a*11+b*37)&255;Host host;std::vector<uint16_t> code{uint16_t(0xf800|a)};if(reg)code.push_back(uint16_t(0xf900|b));code.push_back(uint16_t(0x0700|seed));code.push_back(uint16_t(reg?((op==0?0x1400:op==1?0x1500:0x1600)|0x98):((op==0?0xc800:op==1?0xd800:0xe800)|b)));host.program(code);Cpu cpu(host);cpu.run(0,reg?13:11,1000);
  const auto value=logic(op,a,b);std::array<uint16_t,16> wanted{};wanted[0]=uint16_t(value);if(reg)wanted[1]=uint16_t(b);
  check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==logic_flags(8,value,seed),"393216 byte logic pairs preserve H/C and replace NZV");}
 for(unsigned op=0;op<3;++op)for(unsigned src=0;src<16;++src)for(unsigned dst=0;dst<16;++dst)for(unsigned n=0;n<16;++n){
  std::array<uint16_t,16> wanted{};auto code=init_registers(wanted,n);const unsigned seed=(n*17+src*7)&255;const auto value=logic(op,get(wanted,dst,8),get(wanted,src,8));put(wanted,dst,8,value);code.push_back(uint16_t(0x0700|seed));code.push_back(uint16_t((op==0?0x1400:op==1?0x1500:0x1600)|(src<<4)|dst));Host host;host.program(code);Cpu cpu(host);cpu.run(0,57,1000);check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==logic_flags(8,value,seed),"12288 byte logic aliases and untouched halves");}
 for(unsigned bits:{16u,32u})for(unsigned op=0;op<3;++op)for(unsigned reg=0;reg<(bits==16?16u:8u);++reg)for(unsigned n=0;n<512;++n){
  const uint32_t mask=bits==16?0xffffu:0xffffffffu;const uint32_t a=(n*0x1234567u)&mask,b=(n%7==0?a:n%7==1?0:n%7==2?mask:(n*0xa5a55a5au)&mask);const auto value=logic(op,a,b);const unsigned seed=n&255;
  std::vector<uint16_t> code=bits==16?std::vector<uint16_t>{uint16_t(0x7900|reg),uint16_t(a)}:std::vector<uint16_t>{uint16_t(0x7a00|reg),uint16_t(a>>16),uint16_t(a)};code.push_back(uint16_t(0x0700|seed));code.push_back(uint16_t((bits==16?0x7940:0x7a40)|(op<<4)|reg));if(bits==32)code.push_back(uint16_t(b>>16));code.push_back(uint16_t(b));Host host;host.program(code);Cpu cpu(host);cpu.run(0,bits==16?15:19,1000);std::array<uint16_t,16> wanted{};put(wanted,reg,bits,value);check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==logic_flags(bits,value,seed),"36864 wide immediate logic and flags");}
 unsigned random=123;
 for(unsigned op=0;op<2;++op)for(unsigned n=0;n<10000;++n){
  random=random*1664525u+1013904223u;const uint32_t a=random;random=random*1664525u+1013904223u;const uint32_t b=n%17==0?a:random;const unsigned seed=(n*37)&255;std::vector<uint16_t> code;
  for(unsigned i=0;i<4;++i){code.push_back(uint16_t(0xf800|(i<<8)|uint8_t(a>>(8*i))));code.push_back(uint16_t(0xfc00|(i<<8)|uint8_t(b>>(8*i))));}
  code.push_back(uint16_t(0x0700|seed));for(unsigned i=0;i<4;++i)code.push_back(uint16_t((op?0x1e00:0x0e00)|((12+i)<<4)|(8+i)));Host host;host.program(code);Cpu cpu(host);cpu.run(0,31,1000);const auto e=expected(32,a,b,op!=0,seed,true,false);std::array<uint16_t,16> wanted{};for(unsigned i=0;i<4;++i){wanted[i]=uint8_t(e.value>>(8*i));wanted[i+4]=uint8_t(b>>(8*i));}check(cpu.snapshot().registers==wanted && cpu.snapshot().ccr==e.ccr,"20000 four-byte carry/borrow chains and sticky zero");}
 {Host host;host.program({0xf8ff,0x0705,0x9800});Cpu cpu(host);cpu.run(0,11,1000);check(cpu.snapshot().registers[0]==0 && (cpu.snapshot().ccr&5)==5,"Carry and zero committed before suspended prefetch");cpu.run(11,1,1000);cpu.run(12,1,1000);check(cpu.snapshot().registers[0]==0 && (cpu.snapshot().ccr&5)==5,"Carry operation is not repeated on partial continuation");}
 for(unsigned seed=0;seed<1000;++seed){Host a,b;const std::vector<uint16_t> code{0x7a00,0x12,0x3457,uint16_t(0xf900|(seed&255)),uint16_t(0x0700|(seed&255)),0x9901,0xba00,0x0e9a,0x1ea9,0xc955,0xd9aa,0xe90f,0x149a,0x159a,0x169a,0x0934,0x1934,0x1d34,0x0ab4,0x1ab4,0x1fb4,0x6889,0x680a,0x40d0};a.program(code);b.program(code);Cpu x(a),y(b);x.run(0,512,10000);unsigned r=seed+1;uint64_t start=0;while(start<512){r=r*1664525u+1013904223u;int size=int(1+r%19);if(start+size>512)size=int(512-start);y.run(start,size,1000);start+=size;}check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data,"1000 carry/logic partition histories");}
 std::printf("H8 carry/register/logic: 1048576 carry, 30720 register, 393216 byte logic, 12288 logic aliases, 36864 wide logic, 20000 chains: %u failures\n",failures);return failures?1:0;
}
