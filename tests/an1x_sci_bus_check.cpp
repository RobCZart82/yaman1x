#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <cstdio>
#include <functional>
struct Sink{
 std::vector<std::tuple<unsigned,uint64_t,bool>> wire,clocks;
 std::vector<std::pair<unsigned,uint64_t>> requests;std::vector<int> exceptions;
 std::function<void()> on_tx;
 void tx(unsigned c,uint64_t t,bool v){wire.emplace_back(c,t,v);if(on_tx)on_tx();}
 void clock(unsigned c,uint64_t t,bool v){clocks.emplace_back(c,t,v);}
 bool dma_request(unsigned v,uint64_t t){requests.emplace_back(v,t);return false;}
 void exception_hook(int v){exceptions.push_back(v);}
};
using Bus=yamanix::An1xSciBus<Sink>;using Cpu=yamanix::H8FirstInstructions<Bus>;
static unsigned failures;
static void check(bool ok,const char* why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
template<class F>void rejects(F f,const char* why){bool bad=false;try{f();}catch(const std::exception&){bad=true;}check(bad,why);}
static void words(std::vector<uint8_t>& rom,unsigned address,const std::vector<uint16_t>& p){for(unsigned i=0;i<p.size();++i){rom[address+2*i]=uint8_t(p[i]>>8);rom[address+2*i+1]=uint8_t(p[i]);}}
static std::vector<uint8_t> image(unsigned ch,uint8_t byte,bool rx){
 std::vector<uint8_t> r(0x100000);r[2]=1;const uint16_t base=uint16_t(ch?0xffb8:0xffb0);
 if(!rx)words(r,0x100,{0x7a00,0xff,uint16_t(base+1),0xfa00,0x688a,0x0b00,0xfa20,0x688a,0x0b00,uint16_t(0xfa00|byte),0x688a,0x0b00,0x680b,0xeb7f,0x688b,0x40fe});
 else{
  unsigned vector=53+4*ch;r[4*vector+2]=1;r[4*vector+3]=0x80;
  words(r,0x100,{0x7a07,0xff,0xfefe,0x7a00,0xff,uint16_t(base+1),0xfa00,0x688a,0x0b00,0xfa50,0x688a,0x0700,0x0180,0x40fe});
  words(r,0x180,{0x7a00,0xff,uint16_t(base+4),0x7a01,0x20,0x400,0x680a,0x0b00,0x680b,0x689b,0x1b00,0xeabf,0x688a,0x5670});
 }
 return r;
}
static std::vector<Bus::RxEvent> edges(unsigned ch,uint8_t byte){std::vector<Bus::RxEvent> r;for(unsigned bit=0;bit<10;++bit)r.push_back({200+32*bit,ch,bit==0?false:bit==9?true:((byte>>(bit-1))&1)!=0});return r;}
static void tx_ok(Bus& b,Sink& s,unsigned ch,unsigned byte){check(s.wire.size()==11,"SCI TX start/data/stop/final idle");if(s.wire.size()==11)for(unsigned bit=0;bit<11;++bit){auto[c,t,v]=s.wire[bit];check(c==ch&&v==(bit==0?false:bit>=9?true:((byte>>(bit-1))&1)!=0),"Independent mapped TX wire bits");if(bit)check(t==std::get<1>(s.wire[0])+32*bit,"Mapped TX32-cycle bit spacing");}check((b.peek_sci(ch?0xffffbc:0xffffb4)&0x84)==0x84,"TX completed TDRE/TEND");}
static void rx_ok(Bus& b,Sink& s,Cpu& c,unsigned ch,unsigned byte){check(b.read_byte(0x200400)==byte&&s.exceptions==std::vector<int>{int(53+4*ch)},"CPU-configured RX -> interrupt wakes SLEEP -> ISR stores NVRAM");check(b.consumed_input()==10&&!(b.peek_sci(ch?0xffffbc:0xffffb4)&0x40)&&b.irq().current().level==-1&&c.snapshot().registers[7]==0xfefe&&c.snapshot().registers[15]==0xff,"RX acknowledged,IRQ clear,RTE restores internal-RAM stack");check(s.requests.size()==1&&s.requests[0].first==53+4*ch,"RX DMA request forwarded before normal IRQ");}
int main(){
 for(unsigned ch=0;ch<2;++ch)for(unsigned byte=0;byte<256;++byte){
  {auto rom=image(ch,uint8_t(byte),false);Sink s;Bus b(rom,{},s);Cpu c(b,0,&b.irq());c.run(0,800,20000);tx_ok(b,s,ch,byte);}
  {auto rom=image(ch,0,true);auto e=edges(ch,uint8_t(byte));Sink s;Bus b(rom,e,s);e[0].channel=99;Cpu c(b,0,&b.irq());c.run(0,800,20000);rx_ok(b,s,c,ch,byte);}
 }
 for(bool rx:{false,true})for(unsigned ch=0;ch<2;++ch)for(unsigned cut=0;cut<=800;++cut){auto rom=image(ch,0xa5,rx);auto e=rx?edges(ch,0xa5):std::vector<Bus::RxEvent>{};Sink sa,sb;Bus a(rom,e,sa),b(rom,e,sb);Cpu x(a,0,&a.irq()),y(b,0,&b.irq());x.run(0,800,20000);y.run(0,int(cut),20000);y.run(cut,int(800-cut),20000);check(x.snapshot()==y.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.wire==sb.wire&&sa.clocks==sb.clocks&&sa.requests==sb.requests&&sa.exceptions==sb.exceptions&&a.consumed_input()==b.consumed_input()&&a.peek_sci(ch?0xffffbc:0xffffb4)==b.peek_sci(ch?0xffffbc:0xffffb4),"3204 every-cycle mapped CPU/SCI histories including bus debt");}
 {auto rom=image(0,0,false);Sink sa,sb;Bus a(rom,{},sa),b(rom,{},sb);a.write_word(0xabffffb0,0x0000);check(a.read_word(0xffffb0)==0&&b.read_word(0xffffb0)==0xff,"SCI words,big-endian lanes,24-bit alias and instance isolation");a.write_byte(0x200000,0x5a);check(b.read_byte(0x200000)==0,"RAM isolated with shared immutable ROM input");auto persistent=a.nvram_image();b.load_nvram(persistent);check(b.read_byte(0x200000)==0x5a&&b.read_word(0xffffb0)==0xff,"Fresh-owner NVRAM restore leaves SCI cold state intact");rejects([&]{a.write_word(0xffffb1,0);},"Unaligned SCI word rejected");check(a.failed(),"Bus access failure poisons owner");rejects([&]{a.update(0);},"Failed bus cannot resume");}
 {auto rom=image(0,0,false);Sink s;std::vector<Bus::RxEvent> e{{0,0,false},{0,0,true}};Bus b(rom,e,s,1);rejects([&]{b.update(0);},"Bounded peripheral event work enforced");check(b.failed(),"Event budget exhaustion fail-stop");}
 {auto rom=image(0,0,false);Sink s;std::vector<Bus::RxEvent> bad{{2,0,false},{1,0,true}};rejects([&]{Bus b(rom,bad,s);},"Unsorted RX rejected");bad={{0,2,false}};rejects([&]{Bus b(rom,bad,s);},"Invalid RX channel rejected");}
 for(bool swallow:{false,true}){auto rom=image(0,0,false);Sink s;Bus b(rom,{},s);s.on_tx=[&]{if(swallow){try{b.read_byte(0x200000);}catch(const std::logic_error&) {}}else b.read_byte(0x200000);};rejects([&]{b.write_byte(0xffffb1,0);b.write_byte(0xffffb2,0x20);b.write_byte(0xffffb3,0xa5);auto status=b.read_byte(0xffffb4);b.write_byte(0xffffb4,uint8_t(status&0x7f));b.update(800);},"Sink callback reentry rejected");check(b.failed(),"Callback reentry poisons bus");}
 {auto rom=image(0,0,false);Sink s;Bus b(rom,{},s);rejects([&]{b.read_byte(0x440000);},"Unimplemented DSP MMIO still fails closed");check(b.failed(),"Unknown device failure retained");}
 std::printf("AN1x SCI bus:512 TX+512 RX frames,3204 partitions,word/alias/isolation/budget/reentry/failure controls:%u failures\n",failures);return failures?1:0;
}
