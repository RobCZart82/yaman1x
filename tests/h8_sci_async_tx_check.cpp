#include "h8_sci_async_tx.h"
#include <cstdio>
#include <utility>
#include <vector>

using Tx=yamanix::H8SciAsyncTx;
struct Sink {
    uint64_t now=0;
    std::vector<std::pair<uint64_t,bool>> data, clocks;
    unsigned tx_irqs=0, end_irqs=0;
    void tx(bool bit) { data.emplace_back(now,bit); }
    void clock(bool bit) { clocks.emplace_back(now,bit); }
    void tx_irq() { ++tx_irqs; }
    void end_irq() { ++end_irqs; }
};

int main() {
    unsigned failures=0;
    auto check=[&](bool ok,const char* why) {
        if(!ok && ++failures<10) std::fprintf(stderr,"%s\n",why);
    };
    auto drain=[&](Tx& tx,Sink& sink) {
        unsigned budget=4096;
        while(tx.next_event() && budget) {
            --budget; sink.now=tx.next_event(); tx.service_one(sink.now,sink);
        }
        check(!tx.next_event(),"Bounded drain completes");
    };
    auto prepare=[](Tx& tx,Sink& sink,uint8_t smr=0,uint8_t scr=0xa4) {
        tx.write_smr(smr); tx.write_brr(0); tx.write_scr(scr,sink);
        sink.tx_irqs=sink.end_irqs=0;
    };
    // Every byte and every 7/8-bit, PE/OE, one/two-stop-bit combination.
    for(unsigned format=0;format<16;++format)
    for(unsigned byte=0;byte<256;++byte) {
        Tx tx; Sink sink;
        const uint8_t smr=uint8_t(((format&1)?0x40:0)|((format&2)?0x20:0)|
                                 ((format&4)?0x10:0)|((format&8)?0x08:0));
        prepare(tx,sink,smr);
        tx.write_tdr(uint8_t(byte),false,0,sink);
        check(tx.next_event()==0,"CPU TDR write alone does not start");
        tx.read_ssr(); tx.write_ssr(0,0,sink);
        check(tx.read_ssr(false)==0x80,"Start raises TDRE, leaves TEND clear");
        drain(tx,sink);
        std::vector<bool> expected={false};
        unsigned parity=(smr&0x10)?0:1;
        for(unsigned bit=0;bit<((smr&0x40)?7u:8u);++bit) {
            expected.push_back((byte>>bit)&1); parity^=(byte>>bit)&1;
        }
        if(smr&0x20) expected.push_back(parity!=0); // Preserve upstream OE convention.
        expected.push_back(true); if(smr&8) expected.push_back(true);
        expected.push_back(true); // Last tick returns line to idle after full stop.
        check(sink.data.size()==expected.size(),"Frame length");
        for(unsigned i=0;i<expected.size() && i<sink.data.size();++i)
            check(sink.data[i]==std::make_pair(uint64_t(2+32*i),bool(expected[i])),"Bit and exact cycle boundary");
        check(sink.tx_irqs==1 && sink.end_irqs==1,"TXI at load and TEI at completion");
        check(tx.state()==Tx::State::idle && tx.read_ssr(false)==0x84,"Completed state");
    }
    {
        Tx tx; Sink sink; prepare(tx,sink);
        tx.write_tdr(0x35,false,0,sink); tx.write_ssr(0,0,sink);
        check(!tx.next_event(),"Unobserved TDRE cannot start transmission");
        tx.read_ssr(false); tx.write_ssr(0,0,sink);
        check(!tx.next_event(),"Debugger read cannot acknowledge TDRE");
        tx.read_ssr(); tx.write_ssr(0,0,sink);
        tx.write_tdr(0xca,false,0,sink); tx.read_ssr(); tx.write_ssr(0,0,sink);
        check(!(tx.read_ssr(false)&0x80),"Second byte queued by acknowledgement");
        // TE dropping does not abort an active frame or suppress upstream's stop-state queue load.
        tx.write_scr(0x84,sink); drain(tx,sink);
        check(sink.tx_irqs==2 && sink.end_irqs==1,"Two buffered frames, one completion");
        check(sink.data.size()==21,"No idle gap inserted between queued frames");
        for(unsigned bit=0;bit<8;++bit) {
            check(sink.data[1+bit].second==bool((0x35>>bit)&1),"First byte latched before TDR overwrite");
            check(sink.data[11+bit].second==bool((0xca>>bit)&1),"Queued byte data");
        }
    }
    {
        Tx tx; Sink sink; prepare(tx,sink,0,0xa5);
        tx.write_tdr(0,true,0,sink); drain(tx,sink);
        check(!sink.clocks.empty() && sink.clocks.front()==std::make_pair(uint64_t(2),false),"Clock output falling edge at start");
        check(sink.clocks[1]==std::make_pair(uint64_t(18),true),"Clock output half-bit rising edge");
        check(sink.clocks.back().second,"Clock idles high");
        tx.reset(sink); check(tx.read_ssr(false)==0x84 && tx.tdr()==0xff && !tx.next_event(),"Reset cancels event and restores registers");
        check(sink.data.back().second && sink.clocks.back().second,"Reset lines high");
    }
    {
        Tx tx; Sink sink;
        tx.write_tdr(1,true,0,sink); check(!tx.next_event(),"DMA start requires TE");
        prepare(tx,sink); tx.write_tdr(1,true,0,sink);
        const auto event=tx.next_event();
        for(auto invalid: {uint8_t(0x80),uint8_t(4)}) {
            bool rejected=false;
            try {tx.write_smr(invalid);} catch(const std::invalid_argument&) {rejected=true;}
            check(rejected && tx.smr()==0 && tx.next_event()==event,"Unsupported framing rejected without mutation");
        }
        bool rejected=false;
        try {tx.write_scr(0xa6,sink);} catch(const std::invalid_argument&) {rejected=true;}
        check(rejected && tx.scr()==0xa4 && tx.next_event()==event,"External clock rejected without mutation");
        tx.service_one(event-1,sink); check(sink.data.empty(),"No early edge");
        drain(tx,sink);
    }
    {
        Tx tx; Sink sink;
        tx.write_scr(0xa4,sink);
        check(sink.tx_irqs==1 && sink.end_irqs==1,"Enabling IRQs publishes existing empty/end flags");
        tx.write_scr(0xa4,sink);
        check(sink.tx_irqs==1 && sink.end_irqs==1,"Unchanged enables do not repeat IRQs");
        tx.write_brr(0); tx.write_tdr(0x55,true,0,sink);
        unsigned budget=256;
        while(tx.state()!=Tx::State::last_tick && tx.next_event() && budget) {
            --budget; sink.now=tx.next_event(); tx.service_one(sink.now,sink);
        }
        check(tx.state()==Tx::State::last_tick,"Full stop precedes completion");
        tx.write_tdr(0xaa,true,sink.now,sink);
        // last_tick is a full bit interval (16 clock events), not one edge.
        for(unsigned edge=0;edge<16;++edge) {
            sink.now=tx.next_event(); tx.service_one(sink.now,sink);
            if(edge<15) check(tx.state()==Tx::State::last_tick,"Late queue waits full stop interval");
        }
        check(tx.state()==Tx::State::start && !(tx.read_ssr(false)&4),"Last-tick queued restart leaves TEND clear");
        check(sink.end_irqs==2 && sink.tx_irqs==3,"Late restart preserves upstream TEI then TXI");
        drain(tx,sink);
        check(tx.read_ssr(false)==0x84,"Late restart completes");
    }
    {
        auto render=[&](unsigned block) {
            Tx tx; Sink sink; prepare(tx,sink); tx.write_tdr(0x9a,true,0,sink);
            for(uint64_t horizon=block;horizon<1000;horizon+=block)
                while(tx.next_event() && tx.next_event()<=horizon) {
                    sink.now=tx.next_event(); tx.service_one(sink.now,sink);
                }
            return sink.data;
        };
        check(render(1)==render(37) && render(37)==render(211),"Block partition independent event stepping");
        Tx a,b; Sink sa,sb; prepare(a,sa); prepare(b,sb);
        a.write_tdr(0x12,true,0,sa); b.write_tdr(0x34,true,0,sb);
        a.reset(sa); drain(b,sb);
        check(a.state()==Tx::State::idle && b.state()==Tx::State::idle && sb.data.size()==11,"Instance reset isolation");
    }
    std::printf("4096 asynchronous frame formats/bytes and owner sequences: %u failures\n",failures);
    return failures?1:0;
}
