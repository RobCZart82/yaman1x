#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
struct TrapHost:Host {
 bool fail_hook=false;std::vector<int> exceptions;
 TrapHost(unsigned trap,uint8_t ccr){words[16+trap*2]=0;words[17+trap*2]=0x180;program({0x7a07,0,0x300,uint16_t(0x0700|ccr),uint16_t(0x5700|(trap<<4)),0x40fe});words[0xc0]=0x40fe;}
 void exception_hook(int vector){exceptions.push_back(vector);if(fail_hook)throw std::runtime_error("trap observer");}
};
using Cpu=yamanix::H8FirstInstructions<TrapHost>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 for(unsigned trap=0;trap<4;++trap)for(unsigned ccr=0;ccr<256;++ccr){TrapHost h(trap,uint8_t(ccr));Cpu c(h);c.run(0,50,1000);auto s=c.snapshot();check(h.exceptions==std::vector<int>{int(8+trap)} && s.registers[7]==0x2fc && s.registers[15]==0 && s.ccr==(ccr|0x80) && s.taken_vector==int(8+trap),"TRAPA vectors, committed SP and fixed-SYSCR I masking");check(h.data[0x2fc]==ccr && h.data[0x2fd]==0 && h.data[0x2fe]==1 && h.data[0x2ff]==8,"Original trap frame stores pre-mask CCR and instruction address");}
 for(unsigned split=0;split<=80;++split){TrapHost a(2,0x55),b(2,0x55);Cpu x(a),y(b);x.run(0,80,1000);y.run(0,int(split),1000);y.run(split,int(80-split),1000);check(x.snapshot()==y.snapshot() && a.trace==b.trace && a.data==b.data && a.exceptions==b.exceptions,"TRAPA every-cycle full/partial continuation");}
 for(unsigned fault=1;fault<=2;++fault){TrapHost h(0,0x25);h.fail_word_write=fault;Cpu c(h);bool threw=false;try{c.run(0,50,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed() && h.word_writes==fault && h.exceptions.empty(),"Trap stack failure is fail-stop before vector notification");}
 {TrapHost h(1,0);h.fail_hook=true;Cpu c(h);bool threw=false;try{c.run(0,50,1000);}catch(const std::runtime_error&){threw=true;}check(threw && c.failed() && h.word_writes==2 && c.snapshot().taken_vector==9,"Observer failure preserves frame/vector and fails closed");}
 std::printf("TRAPA fixed SYSCR: 1024 vector/CCR cases, 81 split points and stack faults: %u failures\n",failures);return failures?1:0;
}
