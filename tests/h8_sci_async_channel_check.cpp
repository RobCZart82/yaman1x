#include "h8_sci_async_channel.h"
#include <cstdio>
#include <limits>
#include <utility>
#include <vector>

using Channel=yamanix::H8SciAsyncChannel;
struct Sink {
    uint64_t now=0;
    unsigned tx_count=0,end_count=0,rx_count=0,error_count=0;
    std::vector<std::pair<uint64_t,char>> events;
    std::vector<std::pair<uint64_t,bool>> data;
    void tx(bool b) { events.emplace_back(now,b?'1':'0'); data.emplace_back(now,b); }
    void clock(bool b) { events.emplace_back(now,b?'h':'l'); }
    void tx_irq() { ++tx_count; events.emplace_back(now,'t'); }
    void end_irq() { ++end_count; events.emplace_back(now,'E'); }
    void rx_irq() { ++rx_count; events.emplace_back(now,'R'); }
    void error_irq() { ++error_count; events.emplace_back(now,'e'); }
};

int main() {
    unsigned failures=0;
    auto check=[&](bool ok,const char* why) {
        if(!ok && ++failures<10) std::fprintf(stderr,"%s\n",why);
    };
    auto prepare=[](Channel& c,Sink& sink,uint8_t smr=0,uint8_t scr=0xf5) {
        c.write_smr(smr); c.write_brr(0); c.write_scr(scr,sink);
        sink=Sink{};
    };
    auto before=[&](Channel& c,Sink& sink,uint64_t time) {
        unsigned budget=4096;
        while(c.next_event() && c.next_event()<time && budget) {
            --budget; sink.now=c.next_event(); c.service_one(sink.now,sink);
        }
        check(budget!=0,"Bounded shared scheduler");
    };
    auto drain=[&](Channel& c,Sink& sink) { before(c,sink,std::numeric_limits<uint64_t>::max()); };
    auto wire=[&](Channel& c,Sink& sink,unsigned byte,uint64_t start) {
        std::vector<bool> bits={false}; unsigned parity=(c.smr()&0x10)?0:1;
        for(unsigned bit=0;bit<((c.smr()&0x40)?7u:8u);++bit) {
            bits.push_back((byte>>bit)&1); parity^=(byte>>bit)&1;
        }
        if(c.smr()&0x20) bits.push_back(parity!=0);
        bits.push_back(true);
        for(unsigned bit=0;bit<bits.size();++bit) {
            const auto time=start+32*bit;
            before(c,sink,time); sink.now=time; c.write_rx(bits[bit],time);
        }
        drain(c,sink);
    };
    for(unsigned format=0;format<16;++format)
    for(unsigned byte=0;byte<256;++byte) {
        Channel c; Sink sink;
        const uint8_t smr=uint8_t(((format&1)?0x40:0)|((format&2)?0x20:0)|
                                 ((format&4)?0x10:0)|((format&8)?8:0));
        prepare(c,sink,smr); c.write_tdr(uint8_t(byte),true,0,sink);
        wire(c,sink,byte^0xa5,17);
        check(c.tx_state()==Channel::TxState::idle && c.rx_state()==Channel::RxState::idle,"Both sides finish");
        check(c.read_rdr()==((byte^0xa5)&((smr&0x40)?0x7f:0xff)),"Independent incoming byte decoded during TX");
        check(c.read_ssr(false)==0xc4,"Shared SSR holds TDRE/TEND/RDRF together");
        check(sink.tx_count==1 && sink.end_count==1 && sink.rx_count==1 && sink.error_count==0,"Duplex IRQ counts");
        std::vector<bool> expected={false};unsigned parity=(smr&0x10)?0:1;
        for(unsigned bit=0;bit<((smr&0x40)?7u:8u);++bit) {
            expected.push_back((byte>>bit)&1); parity^=(byte>>bit)&1;
        }
        if(smr&0x20) expected.push_back(parity!=0);
        expected.push_back(true);if(smr&8) expected.push_back(true);expected.push_back(true);
        check(sink.data.size()==expected.size(),"TX frame length during RX");
        for(unsigned i=0;i<expected.size() && i<sink.data.size();++i)
            check(sink.data[i]==std::make_pair(uint64_t(2+32*i),bool(expected[i])),"TX timing unaffected by RX");
        c.read_rdr(true); check(c.read_ssr(false)==0x84,"DMA RDR clears only receive-ready flag");
    }
    {
        Channel c;Sink sink;prepare(c,sink);
        c.write_tdr(0xa5,true,0,sink);
        before(c,sink,17);sink.now=17;c.write_rx(false,17);
        // Keep data zero; change stop level before the phase-15 resync window.
        before(c,sink,304);sink.now=304;c.write_rx(true,304);drain(c,sink);
        std::vector<char> simultaneous;
        for(auto event:sink.events) if(event.first==322) simultaneous.push_back(event.second);
        check(simultaneous==std::vector<char>{'1','E','l','R','h'},"Equal-time TX, TEI, output clock, RXI, idle-clock order");
        check(c.read_rdr()==0 && c.read_ssr()==0xc4,"Simultaneous completion shares status");
        c.write_tdr(0x34,false,sink.now,sink);c.write_ssr(0,sink.now,sink);
        check(c.read_ssr(false)==0x80 && c.tx_state()==Channel::TxState::start,"One SSR acknowledgement clears RX flag and starts TX");
    }
    {
        Channel c;Sink sink;prepare(c,sink);c.write_brr(1);
        c.write_tdr(0x12,true,0,sink);before(c,sink,6);
        c.write_brr(0);c.write_rx(false,7);
        check(c.next_event()==8,"Second side joins existing deadline");
        sink.now=8;c.service_one(8,sink);
        check(c.next_event()==12 && c.tx_phase()==1 && c.rx_phase()==0,"RX joins existing step despite BRR change");
        c.write_scr(0xe5,sink);
        check(c.rx_state()==Channel::RxState::idle && c.next_event()==12,"RE disable stops RX without stopping TX clock");
        drain(c,sink);check(c.read_ssr(false)==0x84,"TX completes after RX abort");
    }
    {
        auto render=[&](unsigned block) {
            Channel c;Sink sink;prepare(c,sink);c.write_tdr(0xa5,true,0,sink);
            const bool bits[]={false,false,true,false,true,true,false,false,true,true};unsigned index=0;
            for(uint64_t horizon=block;horizon<700;horizon+=block) {
                unsigned budget=4096;
                while(budget) {
                    --budget;
                    const auto line=index<10?uint64_t(index*32):std::numeric_limits<uint64_t>::max();
                    const auto edge=c.next_event()?c.next_event():std::numeric_limits<uint64_t>::max();
                    const auto now=line<edge?line:edge;if(now>horizon)break;
                    sink.now=now;if(line<=edge)c.write_rx(bits[index++],now);else c.service_one(now,sink);
                }
                check(budget!=0,"Bounded duplex partition scheduler");
            }
            check(c.read_rdr()==0x9a && c.read_ssr(false)==0xc4,"Partitioned duplex result");
            return sink.events;
        };
        check(render(1)==render(37) && render(37)==render(211),"Full duplex independent of host block partitions");
    }
    {
        auto isolated=[&] {
            Channel c;Sink sink;prepare(c,sink);c.write_tdr(0x9a,true,0,sink);wire(c,sink,0x6d,17);return sink.events;
        };
        Channel a,b;Sink sa,sb;prepare(a,sa);prepare(b,sb);
        a.write_tdr(0x55,true,0,sa);a.write_rx(false,0);b.write_tdr(0x9a,true,0,sb);
        before(a,sa,3);before(b,sb,3);const auto event=b.next_event();a.reset(sa);
        check(b.next_event()==event && b.scr()==0xf5,"Reset other SCI channel preserves deadline/registers");
        wire(b,sb,0x6d,17);
        check(sb.events==isolated() && b.read_rdr()==0x6d && !a.next_event(),"Interleaved channel/reset isolation");
    }
    std::printf("4096 full-duplex byte/formats plus shared clock/status/order/isolation sequences: %u failures\n",failures);
    return failures?1:0;
}
