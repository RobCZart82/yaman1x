#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
static std::vector<uint16_t> init(std::array<uint16_t,16>& r){std::vector<uint16_t> p;for(unsigned n=0;n<8;++n){uint32_t v=0x12345678u+n*0x1020304u;r[n]=uint16_t(v);r[n+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|n),uint16_t(v>>16),uint16_t(v)});}return p;}
static void byte(std::array<uint16_t,16>& r,unsigned lane,unsigned v){if(lane&8)r[lane&7]=uint16_t((r[lane&7]&0xff00)|v);else r[lane&7]=uint16_t((r[lane&7]&255)|(v<<8));}
int main(){
 for(unsigned op=0;op<3;++op)for(unsigned ccr=0;ccr<256;++ccr)for(unsigned operand=0;operand<256;++operand){Host h;h.program({uint16_t(0x0700|ccr),uint16_t(0x0400|(op<<8)|operand)});Cpu c(h);c.run(0,9,100);auto s=c.snapshot();unsigned expected=op==0?(ccr|operand):op==1?(ccr^operand):(ccr&operand);check(s.ccr==expected && s.registers==std::array<uint16_t,16>{},"All immediate CCR operands preserve every bit and registers");}
 for(unsigned lane=0;lane<16;++lane)for(unsigned value=0;value<256;++value){
  std::array<uint16_t,16> want{};auto p=init(want);p.insert(p.end(),{uint16_t(0x0700|value),uint16_t(0x0200|lane)});byte(want,lane,value);Host h;h.program(p);Cpu c(h);c.run(0,57,1000);check(c.snapshot().registers==want && c.snapshot().ccr==value,"STC all byte lanes preserves CCR and other registers");
  for(unsigned seed:{0u,255u,85u}){want={};p=init(want);p.push_back(uint16_t(0xf000|(lane<<8)|value));byte(want,lane,value);p.insert(p.end(),{uint16_t(0x0700|seed),uint16_t(0x0300|lane)});Host g;g.program(p);Cpu x(g);x.run(0,59,1000);check(x.snapshot().registers==want && x.snapshot().ccr==value,"LDC all lanes replaces complete CCR and preserves registers");}
 }
 for(unsigned invalid:{0x0210u,0x0310u,0x0220u,0x0230u,0x0320u,0x0330u}){Host h;h.program({uint16_t(invalid)});Cpu c(h);bool threw=false;try{c.run(0,9,100);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed(),"EXR/MAC encodings stay outside H8H profile");}
 for(unsigned seed=0;seed<1000;++seed){Host a,b;std::vector<uint16_t> p{uint16_t(0x0700|(seed&255)),0x0455,0x05aa,0x060f,0x0209,0x0309,0x40f2};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,400,10000);uint64_t start=0;unsigned random=seed+1;while(start<400){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>400)n=int(400-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.trace==b.trace,"CCR full/partial partition equality");}
 std::printf("CCR: 196608 immediate cases, all byte lanes, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
