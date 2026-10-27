#include "h83002_sci_mmio.h"
#include <array>
#include <cstdio>
#include <limits>
#include <tuple>
#include <type_traits>
#include <vector>

using Adapter=yamanix::H83002SciMmio;
static_assert(!std::is_copy_constructible_v<Adapter>);
static_assert(!std::is_move_constructible_v<Adapter>);
struct Machine {
    yamanix::H8hIrqController irq;
    Adapter sci{irq};
    Machine() { irq.set_filter(0,-1); }
};
struct Sink {
    uint64_t now=0;
    int consume=-1;
    std::vector<unsigned> dma;
    std::vector<std::tuple<unsigned,uint64_t,bool>> tx_events;
    std::vector<std::tuple<unsigned,uint64_t,bool>> clock_events;
    void tx(unsigned channel,bool level) { tx_events.emplace_back(channel,now,level); }
    void clock(unsigned channel,bool level) { clock_events.emplace_back(channel,now,level); }
    bool dma_request(unsigned vector) { dma.push_back(vector); return int(vector)==consume; }
};
static unsigned failures=0;
static void check(bool ok,const char* why) {
    if(!ok && ++failures<12) std::fprintf(stderr,"%s\n",why);
}
static uint32_t base(unsigned channel) { return channel?0xffffb8:0xffffb0; }
static void before(Machine& m,Sink& sink,uint64_t horizon) {
    unsigned budget=4096;
    while(m.sci.next_event() && m.sci.next_event()<horizon && budget) {
        --budget; const auto edge=m.sci.next_event(); sink.now=edge;
        // Fixture convention only: channel 0 before 1 at equal deadlines.
        for(unsigned channel=0;channel<2;++channel)
            if(m.sci.next_event(channel)==edge) m.sci.service_one(channel,edge,sink);
    }
    check(budget!=0,"Bounded fixture scheduler");
}
static void drain(Machine& m,Sink& sink) {
    before(m,sink,std::numeric_limits<uint64_t>::max());
}
static void wire(Machine& m,Sink& sink,unsigned channel,uint8_t byte,uint64_t start=1,bool stop=true) {
    for(unsigned bit=0;bit<10;++bit) {
        const auto time=start+32*bit;
        before(m,sink,time); sink.now=time;
        const bool level=bit==0?false:bit==9?stop:((byte>>(bit-1))&1)!=0;
        m.sci.write_rx(channel,level,time);
    }
    drain(m,sink);
}

