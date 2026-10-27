#include "cycle_dispatcher.h"
#include <cstdio>
#include <limits>
#include <utility>
#include <vector>
using yamanix::CycleDispatcher;
static unsigned failures=0;
static void check(bool ok,const char* why) {
    if(!ok) { ++failures; std::fprintf(stderr,"%s\n",why); }
}
template<class Error,class F> static void throws(F f,const char* why) {
    try { f(); check(false,why); } catch(const Error&) {} catch(...) { check(false,why); }
}
struct Queue {
    std::vector<uint64_t> deadlines, seen;
    std::size_t index=0;
    std::optional<uint64_t> next_event() const {
        if(index==deadlines.size()) return std::nullopt;
        return deadlines[index];
    }
    void service(uint64_t cycle) { seen.push_back(cycle); ++index; }
};
int main() {
    { CycleDispatcher d; Queue q{{0,2,2,8},{}};
      auto r=d.advance_to(2,2,q);
      check(!r.complete && r.events==2 && r.cycle==2 && q.index==2,"Budget stops before tied event");
      r=d.advance_to(2,1,q);
      check(r.complete && r.events==1 && q.seen==std::vector<uint64_t>({0,2,2}),"Inclusive tied deadline resume");
      r=d.advance_to(7,0,q); check(r.complete && r.cycle==7 && r.events==0,"Idle advance needs no budget");
      r=d.advance_to(8,0,q); check(!r.complete && r.cycle==7,"Zero budget preserves pending event and time");
      r=d.advance_to(8,1,q); check(r.complete && r.cycle==8,"Exact horizon event serviced");
      throws<std::invalid_argument>([&]{d.advance_to(7,1,q);},"Reject backwards horizon");
      check(!d.failed(),"Caller horizon error is recoverable");
    }
    // Same owner event stream for whole, partitioned and budget-limited runs.
    for(unsigned seed=0;seed<1000;++seed) {
        Queue a,b,c; uint64_t time=0;
        for(unsigned n=0;n<80;++n) { time+=(seed+n*17)%5; a.deadlines.push_back(time); }
        b.deadlines=c.deadlines=a.deadlines;
        CycleDispatcher x,y,z;
        x.advance_to(time+5,100,a);
        for(uint64_t h=0;h<=time+5;++h) y.advance_to(h,100,b);
        unsigned resumes=0;
        while(!z.advance_to(time+5,3,c).complete && ++resumes<100) {}
        check(a.seen==b.seen && a.seen==c.seen && x.cycle()==y.cycle() && x.cycle()==z.cycle(),"Partition/budget invariance");
    }
    { struct Stuck { unsigned calls=0; std::optional<uint64_t> next_event(){return 0;} void service(uint64_t){++calls;} } q;
      CycleDispatcher d; auto r=d.advance_to(10,7,q);
      check(!r.complete && r.events==7 && q.calls==7 && d.cycle()==0,"No-progress owner is bounded"); }
    { CycleDispatcher d(5); Queue q{{4},{}};
      throws<std::logic_error>([&]{d.advance_to(6,1,q);},"Reject stale deadline");
      check(d.failed() && q.seen.empty(),"Stale deadline poisons dispatcher without dispatch");
      throws<std::logic_error>([&]{d.advance_to(6,1,q);},"Failed dispatcher cannot resume"); }
    { struct Broken { std::optional<uint64_t> next_event(){throw std::runtime_error("next");} void service(uint64_t){} } q;
      CycleDispatcher d; throws<std::runtime_error>([&]{d.advance_to(1,1,q);},"Propagate next-event failure"); check(d.failed(),"Next callback failure poisons"); }
    { struct Broken { std::optional<uint64_t> next_event(){return 1;} void service(uint64_t){throw std::runtime_error("service");} } q;
      CycleDispatcher d; throws<std::runtime_error>([&]{d.advance_to(2,1,q);},"Propagate service failure"); check(d.failed() && d.cycle()==1,"Service failure records partial time"); }
    { struct Reentrant {
          CycleDispatcher& d; std::optional<uint64_t> next_event(){return 0;}
          void service(uint64_t){d.advance_to(1,1,*this);}
      }; CycleDispatcher d; Reentrant q{d};
      throws<std::logic_error>([&]{d.advance_to(1,1,q);},"Reject reentry"); check(d.failed(),"Propagated reentry poisons outer operation"); }
    { CycleDispatcher a,b; Queue qa{{2},{}},qb{{3},{}}; a.advance_to(5,1,qa);
      check(b.cycle()==0 && qb.index==0,"Machine isolation"); b.advance_to(3,1,qb);
      check(a.cycle()==5 && qa.index==1,"Reverse machine isolation"); }
    { struct Inserted {
          unsigned index=0; std::vector<uint64_t> seen;
          std::optional<uint64_t> next_event() const {
              if(index==3) return std::nullopt;
              return index==2?4:2;
          }
          void service(uint64_t t){ seen.push_back(t); ++index; }
      } q; CycleDispatcher d;
      const auto r=d.advance_to(4,3,q);
      check(r.complete && q.seen==std::vector<uint64_t>({2,2,4}),"Callback-generated tied and future events are re-queried"); }
    { struct Retrograde {
          bool dispatched=false;
          std::optional<uint64_t> next_event() const {return dispatched?1:2;}
          void service(uint64_t){dispatched=true;}
      } q; CycleDispatcher d;
      throws<std::logic_error>([&]{d.advance_to(3,1,q);},"Reject deadline regression after callback even at exhausted budget");
      check(d.failed() && d.cycle()==2,"Post-callback failure retains partial time"); }
    { const auto max=std::numeric_limits<uint64_t>::max(); CycleDispatcher d(max-1); Queue q{{max},{}};
      auto r=d.advance_to(max,1,q); check(r.complete && r.cycle==max && q.seen.size()==1,"Maximum cycle without overflow or sentinel collision"); }
    std::printf("Cycle dispatcher: 1000 partition/budget streams and boundary/failure checks: %u failures\n",failures);
    return failures?1:0;
}
