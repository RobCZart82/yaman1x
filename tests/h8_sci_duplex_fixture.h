#include "h8_first_instructions.h"
#include "h83002_sci_mmio.h"
#include "h8_memory_fixture.h"
#include <algorithm>
#include <cstdio>
#pragma once
struct DuplexHost:Host {
 yamanix::H8hIrqController irq;yamanix::H83002SciMmio sci{irq};
 bool stop=true;unsigned txch,rxch,bit=0;uint8_t received;uint64_t now=0,pin_start=160;
 std::vector<std::tuple<unsigned,uint64_t,bool>> wire;std::vector<int> exceptions;
 static uint32_t base(unsigned ch){return ch?0xffffb8:0xffffb0;}
 void tx(unsigned ch,bool level){wire.emplace_back(ch,now,level);}
 void clock(unsigned,bool){} bool dma_request(unsigned){return false;}
 void exception_hook(int vector){exceptions.push_back(vector);}
 uint64_t pin_event()const{return bit<10?pin_start+32*bit:0;}
 uint64_t next_bus_event()const{const auto a=pin_event(),b=sci.next_event();return !a?b:!b?a:std::min(a,b);}
 uint64_t update(uint64_t cycle){unsigned guard=0;while(next_bus_event() && next_bus_event()<=cycle){if(++guard>4096)throw std::runtime_error("Duplex progress");now=next_bus_event();if(pin_event()==now){sci.write_rx(rxch,bit==0?false:bit==9?stop:((received>>(bit-1))&1)!=0,now);++bit;}for(unsigned c=0;c<2;++c)if(sci.next_event(c)==now)sci.service_one(c,now,*this);}return next_bus_event();}
 void bus_cycle(uint64_t cycle){update(cycle);now=cycle;}
 DuplexHost(unsigned t,unsigned r,uint8_t sent,uint8_t recv,bool valid_stop=true):stop(valid_stop),txch(t),rxch(r),received(recv){
 std::vector<uint16_t> p{0x7a07,0,0x300,0x0700};
 for(unsigned ch=0;ch<2;++ch){const uint16_t config[]={0x7a00,0x00ff,uint16_t(base(ch)+1),0xfa00,0x688a,0x0b00,0xfa70,0x688a};p.insert(p.end(),std::begin(config),std::end(config));}
 const uint16_t send[]={0x7a00,0x00ff,uint16_t(base(txch)+3),uint16_t(0xfa00|sent),0x688a,0x0b00,0x680b,0xeb7f,0x688b,0x40fe};p.insert(p.end(),std::begin(send),std::end(send));program(p);
 const auto vector=53+4*rxch;words[2*vector]=0;words[2*vector+1]=0x180;
 const uint16_t handler[]={0x7a04,0x00ff,uint16_t(base(rxch)+4),0x7a05,0,0x400,0x684e,0x0b04,0x684b,0x68db,0x1b04,0xeebf,0x68ce,0x5670};for(unsigned i=0;i<std::size(handler);++i)words[0xc0+i]=handler[i];
 }
 uint8_t read_byte(uint32_t a){if(yamanix::H83002SciMmio::mapped(a)){auto v=sci.read8(a);trace.emplace_back('R',a,v);return v;}return Host::read_byte(a);}
 void write_byte(uint32_t a,uint8_t v){if(yamanix::H83002SciMmio::mapped(a)){sci.write8(a,v,now,*this);trace.emplace_back('W',a,v);return;}Host::write_byte(a,v);}
};
