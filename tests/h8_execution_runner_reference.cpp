// Synthetic host fixture; original BSD methods are inserted only after hash checks.
#include "h8_execution_runner.h"
#include <vector>
#include <utility>
#include <cstdio>
using u64=uint64_t;
using Trace=std::vector<std::pair<char,int64_t>>;
struct Upstream {
 int m_icount=0,m_bcount=0,m_count_before_instruction_step=0;
 int m_inst_substate=0,m_inst_state=0x10000,m_PPC=0,m_NPC=0;
 u64 end=0,event=11;Trace trace;
 u64 total_cycles(){return end-u64(m_icount);}
 bool debugger_enabled(){return false;}
 void debugger_instruction_hook(int){}
 void internal_update(u64 t){trace.emplace_back('U',int64_t(t));if(event<=t)event+=11;recompute_bcount(event);}
 void do_exec_full(){trace.emplace_back('F',m_icount);m_icount-=2;if(m_icount<=m_bcount)m_inst_substate=1;}
 void do_exec_partial(){trace.emplace_back('P',m_icount);--m_icount;m_inst_substate=0;}
 /* ORIGINAL_METHODS */
};
struct Owner {
 u64 event=11;bool pending=false;Trace trace;
 u64 update(u64 t){trace.emplace_back('U',int64_t(t));if(event<=t)event+=11;return event;}
 bool has_partial()const{return pending;}
 void full(yamanix::H8ExecutionBudget& b){trace.emplace_back('F',b.remaining());b.counter()-=2;if(b.remaining()<=b.boundary())pending=true;}
 void partial(yamanix::H8ExecutionBudget& b){trace.emplace_back('P',b.remaining());--b.counter();pending=false;}
};
int main(){
 Upstream ref;Owner owner;yamanix::H8ExecutionRunner runner;
 u64 start=0;unsigned random=123;unsigned long long updates=0,full=0,partial=0;
 for(unsigned n=0;n<200000;++n){
  random=random*1664525u+1013904223u;int size=int(random%19);
  ref.trace.clear();owner.trace.clear();ref.end=start+size;ref.m_icount=size;
  ref.execute_run();runner.run(start,size,100,owner);
  if(ref.trace!=owner.trace || ref.m_count_before_instruction_step!=runner.debt() ||
     ref.m_icount!=runner.counter() || bool(ref.m_inst_substate)!=owner.pending || ref.event!=owner.event){
   std::fprintf(stderr,"Mismatch at slice %u\n",n);return 1;}
  for(auto [kind,value]:owner.trace){updates+=kind=='U';full+=kind=='F';partial+=kind=='P';}
  start+=size;
 }
 if(!updates || !full || !partial)return 2;
 std::printf("Original execute_run: 200000 eventful slices, %llu updates, %llu full and %llu partial callbacks; zero differences\n",updates,full,partial);
}
