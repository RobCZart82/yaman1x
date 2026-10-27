#include "h8_sci_async_rx.h"
#include <cstdio>
#include <limits>
#include <utility>
#include <vector>

using Rx=yamanix::H8SciAsyncRx;
struct Sink {
    uint64_t now=0;
    unsigned received=0, errors=0;
    std::vector<std::pair<uint64_t,char>> events;
    void rx_irq() { ++received; events.emplace_back(now,'r'); }
    void error_irq() { ++errors; events.emplace_back(now,'e'); }
    void clock(bool high) { events.emplace_back(now,high?'H':'L'); }
};

int main() {
    unsigned failures=0;
    auto check=[&](bool ok,const char* why) {
        if(!ok && ++failures<10) std::fprintf(stderr,"%s\n",why);
    };
    auto advance=[&](Rx& rx,Sink& sink,uint64_t before) {
        unsigned budget=8192;
        while(rx.next_event() && rx.next_event()<before && budget) {
            --budget; sink.now=rx.next_event(); rx.service_one(sink.now,sink);
        }
        check(budget!=0,"Bounded receive stepping");
    };
    auto prepare=[](Rx& rx,Sink& sink,uint8_t smr=0,uint8_t scr=0x50) {
        rx.write_smr(smr); rx.write_brr(0); rx.write_scr(scr,sink);
    };
    auto frame=[&](Rx& rx,Sink& sink,unsigned byte,uint64_t start,bool wrong_parity=false,bool bad_stop=false) {
        if(start>=2) { advance(rx,sink,start-2); sink.now=start-2; rx.write_rx(true,sink.now); }
        std::vector<bool> bits={false};
        unsigned parity=(rx.smr()&0x10)?0:1;
        for(unsigned bit=0;bit<((rx.smr()&0x40)?7u:8u);++bit) {
            const bool value=(byte>>bit)&1; bits.push_back(value); parity^=unsigned(value);
        }
        if(rx.smr()&0x20) bits.push_back(bool(parity)^wrong_parity);
        bits.push_back(!bad_stop); // RX checks one stop even with SMR STOP=2.
        for(unsigned i=0;i<bits.size();++i) {
            const auto cycle=start+32*i;
            advance(rx,sink,cycle); sink.now=cycle; rx.write_rx(bits[i],cycle);
        }
        const auto end=start+32*bits.size(); advance(rx,sink,end);
        return end;
    };
    for(unsigned format=0;format<16;++format)
    for(unsigned byte=0;byte<256;++byte) {
        Rx rx; Sink sink;
        const uint8_t smr=uint8_t(((format&1)?0x40:0)|((format&2)?0x20:0)|
                                 ((format&4)?0x10:0)|((format&8)?0x08:0));
        prepare(rx,sink,smr); const auto end=frame(rx,sink,byte,0);
        check(rx.state()==Rx::State::idle && !rx.next_event(),"Frame completes and stops clock");
        check(rx.read_rdr()==(byte&((smr&0x40)?0x7f:0xff)),"Every byte/format decoded");
        check(rx.read_ssr(false)==0xc4,"RDRF set without errors");
        check(sink.received==1 && sink.errors==0,"One RXI for good frame");
        check(sink.events.size()==1 && sink.events.front()==std::make_pair(end-14,'r'),"RXI at first stop-bit center");
    }
    {
        Rx rx; Sink sink; prepare(rx,sink);
        rx.write_rx(false,0); advance(rx,sink,5);
        check(rx.phase()==1,"Phase reaches resynchronization window");
        rx.write_rx(true,5); check(rx.phase()==0,"Input transition resynchronizes phase one");
        advance(rx,sink,100);
        check(rx.state()==Rx::State::idle && !rx.next_event() && sink.received==0 && sink.errors==0,"False start discarded");
        rx.write_rx(false,100); advance(rx,sink,133);
        check(rx.phase()==15,"Phase fifteen window");
        rx.write_rx(true,133); check(rx.phase()==0,"Input transition resynchronizes phase fifteen");
    }
    {
        Rx rx; Sink sink; prepare(rx,sink,0,0x10);
        auto end=frame(rx,sink,0x12,0); frame(rx,sink,0x34,end+32);
        check(rx.read_rdr()==0x12 && rx.read_ssr(false)==0xe4,"Overrun preserves unread first byte");
        check(sink.events.empty(),"RIE disabled suppresses requests");
        rx.write_scr(0x50,sink);
        check(sink.received==1 && sink.errors==1 && sink.events[0].second=='r' && sink.events[1].second=='e',"RIE enable publishes RDRF then error");
        rx.write_scr(0x50,sink); check(sink.events.size()==2,"Unchanged enable does not repeat");
        rx.read_rdr(true,false); check(rx.read_ssr(false)==0xe4,"Debugger DMA read inert");
        rx.read_rdr(true); check(rx.read_ssr(false)==0xa4,"DMA clears RDRF but retains error");
        rx.write_ssr(0); check(rx.read_ssr(false)==0xa4,"Unobserved error cannot clear");
        rx.read_ssr(); rx.write_ssr(0); check(rx.read_ssr(false)==0x84,"Observed errors clear");
        frame(rx,sink,0x56,end+400); check(rx.read_rdr()==0x56 && rx.read_ssr(false)==0xc4,"Reception recovers after acknowledgement");
    }
    {
        Rx rx; Sink sink; prepare(rx,sink,0x20);
        auto end=frame(rx,sink,0x31,0,true);
        check(rx.read_ssr(false)==0x8c && rx.read_rdr()==0 && sink.errors==1,"Parity error rejects new byte");
        frame(rx,sink,0x32,end+32);
        check(rx.read_rdr()==0x32 && rx.read_ssr(false)==0xcc && sink.errors==2,"Sticky PER still allows good byte, requests ERI");
        rx.read_ssr(); rx.write_ssr(0);
        end=frame(rx,sink,0x33,end+500,true,true);
        check(rx.read_ssr(false)==0x94 && rx.read_rdr()==0x32,"Framing error takes precedence over new parity error");
        frame(rx,sink,0x34,end+32);
        check(rx.read_rdr()==0x32 && rx.read_ssr(false)==0x94,"Sticky FER blocks RDR update");
    }
    {
        Rx rx; Sink sink; prepare(rx,sink);
        rx.write_rx(false,0); advance(rx,sink,30); rx.write_scr(0x40,sink);
        check(rx.state()==Rx::State::idle && !rx.next_event(),"Disabling RE aborts frame");
        rx.reset(sink); check(!rx.input() && rx.read_rdr()==0 && rx.read_ssr(false)==0x84,"Reset retains input and clears data");
        prepare(rx,sink); rx.write_rx(false,100); check(!rx.next_event(),"Low input at enable is not a falling edge");
        rx.write_rx(true,101); rx.write_rx(false,102); check(rx.next_event()==104,"New falling edge starts receive");
        const auto event=rx.next_event();
        for(auto invalid: {uint8_t(0x80),uint8_t(4)}) {
            bool rejected=false;try {rx.write_smr(invalid);} catch(const std::invalid_argument&){rejected=true;}
            check(rejected && rx.smr()==0 && rx.next_event()==event,"Unsupported framing rejected atomically");
        }
        for(auto invalid: {uint8_t(0x52),uint8_t(0x70),uint8_t(0xd0),uint8_t(0x54)}) {
            bool rejected=false;try {rx.write_scr(invalid,sink);} catch(const std::invalid_argument&){rejected=true;}
            check(rejected && rx.scr()==0x50 && rx.next_event()==event,"External clock/TX controls rejected atomically");
        }
    }
    {
        auto render=[&](unsigned block) {
            Rx rx; Sink sink; prepare(rx,sink); unsigned line=0;
            const bool bits[]={false,false,true,false,true,true,false,false,true,true}; // 0x9a
            for(uint64_t horizon=block;horizon<600;horizon+=block) {
                unsigned budget=8192;
                while(budget) {
                    --budget;
                    const auto wire=line<10?uint64_t(line*32):std::numeric_limits<uint64_t>::max();
                    const auto edge=rx.next_event()?rx.next_event():std::numeric_limits<uint64_t>::max();
                    const auto now=wire<edge?wire:edge;
                    if(now>horizon) break;
                    sink.now=now;
                    if(wire<=edge) rx.write_rx(bits[line++],now);
                    else rx.service_one(now,sink);
                }
                check(budget!=0,"Bounded partition scheduler");
            }
            check(rx.read_rdr()==0x9a,"Partitioned wire stream decoded");
            return sink.events;
        };
        check(render(1)==render(37) && render(37)==render(211),"Host block partition independent");
        Rx a,b; Sink sa,sb; prepare(a,sa); prepare(b,sb);
        a.write_rx(false,0); a.reset(sa); frame(b,sb,0x57,0);
        check(a.read_rdr()==0 && b.read_rdr()==0x57,"Instance/reset isolation");
    }
    std::printf("4096 asynchronous RX byte/formats plus error/owner sequences: %u failures\n",failures);
    return failures?1:0;
}
