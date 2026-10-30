#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <cstdio>
#include <functional>
#include <tuple>
struct Sink {
 uint8_t input=0x3c,data=0,direction=0;unsigned reads=0,writes=0;
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
 0x078f,0x7a00,0x00ff,0xffc5,0xfaf0,0x688a,0x0b00,0x0b00,0xfaa5,0x688a,
 0x680b,0x7a01,0x0020,0x0400,0x689b,0x40fe};
 for(unsigned i=0;i<p.size();++i){r[0x100+2*i]=uint8_t(p[i]>>8);r[0x101+2*i]=uint8_t(p[i]);}return r;}
int main(){
 {auto r=image();Sink s;Bus b(r,{},s);check(b.read_byte(0xffffc5)==0xff&&b.read_byte(0xffffda)==0&&s.reads==0&&s.writes==0,"DDR ff_r/PCR cold state;no invented startup callback");
 for(unsigned dir=0;dir<256;++dir){b.write_byte(0xffffc5,uint8_t(dir));for(unsigned latch=0;latch<256;++latch){b.write_byte(0xffffc7,uint8_t(latch));check(s.data==uint8_t(latch|uint8_t(~dir))&&s.direction==dir,"Output data/high impedance direction independent oracle");for(uint8_t ext:{0x00,0x55,0xaa,0xff}){s.input=ext;unsigned count=s.reads;uint8_t expected=0;for(unsigned bit=0;bit<8;++bit){auto mask=1u<<bit;if((dir&mask)?(latch&mask):(ext&mask))expected|=uint8_t(mask);}check(b.read_byte(0xabffffc7)==expected,"GPIO pin truth table at24-bit alias,not raw latch");check(s.reads==count+(dir!=255),"Input callback suppressed for all-output DDR");}check(b.read_byte(0xffffc5)==0xff,"DDR reads ff despite written direction");}}
 for(unsigned v=0;v<256;++v){auto count=s.writes;b.write_byte(0xffffda,uint8_t(v));check(b.read_byte(0xffffda)==v&&s.writes==count,"PCR storage all values,no invented electrical output");}
 auto count=s.writes;b.write_byte(0xffffc7,0xff);b.write_byte(0xffffc5,0xff);check(s.writes==count,"Unchanged output/direction notification suppressed");}
 {auto r=image();Sink s;s.record=true;Bus b(r,{},s);b.bus_cycle(37);b.write_byte(0xffffc5,0xf0);b.bus_cycle(41);b.write_byte(0xffffc7,0xa5);b.read_byte(0xffffc7);check(s.output==decltype(s.output){{4,37,0x0f,0xf0},{4,41,0xaf,0xf0}}&&s.samples==decltype(s.samples){{4,41}},"Explicit timestamps and port identifier at committed bus time");}
 for(unsigned cut=0;cut<=240;++cut){auto r=program();Sink sa,sb;sa.record=sb.record=true;Bus a(r,{},sa),b(r,{},sb);Cpu ca(a),cb(b);ca.run(0,240,10000);cb.run(0,int(cut),10000);cb.run(cut,int(240-cut),10000);check(a.read_byte(0x200400)==0xac&&sa.output.size()==2&&sa.samples.size()==1,"CPU configures DDR/latch,reads external pins and stores NVRAM");check(ca.snapshot().ccr==0x89,"MOV pin byte AC sets N,clears Z/V,retains C/I");check(ca.snapshot()==cb.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.output==sb.output&&sa.samples==sb.samples,"241 CPU value/state/output/input timestamp partitions");}
 {auto r=image();Sink sa,sb;Bus a(r,{},sa),b(r,{},sb);a.write_byte(0xffffc5,0xff);a.write_byte(0xffffc7,0xa5);a.write_byte(0xffffda,0x5a);check(b.read_byte(0xffffc7)==0x3c&&b.read_byte(0xffffda)==0&&sb.writes==0,"Port4 state/callbacks isolated between owners");}
 for(uint32_t address:{0xffffc4u,0xffffc6u,0xffffc8u,0xffffd5u,0xffffd9u,0xffffdbu,0xfffff2u,0xfffff3u}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{b.write_byte(address,0xff);},"Other ports/IRQ/SYSCR/holes remain closed");check(s.writes==0&&b.failed(),"Unknown register has no GPIO callback and poisons owner");}
 for(uint32_t address:{0xffffc4u,0xffffc5u,0xffffc6u,0xffffc7u,0xffffdau}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{b.write_word(address,0xffff);},"Mixed mapped/unmapped GPIO word not partially dispatched");check(s.writes==0&&b.failed(),"Rejected word is side-effect free at GPIO boundary");}
 for(bool read:{false,true}){auto r=image();Sink s;Bus b(r,{},s);s.on_gpio=[&]{try{b.write_byte(0x200000,1);}catch(const std::logic_error&) {}};rejects([&]{if(read)b.read_byte(0xffffc7);else b.write_byte(0xffffc5,0xff);},"GPIO callback suppressed reentry still fails outer access");check(b.failed()&&b.nvram_image()[0]==0,"Reentry cannot mutate RAM and retains poison");}
 struct NoGpio {void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}};
 for(bool read:{false,true}){auto r=image();NoGpio s;yamanix::An1xSciBus<NoGpio> b(r,{},s);rejects([&]{if(read)b.read_byte(0xffffc7);else b.write_byte(0xffffc5,0);},"Missing GPIO callbacks do not invent pin levels or discard writes");check(b.failed(),"Missing callback fails closed");}
 std::printf("AN1x GPIO4:262144 pin cases,256 PCR values,241 CPU partitions,address/timestamp/flags/isolation/reentry/word controls:%u failures\n",failures);return failures?1:0;
}
