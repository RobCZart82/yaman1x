// Synthetic research host; locked original methods are inserted locally only.
#include "h8_first_instructions.h"
#include <cstdio>
#include <vector>
#include <stdexcept>
#include <utility>
#include <map>
#include <tuple>
using u8=uint8_t;using s8=int8_t;using s16=int16_t;using s32=int32_t;using u16=uint16_t;using u32=uint32_t;using u64=uint64_t;
struct Host {
 std::array<u16,512> words{};
 std::map<u32,u8> data;
 std::vector<std::tuple<char,u32,u16>> bus;u64 event=11;
 explicit Host(unsigned seed){words[1]=0x100;unsigned random=seed;
  const u32 address=0x120001+seed;
  std::vector<u16> code{0x7a00,u16(address>>16),u16(address),u16(0xf900|((seed*73)&255)),0x6889,0x680a,u16(0x4002|((seed%16)<<8)),0};
  code.push_back(0x7a03);code.push_back(u16(seed*997));code.push_back(u16(seed*197));
  code.insert(code.end(),{0x7905,u16(seed*13),0x6983,0x6904,0x0100,0x6983,0x0100,0x6904});
  if(seed%7==1)code.insert(code.end(),{0x8901,0xa97f,0x089a,0x18a9,0x1c9a,0x7915,u16(seed*19),0x7925,0x8000,0x7935,0xffff,0x7a13,0x1000,u16(seed),0x7a23,0x7fff,0xffff,0x7a33,0xffff,0xffff});
  for(unsigned r=0;r<4;++r){random=random*1664525u+1013904223u;code.push_back(u16(0xf000|(r<<8)|(random>>24)));}
  if(seed%7==0)code.insert(code.end(),{0xf900,u16(0x0700|((seed>>1)&0xfe)),0x9900,0xb900,u16(0x0700|((seed>>1)&255)),0x9901,0xbaff,0x0e9a,0x1ea9,0xc955,0xd9aa,0xe90f,0x149a,0x159a,0x169a,0x0934,0x1934,0x1d34,0x0ab4,0x1ab4,0x1fb4,0x7945,0xaaaa,0x7955,0x5555,0x7965,0xff00,0x7a43,0x1234,0x5678,0x7a53,0xaaaa,0x5555,0x7a63,0xffff,0xff00});
  if(seed%7==2)code.insert(code.end(),{u16(0x0700|(seed&255)),0x6412,0x6523,0x6634,0x01f0,0x6412,0x01f0,0x6523,0x01f0,0x6634});
  if(seed%7==3){code.insert(code.end(),{0x7a03,0,0,0x0701,0x1333,0x0700,0x1333});code.push_back(u16(0x0700|(seed&255)));
   code.insert(code.end(),{0x1009,0x1015,0x1033});
   code.insert(code.end(),{0x1089,0x1095,0x10b3});
   code.insert(code.end(),{0x1109,0x1115,0x1133});
   code.insert(code.end(),{0x1189,0x1195,0x11b3});
   code.insert(code.end(),{0x1209,0x1215,0x1233});
   code.insert(code.end(),{0x1289,0x1295,0x12b3});
   code.insert(code.end(),{0x1309,0x1315,0x1333});
   code.insert(code.end(),{0x1389,0x1395,0x13b3});
  }
  if(seed%7==4){code.insert(code.end(),{0xf908,u16(0x0700|(seed&255))});
   code.insert(code.end(),{0x609a,0x619a,0x629a,0x639a,0x702a,0x712a,0x722a,0x732a,0x672a,0x67aa,0x742a,0x74aa,0x752a,0x75aa,0x762a,0x76aa,0x772a,0x77aa});
  }
  if(seed%7==5){code={0x7a00,0x12,u16(address),0xf908,u16(0x0700|(seed&255))};
   code.insert(code.end(),{0x7d00,0x6090});
   code.insert(code.end(),{0x7d00,0x6190});
   code.insert(code.end(),{0x7d00,0x6290});
   code.insert(code.end(),{0x7c00,0x6390});
   code.insert(code.end(),{0x7d00,0x7020});
   code.insert(code.end(),{0x7d00,0x7120});
   code.insert(code.end(),{0x7d00,0x7220});
   code.insert(code.end(),{0x7c00,0x7320});
   code.insert(code.end(),{0x7d00,0x6720});
   code.insert(code.end(),{0x7d00,0x67a0});
   code.insert(code.end(),{0x7c00,0x7420});
   code.insert(code.end(),{0x7c00,0x74a0});
   code.insert(code.end(),{0x7c00,0x7520});
   code.insert(code.end(),{0x7c00,0x75a0});
   code.insert(code.end(),{0x7c00,0x7620});
   code.insert(code.end(),{0x7c00,0x76a0});
   code.insert(code.end(),{0x7c00,0x7720});
   code.insert(code.end(),{0x7c00,0x77a0});
  }
  if(seed%7==6){code={0x7a00,0x12,u16(address),0xf908,u16(0x0700|(seed&255))};
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x6090});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x6190});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x6290});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x6390});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x7020});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x7120});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x7220});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x7320});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x6720});
   code.insert(code.end(),{u16(0x7f00|(seed&255)),0x67a0});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x7420});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x74a0});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x7520});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x75a0});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x7620});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x76a0});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x7720});
   code.insert(code.end(),{u16(0x7e00|(seed&255)),0x77a0});
  }
  code.push_back(0);const int displacement=-2*int(code.size()+1);if(displacement < -128)throw std::logic_error("Reference rel8 loop exceeds range");code.push_back(u16(0x4000|u8(displacement)));
  for(unsigned n=0;n<code.size();++n)words[0x80+n]=code[n];
 }
 u16 read_instruction_word(u32 address){if((address&1)||address/2>=words.size())throw std::out_of_range("bus");auto v=words[address/2];bus.emplace_back('I',address,v);return v;}
 u8 read_byte(u32 address){auto v=data[address&0xffffff];bus.emplace_back('R',address,v);return v;}
 void write_byte(u32 address,u8 value){data[address&0xffffff]=value;bus.emplace_back('W',address,value);}
 u16 read_word(u32 address){if(address&1)throw std::logic_error("unaligned data word");auto v=u16((u16(data[address&0xffffff])<<8)|data[(address+1)&0xffffff]);bus.emplace_back('Q',address,v);return v;}
 void write_word(u32 address,u16 value){if(address&1)throw std::logic_error("unaligned data word");data[address&0xffffff]=u8(value>>8);data[(address+1)&0xffffff]=u8(value);bus.emplace_back('S',address,value);}
 u64 update(u64 cycle){while(event<=cycle)event+=11;return event;}
};
using Cpu=yamanix::H8FirstInstructions<Host>;
struct Original {
 Host& host;
 struct Cache {Host& host;u16 read_word(u32 a){return host.read_instruction_word(a);}}m_cache;
 struct Program {Host& host;u8 read_byte(u32 a){return host.read_byte(a);}void write_byte(u32 a,u8 v){host.write_byte(a,v);}u16 read_word(u32 a){return host.read_word(a);}void write_word(u32 a,u16 v){host.write_word(a,v);}}m_program;
 int m_icount=0,m_bcount=0,m_count_before_instruction_step=0,m_inst_substate=0;
 int m_inst_state=0x10000,m_requested_state=-1,m_current_dma=-1,m_irq_vector=0;
 void* m_current_dtc=nullptr;
 bool m_has_trace=false,m_has_exr=false,m_has_hc=true,m_mode_advanced=true;
 u32 m_PC=0,m_PPC=0,m_NPC=0,m_TMP1=0,m_TMP2=0;
 std::array<u16,16> m_R{};std::array<u16,3> m_IR{};u16 m_PIR=0;u8 m_CCR=0,m_EXR=0;
 static constexpr int STATE_DMA=0x10003,STATE_DTC=0x10004,STATE_TRACE=0x10002;
 static constexpr u8 F_I=0x80,F_H=0x20,F_N=8,F_Z=4,F_V=2,F_C=1,EXR_T=0x80,EXR_I=7,EXR_NC=0x78;
 u64 end=0;
 explicit Original(Host& h,u8 ccr):host(h),m_cache{h},m_program{h},m_CCR(ccr){}
 bool access_to_be_redone(){return false;}
 bool exr_in_stack(){return false;}
 bool debugger_enabled(){return false;}
 void debugger_instruction_hook(u32){}
 void take_interrupt(){throw std::runtime_error("Unsupported IRQ");}
 void illegal(){throw std::runtime_error("Unsupported opcode");}
 void update_irq_filter(){}
 u64 total_cycles(){return end-u64(m_icount);}
 void internal_update(u64 cycle){recompute_bcount(host.update(cycle));}
 Cpu::Snapshot snapshot()const{return {m_R,m_PC,m_PPC,m_NPC,u32(m_inst_state),u32(m_inst_substate),m_TMP1,m_TMP2,m_IR,m_PIR,m_CCR,m_EXR,m_icount,m_count_before_instruction_step};}
 /* HOST */
 /* SELECTED */
 /* DISPATCH */
 /* STUBS */
};
int main(){
 unsigned random=123;unsigned long long instruction_reads=0,data_reads=0,data_writes=0,word_reads=0,word_writes=0;
 for(unsigned seed=0;seed<100;++seed){
  Host a(seed+1),b(seed+1);Cpu cpu(a,u8(seed));Original ref(b,u8(seed));u64 start=0;
  for(unsigned n=0;n<2000;++n){random=random*1664525u+1013904223u;int size=int(random%19);
   ref.end=start+size;ref.m_icount=size;ref.execute_run();cpu.run(start,size,1000);
   if(ref.snapshot()!=cpu.snapshot() || a.bus!=b.bus || a.data!=b.data || a.event!=b.event){std::fprintf(stderr,"Mismatch seed %u slice %u\n",seed,n);return 1;}
   for(auto [kind,address,value]:a.bus){instruction_reads+=kind=='I';data_reads+=kind=='R';data_writes+=kind=='W';word_reads+=kind=='Q';word_writes+=kind=='S';}a.bus.clear();b.bus.clear();start+=size;
  }
 }
 std::printf("Original generated H8H decoder/handlers + execute_run: 200000 slices, %llu instruction reads, %llu data reads, %llu data writes, %llu word reads, %llu word writes; zero differences\n",instruction_reads,data_reads,data_writes,word_reads,word_writes);
}
