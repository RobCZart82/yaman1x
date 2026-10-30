#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <array>
#include <cstdio>
#include <functional>
struct Sink {
 std::array<uint8_t,9> pins{};std::vector<std::pair<unsigned,uint64_t>> samples;
 std::function<void()> on_read;unsigned writes=0;
 uint8_t gpio_read(unsigned p,uint64_t t){samples.emplace_back(p,t);if(on_read)on_read();return pins.at(p);}
 void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++writes;}
 void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}
 bool dma_request(unsigned,uint64_t){return false;}
};
using Bus=yamanix::An1xSciBus<Sink>;using Cpu=yamanix::H8FirstInstructions<Bus>;
static unsigned failures;
static void check(bool b,const char* s){if(!b&&++failures<12)std::fprintf(stderr,"%s\n",s);}
template<class F>void rejects(F f,const char* s){bool bad=false;try{f();}catch(const std::exception&){bad=true;}check(bad,s);}
static std::vector<uint8_t> image(bool code=false){std::vector<uint8_t> r(0x100000);r[2]=1;if(code){std::vector<uint16_t> p{0x078f,0x7a00,0x00ff,0xffce,0x680b,0x7a01,0x0020,0x0400,0x689b,0x40fe};for(unsigned i=0;i<p.size();++i){r[0x100+2*i]=uint8_t(p[i]>>8);r[0x101+2*i]=uint8_t(p[i]);}}return r;}
int main(){
 {auto r=image();Sink s;Bus b(r,{},s);check(s.samples.empty()&&s.writes==0,"No invented startup input/output");for(unsigned v=0;v<256;++v){s.pins[7]=uint8_t(v);b.bus_cycle(v*3);check(b.read_byte(0xabffffce)==v&&s.samples.back()==std::pair<unsigned,uint64_t>{7,v*3},"All256 pin bytes,24-bit alias,port7 identity and exact cycle");check(s.samples.size()==v+1&&s.writes==0,"Each read samples input once and never emits output");}}
 for(unsigned v=0;v<256;++v){auto r=image(true);Sink s;s.pins[7]=uint8_t(v);Bus b(r,{},s);Cpu cpu(b);cpu.run(0,160,10000);uint8_t ccr=uint8_t(0x81|(v==0?4:0)|(v&0x80?8:0));check(b.read_byte(0x200400)==v&&cpu.snapshot().ccr==ccr,"CPU reads every port7 value to NVRAM,independent N/Z/V/C/I oracle");check(s.samples.size()==1&&s.samples[0].first==7&&s.samples[0].second>0&&s.writes==0,"CPU input sampling is timed and input-only");}
 for(unsigned cut=0;cut<=160;++cut){auto r=image(true);Sink sa,sb;sa.pins[7]=sb.pins[7]=0x96;Bus a(r,{},sa),b(r,{},sb);Cpu ca(a),cb(b);ca.run(0,160,10000);cb.run(0,int(cut),10000);cb.run(cut,int(160-cut),10000);check(ca.snapshot()==cb.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.samples==sb.samples&&sa.writes==sb.writes,"161 CPU/value/flag/input timestamp partitions");}
 {auto r=image();Sink sa,sb;sa.pins[7]=0x36;sb.pins[7]=0x92;sa.pins[4]=0x14;sa.pins[6]=0x25;sa.pins[8]=0x0b;Bus a(r,{},sa),b(r,{},sb);check(a.read_byte(0xffffce)==0x36&&b.read_byte(0xffffce)==0x92&&a.read_byte(0xffffc7)==0x14&&a.read_byte(0xffffcb)==0xa5&&a.read_byte(0xffffcf)==0xeb,"Distinct owners and port4/6/7/8 input identities");a.write_byte(0xffffcd,0xff);a.write_byte(0xffffcf,0);check(a.read_byte(0xffffce)==0x36&&b.read_byte(0xffffce)==0x92,"Port8 drive changes cannot change port7 or other machine");}
 {auto r=image();Sink s;Bus b(r,{},s);rejects([&]{b.write_byte(0xffffce,0xff);},"Read-only port7 byte write fails closed");check(s.samples.empty()&&s.writes==0&&b.failed(),"Rejected write never samples or drives port7/port4");}
 for(bool read:{false,true})for(uint32_t a:{0xffffcdu,0xffffceu,0xffffcfu}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{if(read)b.read_word(a);else b.write_word(a,0xffff);},"Port7+port8 word remains explicitly unsupported");check(s.samples.empty()&&s.writes==0&&b.failed(),"No partial GPIO callbacks on rejected word");}
 {auto r=image();Sink s;Bus b(r,{},s);s.on_read=[&]{try{b.write_byte(0x200000,1);}catch(const std::logic_error&) {}};rejects([&]{b.read_byte(0xffffce);},"Port7 input reentry fails even when sink suppresses error");check(b.failed()&&b.nvram_image()[0]==0,"Rejected reentry leaves RAM untouched and fails owner");}
 struct NoGpio {void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}};
 {auto r=image();NoGpio s;yamanix::An1xSciBus<NoGpio>b(r,{},s);rejects([&]{b.read_byte(0xffffce);},"No invented port7 input if sink absent");check(b.failed(),"Missing input sink fails owner");}
 std::printf("AN1x GPIO7:256 direct inputs,256 CPU value/flag cases,161 partitions,input-only/identity/time/isolation/word/reentry controls:%u failures\n",failures);return failures?1:0;
}
