#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <cstdio>
struct Sink {std::vector<int> exceptions;void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}void exception_hook(int v){exceptions.push_back(v);}};
using Bus=yamanix::An1xSciBus<Sink>;
static unsigned failures;static void check(bool ok,const char*s){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",s);}
template<class F>void rejects(F f,const char*s){bool bad=false;try{f();}catch(const std::exception&){bad=true;}check(bad,s);}
static std::vector<uint8_t> image(){std::vector<uint8_t>r(0x100000);r[2]=1;return r;}
static int first(unsigned bits){for(unsigned line=0;line<8;++line)if(bits&(1u<<line))return 12+int(line);return 0;}
static void words(std::vector<uint8_t>&r,unsigned at,const std::vector<uint16_t>&p){for(unsigned i=0;i<p.size();++i){r[at+2*i]=uint8_t(p[i]>>8);r[at+2*i+1]=uint8_t(p[i]);}}
struct PinBus:Bus {
 bool raised=false;unsigned line;static constexpr uint64_t trigger=100;
 PinBus(std::span<const uint8_t> r,Sink&s,unsigned n):Bus(r,{},s),line(n){}
 void pin(uint64_t t){if(!raised&&t>=trigger){raised=true;irq().irq_input(line,true);}}
 uint64_t next_bus_event()const {auto next=Bus::next_bus_event();return !raised&&(!next||trigger<next)?trigger:next;}
 uint64_t update(uint64_t t){Bus::update(t);pin(cycle());return next_bus_event();}
 void bus_cycle(uint64_t t){Bus::bus_cycle(t);pin(cycle());}
};
int main(){
 for(unsigned pins=0;pins<256;++pins){auto r=image();Sink s;Bus b(r,{},s);b.irq().set_filter(0,-1);for(unsigned n=0;n<8;++n)b.irq().irq_input(n,(pins&(1u<<n))!=0);
  check(b.read_byte(0xfffff6)==pins,"Mapped level ISR latches all physical inputs");
  for(unsigned enable=0;enable<256;++enable){b.write_byte(0xabfffff5,uint8_t(enable));auto selected=b.irq().current();check(b.read_byte(0xfffff5)==enable&&selected.vector==first(pins&enable)&&selected.level==((pins&enable)?0:-1),"65536 independent IER/ISR/vector selections and alias");b.write_byte(0xfffff6,0);check(b.read_byte(0xfffff6)==pins,"Asserted level relatches on mapped ISR clear");}
  for(unsigned n=0;n<8;++n)b.irq().irq_input(n,false);
  unsigned latched=pins;for(unsigned value=255;;--value){b.write_byte(0xfffff6,uint8_t(value));latched&=value;check(b.read_byte(0xfffff6)==latched,"65536 ISR write-zero clear values with deasserted pins");if(!value)break;}
 }
 {auto r=image();Sink s;Bus b(r,{},s);for(unsigned value=0;value<65536;++value){b.write_word(0xabfffff4,uint16_t(value));check(b.read_word(0xfffff4)==value,"65536 big-endian ISCR/IER word readback");check(b.irq().iscr()==value/256&&b.irq().ier()==(value&255),"ISCR high and IER low are distinct registers");b.write_word(0xfffff8,uint16_t(value));check(b.read_word(0xfffff8)==uint16_t(value&255),"65536 original ICR noncomplemented mask preserves high-before-low write quirks");}}
 for(unsigned line=0;line<8;++line)for(unsigned mode=0;mode<256;++mode){auto r=image();Sink s;Bus b(r,{},s);b.irq().set_filter(0,-1);auto bit=uint8_t(1u<<line);b.write_byte(0xfffff4,uint8_t(mode));b.write_byte(0xfffff5,bit);b.irq().irq_input(line,true);check(b.read_byte(0xfffff6)==bit&&b.irq().current().vector==int(12+line),"2048 line/ISCR modes publish enabled input");b.write_byte(0xfffff6,0);check(b.read_byte(0xfffff6)==((mode&bit)?0:bit),"Edge clear stays clear while level relatches");b.irq().irq_input(line,false);b.irq().irq_input(line,true);check(b.read_byte(0xfffff6)==bit,"New edge or level latches again");b.irq().taken(12+line);check(b.read_byte(0xfffff6)==((mode&bit)?0:bit),"Mapped ISR reflects controller acknowledgement");}
 {auto r=image();Sink s;Bus b(r,{},s);b.irq().set_filter(1,-1);b.write_byte(0xfffff5,1);b.irq().irq_input(0,true);check(b.irq().current().level==-1,"Priority filter initially blocks");b.write_byte(0xfffff8,0x80);check(b.read_byte(0xfffff8)==0x80&&b.irq().current().level==-1,"ICR write defers IRQ publication");b.write_byte(0xfffff5,1);check(b.irq().current().vector==12&&b.irq().current().level==1,"IER write publishes changed ICR priority");b.write_byte(0xfffff9,0x12);check(b.read_byte(0xfffff8)==0&&b.read_byte(0xfffff9)==0x12,"ICR offset1 clears offset0 per pinned source");b.write_byte(0xfffff9,1);check(b.read_byte(0xfffff9)==0x13,"Same-byte ICR writes OR per pinned source");}
 for(bool read:{false,true})for(uint32_t a:{0xfffff0u,0xfffff3u,0xfffff7u,0xfffffau}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{if(read)b.read_byte(a);else b.write_byte(a,0xff);},"SYSCR and IRQ holes remain unsupported");check(b.failed()&&b.irq().ier()==0&&b.irq().iscr()==0,"Unknown byte cannot change IRQ registers");}
 for(bool read:{false,true})for(uint32_t a:{0xfffff3u,0xfffff5u,0xfffff6u,0xfffff7u,0xfffff9u}){auto r=image();Sink s;Bus b(r,{},s);rejects([&]{if(read)b.read_word(a);else b.write_word(a,0xffff);},"Odd or incomplete IRQ words reject before mutation");check(b.failed()&&b.irq().iscr()==0&&b.irq().ier()==0&&b.irq().icr(0)==0&&b.irq().icr(1)==0,"IRQ pair preflight preserves complete register image");rejects([&]{b.read_byte(0xfffff5);},"Failed owner cannot resume");}
 {auto r=image();Sink sa,sb;Bus a(r,{},sa),b(r,{},sb);a.write_word(0xfffff4,0xff55);a.write_word(0xfffff8,0xa55a);check(b.read_word(0xfffff4)==0&&b.read_word(0xfffff8)==0,"IRQ register owners isolated");check(a.read_byte(0xffffd4)==0xff&&a.read_word(0xffffd0)==0xffff&&a.peek_sci(0xffffb1)==0xff,"IRQ writes preserve GPIO/SCI cold states");}
 for(unsigned line=0;line<8;++line)for(bool masked:{false,true}){auto r=image();auto bit=uint16_t(1u<<line);auto vector=12+line;r[4*vector+2]=1;r[4*vector+3]=0x80;words(r,0x100,{0x7a07,0xff,0xfefe,0x7a00,0xff,0xfff4,0x7901,uint16_t((bit<<8)|bit),0x6981,uint16_t(masked?0x0780:0x0700),0x0180,0xf85a,0x40fe});words(r,0x180,{0x7a03,0x20,0x400,0xfaa5,0x68ba,0x5670});
  for(unsigned cut=0;cut<=240;++cut){Sink sa,sb;PinBus a(r,sa,line),b(r,sb,line);yamanix::H8FirstInstructions<PinBus>x(a,0,&a.irq()),y(b,0,&b.irq());x.run(0,240,10000);y.run(0,int(cut),10000);y.run(cut,int(240-cut),10000);check(a.raised&&a.read_byte(0x200400)==(masked?0:0xa5)&&sa.exceptions==std::vector<int>(masked?0:1,int(vector)),"CPU-configured mapped edge IRQ wakes SLEEP,ISR stores NVRAM,CCR mask blocks");check(x.snapshot().registers[7]==0xfefe&&x.snapshot().registers[15]==0xff&&a.read_byte(0xfffff6)==(masked?bit:0),"RTE restores SP,edge acknowledged only if taken");check(x.snapshot()==y.snapshot()&&a.nvram_image()==b.nvram_image()&&sa.exceptions==sb.exceptions&&a.irq().current().vector==b.irq().current().vector&&a.read_word(0xfffff4)==b.read_word(0xfffff4)&&a.read_byte(0xfffff6)==b.read_byte(0xfffff6),"3856 every-cycle mapped IRQ/CPU/NVRAM/register histories");}
 }
 std::printf("IRQ MMIO:65536 enable/level +65536 clear +131072 word cases,2048 edge/level modes,3856 CPU partitions,priority/preflight/isolation controls:%u failures\n",failures);return failures?1:0;
}
