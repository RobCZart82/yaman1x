#pragma once
#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <memory>
namespace yamanix {
// Project-owned cold reconstruction lifecycle,not hardware warm reset.
// Sink outlives machine;no retained internal references,threads or callback reentry.
template<class Sink>class An1xResearchMachine {
public:
 using Bus=An1xSciBus<Sink>;using Cpu=H8FirstInstructions<Bus>;using RxEvent=typename Bus::RxEvent;
 enum class NvramPolicy{preserve,clear};
 An1xResearchMachine(std::span<const uint8_t>rom,std::span<const RxEvent>input,Sink&sink)
 :rom_(copy_rom(rom)),sink_(sink),state_(std::make_unique<State>(rom_,input,sink_)){}
 An1xResearchMachine(const An1xResearchMachine&)=delete;
 An1xResearchMachine& operator=(const An1xResearchMachine&)=delete;
 std::size_t consumed_input()const{return state_->bus.consumed_input();}
 uint64_t generation()const{return generation_;}uint64_t cycle()const{return cursor_;}
 bool failed()const{return poisoned_||state_->bus.failed()||state_->cpu.failed();}
 typename Cpu::Snapshot snapshot()const{return state_->cpu.snapshot();}
 std::vector<uint8_t> nvram_image()const{return state_->bus.nvram_image();}
 uint8_t read_byte(uint32_t a){Guard g(*this);require_live();auto value=state_->bus.read_byte(a);if(poisoned_)throw std::logic_error("Machine reentry");return value;}
 void write_byte(uint32_t a,uint8_t v){Guard g(*this);require_live();state_->bus.write_byte(a,v);if(poisoned_)throw std::logic_error("Machine reentry");}
 std::size_t run(int cycles,std::size_t limit){Guard g(*this);require_live();
  if(cycles<0||uint64_t(cycles)>UINT64_MAX-cursor_)throw std::invalid_argument("Machine cycle range");
  try{auto calls=state_->cpu.run(cursor_,cycles,limit);if(poisoned_)throw std::logic_error("Machine reentry");cursor_+=uint64_t(cycles);return calls;}
  catch(...){poisoned_=true;throw;}
 }
 // Replacement event schedule is epoch-relative;old pending edges are discarded.
 // Validation/allocation failure leaves the old epoch unchanged,including failure status.
 void cold_restart(NvramPolicy policy,std::span<const RxEvent>input={}){Guard g(*this);
  if(policy!=NvramPolicy::preserve&&policy!=NvramPolicy::clear)throw std::invalid_argument("NVRAM policy");
  if(generation_==UINT64_MAX)throw std::overflow_error("Machine generation overflow");
  std::vector<uint8_t>saved;if(policy==NvramPolicy::preserve)saved=state_->bus.nvram_image();
  auto candidate=std::make_unique<State>(rom_,input,sink_);
  if(policy==NvramPolicy::preserve)candidate->bus.load_nvram(saved);
  state_.swap(candidate);cursor_=0;++generation_;poisoned_=false;
 }
private:
 struct State{Bus bus;Cpu cpu;State(std::span<const uint8_t>rom,std::span<const RxEvent>input,Sink&sink):bus(rom,input,sink),cpu(bus,0,&bus.irq()) {}};
 struct Guard{An1xResearchMachine&m;explicit Guard(An1xResearchMachine&x):m(x){if(m.busy_){m.poisoned_=true;throw std::logic_error("Machine reentry");}m.busy_=true;}~Guard(){m.busy_=false;}};
 void require_live()const{if(failed())throw std::logic_error("Failed machine epoch");}
 static std::vector<uint8_t>copy_rom(std::span<const uint8_t>rom){if(rom.size()!=An1xMemoryMap::rom_size)throw std::invalid_argument("Machine ROM size");return {rom.begin(),rom.end()};}
 const std::vector<uint8_t>rom_;Sink&sink_;std::unique_ptr<State>state_;
 uint64_t generation_=0,cursor_=0;bool busy_=false,poisoned_=false;
};
}
