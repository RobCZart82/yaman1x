#include "h8h_bus_access.h"
#include <array>
#include <cstdio>
#include <limits>
#include <functional>
#include <tuple>
#include <vector>
using u8=uint8_t; using u16=uint16_t; using u32=uint32_t;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
template<class E,class F>void throws(F f,const char* why){try{f();check(false,why);}catch(const E&){}catch(...){check(false,why);}}
struct Bus {
    std::array<u8,65536> memory{};
    std::vector<std::tuple<char,u32,u16,int>> trace;
    int* budget=nullptr;
    bool fault=false;
    Bus(){for(unsigned i=0;i<memory.size();++i)memory[i]=u8(i^(i>>8));}
    void record(char kind,u32 a,u16 value){trace.emplace_back(kind,a,value,budget?*budget:0);if(fault)throw std::runtime_error("bus fault");}
    u16 word(u32 a)const{return u16((u16(memory[a&65535])<<8)|memory[(a+1)&65535]);}
    u8 read_byte(u32 a){const auto v=memory[a&65535];record('b',a,v);return v;}
    u16 read_word(u32 a){const auto v=word(a);record('w',a,v);return v;}
    u16 read_instruction_word(u32 a){const auto v=u16(word(a)^0xa55a);record('i',a,v);return v;}
    void write_byte(u32 a,u8 v){record('B',a,v);memory[a&65535]=v;}
    void write_word(u32 a,u16 v){record('W',a,v);memory[a&65535]=u8(v>>8);memory[(a+1)&65535]=u8(v);}
};
#ifdef YAMANIX_H8_BUS_REFERENCE_HEADER
#include YAMANIX_H8_BUS_REFERENCE_HEADER
#endif
int main(){
    { int cycles=1;Bus b;b.budget=&cycles;yamanix::H8hBusAccess<Bus> cpu(b,cycles);
      cpu.write16(0x123457,0xabcd);check(cycles==-1 && b.memory[0x3456]==0xab && b.memory[0x3457]==0xcd,"Word write aligns down and stores big endian");
      check(cpu.read8(0x123457)==0xcd,"Byte address unchanged");
      check(cpu.read16(0x123457)==0xabcd,"Word read aligns down");
      check(cpu.read16i(0x123457)==u16(0xabcd^0xa55a),"Instruction bus differs from data bus");
      check(b.trace==std::vector<std::tuple<char,u32,u16,int>>({{'W',0x123456,0xabcd,-1},{'b',0x123457,0xcd,-3},{'w',0x123456,0xabcd,-5},{'i',0x123456,u16(0xabcd^0xa55a),-7}}),"Exact callback order and pre-access cycle charge");
      cpu.write8(0xffffffff,0x5a);check(std::get<1>(b.trace.back())==0xffffffff,"Address truncation belongs to host bus"); }
    { int cycles=std::numeric_limits<int>::min()+2;Bus b;yamanix::H8hBusAccess<Bus> cpu(b,cycles);
      cpu.read8(0);check(cycles==std::numeric_limits<int>::min(),"Last representable charge");
      throws<std::overflow_error>([&]{cpu.write8(0,1);},"Reject underflow before callback");
      check(b.trace.size()==1 && !cpu.failed(),"Underflow has no bus effect"); }
    { int cycles=4;Bus b;b.fault=true;yamanix::H8hBusAccess<Bus> cpu(b,cycles);
      throws<std::runtime_error>([&]{cpu.read16(1);},"Bus exception propagates");
      check(cycles==2 && cpu.failed(),"Failed callback keeps charged cycles and poisons owner");
      b.fault=false;throws<std::logic_error>([&]{cpu.read8(0);},"Failed owner cannot continue");check(cycles==2 && b.trace.size()==1,"No callbacks after failure"); }
    { int a=2,b=8;Bus x,y;yamanix::H8hBusAccess<Bus> p(x,a),q(y,b);p.write8(1,7);
      check(b==8 && y.memory[1]==1 && y.trace.empty(),"Bus and cycle isolation");q.read8(2);check(a==0 && x.trace.size()==1,"Reverse isolation"); }
    { int cycles=0;Bus b;yamanix::H8hBusAccess<Bus> cpu(b,cycles);
      for(unsigned v=0;v<65536;++v){
        cpu.write16(0x100001,u16(v));
        check(cpu.read16(0x100001)==v && cpu.read16i(0x100001)==u16(v^0xa55a),"All word values across odd-address fetch/data lanes");
        check(b.memory[0]==u8(v>>8) && b.memory[1]==u8(v),"All word values preserve big-endian byte order");
        b.trace.clear();
      }
      check(cycles==-6*65536,"Exhaustive access-cycle accounting"); }
    { struct ReentrantBus:Bus {
          std::function<void()> nested;
          u8 read_byte(u32){nested();return 0;}
      } bus; int cycles=10;yamanix::H8hBusAccess<ReentrantBus> cpu(bus,cycles);
      bus.nested=[&]{cpu.read8(1);};
      throws<std::logic_error>([&]{cpu.read8(0);},"Reject callback reentry");
      check(cycles==8 && cpu.failed(),"Reentry propagates without a second cycle charge"); }
#ifdef YAMANIX_H8_BUS_REFERENCE_HEADER
    { int cycles=0;Bus actual,reference;actual.budget=&cycles;
      Upstream ref(reference);reference.budget=&ref.m_icount;
      yamanix::H8hBusAccess<Bus> candidate(actual,cycles);
      uint32_t random=0x19283746;
      for(unsigned n=0;n<200000;++n){
        random=random*1664525u+1013904223u;const u32 a=random;
        if(n%101==0)cycles=ref.m_icount=int(n%17);
        u16 x=0,y=0;const u16 data=u16(random>>7);
        switch(n%5){
        case 0:x=candidate.read8(a);y=ref.read8(a);break;
        case 1:x=candidate.read16(a);y=ref.read16(a);break;
        case 2:x=candidate.read16i(a);y=ref.read16i(a);break;
        case 3:candidate.write8(a,u8(data));ref.write8(a,u8(data));break;
        case 4:candidate.write16(a,data);ref.write16(a,data);break;
        }
        check(x==y && cycles==ref.m_icount && actual.trace==reference.trace,"Pinned upstream operation equivalence");
        actual.trace.clear();reference.trace.clear();
      }
      check(actual.memory==reference.memory,"Pinned upstream final memory equality");
      std::printf("Pinned upstream comparison: 200000 operations\n"); }
#endif
    std::printf("H8H bus access: %u failures\n",failures);return failures?1:0;
}
