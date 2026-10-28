#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <algorithm>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
struct Expected{uint32_t value;uint8_t ccr;};
// Independent bit-lane oracle; SHAL overflow uses signed range, not sign xor.
static Expected oracle(unsigned bits,unsigned op,uint32_t v,unsigned seed){
 std::array<bool,32> in{},out{};for(unsigned i=0;i<bits;++i)in[i]=(uint64_t(v)/(uint64_t{1}<<i))%2;
 const bool left=op==0||op==1||op==4||op==5,carry=left?in[bits-1]:in[0];
 for(unsigned i=0;i<bits;++i)out[i]=left?(i?in[i-1]:false):(i+1<bits?in[i+1]:false);
 if(op==3)out[bits-1]=in[bits-1];if(op==4)out[0]=seed&1;if(op==5)out[0]=carry;if(op==6)out[bits-1]=seed&1;if(op==7)out[bits-1]=carry;
 uint32_t result=0;for(unsigned i=0;i<bits;++i)if(out[i])result+=uint32_t(uint64_t{1}<<i);
 unsigned flags=seed&0xf0;if(!result)flags|=4;if(out[bits-1])flags|=8;if(carry)flags|=1;
 const int64_t sign=int64_t{1}<<(bits-1),signed_value=in[bits-1]?int64_t(v)-(sign*2):v;
 if(op==1 && (signed_value*2 < -sign || signed_value*2>=sign))flags|=2;
 return {result,uint8_t(flags)};
}
static uint16_t opcode(unsigned bits,unsigned op,unsigned reg){const unsigned high=0x1000+(op/2)*0x100+((op&1)?0x80:0);return uint16_t(high+(bits==8?0:bits==16?0x10:0x30)+reg);}
static void single(unsigned bits,unsigned op,uint32_t v,unsigned seed){Host h;std::vector<uint16_t> p;if(bits==8)p={uint16_t(0xf800|v)};else if(bits==16)p={0x7900,uint16_t(v)};else p={0x7a00,uint16_t(v>>16),uint16_t(v)};p.push_back(uint16_t(0x0700|seed));p.push_back(opcode(bits,op,bits==8?8:0));h.program(p);Cpu c(h);c.run(0,bits==8?11:bits==16?13:15,1000);auto e=oracle(bits,op,v,seed);std::array<uint16_t,16> want{};want[0]=uint16_t(e.value);if(bits==32)want[8]=uint16_t(e.value>>16);check(c.snapshot().registers==want && c.snapshot().ccr==e.ccr,"Independent shift/rotate result and complete CCR");}
int main(){
 for(unsigned op=0;op<8;++op)for(unsigned v=0;v<256;++v)for(unsigned seed=0;seed<256;++seed)single(8,op,v,seed);
 for(unsigned op=0;op<8;++op)for(unsigned v=0;v<65536;++v)for(unsigned carry=0;carry<2;++carry)single(16,op,v,((v*37)&254)|carry);
 for(unsigned op=0;op<8;++op)for(unsigned seed=0;seed<4096;++seed)for(unsigned carry=0;carry<2;++carry){uint32_t v=seed*0x1234567u;single(32,op,v,((seed*17)&254)|carry);}
 for(unsigned op=0;op<8;++op)for(uint32_t v:{0u,1u,2u,0x3fffffffu,0x40000000u,0x7fffffffu,0x80000000u,0x80000001u,0xbfffffffu,0xc0000000u,0xfffffffeu,0xffffffffu})for(unsigned seed=0;seed<256;++seed)single(32,op,v,seed);
 for(unsigned bits:{8u,16u,32u})for(unsigned op=0;op<8;++op)for(unsigned reg=0;reg<(bits==32?8u:16u);++reg)for(unsigned seed=0;seed<256;++seed){std::array<uint16_t,16> want{};std::vector<uint16_t> p;for(unsigned n=0;n<8;++n){uint32_t v=(0x1234567u*(n+1))^(seed*0x11111111u);want[n]=uint16_t(v);want[n+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|n),uint16_t(v>>16),uint16_t(v)});}uint32_t v=bits==8?uint8_t(reg&8?want[reg&7]:want[reg&7]/256):bits==16?want[reg]:uint32_t(want[reg])|(uint32_t(want[reg+8])<<16);auto e=oracle(bits,op,v,seed);if(bits==8){auto& r=want[reg&7];r=uint16_t(reg&8?((r&0xff00)|e.value):((r&255)|(e.value<<8)));}else{want[reg]=uint16_t(e.value);if(bits==32)want[reg+8]=uint16_t(e.value>>16);}p.push_back(uint16_t(0x0700|seed));p.push_back(opcode(bits,op,reg));Host h;h.program(p);Cpu c(h);c.run(0,57,1000);check(c.snapshot().registers==want && c.snapshot().ccr==e.ccr,"81920 lane and CCR preservation cases");}
 {Host h;h.program({0xf881,0x0700,0x1008});Cpu c(h);c.run(0,11,100);check(c.snapshot().registers[0]==2 && (c.snapshot().ccr&1),"Shift committed before suspended prefetch");auto trace=h.trace;c.run(11,1,100);check(h.trace==trace,"Debt repayment does not refetch");c.run(12,1,100);check(c.snapshot().registers[0]==2 && (c.snapshot().ccr&1) && std::count_if(h.trace.begin(),h.trace.end(),[](auto e){return std::get<1>(e)==0x106;})==1,"Partial continuation never shifts twice");}
 for(unsigned seed=0;seed<1000;++seed){Host a,b;std::vector<uint16_t> p{0x7a00,uint16_t(seed*17),uint16_t(seed*73),uint16_t(0x0700|(seed&255))};for(unsigned op=0;op<8;++op)for(unsigned bits:{8u,16u,32u})p.push_back(opcode(bits,op,bits==8?8:0));p.push_back(0x40ce);a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,512,10000);uint64_t start=0;unsigned r=seed+1;while(start<512){r=r*1664525u+1013904223u;int n=int(1+r%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data,"1000 mixed shift/rotate partition histories");}
 std::printf("H8 shift/rotate: 524288 byte, 1048576 word, 65536 long random, 24576 long boundary, 81920 lane/CCR, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
