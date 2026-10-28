#include "h8_execution_runner.h"
#include <cstdio>
#include <vector>
#include <utility>
using namespace yamanix;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok){++failures;std::fprintf(stderr,"%s\n",why);}}
template<class E,class F>void throws(F f){try{f();check(false,"Missing exception");}catch(const E&){}catch(...){check(false,"Wrong exception");}}
struct Owner {
    std::vector<std::pair<char,uint64_t>> trace;
    uint64_t event=3;
    bool pending=true;
    uint64_t update(uint64_t cycle){trace.emplace_back('U',cycle);if(cycle>=event)event=0;return event;}
    bool has_partial()const{return pending;}
    void partial(H8ExecutionBudget& b){trace.emplace_back('P',b.remaining());--b.counter();pending=false;}
    void full(H8ExecutionBudget& b){trace.emplace_back('F',b.remaining());b.counter()-=2;}
};
int main(){
    {H8ExecutionRunner r;Owner o;
     check(r.run(0,6,20,o)==6,"Callback accounting");
     check(o.trace==std::vector<std::pair<char,uint64_t>>{{'U',0},{'P',6},{'F',5},{'U',3},{'F',3},{'F',1}},"Initial partial and exact event ordering");
     check(r.debt()==1,"Overshoot carried");
     o.trace.clear();r.run(6,0,20,o);check(r.debt()==1 && o.trace.size()==1,"Zero slice updates but retains debt");
     o.trace.clear();r.run(6,1,20,o);check(r.debt()==0 && o.trace.size()==1,"Short slice only pays debt");}
    {H8ExecutionRunner r;Owner o;o.pending=false;o.event=4;
     r.run(0,4,20,o);check(o.event==4,"End-equal event excluded");
     o.trace.clear();r.run(4,2,20,o);check(o.trace.front()==std::pair<char,uint64_t>{'U',4} && !o.event,"Next slice catches end-equal event");}
    {H8ExecutionRunner r;Owner o;o.pending=false;o.event=1;
     r.run(10,0,20,o);check(o.trace==std::vector<std::pair<char,uint64_t>>{{'U',10}},"Initial update catches stale event with zero budget");}
    {H8ExecutionRunner r;Owner o;throws<std::invalid_argument>([&]{r.run(0,-1,20,o);});
     check(!r.failed() && o.trace.empty(),"Bad input does not touch owner");r.run(0,1,20,o);}
    {H8ExecutionRunner r;Owner o;throws<std::runtime_error>([&]{r.run(0,2,0,o);});
     check(r.failed() && o.trace.empty(),"Limit checked before callback");
     throws<std::logic_error>([&]{r.run(0,2,20,o);});}
    {struct Stuck:Owner {uint64_t update(uint64_t){return 1;}}o;H8ExecutionRunner r;
     throws<std::runtime_error>([&]{r.run(1,4,8,o);});check(r.failed(),"Nonadvancing event bounded");}
    {struct Stuck:Owner {void full(H8ExecutionBudget&){} }o;H8ExecutionRunner r;o.pending=false;o.event=0;
     throws<std::runtime_error>([&]{r.run(0,4,8,o);});check(r.failed(),"Nonadvancing handler bounded");}
    {struct Broken:Owner {void partial(H8ExecutionBudget&){throw std::runtime_error("partial");}}o;H8ExecutionRunner r;
     throws<std::runtime_error>([&]{r.run(0,4,8,o);});check(r.failed(),"Partial callback exception poisons");}
    {H8ExecutionRunner r;struct Reenter:Owner {H8ExecutionRunner& r;Reenter(H8ExecutionRunner& x):r(x){}
     uint64_t update(uint64_t){r.run(0,0,1,*this);return 0;}}o(r);
     throws<std::logic_error>([&]{r.run(0,4,8,o);});check(r.failed(),"Reentry poisons outer owner");}
    for(unsigned seed=0;seed<1000;++seed){
     H8ExecutionRunner a,b;Owner x,y;x.pending=y.pending=false;x.event=y.event=0;
     a.run(0,1000,2000,x);unsigned random=seed+1;uint64_t start=0;
     while(start<1000){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>1000)n=int(1000-start);b.run(start,n,100,y);start+=n;}
     unsigned ax=0,by=0;for(auto e:x.trace)ax+=e.first=='F';for(auto e:y.trace)by+=e.first=='F';
     check(ax==500 && by==500 && a.debt()==b.debt(),"Partition action count and isolated debt");}
    std::printf("H8 execution runner: 1000 partitions, ordering and failure checks: %u failures\n",failures);
    return failures?1:0;
}
