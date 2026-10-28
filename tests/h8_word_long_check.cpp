#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
#include <algorithm>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures=0;
static void check(bool ok,const char* why){if(!ok && ++failures<10)std::fprintf(stderr,"%s\n",why);}
static unsigned count(const Host& h,char kind){return unsigned(std::count_if(h.trace.begin(),h.trace.end(),[&](auto e){return std::get<0>(e)==kind;}));}
static std::vector<uint16_t> imm32(unsigned r,uint32_t v){return {uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)};}
static void append(std::vector<uint16_t>& code,const std::vector<uint16_t>& extra){code.insert(code.end(),extra.begin(),extra.end());}
static void put16(std::map<uint32_t,uint8_t>& data,uint32_t a,uint16_t v){a&=~uint32_t{1};data[a&0xffffff]=uint8_t(v>>8);data[(a+1)&0xffffff]=uint8_t(v);}
static unsigned flags(uint32_t v,unsigned sign,unsigned seed=0){return ((seed|0x80)&~0x0e)|(v==0?4:((v&sign)?8:0));}
static uint32_t pattern(unsigned n){const uint32_t special[]={0,1,0x7fffffff,0x80000000,0xffffffff,0xdeadbeef};return n<6?special[n]:((n*0x01010101u)^0xa55a0000u);}
static void word_case(unsigned p,unsigned r,uint16_t value,unsigned seed,bool store){
 Host host;const uint32_t address=0xab123457u+(p<<16);auto code=imm32(p,address);uint32_t target=address;
 if(store){append(code,{uint16_t(0x7900|r),value});if(r==p)target=(address&0xffff0000)|value;else if(r==p+8)target=(uint32_t(value)<<16)|(address&0xffff);}
 code.push_back(uint16_t((store?0x6980:0x6900)|(p<<4)|r));host.program(code);
 if(!store)put16(host.data,target,value);
 Cpu cpu(host,uint8_t(seed));cpu.run(0,store?21:17,1000);auto s=cpu.snapshot();
 check(s.ccr==flags(value,0x8000,seed),"Word NZV and unrelated flags");
 if(store){std::map<uint32_t,uint8_t> expected;put16(expected,target,value);check(host.data==expected && count(host,'S')==1 && count(host,'Q')==0,"Word store image, byte order and exactly-once transaction");}
 else{std::array<uint16_t,16> expected{};expected[p]=uint16_t(address);expected[p+8]=uint16_t(address>>16);expected[r]=value;check(s.registers==expected && count(host,'Q')==1 && count(host,'S')==0,"Word load halves/alias and exactly-once transaction");}
 auto it=std::find_if(host.trace.begin(),host.trace.end(),[&](auto e){return std::get<0>(e)==(store?'S':'Q');});
 check(it!=host.trace.end() && std::get<1>(*it)==(target&~uint32_t{1}),"CPU aligns odd word addresses before host access");
}
static void long_case(unsigned p,unsigned r,uint32_t value,unsigned n,bool store){
 Host host;const uint32_t address=n%16?0xab123457u+(p<<16):0xffffffffu;uint32_t target=address;auto code=imm32(p,address);
 if(store){append(code,imm32(r,value));if(p==r)target=value;}
 append(code,{0x0100,uint16_t((store?0x6980:0x6900)|(p<<4)|r)});host.program(code);
 if(!store){put16(host.data,target,uint16_t(value>>16));put16(host.data,target+2,uint16_t(value));}
 Cpu cpu(host);cpu.run(0,store?27:21,1000);auto s=cpu.snapshot();
 check(s.ccr==flags(value,0x80000000),"Long NZV flags");
 if(store){std::map<uint32_t,uint8_t> expected;put16(expected,target,uint16_t(value>>16));put16(expected,target+2,uint16_t(value));check(host.data==expected && count(host,'S')==2 && count(host,'Q')==0,"Long store big-endian halves, alias and wrap");}
 else{std::array<uint16_t,16> expected{};expected[p]=uint16_t(address);expected[p+8]=uint16_t(address>>16);expected[r]=uint16_t(value);expected[r+8]=uint16_t(value>>16);check(s.registers==expected && count(host,'Q')==2 && count(host,'S')==0,"Long load full register/alias and two word reads");}
 std::vector<uint32_t> addresses;for(auto [kind,a,v]:host.trace)if(kind==(store?'S':'Q'))addresses.push_back(a);
 check(addresses==std::vector<uint32_t>{target&~uint32_t{1},(target+2)&~uint32_t{1}},"Long first/second word address order and unsigned wrap");
}
template<class F>void rejects(F f){try{f();check(false,"Expected runtime failure");}catch(const std::runtime_error&){} }
int main(){
 for(unsigned value=0;value<65536;++value){word_case(0,3,uint16_t(value),value>>8,false);word_case(0,3,uint16_t(value),value>>8,true);}
 for(unsigned p=0;p<8;++p)for(unsigned r=0;r<16;++r)for(unsigned n=0;n<256;++n){const auto v=uint16_t((n<<8)|(n^0xa5));word_case(p,r,v,n,false);word_case(p,r,v,n,true);}
 for(unsigned p=0;p<8;++p)for(unsigned r=0;r<8;++r)for(unsigned n=0;n<256;++n){long_case(p,r,pattern(n),n,false);long_case(p,r,pattern(n),n,true);}
 for(unsigned extension=0;extension<65536;++extension){
  const bool supported=(extension&0xff08)==0x6900;Host host;host.program({0x0100,uint16_t(extension)});Cpu cpu(host);bool rejected=false;
  try{cpu.run(0,9,100);}catch(const std::runtime_error&){rejected=true;}
  check(rejected!=supported && cpu.failed()==rejected,"Exhaustive prefix extension decoding rejects byte/other instructions");}
 {Host host;host.program({0x0100,0x6901});Cpu cpu(host);cpu.run(0,7,100);auto s=cpu.snapshot();check(s.state==0x0100 && s.substate==2 && s.debt==1,"Prefix read suspended with debt");cpu.run(7,1,100);check(cpu.snapshot().state==0x0100,"Debt repayment does not repeat extension fetch");cpu.run(8,1,100);check(cpu.snapshot().state==0x26901 && cpu.snapshot().substate==2 && cpu.snapshot().ppc==0x100,"Prefix continuation changes internal state before long handler");}
 {Host host;auto code=imm32(0,0x123457);append(code,{0x0100,0x6901});host.program(code);put16(host.data,0x123457,0x89ab);put16(host.data,0x123459,0xcdef);Cpu cpu(host);cpu.run(0,18,100);check(count(host,'Q')==1 && cpu.snapshot().substate==4,"Long load stops after first word");cpu.run(18,1,100);check(count(host,'Q')==2 && cpu.snapshot().substate==6,"Partial load performs second word once");cpu.run(19,1,100);cpu.run(20,1,100);auto s=cpu.snapshot();check(count(host,'Q')==2 && s.registers[1]==0xcdef && s.registers[9]==0x89ab,"Completed low word is not retried after debt repayment");}
 for(unsigned fail_at:{1u,2u}){Host host;auto code=imm32(0,0x123457);append(code,imm32(1,0x89abcdef));append(code,{0x0100,0x6981});host.program(code);host.fail_word_write=fail_at;Cpu cpu(host);rejects([&]{cpu.run(0,30,100);});check(cpu.failed() && count(host,'S')==fail_at,"Mid-transfer write failure poisons owner");check(host.data[0x123456]==0x89 && host.data[0x123457]==0xab,"First written word remains committed after failure");if(fail_at==1)check(!host.data.contains(0x123458),"Failure on first word does not execute second write");else check(host.data[0x123458]==0xcd && host.data[0x123459]==0xef,"Failure after second mutation retains both words");}
 for(unsigned seed=0;seed<1000;++seed){Host a,b;const std::vector<uint16_t> code{0x7a00,0x12,0x3457,0x7a01,uint16_t(seed),uint16_t(seed^0xa55a),0x0100,0x6981,0x0100,0x6902,0x7903,uint16_t(seed*7),0x6983,0x6904,0x40e2};a.program(code);b.program(code);Cpu x(a),y(b);x.run(0,512,10000);unsigned random=seed+1;uint64_t start=0;while(start<512){random=random*1664525u+1013904223u;int n=int(1+random%19);if(start+n>512)n=int(512-start);y.run(start,n,1000);start+=n;}check(x.snapshot()==y.snapshot() && a.data==b.data && a.trace==b.trace,"1000 mixed word/long histories preserve state, memory and bus trace");}
 std::printf("H8 word/long: exhaustive 16-bit values + alias matrices, 65536 prefix extensions, 1000 partitions: %u failures\n",failures);return failures?1:0;
}
