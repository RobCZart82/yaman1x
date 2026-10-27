#include "h8_sci_cycle_clock.h"
#include <cstdio>
#include <initializer_list>
#include <limits>
#include <utility>
#include <vector>

int main() {
    using Clock = yamanix::H8SciCycleClock;
    using Side = Clock::Side;
    unsigned failures = 0;
    auto check = [&](bool ok, const char *name) {
        if (!ok && ++failures < 8) std::fprintf(stderr,"%s\n",name);
    };
    for (unsigned cks = 0; cks < 4; ++cks)
    for (unsigned brr = 0; brr < 256; ++brr) {
        Clock clock;
        clock.configure(uint8_t(cks),uint8_t(brr),0);
        const auto period = uint64_t(2 * (1u << (2*cks)) * (brr+1));
        check(clock.divider() == period,"All CKS/BRR divisors");
        clock.start(Side::tx,period*2,period*9);
        check(clock.next_event() == period*3,"TX uses total_cycles and next strict boundary");
        clock.reset(); clock.configure(uint8_t(cks),uint8_t(brr),0);
        clock.start(Side::rx,period*2,period*9);
        check(clock.next_event() == period*10,"RX uses now_as_cycles");
    }
    Clock a,b;
    a.configure(0,15,0); b.configure(0,15,0);
    a.start(Side::tx,0,0); a.start(Side::rx,0,97);
    b.start(Side::tx,0,0); b.start(Side::rx,0,97);
    check(a.next_event() == 32 && !a.start(Side::tx,999,999),"Shared phase / duplicate start");
    std::vector<std::pair<uint64_t,int>> whole,split;
    auto drain = [&](Clock &clock, uint64_t horizon, auto &events) {
        unsigned budget = 10000;
        while (clock.next_event() && clock.next_event() <= horizon) {
            if (!budget) { check(false,"Owner budget exhausted"); break; }
            --budget;
            clock.service_one(horizon,
                [&](auto){events.emplace_back(clock.next_event(),1);},
                [&](auto){events.emplace_back(clock.next_event(),2);}, []{});
        }
        check(budget > 0,"Bounded owner drain");
    };
    drain(a,10000,whole);
    for (auto horizon : {31u,32u,33u,127u,2500u,9999u,10000u}) drain(b,horizon,split);
    check(whole == split && a.next_event() == b.next_event(),"Partition equality and TX-before-RX");
    check(whole[0] == std::make_pair(uint64_t(32),1) && whole[1].second == 2,"First edge order");
    Clock single;
    single.configure(0,0,0); single.start(Side::tx,0,0);
    unsigned ticks=0;
    single.service_one(1,[&](auto){++ticks;},[](auto){},[]{});
    check(ticks == 0 && single.next_event() == 2,"No callback before edge");
    single.service_one(1000,[&](auto){++ticks;},[](auto){},[]{});
    check(ticks == 1 && single.next_event() == 4,"One edge per upstream update call");
    single.configure(0,7,1);
    check(single.step() == 2 && single.divider() == 16,"Active step survives reconfiguration");
    single.stop(Side::tx); single.start(Side::rx,0,17);
    check(single.step() == 16 && single.next_event() == 32,"Restart uses new divisor");
    single.standby_exit(100,60);
    check(single.next_event() == 72,"Standby offset");
    single.start(Side::tx,0,0);
    unsigned rx_ticks=0,idles=0;
    single.service_one(72,[&](auto){single.stop(Side::rx); single.stop(Side::tx);},
                       [&](auto){++rx_ticks;},[&]{++idles;});
    check(!single.next_event() && !rx_ticks && idles == 1,"TX stop suppresses RX and releases output clock");
    check(b.active(Side::tx),"Independent instances");
    Clock reentrant;
    reentrant.configure(0,0,0); reentrant.start(Side::tx,0,0); reentrant.start(Side::rx,0,0);
    reentrant.service_one(2,[&](auto){reentrant.configure(0x80,0,0);},
        [&](auto mode){check(mode == Clock::Mode::async,"Shared edge dispatch mode is latched");},[]{});
    const auto old_divider=b.divider();
    bool rejected=false;
    try { b.configure(0,0,2); } catch(const std::invalid_argument&) {rejected=true;}
    check(rejected && b.divider() == old_divider,"External mode rejected without mutation");
    const auto old_event = b.next_event();
    rejected = false;
    try { b.standby_exit(1,2); } catch(const std::invalid_argument&) {rejected=true;}
    check(rejected && b.next_event() == old_event,"Invalid standby order atomicity");
    Clock overflow;
    rejected=false;
    try {overflow.start(Side::tx,std::numeric_limits<uint64_t>::max(),0);}
    catch(const std::overflow_error&){rejected=true;}
    check(rejected && !overflow.next_event() && !overflow.active(Side::tx),"Startup overflow atomicity");
    Clock sync;
    sync.configure(0x80,0,1);
    check(sync.mode() == Clock::Mode::sync_out,"Internal sync mode selection");
    sync.reset();
    check(sync.divider() == 512 && sync.mode() == Clock::Mode::async,"SCI reset defaults");
    std::printf("H8 SCI internal cycle clock: %s (%u failures)\n",failures?"FAIL":"PASS",failures);
    return failures?1:0;
}
