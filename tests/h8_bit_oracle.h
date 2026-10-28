#pragma once
#include <array>
#include <cstdint>
struct Result{uint8_t value,ccr;};
static Result oracle(unsigned op,unsigned v,unsigned index,unsigned flags){
 std::array<bool,8> b{};for(unsigned i=0;i<8;++i)b[i]=(v/(1u<<i))%2;const bool old=b[index],carry=flags%2;bool c=carry;
 switch(op){case 0:b[index]=true;break;case 1:b[index]=!old;break;case 2:b[index]=false;break;case 3:flags=(flags&251)|(!old?4:0);break;case 4:b[index]=carry;break;case 5:b[index]=!carry;break;case 6:c=carry||old;break;case 7:c=carry||!old;break;case 8:c=carry!=old;break;case 9:c=carry==old;break;case 10:c=carry&&old;break;case 11:c=carry&&!old;break;case 12:c=old;break;case 13:c=!old;break;}
 if(op>=6)flags=(flags&254)|unsigned(c);unsigned value=0;for(unsigned i=0;i<8;++i)if(b[i])value+=1u<<i;return {uint8_t(value),uint8_t(flags)};
}
static const unsigned codes[]={0x7000,0x7100,0x7200,0x7300,0x6700,0x6780,0x7400,0x7480,0x7500,0x7580,0x7600,0x7680,0x7700,0x7780};