int main() {
    {
        Machine m;Sink sink;
        m.sci.reset(sink);
        check(sink.tx_events.size()==2 && sink.clock_events.size()==2 &&
              std::get<0>(sink.tx_events[0])==0 && std::get<0>(sink.tx_events[1])==1 &&
              std::get<2>(sink.clock_events[0]) && std::get<2>(sink.clock_events[1]),"Reset publishes idle pins for each channel");
        for(unsigned channel=0;channel<2;++channel) {
            const auto b=base(channel);
            check(m.sci.read16(b)==0x00ff && m.sci.read16(b+2)==0x00ff &&
                  m.sci.read16(b+4)==0x8400,"Both channels expose reset registers");
            m.sci.write16(b,0x4007,0xffff,0,sink);
            check(m.sci.read16(b)==0x4007,"Big-endian SMR/BRR word");
            check(m.sci.read16(b,0xff00)==0x40ff && m.sci.read16(b,0x00ff)==0xff07 &&
                  m.sci.read16(b,0)==0xffff,"Inactive read lanes are ones");
            m.sci.write16(b,0,0,0,sink);
            check(m.sci.read16(b)==0x4007,"Zero mask has no write effects");
            m.sci.write16(b+2,0x205a,0xff00,0,sink);
            check(m.sci.read8(b+2)==0x20 && m.sci.read8(b+3)==0xff,"SCR high lane excludes TDR");
            m.sci.write16(b+2,0xa56d,0x00ff,0,sink);
            check(m.sci.read16(b+2)==0x206d && !m.sci.next_event(),"TDR low lane excludes SCR and does not start CPU TX alone");
            m.sci.write8(b+5,0xaa,0,sink);
            check(m.sci.read8(b+5)==0,"RDR writes are ignored");
        }
        const auto saved=m.sci.read16(base(0));
        bool rejected=false;
        try { m.sci.write16(base(0),0xffff,0x0f00,0,sink); }
        catch(const std::invalid_argument&) { rejected=true; }
        check(rejected && m.sci.read16(base(0))==saved,"Partial byte mask rejected before mutation");
        for(auto address:{0xffffafu,0xffffb6u,0xffffb7u,0xffffbeu,0xffffbfu,0x0fffb0u}) {
            rejected=false;
            try { m.sci.read8(address); } catch(const std::out_of_range&) { rejected=true; }
            check(rejected && !Adapter::mapped(address),"Holes and unsupported aliases rejected");
        }
        rejected=false;
        try { m.sci.read16(0xffffb1); } catch(const std::out_of_range&) { rejected=true; }
        check(rejected,"Unaligned word rejected");
        rejected=false;
        try { m.sci.write_rx(2,false,0); } catch(const std::out_of_range&) { rejected=true; }
        check(rejected,"Invalid channel rejected");
        rejected=false;
        try { m.sci.write16(base(0),0x80aa,0xffff,0,sink); }
        catch(const std::invalid_argument&) { rejected=true; }
        check(rejected && m.sci.read16(base(0))==saved,"Unsupported SMR rejects whole word before BRR write");
        const auto saved_scr_tdr=m.sci.read16(base(0)+2);
        rejected=false;
        try { m.sci.write16(base(0)+2,0x2277,0xffff,0,sink); }
        catch(const std::invalid_argument&) { rejected=true; }
        check(rejected && m.sci.read16(base(0)+2)==saved_scr_tdr,"Unsupported external SCR clock rejects word before TDR write");
    }
    for(unsigned channel=0;channel<2;++channel) {
        Machine m;Sink sink;const auto b=base(channel);const unsigned eri=52+4*channel;
        m.sci.write8(b+1,0,0,sink);
        m.sci.write8(b+2,0xa4,0,sink);
        check(sink.dma==std::vector<unsigned>{eri+2,eri+3},"TXI/TEI route through DMA qualification");
        check(m.irq.current().vector==int(eri+2),"Channel-specific TXI reaches IRQ controller");
        m.irq.taken(eri+2);m.irq.taken(eri+3);sink.dma.clear();
        m.sci.write8(b+3,0xa5,0,sink);
        m.sci.read8(b+4);
        m.sci.write8(b+4,0x04,0,sink);
        check(m.sci.next_event(channel)==2 && m.irq.current().vector==int(eri+2),"CPU SSR acknowledgement starts TX and publishes TXI");
        m.irq.taken(eri+2);drain(m,sink);
        check(m.irq.current().vector==int(eri+3) && m.sci.read8(b+4,false)==0x84,"Transmit completion publishes TEI");
        const bool expected[]={false,true,false,true,false,false,true,false,true,true,true};
        check(sink.tx_events.size()==11,"One 8N1 frame plus idle completion");
        for(unsigned i=0;i<sink.tx_events.size() && i<11;++i)
            check(sink.tx_events[i]==std::make_tuple(channel,uint64_t(2+32*i),expected[i]),"Routed TX frame preserves exact cycles/levels");
    }
    for(unsigned channel=0;channel<2;++channel)
    for(unsigned byte=0;byte<256;++byte) {
        Machine m;Sink sink;const auto b=base(channel);const unsigned rxi=53+4*channel;
        m.sci.write8(b+1,0,0,sink);m.sci.write8(b+2,0x50,0,sink);
        wire(m,sink,channel,uint8_t(byte));
        check(m.irq.current().vector==int(rxi) && sink.dma==std::vector<unsigned>{rxi},"RXI vector for each byte/channel");
        check(m.sci.read8(b+5)==byte && m.sci.read8(b+4,false)==0xc4,"CPU RDR read retains ready flag");
        m.sci.read8(b+4,false);m.sci.write8(b+4,0,400,sink);
        check(m.sci.read8(b+4,false)==0xc4,"Debugger SSR read does not arm flag acknowledgement");
        m.sci.read8(b+5,false,true);
        check(m.sci.read8(b+4,false)==0xc4,"Debugger DMA RDR read retains ready flag");
        const auto combined=m.sci.read16(b+4,0xffff,true,true);
        check(combined==(0xc400|byte) && m.sci.read8(b+4,false)==0x84,"DMA word reads SSR high then clears RDRF low");
        m.irq.taken(rxi);check(m.irq.current().level==-1,"IRQ take is distinct from SSR/RDR acknowledgement");
    }
    for(unsigned channel=0;channel<2;++channel) {
        const auto b=base(channel);const unsigned eri=52+4*channel;
        Machine bad;Sink sb;bad.sci.write8(b+1,0,0,sb);bad.sci.write8(b+2,0x50,0,sb);
        wire(bad,sb,channel,0x55,1,false);
        check(bad.irq.current().vector==int(eri) && sb.dma==std::vector<unsigned>{eri},"Framing error routes ERI");
        check((bad.sci.read8(b+4,false)&0x10) && bad.sci.read8(b+5)==0,"Error preserves old RDR");
        Machine over;Sink so;over.sci.write8(b+1,0,0,so);over.sci.write8(b+2,0x50,0,so);
        wire(over,so,channel,0x12);over.irq.taken(eri+1);
        wire(over,so,channel,0x34,401);
        check(over.irq.current().vector==int(eri) && over.sci.read8(b+5)==0x12,"Overrun routes ERI without replacing unread RDR");
        Machine dma;Sink sd;sd.consume=int(eri+1);
        dma.sci.write8(b+1,0,0,sd);dma.sci.write8(b+2,0x50,0,sd);wire(dma,sd,channel,0x6d);
        check(dma.irq.current().level==-1 && dma.sci.read8(b+5)==0x6d,"DMA-consumed RXI does not latch CPU IRQ");
        Machine tx;Sink st;st.consume=int(eri+2);
        tx.sci.write8(b+1,0,0,st);tx.sci.write8(b+2,0xa0,0,st);
        tx.sci.write8(b+3,0x6d,0,st,true);
        check(tx.sci.next_event()==2 && tx.irq.current().level==-1,"Qualified DMA TDR write starts TX while consumed TXI stays unlatched");
        drain(tx,st);
        Machine held;Sink sh;
        held.sci.write8(b+2,0xa0,0,sh);sh.consume=int(eri+2);
        held.sci.write8(b+3,0x12,0,sh,true);
        check(held.irq.current().vector==int(eri+2),"DMA consumption does not clear an already-pending IRQ");
        Machine disabled;Sink off;
        disabled.sci.write8(b+3,0x12,0,off,true);
        check(!disabled.sci.next_event() && off.dma.empty() && disabled.sci.read8(b+3)==0x12,"DMA access with TE disabled stores TDR without starting TX");
        Machine clocked;Sink cs;
        clocked.sci.write8(b+1,0,0,cs);clocked.sci.write8(b+2,0x21,0,cs);
        clocked.sci.write8(b+3,0x12,0,cs,true);drain(clocked,cs);
        check(!cs.clock_events.empty() && std::get<2>(cs.clock_events.back()),"Output-clock mode ends idle high");
        for(const auto& event:cs.clock_events)
            check(std::get<0>(event)==channel,"Clock callbacks carry the owning channel");
    }
    {
        Machine a,b;Sink sa,sb;
        for(unsigned channel=0;channel<2;++channel) {
            b.sci.write8(base(channel)+1,0,0,sb);b.sci.write8(base(channel)+2,0xa0,0,sb);
            b.sci.write8(base(channel)+3,0x6d,0,sb,true);
        }
        check(b.irq.current().vector==54,"Both channels share IRQ arbitration");
        b.irq.taken(54);check(b.irq.current().vector==58,"Taking SCI0 TXI reveals SCI1 TXI");
        const auto event=b.sci.next_event(1);a.sci.reset(sa);
        check(b.sci.next_event(1)==event && b.sci.read8(0xffffba)==0xa0,"Other-machine reset preserves UART and IRQ ownership");
        b.sci.reset_channel(0,sb);
        check(!b.sci.next_event(0) && b.sci.next_event(1)==event && b.irq.current().vector==58,"SCI0 reset preserves SCI1 deadline and pending IRQ");
    }
    std::printf("H83002 SCI MMIO/IRQ: 512 RX byte/channel cases plus bus, TX, DMA, error and isolation checks: %u failures\n",failures);
    return failures?1:0;
}
