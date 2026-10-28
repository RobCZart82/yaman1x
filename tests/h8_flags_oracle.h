#pragma once
#include <cstdint>
struct Expected{uint32_t value;uint8_t ccr;};
// Independent signed-range/modular oracle, not the source's xor overflow formula.
static Expected expected(unsigned bits,uint32_t a,uint32_t b,bool sub,unsigned seed,bool carry_input=false,bool reset_i=true){
 const uint64_t modulus=uint64_t{1}<<bits,half=uint64_t{1}<<(bits-4),sign=modulus/2;
 a=uint32_t(a%modulus);b=uint32_t(b%modulus);
 const int64_t sa=a>=sign?int64_t(a)-int64_t(modulus):a,sb=b>=sign?int64_t(b)-int64_t(modulus):b;
 const unsigned carry=carry_input && (seed&1)?1:0;
 const int64_t signed_result=sub?sa-sb-carry:sa+sb+carry;
 const uint64_t result=sub?(uint64_t(a)+modulus-b-carry)%modulus:(uint64_t(a)+b+carry)%modulus;
 unsigned flags=(seed|(reset_i?0x80:0))&0xd0;
 if(!result && (!carry_input || (seed&4)))flags|=4;
 if(result>=sign)flags|=8;
 if(signed_result < -int64_t(sign) || signed_result>=int64_t(sign))flags|=2;
 if(sub?(uint64_t(a)<uint64_t(b)+carry):(uint64_t(a)+b+carry>=modulus))flags|=1;
 if(sub?((a%half)<(b%half)+carry):((a%half)+(b%half)+carry>=half))flags|=0x20;
 return {uint32_t(result),uint8_t(flags)};
}
