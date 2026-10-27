#include "h8_sci_status.h"
#include <cstdio>

int main() {
    using S=yamanix::H8SciStatus;
    unsigned failures=0;
    auto check=[&](bool ok,const char* name) {
        if(!ok && ++failures<8) std::fprintf(stderr,"%s\n",name);
    };
    // All observed initial bytes, newly raised flags, software writes and TE states.
    // Independent per-bit oracle instead of restating the upstream mask expression.
    unsigned long long cases=0;
    for(unsigned initial=0;initial<256;++initial) {
        S base;
        base.read(); base.write(0,true); // Clear reset TDRE/TEND.
        base.write(uint8_t(initial&1),false);
        base.raise_flags(uint8_t(initial));
        base.read();
        for(unsigned raised=0;raised<256;++raised)
        for(unsigned data=0;data<256;++data)
        for(unsigned te=0;te<2;++te) {
            S s=base;
            s.raise_flags(uint8_t(raised));
            const unsigned before=initial|(raised&254);
            unsigned expected=data&1;
            for(unsigned bit=1;bit<8;++bit) {
                const unsigned mask=1u<<bit;
                bool retained=(before&mask)!=0;
                if(bit==7 || bit==2) {
                    if(te && (initial&128) && !(data&128)) retained=false;
                } else if(bit!=1 && (initial&mask) && !(data&mask)) retained=false;
                if(retained) expected|=mask;
            }
            s.write(uint8_t(data),te!=0);
            check(s.value()==expected,"SSR per-bit exhaustive oracle");
            check(s.read_latch()==(initial&expected),"Read latch follows cleared flags");
            check(s.receive_error()==bool(expected&0x38),"Receive error predicate");
            ++cases;
        }
    }
    S s;
    s.write(0,true); check(s.value()==0x84,"No read: TDRE not acknowledged");
    s.read(false); s.write(0,true); check(s.value()==0x84,"Debugger read cannot acknowledge");
    s.read(); s.write(0,false); check(s.value()==0x84,"TE disabled preserves transmit flags");
    s.write(0,true); check(s.value()==0,"Observed TDRE clears both transmit flags");
    s.raise_flags(S::rdrf); s.write(0,false);
    check(s.value()==S::rdrf,"New unobserved receive flag retained");
    s.read(); s.dma_receive_read(false); check(s.value()==S::rdrf,"Debugger DMA read inert");
    s.dma_receive_read(); check(s.value()==0 && s.read_latch()==0,"DMA clears receive and latch");
    s.reset(); s.read(); s.dma_transmit_write(false); check(s.value()==0x84,"DMA TE gate");
    s.dma_transmit_write(true); check(s.value()==0 && s.read_latch()==0,"DMA transmit acknowledgement");
    s.raise_flags(S::orer|S::fer|S::per|S::mpb); s.read();
    s.reset(); check(s.value()==0x84 && s.read_latch()==0x3a,"Reset retains upstream read latch");
    S other; other.raise_flags(S::rdrf);
    check(s.value()==0x84 && other.value()==0xc4,"Instance isolation");
    s.raise_flags(S::mpbt); check(!(s.value()&S::mpbt),"MPBT software-only");
    std::printf("%llu SSR transitions, %u failures\n",cases,failures);
    return failures?1:0;
}
