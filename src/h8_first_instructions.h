// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Minimal advanced-H8H execution host adapted from pinned MAME helpers.
// See research/h8-first-instructions-provenance.json. NOT a complete CPU.
#pragma once
#include "h8_execution_runner.h"
#include "h8h_bus_access.h"
#include <array>

namespace yamanix {
// Host: read_instruction_word(address), update(cycle)->next absolute deadline.
// Fixed profile: advanced H8H, no IRQ/DMA/DTC/trace/retry/standby support.
// Bus must perform completed synchronous instruction reads and own address mapping.
// Both host and immutable code memory must outlive the instance; no reentry.
template<class Host> class H8FirstInstructions {
public:
    struct Snapshot {
        std::array<uint16_t,16> registers;
        uint32_t pc, ppc, npc, state, substate, tmp1, tmp2;
        std::array<uint16_t,2> ir;
        uint16_t pir;
        uint8_t ccr, exr;
        int remaining, debt;
        bool operator==(const Snapshot&) const = default;
    };
    // Deterministic research cold state, not verified hardware power-on values.
    explicit H8FirstInstructions(Host& host, uint8_t initial_ccr=0)
        : host_(host), m_icount(runner_.counter()), bus_(host,m_icount), m_CCR(initial_ccr) {}
    H8FirstInstructions(const H8FirstInstructions&)=delete;
    H8FirstInstructions& operator=(const H8FirstInstructions&)=delete;
    std::size_t run(uint64_t start,int cycles,std::size_t limit) {
        Owner owner{*this};return runner_.run(start,cycles,limit,owner);
    }
    bool failed() const {return runner_.failed();}
    Snapshot snapshot() const {
        return {m_R,m_PC,m_PPC,m_NPC,m_inst_state,m_inst_substate,m_TMP1,m_TMP2,
                m_IR,m_PIR,m_CCR,m_EXR,m_icount,runner_.debt()};
    }
private:
    using u8=uint8_t; using s8=int8_t;
    static constexpr u8 F_I=0x80,F_H=0x20,F_N=0x08,F_Z=0x04,F_V=0x02;
    static constexpr u8 EXR_I=0x07,EXR_NC=0x78;
    static constexpr bool m_has_hc=true,m_mode_advanced=true;
    struct Owner {
        H8FirstInstructions& cpu;
        uint64_t update(uint64_t cycle){return cpu.host_.update(cycle);}
        bool has_partial()const{return cpu.m_inst_substate!=0;}
        void full(H8ExecutionBudget& b){
            cpu.m_bcount=b.boundary();
            if(cpu.m_inst_state<0x10000)cpu.m_PPC=cpu.m_NPC;
            cpu.dispatch(false);
        }
        void partial(H8ExecutionBudget& b){cpu.m_bcount=b.boundary();cpu.dispatch(true);}
    };
    void dispatch(bool partial) {
        if(m_inst_state==0x10000) {if(partial)state_reset_partial();else state_reset_full();}
        else if(m_inst_state==0) {if(partial)nop_partial();else nop_full();}
        else if((m_inst_state&0xf000)==0xf000 && m_inst_state<=0xffff) {
            if(partial)mov_b_imm8_r8u_partial();else mov_b_imm8_r8u_full();
        } else if((m_inst_state&0xff00)==0x4000 && m_inst_state<=0xffff) {
            if(partial)bt_rel8_partial();else bt_rel8_full();
        } else throw std::runtime_error("Unsupported first-H8 instruction");
    }
    uint16_t read16i(uint32_t address){return bus_.read16i(address);}
    bool access_to_be_redone()const{return false;}
    void update_irq_filter() {} // IRQ entry is not supported by this fixed profile.
    void prefetch_done(){m_inst_state=m_IR[0]=m_PIR;}
    void prefetch_done_noirq(){prefetch_done();}
    void prefetch_switch(uint32_t pc,uint16_t ir){m_NPC=pc&0xffffff;m_PC=pc+2;m_PIR=ir;}
    void set_nzv8(u8 v){m_CCR&=~(F_N|F_V|F_Z);if(!v)m_CCR|=F_Z;else if(s8(v)<0)m_CCR|=F_N;}
    void r8_w(int reg,u8 val){if(reg&8)m_R[reg&7]=(m_R[reg&7]&0xff00)|val;else m_R[reg&7]=(m_R[reg&7]&0xff)|(uint16_t(val)<<8);}
    Host& host_;
    H8ExecutionRunner runner_;
    int& m_icount;
    H8hBusAccess<Host> bus_;
    int m_bcount=0;
    std::array<uint16_t,16> m_R{};
    uint32_t m_PC=0,m_PPC=0,m_NPC=0,m_inst_state=0x10000,m_inst_substate=0,m_TMP1=0,m_TMP2=0;
    std::array<uint16_t,2> m_IR{};
    uint16_t m_PIR=0;
    uint8_t m_CCR=0,m_EXR=0;
#include "h8_first_handlers.h"
};
} // namespace yamanix
