#include "sci_cycle_timeline.h"
#include <cstdio>
#include <tuple>
#include <vector>
#include <limits>
#include <algorithm>
using Timeline=yamanix::SciCycleTimeline;
using Event=Timeline::RxEvent;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12) std::fprintf(stderr,"%s\n",why);}
template<class E,class F> void throws(F f,const char* why){try{f();check(false,why);}catch(const E&){}catch(...){check(false,why);}}
struct Sink {
    std::vector<std::tuple<char,unsigned,uint64_t,bool>> trace;
    bool consume=false;
    void tx(unsigned c,uint64_t t,bool v){trace.emplace_back('T',c,t,v);}
    void clock(unsigned c,uint64_t t,bool v){trace.emplace_back('C',c,t,v);}
    bool dma_request(unsigned v,uint64_t t){trace.emplace_back('I',v,t,consume);return consume;}
};
struct InitSink {
    Sink& s;
    void tx(unsigned c,bool v){s.tx(c,0,v);}
    void clock(unsigned c,bool v){s.clock(c,0,v);}
    bool dma_request(unsigned v){return s.dma_request(v,0);}
};
struct Machine {
    yamanix::H8hIrqController irq;
    yamanix::H83002SciMmio sci{irq};
    Sink sink;
    Machine(){irq.set_filter(0,-1);}
    void init(unsigned byte){
        InitSink s{sink};
        for(unsigned c=0;c<2;++c){
            const uint32_t b=c?0xffffb8:0xffffb0;
            sci.write8(b+1,0,0,s);
            sci.write8(b+2,0xf4,0,s); // RX/TX + interrupts including TEI
            sci.write8(b+3,uint8_t(byte+c),0,s,true);
        }
    }
};
static std::vector<Event> wire(unsigned byte){
    std::vector<Event> events;
    for(unsigned bit=0;bit<10;++bit)for(unsigned c=0;c<2;++c){
        const unsigned data=c?(byte^0xff):byte;
        events.push_back({1+32*bit,c,bit==0?false:bit==9?true:bool((data>>(bit-1))&1)});
    }
    return events;
}
int main(){
    for(unsigned byte=0;byte<256;++byte){
        auto input=wire(byte);Machine a,b,c;a.init(byte);b.init(byte);c.init(byte);
        Timeline x(a.sci,input),y(b.sci,input),z(c.sci,input);
        check(x.advance_to(400,4096,a.sink).complete,"Whole run completes");
        for(uint64_t t=0;t<=400;++t)check(y.advance_to(t,4096,b.sink).complete,"Partition completes");
        unsigned calls=0;
        while(!z.advance_to(400,1,c.sink).complete && ++calls<4096){}
        check(calls<4096,"Budget resume bounded");
        check(a.sink.trace==b.sink.trace && a.sink.trace==c.sink.trace,"Exact TX/clock/IRQ traces independent of partition and budget");
        for(unsigned ch=0;ch<2;++ch){const uint32_t base=ch?0xffffb8:0xffffb0;
            check(a.sci.read8(base+5)==(ch?(byte^0xff):byte),"Both channels receive known 8N1 byte");
            check(a.sci.read16(base+4,0xffff,false)==b.sci.read16(base+4,0xffff,false) &&
                  a.sci.read16(base+4,0xffff,false)==c.sci.read16(base+4,0xffff,false),"Final register equality");
        }
        check(x.consumed_input()==input.size() && !a.sci.next_event() && x.cycle()==400,"Input drained and channel clocks idle");
    }
    { Machine m;m.init(0);auto q=wire(0);
      // At cycle 50 RX samples data bit 0. Two tied input changes must
      // both precede sampling, retaining source order (final level high).
      q.push_back({50,0,false});q.push_back({50,0,true});
      std::stable_sort(q.begin(),q.end(),[](auto a,auto b){return a.cycle<b.cycle;});
      Timeline t(m.sci,q);t.advance_to(400,4096,m.sink);
      check(m.sci.read8(0xffffb5)==1,"FIFO wire transitions precede exact-cycle RX sample");
      std::vector<unsigned> channels;
      for(auto [kind,ch,time,value]:m.sink.trace)if(kind=='T' && time==2)channels.push_back(ch);
      check(channels==std::vector<unsigned>({0,1}),"Equal-deadline SCI0 output precedes SCI1"); }
    { Machine m;m.sink.consume=true;m.init(0);auto q=wire(0x90);Timeline t(m.sci,q);
      t.advance_to(400,4096,m.sink);
      check(m.irq.current().level==-1 && m.sci.read8(0xffffb5)==0x90,"DMA-qualified timeline requests preserve RX data without CPU IRQ latch"); }
    { Machine m; std::vector<Event> q{{2,0,false},{1,1,true}};
      throws<std::invalid_argument>([&]{Timeline t(m.sci,q);},"Reject unsorted input");
      q={{0,2,false}};throws<std::invalid_argument>([&]{Timeline t(m.sci,q);},"Reject invalid channel");
      q={{1,0,false}};throws<std::invalid_argument>([&]{Timeline t(m.sci,q,2);},"Reject stale input");
      m.init(0);throws<std::invalid_argument>([&]{Timeline t(m.sci,{},3);},"Reject stale initial clock"); }
    { Machine m; std::vector<Event> q{{0,0,false},{0,1,false}};Timeline t(m.sci,q);
      auto r=t.advance_to(0,0,m.sink);check(!r.complete && !t.consumed_input(),"Zero budget retains zero-cycle input");
      r=t.advance_to(0,1,m.sink);check(!r.complete && t.consumed_input()==1,"One budget processes one tied input");
      r=t.advance_to(0,1,m.sink);check(r.complete && t.consumed_input()==2,"Tied input resumes exactly"); }
    { Machine a,b; auto q=wire(0x90);a.init(0);Timeline x(a.sci,q),y(b.sci,{});
      x.advance_to(400,4096,a.sink);check(y.cycle()==0 && b.sink.trace.empty(),"Machine isolation");
      auto r=y.advance_to(std::numeric_limits<uint64_t>::max(),0,b.sink);
      check(r.complete && r.cycle==std::numeric_limits<uint64_t>::max(),"Idle maximum horizon"); }
    { Machine m;m.init(1);Timeline t(m.sci,{});
      struct Broken {void tx(unsigned,uint64_t,bool){throw std::runtime_error("pin");}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}} s;
      throws<std::runtime_error>([&]{t.advance_to(400,4096,s);},"Propagate sink failure");check(t.failed(),"Failed sink poisons timeline");
      throws<std::logic_error>([&]{t.advance_to(400,4096,m.sink);},"Reject resume after partial sink failure"); }
    std::printf("SCI timeline: 256 dual-channel duplex streams across 3 partitions plus boundaries: %u failures\n",failures);
    return failures?1:0;
}
