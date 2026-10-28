#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <limits>
#include <cstdio>
struct TimedHost:Host {
 std::vector<uint64_t> times;bool fault=false;
 void bus_cycle(uint64_t cycle){times.push_back(cycle);if(fault)throw std::runtime_error("timestamp observer");}
};
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
int main(){
 // Independent two-cycle completion oracle, including an overrun past slice end.
 {TimedHost h;int remaining=5;yamanix::H8hBusAccess<TimedHost> b(h,remaining);b.set_slice_end(100);b.read16i(0);b.read8(0);b.write8(1,2);b.read16(0);b.write16(2,3);check(h.times==std::vector<uint64_t>{97,99,101,103,105},"Absolute completion time for all five access types");}
 for(unsigned split=0;split<=200;++split){TimedHost a,b;std::vector<uint16_t> code{0x7a00,0,0x300,0xf85a,0x6888,0x6809,0x40fa};a.program(code);b.program(code);yamanix::H8FirstInstructions<TimedHost> x(a),y(b);x.run(0,200,10000);y.run(0,int(split),10000);y.run(split,int(200-split),10000);check(a.times==b.times && a.trace==b.trace && x.snapshot()==y.snapshot(),"Timestamp and bus-trace invariance including cycle debt");check(a.times.size()==a.trace.size(),"One timestamp per completed bus callback");}
 {TimedHost h;int n=0;yamanix::H8hBusAccess<TimedHost> b(h,n);b.set_slice_end(std::numeric_limits<uint64_t>::max());bool threw=false;try{b.read8(0);}catch(const std::overflow_error&){threw=true;}check(threw && b.failed() && h.trace.empty() && h.times.empty(),"Overflow fails before observer or memory effect");}
 {TimedHost h;h.fault=true;int n=4;yamanix::H8hBusAccess<TimedHost> b(h,n);b.set_slice_end(4);bool threw=false;try{b.write8(0,1);}catch(const std::runtime_error&){threw=true;}check(threw && b.failed() && n==2 && h.trace.empty(),"Observer failure retains charge and prevents memory operation");}
 {TimedHost h;int n=4;yamanix::H8hBusAccess<TimedHost> b(h,n);bool threw=false;try{b.read8(0);}catch(const std::logic_error&){threw=true;}check(threw && b.failed() && h.times.empty() && h.trace.empty(),"Missing time origin fails before callbacks");}
 {TimedHost h;int n=10;yamanix::H8hBusAccess<TimedHost> b(h,n);b.set_slice_end(1);bool threw=false;try{b.read8(0);}catch(const std::overflow_error&){threw=true;}check(threw && b.failed() && h.times.empty(),"Negative absolute time rejected");}
 std::printf("Bus completion timestamps: 201 CPU splits, access types and faults: %u failures\n",failures);return failures?1:0;
}
