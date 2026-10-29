#include "an1x_memory_map.h"
#include "h8_first_instructions.h"
#include <cstdio>
using Map=yamanix::An1xMemoryMap;
static unsigned failures;
static void check(bool ok,const char* why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
template<class F>static void rejects(F f,const char* why){bool bad=false;try{f();}catch(const std::exception&){bad=true;}check(bad,why);}
struct Host {
 Map memory;
 explicit Host(std::span<const uint8_t> image):memory(image){}
 uint16_t read_instruction_word(uint32_t a){return memory.read_instruction_word(a);}
 uint8_t read_byte(uint32_t a){return memory.read_byte(a);}
 uint16_t read_word(uint32_t a){return memory.read_word(a);}
 void write_byte(uint32_t a,uint8_t v){memory.write_byte(a,v);}
 void write_word(uint32_t a,uint16_t v){memory.write_word(a,v);}
 uint64_t update(uint64_t){return 0;}
};
static std::vector<uint8_t> image(const std::vector<uint16_t>& code){std::vector<uint8_t> r(Map::rom_size);r[3]=0x100&255;r[2]=1; // Reset vector00000100.
 for(unsigned i=0;i<code.size();++i){r[0x100+2*i]=uint8_t(code[i]>>8);r[0x101+2*i]=uint8_t(code[i]);}return r;}
int main(){
 std::vector<uint8_t> rom(Map::rom_size);for(unsigned i=0;i<rom.size();++i)rom[i]=uint8_t((i*73)^(i>>8));
 Map a(rom),b(rom);for(unsigned i=0;i<rom.size();++i)check(a.read_byte(i)==rom[i],"All logical ROM bytes retained");
 for(unsigned i=0;i<rom.size();i+=2)check(a.read_instruction_word(i)==uint16_t((uint16_t(rom[i])<<8)|rom[i+1]),"Big endian instruction word for all ROM pairs");
 rom[0]^=255;check(a.read_byte(0)!=rom[0],"ROM owned snapshot isolated from caller mutation");
 for(auto [base,size]:{std::pair<uint32_t,unsigned>{0x200000,0x40000},{0xfffd10,0x200}}){
  for(unsigned i=0;i<size;++i){check(a.read_byte(base+i)==0,"Deterministic research RAM initialization");a.write_byte(base+i,uint8_t(i*97));}
  for(unsigned i=0;i<size;++i)for(unsigned tag:{0u,0xab000000u,0xff000000u})check(a.read_byte(tag|(base+i))==uint8_t(i*97),"Every RAM byte and24-bit high aliases");
  for(unsigned i=0;i<size;i+=2){a.write_word(base+i,uint16_t(i^0xa55a));check(a.read_word(base+i)==uint16_t(i^0xa55a),"Every aligned RAM word and lane");}
  for(unsigned i=0;i<size;++i)check(b.read_byte(base+i)==0,"Separate machine memory isolation");
 }
 for(uint32_t p:{0x100000u,0x1fffffu,0x240000u,0x400001u,0x440000u,0x480000u,0xfffd0fu,0xffff10u,0xffffb0u,0xffffffu}){check(Map::region(p)==Map::Region::unmapped,"Holes/MMIO not invented as RAM");rejects([&]{a.read_byte(p);},"Unmapped byte read");rejects([&]{a.write_byte(p,1);},"Unmapped byte write");}
 for(uint32_t p:{0u,0xfffffu}){auto v=a.read_byte(p);rejects([&]{a.write_byte(p,0);},"ROM byte writes fail closed");check(a.read_byte(p)==v,"ROM write rejection preserves contents");}
 for(uint32_t p:{0u,0xffffeu}){auto v=a.read_word(p);rejects([&]{a.write_word(p,0);},"ROM word writes fail closed");check(a.read_word(p)==v,"ROM pair unmodified");}
 auto saved=a.nvram_image();for(uint32_t p:{1u,0x200001u,0x23ffffu,0xfffd11u,0xffff0fu}){rejects([&]{a.read_word(p);},"Odd word rejected");rejects([&]{a.write_word(p,0);},"Odd write rejected before mutation");}check(a.nvram_image()==saved,"Invalid pairs preserve NVRAM");
 rejects([&]{Map invalid(std::span<const uint8_t>(rom.data(),rom.size()-1));},"Short ROM rejected");rejects([&]{a.load_nvram(std::span<const uint8_t>(saved.data(),saved.size()-1));},"Short NVRAM rejected before changes");check(a.nvram_image()==saved,"Invalid NVRAM import preserves previous state");b.load_nvram(saved);check(b.nvram_image()==saved,"Exact NVRAM roundtrip");a.clear_internal_ram();check(a.nvram_image()==saved,"Explicit internal clear preserves persistent RAM");for(unsigned i=0;i<512;++i)check(a.read_byte(0xfffd10+i)==0,"Internal RAM clear");
 const std::vector<uint16_t> code{0x7a00,0x20,1,0xf95a,0x6889,0x680a,0x7a03,0xff,0xfd10,0x7904,0x89ab,0x69b4,0x6935,0x7a06,0x1234,0x5678,0x0100,0x6ba6,0x23,0xfffc,0x0100,0x6b27,0x23,0xfffc,0x40fe};
 for(unsigned cut=0;cut<=240;++cut){auto bytes=image(code);Host x(bytes),y(bytes);yamanix::H8FirstInstructions<Host>cx(x),cy(y);cx.run(0,240,10000);cy.run(0,int(cut),10000);cy.run(cut,int(240-cut),10000);check(cx.snapshot()==cy.snapshot()&&x.memory.nvram_image()==y.memory.nvram_image(),"241 actual-map CPU reset/load/store partitions");for(unsigned i=0;i<512;++i)check(x.memory.read_byte(0xfffd10+i)==y.memory.read_byte(0xfffd10+i),"Internal RAM identical across every split");check(cx.snapshot().registers[2]==0x5a&&cx.snapshot().registers[5]==0x89ab&&cx.snapshot().registers[7]==0x5678&&cx.snapshot().registers[15]==0x1234,"CPU byte/word/long results through single mapped bus");check(x.memory.read_word(0xfffd10)==0x89ab&&x.memory.read_word(0x23fffc)==0x1234&&x.memory.read_word(0x23fffe)==0x5678,"CPU writes at internal/external RAM boundaries");}
 {auto bytes=image({0xf955,0x6a89,0x0001});Host h(bytes);yamanix::H8FirstInstructions<Host>c(h);rejects([&]{c.run(0,80,1000);},"CPU ROM store rejected");check(c.failed()&&h.memory.read_byte(1)==0,"Rejected store poisons CPU without changing reset ROM");}
 std::printf("AN1x memory map:full ROM/RAM sweeps,aliases,boundaries,ROM protection,persistence/isolation,241 CPU partitions:%u failures\n",failures);return failures?1:0;
}
