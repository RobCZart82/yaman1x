#include "h83002_syscr_modes.h"
#include <cstdio>
using Mode=yamanix::H83002SyscrModes;
int main(){unsigned failures=0;const unsigned table[2][4]={{0,0,1,2},{0,0,2,2}};
 for(unsigned sys=0;sys<256;++sys)for(unsigned ccr=0;ccr<256;++ccr){
  const auto index=((ccr>>6)&3);const auto bank=(sys>>3)&1;
  failures+=Mode::irq_filter(uint8_t(sys),uint8_t(ccr))!=table[bank][index];
  // Independent per-bit entry oracle: I set,UI set only in dual-mask mode;
  // every other bit,and a previously set UI in single-mask mode,preserved.
  unsigned expected=0;for(unsigned bit=0;bit<8;++bit){bool set=(ccr&(1u<<bit))!=0;if(bit==7||(bit==6&&!bank))set=true;if(set)expected|=1u<<bit;}
  failures+=Mode::entry_ccr(uint8_t(sys),uint8_t(ccr))!=expected;
  failures+=Mode::entry_ccr(uint8_t(sys),Mode::entry_ccr(uint8_t(sys),uint8_t(ccr)))!=expected;
  failures+=Mode::irq_filter(uint8_t(sys),uint8_t(ccr))!=Mode::irq_filter(uint8_t(sys^0xf7),uint8_t(ccr));
 }
 failures+=Mode::cold_value!=9||Mode::trapa_base!=8;
 std::printf("SYSCR pure contract:65536 mode/CCR combinations,entry preservation/idempotence and unrelated SYSCR-bit controls:%u failures\n",failures);return failures?1:0;
}
