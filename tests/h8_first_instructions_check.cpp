#include "h8_first_instructions.h"
#include <cstdio>
#include <vector>
#include <utility>
#include <stdexcept>
using namespace yamanix;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
struct Host {
    std::array<uint16_t,512> words{};
    std::vector<std::pair<uint32_t,uint16_t>> reads;
    uint64_t event=11;
    Host(){words[1]=0x100;words[0x80]=0xf012;words[0x81]=0xf834;words[0x82]=0x40fa;}
    uint16_t read_instruction_word(uint32_t address){
        if((address&1) || address/2>=words.size())throw std::out_of_range("Synthetic instruction bus");
        auto value=words[address/2];reads.emplace_back(address,value);return value;
    }
    uint64_t update(uint64_t cycle){while(event<=cycle)event+=11;return event;}
};
using Cpu=H8FirstInstructions<Host>;
template<class E,class F>void throws(F f){try{f();check(false,"Missing exception");}catch(const E&){}catch(...){check(false,"Wrong exception");}}
int main(){
    {Host host;Cpu cpu(host);cpu.run(0,6,100);
     auto s=cpu.snapshot();check(host.reads==std::vector<std::pair<uint32_t,uint16_t>>{{0,0},{2,0x100},{0x100,0xf012}},"Reset vector and initial prefetch path");
     check(s.pc==0x100 && s.ccr==0x80 && s.exr==0x7f && s.substate==8,"Reset suspended at post-prefetch boundary");
     cpu.run(6,2,100);s=cpu.snapshot();check(s.registers[0]==0x1200 && s.ccr==0x80 && s.substate==2,"First real immediate move executes before boundary");
     cpu.run(8,2,100);s=cpu.snapshot();check(s.registers[0]==0x1234,"Low byte preserves high byte");
     cpu.run(10,4,100);s=cpu.snapshot();check(host.reads.back().first==0x100,"Backward BRA fetches target");}
    for(unsigned displacement=0;displacement<256;++displacement){
     Host host;host.words.fill(0);host.words[1]=0x100;host.words[0x80]=uint16_t(0x4000|displacement);
     Cpu cpu(host);cpu.run(0,10,100);const uint32_t target=uint32_t(0x102+int(int8_t(displacement)));
     check(host.reads.back().first==(target&~uint32_t{1}),"256 signed BRA offsets and aligned target reads");
     cpu.run(10,1,100);check(cpu.snapshot().pc==target+2,"Branch prefetch switch preserves raw PC increment");}
    for(unsigned reg=0;reg<16;++reg)for(unsigned value=0;value<256;++value){
     Host host;host.event=0x100000;host.words[0x80]=uint16_t(0xf000|(reg<<8)|value);Cpu cpu(host);cpu.run(0,8,100);auto s=cpu.snapshot();
     const auto expected=reg&8?value:value<<8;
     check(s.registers[reg&7]==expected,"4096 register/immediate combinations");
     for(unsigned r=0;r<16;++r)if(r!=(reg&7))check(s.registers[r]==0,"Unaffected register isolation");}
    for(unsigned seed=0;seed<256;++seed)for(unsigned value=0;value<256;++value){
     Host host;host.event=0x100000;host.words[0x80]=uint16_t(0xf000|value);Cpu cpu(host,uint8_t(seed));cpu.run(0,8,100);
     const auto expected=((seed|0x80)&~0x0e)|(value==0?4:(value&0x80?8:0));
     check(cpu.snapshot().ccr==expected,"65536 initial-CCR/value combinations preserve unrelated flags");}
    {Host host;Cpu cpu(host);cpu.run(0,0,100);check(host.reads.empty(),"Zero slice never fetches");cpu.run(0,1,100);
     const auto s=cpu.snapshot();check(s.debt==1 && host.reads.size()==1,"Odd slice retains bus overshoot");
     cpu.run(1,1,100);check(cpu.snapshot().debt==0 && host.reads.size()==1,"Next slice pays debt without repeated fetch");}
    for(unsigned seed=0;seed<1000;++seed){
     Host a,b;Cpu x(a),y(b);x.run(0,512,10000);uint64_t start=0;unsigned random=seed+1;
     while(start<512){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}
     check(x.snapshot()==y.snapshot() && a.reads==b.reads,"1000 split histories preserve real instruction state and bus trace");}
    {Host a,b;Cpu x(a),y(b);x.run(0,20,1000);check(b.reads.empty() && y.snapshot().state==0x10000,"Machine state isolation");}
    {Host host;host.words[0x80]=0x1234;Cpu cpu(host);
     throws<std::runtime_error>([&]{cpu.run(0,20,1000);});check(cpu.failed(),"Unsupported opcode explicitly poisons instance");}
    {Host host;host.words[1]=0x8000;Cpu cpu(host);
     throws<std::out_of_range>([&]{cpu.run(0,20,1000);});check(cpu.failed(),"Bus failure poisons instance");}
    std::printf("First generated H8 handlers: 4096 register/value and 65536 flag cases; 1000 partition histories; %u failures\n",failures);
    return failures?1:0;
}
