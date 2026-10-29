#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <functional>
static unsigned failures;
static void check(bool ok,const char* why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
struct Observed:Host {
 std::function<void()> observe;
 uint8_t read_byte(uint32_t a){observe();return Host::read_byte(a);}
 void write_byte(uint32_t a,uint8_t v){observe();Host::write_byte(a,v);}
};
using Cpu=yamanix::H8FirstInstructions<Observed>;
static void one(bool store,unsigned reg,unsigned address,unsigned value,unsigned flags){
 Observed h;std::array<uint16_t,16> want{};std::vector<uint16_t> p;
 for(unsigned r=0;r<8;++r){const uint32_t v=(r+1)*0x1234567u;want[r]=uint16_t(v);want[r+8]=uint16_t(v>>16);p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)});}
 if(store){p.push_back(uint16_t(0xf000|(reg<<8)|value));auto& w=want[reg&7];w=uint16_t(reg&8?((w&0xff00)|value):((w&255)|(value<<8)));}
 const auto before=want;const uint32_t full=address<32768?address:0xffff0000u|address,physical=full&0xffffff;
 h.data[physical]=uint8_t(store?value^0xff:value);
 p.insert(p.end(),{uint16_t(0x0700|flags),uint16_t((store?0x6ac0:0x6a40)|reg),uint16_t(address)});
 h.program(p);Cpu c(h);unsigned callbacks=0;const uint8_t expected=uint8_t((flags&~14u)|(value==0?4u:(value&128?8u:0u)));
 h.observe=[&]{++callbacks;check(c.snapshot().registers==before&&c.snapshot().ccr==(store?expected:flags),"Callback sees pre-load registers/CCR and pre-store updated flags");};
 c.run(0,110,1000);
 if(!store){auto& w=want[reg&7];w=uint16_t(reg&8?((w&0xff00)|value):((w&255)|(value<<8)));}
 check(callbacks==1&&c.snapshot().registers==want&&c.snapshot().ccr==expected,"Complete registers,lane preservation,NZV and unrelated CCR");
 check(h.data.size()==1&&h.data[physical]==value,"Exact byte footprint");
 std::vector<std::tuple<char,uint32_t,uint16_t>> data;for(auto e:h.trace)if(std::get<0>(e)!='I')data.push_back(e);
 check(data==std::vector<std::tuple<char,uint32_t,uint16_t>>{{store?'W':'R',full,uint16_t(value)}},"Single byte access,sign-extended full address,no word alignment");
}
int main(){
 for(bool store:{false,true})for(unsigned reg=0;reg<16;++reg)for(unsigned value=0;value<256;++value)for(unsigned flags:{0u,0x55u,0xffu})one(store,reg,0x8001,value,flags);
 for(bool store:{false,true})for(unsigned address=0;address<65536;++address)one(store,address&15,address,(address*73)&255,address&255);
 for(bool store:{false,true})for(unsigned flags=0;flags<256;++flags)for(unsigned value:{0u,1u,0x7fu,0x80u,0xffu})one(store,9,0xffff,value,flags);
 const std::vector<uint16_t> program{0xf95a,0x6ac9,0x8001,0x6a4a,0x8001,0x40fe};
 for(unsigned cut=0;cut<=200;++cut){Host a,b;a.program(program);b.program(program);yamanix::H8FirstInstructions<Host>x(a),y(b);x.run(0,200,10000);y.run(0,int(cut),10000);y.run(cut,int(200-cut),10000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"201 every-cycle snapshots,RAM and ordered traces");}
 for(bool store:{false,true})for(unsigned reg=0;reg<16;++reg){Host h;h.program({uint16_t(0xf000|(reg<<8)|0x80),0x0755,uint16_t((store?0x6ac0:0x6a40)|reg),0x8001});h.bad_read=!store;h.bad_write=store;yamanix::H8FirstInstructions<Host> c(h);bool fail=false;try{c.run(0,80,1000);}catch(const std::runtime_error&){fail=true;}check(fail&&c.failed()&&c.snapshot().ccr==(store?0x59:0x55),"Fault CCR reflects original pre-store/post-load ordering");check((reg&8?c.snapshot().registers[reg&7]&255:c.snapshot().registers[reg&7]>>8)==0x80,"Fault preserves register data");if(store)check(h.data[0xff8001]==0x80,"Write mutation retained");bool again=false;try{c.run(80,1,1000);}catch(const std::logic_error&){again=true;}check(again,"Poisoned instance cannot resume");}
 // Unsupported neighbors in the same major opcode remain fail-closed.
 for(unsigned op:{0x6a10u,0x6a30u,0x6a50u,0x6a60u,0x6a70u,0x6a90u,0x6ab0u,0x6ad0u,0x6ae0u,0x6af0u}){Host h;h.program({uint16_t(op),0});yamanix::H8FirstInstructions<Host>c(h);bool fail=false;try{c.run(0,30,100);}catch(const std::runtime_error&){fail=true;}check(fail&&c.failed(),"Neighbor mode remains unsupported");}
 std::printf("Peripheral moves:24576 lane/value/flag cases,131072 addresses,2560 CCR boundaries,201 splits,32 faults,neighbor guards:%u failures\n",failures);return failures?1:0;
}
