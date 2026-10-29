#include "h8_first_instructions.h"
#include "h8_memory_fixture.h"
#include <cstdio>
using Cpu=yamanix::H8FirstInstructions<Host>;
static unsigned failures;
static void check(bool ok,const char* why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
static std::vector<uint16_t> program(bool wide,unsigned count,uint32_t src,uint32_t dst,unsigned flags){
 std::vector<uint16_t> p;
 for(unsigned r=0;r<8;++r){uint32_t v=r==4?(0xabcd0000u|(wide?count:0x1200u|count)):r==5?src:r==6?dst:(r+1)*0x1234567u;p.insert(p.end(),{uint16_t(0x7a00|r),uint16_t(v>>16),uint16_t(v)});}
 p.insert(p.end(),{uint16_t(0x0700|flags),uint16_t(wide?0x7bd4:0x7b5c),0x598f,0x40fe});return p;
}
static void seed(Host& h,unsigned count,uint32_t src,uint32_t dst){
 for(unsigned i=0;i<count;++i){h.data[(src+i)&0xffffff]=uint8_t(i*73+0x5a);h.data[(dst+i)&0xffffff]=uint8_t(i*97+0xa5);}
}
static void one(bool wide,unsigned count,uint32_t src,uint32_t dst,unsigned flags){
 Host h;seed(h,count,src,dst);h.program(program(wide,count,src,dst,flags));auto memory=h.data;
 std::vector<std::tuple<char,uint32_t,uint16_t>> expected;
 for(unsigned i=0;i<count;++i){uint32_t a=src+i,b=dst+i;uint8_t v=memory[a&0xffffff];expected.emplace_back('R',a,v);expected.emplace_back('W',b,v);memory[b&0xffffff]=v;}
 std::array<uint16_t,16>want{};for(unsigned r=0;r<8;++r){uint32_t v=r==4?(wide?0xabcd0000u:0xabcd1200u):r==5?src+count:r==6?dst+count:(r+1)*0x1234567u;want[r]=uint16_t(v);want[r+8]=uint16_t(v>>16);}
 Cpu c(h);c.run(0,int(4*count+120),1000000);std::vector<std::tuple<char,uint32_t,uint16_t>> got;for(auto e:h.trace)if(std::get<0>(e)!='I')got.push_back(e);
 check(c.snapshot().registers==want&&c.snapshot().ccr==flags,"Count width,pointer wrapping,unrelated register/CCR preservation");
 check(got==expected&&h.data==memory,"Sequential byte copy,overlap propagation,exact ordered full-address history and RAM");
}
struct FaultHost:Host{
 unsigned reads=0,writes=0,fail_read=0,fail_write=0;
 uint8_t read_byte(uint32_t a){if(++reads==fail_read)throw std::runtime_error("read fault");return Host::read_byte(a);}
 void write_byte(uint32_t a,uint8_t v){Host::write_byte(a,v);if(++writes==fail_write)throw std::runtime_error("write after mutation");}
};
int main(){
 for(unsigned count=0;count<256;++count)for(unsigned flags:{0u,0x55u,0xffu})one(false,count,0xab123457,0xcd234569,flags);
 for(unsigned count:{0u,1u,2u,255u,256u,257u,32767u,32768u,65534u,65535u})one(true,count,0xfffffffdu,0x1000001,0xa5);
 for(bool wide:{false,true})for(unsigned count:{0u,1u,2u,3u,17u,255u})for(uint32_t src:{0u,1u,0xfffffeu,0xffffffu,0x1000000u,0xfffffffeu,0xffffffffu})for(unsigned delta:{0u,1u,0xffffffffu,0x100u})one(wide,count,src,src+delta,0x5a);
 for(bool wide:{false,true})for(unsigned cut=0;cut<=180;++cut){Host a,b;seed(a,7,0xfffffffe,0x200);b.data=a.data;a.program(program(wide,7,0xfffffffe,0x200,0xa5));b.words=a.words;Cpu x(a),y(b);x.run(0,180,10000);y.run(0,int(cut),10000);y.run(cut,int(180-cut),10000);check(x.snapshot()==y.snapshot()&&a.trace==b.trace&&a.data==b.data,"362 every-cycle partitions,read/write midpoint temporaries and byte commits");}
 for(bool wide:{false,true})for(unsigned ext=0;ext<65536;++ext){Host h;h.program({uint16_t(wide?0x7bd4:0x7b5c),uint16_t(ext)});Cpu c(h);bool fail=false;try{c.run(0,11,100);}catch(const std::runtime_error&){fail=true;}check(fail!=(ext==0x598f)&&c.failed()==fail,"131072 extension guards exact598f only");}
 for(bool wide:{false,true})for(bool store:{false,true})for(unsigned at:{1u,2u,3u}){FaultHost h;seed(h,3,0xffffffff,0x200);h.program(program(wide,3,0xffffffff,0x200,0xa5));if(store)h.fail_write=at;else h.fail_read=at;yamanix::H8FirstInstructions<FaultHost>c(h);bool fail=false;try{c.run(0,200,1000);}catch(const std::runtime_error&){fail=true;}const auto s=c.snapshot();const unsigned done=at-1;check(fail&&c.failed()&&s.registers[4]==(wide?3-done:0x1200|3-done)&&s.registers[12]==0xabcd&&s.registers[5]==uint16_t(0xffffffffu+done)&&s.registers[13]==uint16_t((0xffffffffu+done)>>16)&&s.registers[6]==0x200+done&&s.ccr==0xa5,"Fault count/pointers commit only after successful read+write pair");check(h.reads==at&&h.writes==(store?at:done),"No access after selected read/write failure");for(unsigned i=0;i<done;++i)check(h.data[0x200+i]==uint8_t(i*73+0x5a),"Completed copies retained before fault");if(store)check(h.data[0x200+done]==uint8_t(done*73+0x5a),"Faulting write mutation retained before pointer/count commit");bool again=false;try{c.run(200,1,100);}catch(const std::logic_error&){again=true;}check(again,"Poisoned CPU cannot resume");}
 std::printf("EEPMOV:768 byte-count cases,10 word boundaries,336 overlap/wrap cases,362 splits,131072 extension guards,12 faults:%u failures\n",failures);return failures?1:0;
}
