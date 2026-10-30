#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <cstdio>
#include <functional>
#include <tuple>
struct Sink {
 uint8_t input=0xa5,data=0,direction=0;unsigned reads=0,writes=0;
 bool record=false;std::vector<std::tuple<unsigned,uint64_t,uint8_t,uint8_t>> output;
 std::vector<std::pair<unsigned,uint64_t>> samples;std::function<void()> on_gpio;
 uint8_t gpio_read(unsigned p,uint64_t t){++reads;if(record)samples.emplace_back(p,t);if(on_gpio)on_gpio();return input;}
 void gpio_write(unsigned p,uint64_t t,uint8_t d,uint8_t dir){++writes;data=d;direction=dir;if(record)output.emplace_back(p,t,d,dir);if(on_gpio)on_gpio();}
 void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}
 bool dma_request(unsigned,uint64_t){return false;}
};
using Bus=yamanix::An1xSciBus<Sink>;using Cpu=yamanix::H8FirstInstructions<Bus>;
static unsigned failures;
static void check(bool b,const char* s){if(!b&&++failures<12)std::fprintf(stderr,"%s\n",s);}
template<class F>void rejects(F f,const char* s){bool bad=false;try{f();}catch(const std::exception&){bad=true;}check(bad,s);}
static std::vector<uint8_t> image(){std::vector<uint8_t> r(0x100000);r[2]=1;return r;}
static std::vector<uint8_t> program(){auto r=image();std::vector<uint16_t> p{
 0x078f,0x7a00,0x00ff,0xffc9,0xfa0f,0x688a,0x0b00,0x0b00,0xfa5a,0x688a,
 0x680b,0x7a01,0x0020,0x0400,0x689b,0x40fe};
 for(unsigned i=0;i<p.size();++i){r[0x100+2*i]=uint8_t(p[i]>>8);r[0x101+2*i]=uint8_t(p[i]);}return r;}
int main(){
 {auto r=image();Sink s;Bus b(r,{},s);check(b.read_byte(0xffffc9)==0xff&&s.reads==0&&s.writes==0,"DDR FF cold state,no implicit board reset callback");check(b.read_byte(0xffffcb)==0xa5,"Cold port6 lower bits inputs,bit7 reads high");
 for(unsigned dir=0;dir<256;++dir){b.write_byte(0xffffc9,uint8_t(dir));for(unsigned latch=0;latch<256;++latch){b.write_byte(0xffffcb,uint8_t(latch));check(s.data==uint8_t((latch|uint8_t(~dir))&0x7f)&&s.direction==(dir&0x7f),"Port6 reserved pin excluded from output/direction");for(uint8_t ext:{0x00,0x55,0xaa,0xff}){s.input=ext;unsigned count=s.reads;uint8_t expected=0x80;for(unsigned bit=0;bit<7;++bit){auto mask=1u<<bit;if((dir&mask)?(latch&mask):(ext&mask))expected|=uint8_t(mask);}check(b.read_byte(0xabffffcb)==expected,"Port6 independent7-bit pin truth table plus reserved high bit");check(s.reads==count+((dir&0x7f)!=0x7f),"All lower outputs suppress input regardless reserved DDR bit");}check(b.read_byte(0xffffc9)==0xff,"DDR reads FF independent of latch/direction");}}
 b.write_byte(0xffffc9,0x7f);b.write_byte(0xffffcb,0x55);auto count=s.writes;b.write_byte(0xffffc9,0xff);b.write_byte(0xffffcb,0xd5);check(s.writes==count,"Reserved-bit-only DDR/latch changes do not notify output");}
 {auto r=image();Sink s;s.record=true;Bus b(r,{},s);b.bus_cycle(37);b.write_byte(0xffffc9,0x0f);b.bus_cycle(41);b.write_byte(0xffffcb,0x5a);b.read_byte(0xffffcb);check(s.output==decltype(s.output){{6,37,0x70,0x0f},{6,41,0x7a,0x0f}}&&s.samples==decltype(s.samples){{6,41}},"Port6 callback identity/data/direction and bus timestamps");}
 for(unsigned cut=0;cut<=240;++cut){auto r=program();Sink sa,sb;sa.record=sb.record=true;Bus a(r,{},sa),b(r,{},sb);Cpu ca(a),cb(b);ca.run(0,240,10000);cb.run(0,int(cut),10000);cb.run(cut,int(240-cut),10000);check(a.read_byte(0x200400)==0xaa&&sa.output.size()==2&&sa.samples.size()==1,"CPU DDR0F/latch5A/externalA5 yields AA in NVRAM");check(ca.snapshot().ccr==0x89,"MOV AA sets N,clears Z/V,retains C/I");check(ca.snapshot()==cb.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.output==sb.output&&sa.samples==sb.samples,"241 CPU/data/flag/input/output timestamp partitions");}
 {auto r=image();Sink sa,sb;Bus a(r,{},sa),b(r,{},sb);a.write_byte(0xffffc9,0xff);a.write_byte(0xffffcb,0);check(a.read_byte(0xffffcb)==0x80&&a.read_byte(0xffffc7)==0xa5&&b.read_byte(0xffffcb)==0xa5&&sb.writes==0,"Port6 owner and port4 state isolation");a.write_byte(0xffffc5,0xff);a.write_byte(0xffffc7,0x12);check(a.read_byte(0xffffcb)==0x80&&a.read_byte(0xffffc7)==0x12,"Port4 changes cannot alter port6");}
 for(uint32_t address:{0xffffc8u,0xffffcau,0xffffccu,0xffffd5u,0xffffdbu,0xfffff2u,0xfffff3u}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{b.write_byte(address,0xff);},"Neighbor/other port/IRQ/SYSCR remains unmapped");check(s.writes==0&&b.failed(),"Unmapped operation has no pin callback");}
 for(bool read:{false,true})for(uint32_t address:{0xffffc8u,0xffffc9u,0xffffcau,0xffffcbu}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{if(read)b.read_word(address);else b.write_word(address,0xffff);},"Mixed mapped/unmapped port6 words reject before pin side effects");check(s.writes==0&&s.reads==0&&b.failed(),"Rejected word does not dispatch neighboring port6 byte");}
 for(bool read:{false,true}){auto r=image();Sink s;Bus b(r,{},s);s.on_gpio=[&]{try{b.write_byte(0x200000,1);}catch(const std::logic_error&) {}};rejects([&]{if(read)b.read_byte(0xffffcb);else b.write_byte(0xffffc9,0xff);},"Port6 callback reentry fails even if inner error suppressed");check(b.failed()&&b.nvram_image()[0]==0,"Port6 callback cannot reenter RAM");}
 struct NoGpio {void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}};
 for(bool read:{false,true}){auto r=image();NoGpio s;yamanix::An1xSciBus<NoGpio>b(r,{},s);rejects([&]{if(read)b.read_byte(0xffffcb);else b.write_byte(0xffffc9,0);},"Missing port6 callbacks fail closed");check(b.failed(),"No invented input or discarded output");}
 std::printf("AN1x GPIO6:262144 masked pin cases,241 CPU partitions,FF DDR,callback suppression/identity/time/isolation/word/reentry controls:%u failures\n",failures);return failures?1:0;
}
