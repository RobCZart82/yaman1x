// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Minimal advanced-H8H execution host adapted from pinned MAME helpers.
// See research/h8-first-instructions-provenance.json. NOT a complete CPU.
#pragma once
#include "h8_execution_runner.h"
#include "h8h_bus_access.h"
#include <array>

namespace yamanix {
// Host: read_instruction_word/read_byte/write_byte, update(cycle)->next deadline.
// Fixed profile: advanced H8H, no IRQ/DMA/DTC/trace/retry/standby support.
// Bus must perform completed synchronous instruction reads and own address mapping.
// Both host and immutable code memory must outlive the instance; no reentry.
template<class Host> class H8FirstInstructions {
public:
    struct Snapshot {
        std::array<uint16_t,16> registers;
        uint32_t pc, ppc, npc, state, substate, tmp1, tmp2;
        std::array<uint16_t,3> ir;
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
    using u8=uint8_t; using s8=int8_t; using s32=int32_t;
    static constexpr u8 F_I=0x80,F_H=0x20,F_N=0x08,F_Z=0x04,F_V=0x02,F_C=0x01;
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
        } else if((m_inst_state&0xf000)==0x4000 && m_inst_state<=0x4fff) {
            switch((m_inst_state>>8)&15) {
            case 0: if(partial)bt_rel8_partial();else bt_rel8_full();break;
            case 1: if(partial)bf_rel8_partial();else bf_rel8_full();break;
            case 2: if(partial)bhi_rel8_partial();else bhi_rel8_full();break;
            case 3: if(partial)bls_rel8_partial();else bls_rel8_full();break;
            case 4: if(partial)bcc_rel8_partial();else bcc_rel8_full();break;
            case 5: if(partial)bcs_rel8_partial();else bcs_rel8_full();break;
            case 6: if(partial)bne_rel8_partial();else bne_rel8_full();break;
            case 7: if(partial)beq_rel8_partial();else beq_rel8_full();break;
            case 8: if(partial)bvc_rel8_partial();else bvc_rel8_full();break;
            case 9: if(partial)bvs_rel8_partial();else bvs_rel8_full();break;
            case 10: if(partial)bpl_rel8_partial();else bpl_rel8_full();break;
            case 11: if(partial)bmi_rel8_partial();else bmi_rel8_full();break;
            case 12: if(partial)bge_rel8_partial();else bge_rel8_full();break;
            case 13: if(partial)blt_rel8_partial();else blt_rel8_full();break;
            case 14: if(partial)bgt_rel8_partial();else bgt_rel8_full();break;
            case 15: if(partial)ble_rel8_partial();else ble_rel8_full();break;
            }
        } else if((m_inst_state&0xfff8)==0x7a00) {
            if(partial)mov_l_imm32_r32l_partial();else mov_l_imm32_r32l_full();
        } else if((m_inst_state&0xff00)==0x6800) {
            if(m_inst_state&0x80) {if(partial)mov_b_r8l_r32ih_partial();else mov_b_r8l_r32ih_full();}
            else {if(partial)mov_b_r32ih_r8l_partial();else mov_b_r32ih_r8l_full();}
        } else throw std::runtime_error("Unsupported first-H8 instruction");
    }
    uint16_t read16i(uint32_t address){return bus_.read16i(address);}
    uint8_t read8(uint32_t address){return bus_.read8(address);}
    void write8(uint32_t address,uint8_t data){bus_.write8(address,data);}
    bool access_to_be_redone()const{return false;}
    void update_irq_filter() {} // IRQ entry is not supported by this fixed profile.
    void prefetch_done(){m_inst_state=m_IR[0]=m_PIR;}
    void prefetch_done_noirq(){prefetch_done();}
    void prefetch_switch(uint32_t pc,uint16_t ir){m_NPC=pc&0xffffff;m_PC=pc+2;m_PIR=ir;}
    void set_nzv8(u8 v){m_CCR&=~(F_N|F_V|F_Z);if(!v)m_CCR|=F_Z;else if(s8(v)<0)m_CCR|=F_N;}
    void r8_w(int reg,u8 val){if(reg&8)m_R[reg&7]=(m_R[reg&7]&0xff00)|val;else m_R[reg&7]=(m_R[reg&7]&0xff)|(uint16_t(val)<<8);}
    uint8_t r8_r(int reg)const{return reg&8?uint8_t(m_R[reg&7]):uint8_t(m_R[reg&7]>>8);}
    uint32_t r32_r(int reg)const{return uint32_t(m_R[reg&7])|(uint32_t(m_R[(reg&7)|8])<<16);}
    void r32_w(int reg,uint32_t value){m_R[reg&7]=uint16_t(value);m_R[(reg&7)|8]=uint16_t(value>>16);}
    void set_nzv32(uint32_t value){m_CCR&=~(F_N|F_V|F_Z);if(!value)m_CCR|=F_Z;else if(s32(value)<0)m_CCR|=F_N;}
    Host& host_;
    H8ExecutionRunner runner_;
    int& m_icount;
    H8hBusAccess<Host> bus_;
    int m_bcount=0;
    std::array<uint16_t,16> m_R{};
    uint32_t m_PC=0,m_PPC=0,m_NPC=0,m_inst_state=0x10000,m_inst_substate=0,m_TMP1=0,m_TMP2=0;
    std::array<uint16_t,3> m_IR{};
    uint16_t m_PIR=0;
    uint8_t m_CCR=0,m_EXR=0;
#include "h8_first_handlers.h"
#include "h8_first_h8h_handlers.h"
};
} // namespace yamanix
