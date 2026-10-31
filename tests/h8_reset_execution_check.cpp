#include "h8_first_instructions.h"
#include <cstdio>
#include <tuple>
#include <vector>
using namespace yamanix;
static unsigned failures=0;
static void check(bool ok,const char* message){if(!ok && ++failures<8)std::fprintf(stderr,"%s\n",message);}
struct Host {
    using Event=std::tuple<char,uint64_t,uint32_t,unsigned>;
    std::vector<Event> events;
    // For filter events this records the preceding bus completion,not a new event timestamp.
    uint64_t now=0;
    static constexpr uint32_t target=0x123400;
    void bus_cycle(uint64_t cycle){now=cycle;}
    uint64_t update(uint64_t){return 0;}
    uint8_t syscr_value()const{return 9;}
    void sync_cpu_ccr(uint8_t ccr){events.emplace_back('F',now,0,ccr);}
    uint16_t read_instruction_word(uint32_t address){
        if(address!=0 && address!=2 && address!=target && address!=target+2)
            throw std::runtime_error("Unexpected reset fetch");
        const uint16_t value=address==0?0x12:address==2?0x3400:0; // NOP after vector
        events.emplace_back('R',now,address,value);return value;
    }
    uint8_t read_byte(uint32_t){throw std::runtime_error("Unexpected data read");}
    uint16_t read_word(uint32_t){throw std::runtime_error("Unexpected data read");}
    void write_byte(uint32_t,uint8_t){throw std::runtime_error("Unexpected data write");}
    void write_word(uint32_t,uint16_t){throw std::runtime_error("Unexpected data write");}
};
int main(){
    // Independent literal bus sequence; no expected values taken from CPU snapshots.
    for(uint64_t origin:{uint64_t{0},uint64_t{1000},uint64_t{1}<<40})
    for(unsigned flags=0;flags<256;++flags) {
        const auto ccr=uint8_t(flags|0x80);
        const std::vector<Host::Event> expected={{'R',origin+2,0,0x12},
            {'R',origin+4,2,0x3400},{'F',origin+4,0,ccr},
            {'R',origin+6,Host::target,0}};
        Host whole;H8FirstInstructions<Host> cpu(whole,uint8_t(flags));
        cpu.run(origin,6,100);const auto reference=cpu.snapshot();
        check(whole.events==expected,"Vector words, filter and first prefetch order/time");
        check(reference.ccr==ccr && reference.exr==0x7f,"Reset preserves all CCR bits except setting I;EXR7F");
        check(reference.pc==Host::target && reference.npc==Host::target && reference.substate==8,
              "Reset suspends after first prefetch before PC increment");
        for(unsigned cut=0;cut<=6;++cut) {
            Host split;H8FirstInstructions<Host> part(split,uint8_t(flags));
            part.run(origin,int(cut),100);
            check(part.snapshot().debt==(cut%2),"Odd reset slice carries exactly one completed bus cycle");
            const auto reads_before=split.events.size();
            part.run(origin+cut,6-int(cut),100);
            check(part.snapshot()==reference && split.events==expected,"Every reset cut resumes without repeated bus/filter effects");
            if(cut==6)check(split.events.size()==reads_before,"Zero continuation adds no effect");
            part.run(origin+6,1,100);
            check(part.snapshot().debt==1 && part.snapshot().pc==Host::target+2,
                  "Reset continuation advances PC once and next NOP incurs debt");
            check(split.events.size()==5 && split.events.back()==Host::Event{'R',origin+8,Host::target+2,0},
                  "Next instruction fetch continues absolute time without duplicated reset");
        }
    }
    std::printf("Reset execution:256 CCR seeds,3 time origins,7 cuts;%u failures\n",failures);
    return failures?1:0;
}
