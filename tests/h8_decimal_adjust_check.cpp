#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
static unsigned bcd(unsigned n){return 16*(n/10)+n%10;}
static void one(bool subtract,unsigned left,unsigned right,unsigned reg,unsigned unrelated){
 unsigned a=bcd(left),b=bcd(right);int binary=subtract?int(a)-int(b):int(a+b);unsigned raw=unsigned(binary)&255;
 unsigned initial=unrelated&0xd0u; // C/H come from preceding binary operation.
 if(subtract){if(a<b)initial|=1;if((a%16)<(b%16))initial|=32;}else{if(a+b>255)initial|=1;if(a%16+b%16>15)initial|=32;}
 int decimal=subtract?int(left)-int(right):int(left+right);unsigned wanted=bcd(unsigned((decimal+100)%100));
 unsigned correction=(wanted+256-raw)%256,wide=raw+correction;
 unsigned flags=initial&0xd0u;if(wanted==0)flags|=4;if(wanted&128)flags|=8;if(raw%16+correction%16>15)flags|=32;if(wide>255)flags|=1;
 if((~(raw^correction)&(raw^wanted)&128)!=0)flags|=2;
 Host h;std::array<uint16_t,16> expected{};std::vector<uint16_t> code;
 for(unsigned i=0;i<16;++i)expected[i]=uint16_t(0xa500+i*0x321);
 auto& lane=expected[reg&7];lane=uint16_t(reg&8?((lane&0xff00)|raw):((lane&255)|(raw<<8)));
 for(unsigned i=0;i<8;++i)code.insert(code.end(),{uint16_t(0x7a00|i),expected[i+8],expected[i]});
 code.insert(code.end(),{uint16_t(0x0700|initial),uint16_t((subtract?0x1f00:0x0f00)|reg),0x40fe});h.program(code);
 lane=uint16_t(reg&8?((lane&0xff00)|wanted):((lane&255)|(wanted<<8)));Cpu c(h);c.run(0,160,1000);
 check(c.snapshot().registers==expected,"Decimal arithmetic oracle and whole register image");check(c.snapshot().ccr==flags,"Correction-add flags and preserved I/UI/unused bit");check(h.data.empty(),"No data bus access");
}
int main(){
 for(bool sub:{false,true})for(unsigned a=0;a<100;++a)for(unsigned b=0;b<100;++b)for(unsigned f:{0u,0xd0u})one(sub,a,b,8,f);
 for(bool sub:{false,true})for(unsigned r=0;r<16;++r)for(unsigned a:{0u,1u,9u,10u,49u,50u,90u,99u})for(unsigned b:{0u,1u,9u,10u,49u,50u,90u,99u})one(sub,a,b,r,0xd0);
 for(unsigned split=0;split<=240;++split){Host a,b;const std::vector<uint16_t> p{0xf899,0x0700,0x0f08,0xf800,0x0701,0x1f08,0xf8ff,0x0721,0x0f08,0x1f08,0x40fe};a.program(p);b.program(p);Cpu x(a),y(b);x.run(0,240,1000);y.run(0,int(split),1000);y.run(split,int(240-split),1000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace,"241 split snapshots/traces;adjust must not repeat on prefetch suspension");}
 std::printf("Decimal adjust:40000 valid decimal pairs/CCR cases,2048 register boundaries,241 splits:%u failures\n",failures);return failures?1:0;
}
