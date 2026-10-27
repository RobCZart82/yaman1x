#include "h8_execution_budget.h"
#include "h8h_bus_access.h"
#include <cstdio>
#include <limits>
#include <vector>
using yamanix::H8ExecutionBudget;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<12)std::fprintf(stderr,"%s\n",why);}
template<class E,class F>void throws(F f,const char* why){try{f();check(false,why);}catch(const E&){}catch(...){check(false,why);}}
// ROM-free owner fixture: one synthetic action costs two cycles. No H8 opcode.
struct Bus {
    unsigned actions=0;
    uint16_t read_instruction_word(uint32_t){++actions;return 0;}
};
int main(){
    { H8ExecutionBudget b;b.begin_slice(100,3);b.recompute_deadline(102);b.apply_carry();
      check(b.boundary()==1 && b.full_step_allowed(),"Future event splits slice");
      b.counter()-=2;
      check(b.event_due() && b.due_cycle()==102 && !b.full_step_allowed(),"Exact peripheral boundary wins over full step");
      b.recompute_deadline(103);check(b.boundary()==0 && b.full_step_allowed(),"End-equal event deferred");
      b.counter()-=2;b.finish_slice();check(b.debt()==1 && b.remaining()==0,"Negative execution becomes next-slice debt");
      b.begin_slice(103,0);b.recompute_deadline(0);b.apply_carry();b.finish_slice();check(b.debt()==1,"Zero slice preserves debt");
      b.begin_slice(103,1);b.recompute_deadline(0);b.apply_carry();check(!b.can_execute() && b.debt()==0,"Slice pays debt exactly");b.finish_slice(); }
    { H8ExecutionBudget b;b.begin_slice(20,10);b.recompute_deadline(15);b.apply_carry();
      check(b.event_due() && b.due_cycle()==15 && b.boundary()==15,"Already-due event retained for owner catch-up");
      b.recompute_deadline(0);b.counter()=0;b.finish_slice(); }
    { H8ExecutionBudget b;
      throws<std::logic_error>([&]{b.apply_carry();},"Carry requires open slice");
      throws<std::invalid_argument>([&]{b.begin_slice(0,-1);},"Reject negative budget");
      throws<std::overflow_error>([&]{b.begin_slice(std::numeric_limits<uint64_t>::max(),1);},"Reject overflowing end");
      b.begin_slice(0,1);throws<std::logic_error>([&]{b.apply_carry();},"Initial deadline required before carry");
      throws<std::logic_error>([&]{b.begin_slice(0,1);},"No nested slice");
      b.recompute_deadline(0);b.apply_carry();throws<std::logic_error>([&]{b.finish_slice();},"No dropped remaining cycles");
      throws<std::logic_error>([&]{b.due_cycle();},"No invented due event");
      b.counter()=0;b.finish_slice();throws<std::invalid_argument>([&]{b.begin_slice(2,1);},"Slices must be contiguous"); }
    { H8ExecutionBudget b;b.begin_slice(uint64_t(std::numeric_limits<int>::max())+10,1);
      throws<std::overflow_error>([&]{b.recompute_deadline(1);},"Reject unrepresentable event distance");
      b.recompute_deadline(0);b.apply_carry();b.counter()=std::numeric_limits<int>::min();
      throws<std::overflow_error>([&]{b.finish_slice();},"Reject unrepresentable debt"); }
    { H8ExecutionBudget a,b;Bus x,y;yamanix::H8hBusAccess<Bus> ax(x,a.counter()),by(y,b.counter());
      a.begin_slice(0,1);a.recompute_deadline(0);a.apply_carry();ax.read16i(1);a.finish_slice();
      check(a.debt()==1 && b.debt()==0 && y.actions==0,"Debt and bus owner isolation");
      b.begin_slice(0,2);b.recompute_deadline(0);b.apply_carry();by.read16i(0);b.finish_slice();check(a.debt()==1 && b.debt()==0,"Reverse isolation"); }
    // Every partition pays the same final debt and emits the same bus actions.
    for(unsigned seed=0;seed<1000;++seed){
      H8ExecutionBudget b;Bus bus;yamanix::H8hBusAccess<Bus> access(bus,b.counter());
      uint64_t position=0;unsigned random=seed+1;
      while(position<1000){random=random*1664525u+1013904223u;
        const unsigned size=unsigned((1000-position)<(1+random%17)?(1000-position):(1+random%17));
        b.begin_slice(position,int(size));b.recompute_deadline(0);b.apply_carry();
        while(b.full_step_allowed())access.read16i(0);
        b.finish_slice();position+=size;
      }
      check(bus.actions==500 && b.debt()==0,"1000 split histories retain two-cycle action count and final debt");
    }
    std::printf("H8 execution budget: 1000 partition histories plus debt/deadline/error checks: %u failures\n",failures);
    return failures?1:0;
}
