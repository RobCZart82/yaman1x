#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <cstdio>
#include <functional>
#include <tuple>
struct Sink {
 std::array<uint8_t,12> input{};std::vector<std::tuple<char,unsigned,uint64_t,uint8_t,uint8_t>> trace;
 std::function<void(unsigned)> hook;
 uint8_t gpio_read(unsigned p,uint64_t t){trace.emplace_back('R',p,t,input[p],0);if(hook)hook(p);return input[p];}
 void gpio_write(unsigned p,uint64_t t,uint8_t d,uint8_t dir){trace.emplace_back('W',p,t,d,dir);if(hook)hook(p);}
 void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}
};
using Bus=yamanix::An1xSciBus<Sink>;using Cpu=yamanix::H8FirstInstructions<Bus>;
static unsigned failures;static void check(bool b,const char*s){if(!b&&++failures<12)std::fprintf(stderr,"%s\n",s);}
template<class F>void rejects(F f,const char*s){bool bad=false;try{f();}catch(const std::exception&){bad=true;}check(bad,s);}
static std::vector<uint8_t> image(){std::vector<uint8_t> r(0x100000);r[2]=1;return r;}
static uint8_t pins(unsigned d,unsigned latch,unsigned input,unsigned mask){return uint8_t(mask|(latch&d)|(input&~d));}
int main(){
 {auto r=image();Sink s;Bus b(r,{},s);b.bus_cycle(37);
 for(unsigned value=0;value<65536;++value){b.write_word(0xabffffd0,uint16_t(value));check(b.read_word(0xffffd0)==0xffff,"All65536 DDR words read FF/FF");auto hi=value>>8,lo=value&255;check(std::get<1>(s.trace.back())==10&&std::get<2>(s.trace.back())==37,"DDR low lane port/time");b.write_word(0xffffd2,uint16_t(value^0xa55a));s.input[9]=uint8_t(value*73);s.input[10]=uint8_t(value*197);unsigned want=(unsigned(pins(hi&0x3f,(value^0xa55a)>>8,s.input[9],0xc0))<<8)|pins(lo,(value^0xa55a)&255,s.input[10],0);check(b.read_word(0xabffffd2)==want,"65536 independent paired DDR/latch/pin lanes and aliases");s.trace.clear();}
 }
 {auto r=image();Sink s;Bus b(r,{},s);b.bus_cycle(41);
 for(unsigned value=0;value<65536;++value){s.input[7]=uint8_t(value>>8);s.input[8]=uint8_t(value);auto n=s.trace.size();check(b.read_word(0xffffce)==uint16_t((value&0xff00)|0xe0|(value&0x0f)),"65536 read-only port7/port8 cold masked words");check(s.trace.size()==n+2&&std::get<1>(s.trace[n])==7&&std::get<1>(s.trace[n+1])==8&&std::get<2>(s.trace[n])==41&&std::get<2>(s.trace[n+1])==41,"High-before-low input sampling same committed time");s.trace.clear();}
 }
 {auto r=image();Sink s;Bus b(r,{},s);b.bus_cycle(53);b.write_word(0xffffd0,0x0f0f);check(s.trace==decltype(s.trace){{'W',9,53,0x30,0x0f},{'W',10,53,0xf0,0x0f}},"High-before-low DDR callbacks");s.trace.clear();b.write_word(0xffffd2,0x5aa5);check(s.trace==decltype(s.trace){{'W',9,53,0x3a,0x0f},{'W',10,53,0xf5,0x0f}},"High-before-low data callbacks");s.trace.clear();b.write_word(0xffffd2,0x5aa5);check(s.trace.empty(),"Unchanged word suppresses callbacks");}
 for(bool read:{false,true})for(uint32_t a:{0xffffc4u,0xffffc5u,0xffffccu,0xffffcdu,0xffffcfu,0xffffd1u,0xffffd3u,0xffffd4u,0xffffd5u,0xffffd6u,0xffffdau}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{if(read)b.read_word(a);else b.write_word(a,0xffff);},"Odd/incomplete word rejected before callbacks");check(b.failed()&&s.trace.empty(),"Preflight has no partial side effect");}
 {auto r=image();Sink s;Bus b(r,{},s);rejects([&]{b.write_word(0xffffce,0xffff);},"Read-only high lane rejects entire write");check(b.failed()&&s.trace.empty(),"Read-only pair no callbacks");}
 for(bool read:{false,true})for(bool swallow:{false,true}){auto r=image();Sink s;Bus b(r,{},s);s.hook=[&](unsigned p){if(p==9){if(swallow){try{b.read_byte(0x200000);}catch(const std::logic_error&) {}}else b.read_byte(0x200000);}};rejects([&]{if(read)b.read_word(0xffffd2);else b.write_word(0xffffd0,0xffff);},"High lane reentry stops low lane even when inner error swallowed");check(b.failed()&&s.trace.size()==1&&std::get<1>(s.trace[0])==9,"No low callback after high failure");rejects([&]{b.read_word(0xffffd0);},"Failed bus cannot resume");}
 for(bool read:{false,true})for(unsigned bad:{9u,10u}){auto r=image();Sink s;Bus b(r,{},s);s.hook=[&](unsigned p){if(p==bad)throw std::runtime_error("pin fault");};rejects([&]{if(read)b.read_word(0xffffd2);else b.write_word(0xffffd0,0xffff);},"Lane callback failure retained");check(b.failed()&&s.trace.size()==(bad==9?1u:2u),"Earlier callbacks remain committed,no rollback");}
 struct NoGpio {void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}};
 for(bool read:{false,true}){auto r=image();NoGpio s;yamanix::An1xSciBus<NoGpio> b(r,{},s);check(b.read_word(0xffffd0)==0xffff,"DDR read needs no pin sink");rejects([&]{if(read)b.read_word(0xffffd2);else b.write_word(0xffffd0,0);},"Missing GPIO sink rejects word");check(b.failed(),"Missing sink poisons owner");}
 // Word CPU path: configure paired DDR,write latches,read pins,store NVRAM.
 auto rom=image();std::vector<uint16_t> p{0x0781,0x7a00,0xff,0xffd0,0x7901,0x0f0f,0x6981,0x0b80,0x7901,0x5aa5,0x6981,0x6902,0x7a03,0x20,0x400,0x69b2,0x40fe};for(unsigned i=0;i<p.size();++i){rom[0x100+2*i]=uint8_t(p[i]>>8);rom[0x101+2*i]=uint8_t(p[i]);}
 for(unsigned cut=0;cut<=240;++cut){Sink sa,sb;sa.input[9]=sb.input[9]=0x15;sa.input[10]=sb.input[10]=0x62;Bus a(rom,{},sa),b(rom,{},sb);Cpu x(a),y(b);x.run(0,240,10000);y.run(0,int(cut),10000);y.run(cut,int(240-cut),10000);check(a.read_word(0x200400)==0xda65&&x.snapshot().registers[2]==0xda65&&x.snapshot().ccr==0x89,"CPU word big-endian value/register/CCR oracle");check(x.snapshot()==y.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.trace==sb.trace,"241 every-cycle word CPU/data/timestamp histories");}
 std::printf("GPIO words:196608 lane cases,241 CPU partitions,ordering/preflight/reentry/fault controls:%u failures\n",failures);return failures?1:0;
}
