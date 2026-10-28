// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Minimal advanced-H8H execution host adapted from pinned MAME helpers.
// See research/h8-first-instructions-provenance.json. NOT a complete CPU.
#pragma once
#include "h8_execution_runner.h"
#include "h8h_bus_access.h"
#include <array>

namespace yamanix {
// Host: read_instruction_word, read/write_byte, read/write_word, update(cycle).
// Fixed profile: advanced H8H, no IRQ/DMA/DTC/trace/retry/standby support.
// Bus accesses complete synchronously; the host owns address mapping.
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
        else if((m_inst_state&0xffff0000)==0x20000) {
            if((m_inst_state&0xff08)!=0x6900)throw std::runtime_error("Unsupported H8 long-memory extension");
            if(m_inst_state&0x80) {if(partial)mov_l_r32l_r32ih_partial();else mov_l_r32l_r32ih_full();}
            else {if(partial)mov_l_r32ih_r32l_partial();else mov_l_r32ih_r32l_full();}
        }
        else if((m_inst_state&0xffff0000)==0x110000) {
            const auto code=m_inst_state&0xff88;
            if(code==0x6400) {if(partial)or_l_r32h_r32l_partial();else or_l_r32h_r32l_full();}
            else if(code==0x6500) {if(partial)xor_l_r32h_r32l_partial();else xor_l_r32h_r32l_full();}
            else if(code==0x6600) {if(partial)and_l_r32h_r32l_partial();else and_l_r32h_r32l_full();}
            else throw std::runtime_error("Unsupported H8 long-logic extension");
        }
        else if(m_inst_state==0x01f0) {if(partial)dispatch_01f0_partial();else dispatch_01f0_full();}
        else if((m_inst_state&0xff00)==0x6400) {if(partial)or_w_r16h_r16l_partial();else or_w_r16h_r16l_full();}
        else if((m_inst_state&0xff00)==0x6500) {if(partial)xor_w_r16h_r16l_partial();else xor_w_r16h_r16l_full();}
        else if((m_inst_state&0xff00)==0x6600) {if(partial)and_w_r16h_r16l_partial();else and_w_r16h_r16l_full();}
        else if(m_inst_state>0xffff)throw std::runtime_error("Unsupported H8 internal instruction state");
        else if(m_inst_state==0x0100) {if(partial)dispatch_0100_partial();else dispatch_0100_full();}
        else if((m_inst_state&0xff00)==0x0700) {if(partial)ldc_imm8_ccr_partial();else ldc_imm8_ccr_full();}
        else if((m_inst_state&0xff00)==0x0900) {if(partial)add_w_r16h_r16l_partial();else add_w_r16h_r16l_full();}
        else if((m_inst_state&0xff00)==0x1900) {if(partial)sub_w_r16h_r16l_partial();else sub_w_r16h_r16l_full();}
        else if((m_inst_state&0xff00)==0x1d00) {if(partial)cmp_w_r16h_r16l_partial();else cmp_w_r16h_r16l_full();}
        else if((m_inst_state&0xff88)==0x0a80) {if(partial)add_l_r32h_r32l_partial();else add_l_r32h_r32l_full();}
        else if((m_inst_state&0xff88)==0x1a80) {if(partial)sub_l_r32h_r32l_partial();else sub_l_r32h_r32l_full();}
        else if((m_inst_state&0xff88)==0x1f80) {if(partial)cmp_l_r32h_r32l_partial();else cmp_l_r32h_r32l_full();}
        else if((m_inst_state&0xf000)==0x9000) {if(partial)addx_b_imm8_r8u_partial();else addx_b_imm8_r8u_full();}
        else if((m_inst_state&0xf000)==0xb000) {if(partial)subx_b_imm8_r8u_partial();else subx_b_imm8_r8u_full();}
        else if((m_inst_state&0xff00)==0x0e00) {if(partial)addx_b_r8h_r8l_partial();else addx_b_r8h_r8l_full();}
        else if((m_inst_state&0xff00)==0x1e00) {if(partial)subx_b_r8h_r8l_partial();else subx_b_r8h_r8l_full();}
        else if((m_inst_state&0xf000)==0xc000) {if(partial)or_b_imm8_r8u_partial();else or_b_imm8_r8u_full();}
        else if((m_inst_state&0xf000)==0xd000) {if(partial)xor_b_imm8_r8u_partial();else xor_b_imm8_r8u_full();}
        else if((m_inst_state&0xf000)==0xe000) {if(partial)and_b_imm8_r8u_partial();else and_b_imm8_r8u_full();}
        else if((m_inst_state&0xff00)==0x1400) {if(partial)or_b_r8h_r8l_partial();else or_b_r8h_r8l_full();}
        else if((m_inst_state&0xff00)==0x1500) {if(partial)xor_b_r8h_r8l_partial();else xor_b_r8h_r8l_full();}
        else if((m_inst_state&0xff00)==0x1600) {if(partial)and_b_r8h_r8l_partial();else and_b_r8h_r8l_full();}
        else if((m_inst_state&0xf000)==0x8000) {if(partial)add_b_imm8_r8u_partial();else add_b_imm8_r8u_full();}
        else if((m_inst_state&0xf000)==0xa000) {if(partial)cmp_b_imm8_r8u_partial();else cmp_b_imm8_r8u_full();}
        else if((m_inst_state&0xff00)==0x0800) {if(partial)add_b_r8h_r8l_partial();else add_b_r8h_r8l_full();}
        else if((m_inst_state&0xff00)==0x1800) {if(partial)sub_b_r8h_r8l_partial();else sub_b_r8h_r8l_full();}
        else if((m_inst_state&0xff00)==0x1c00) {if(partial)cmp_b_r8h_r8l_partial();else cmp_b_r8h_r8l_full();}
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
        } else if((m_inst_state&0xff00)==0x7900 && (m_inst_state&0xf0)<=0x60) {
            switch((m_inst_state>>4)&7) {
            case 0:if(partial)mov_w_imm16_r16l_partial();else mov_w_imm16_r16l_full();break;
            case 1:if(partial)add_w_imm16_r16l_partial();else add_w_imm16_r16l_full();break;
            case 2:if(partial)cmp_w_imm16_r16l_partial();else cmp_w_imm16_r16l_full();break;
            case 3:if(partial)sub_w_imm16_r16l_partial();else sub_w_imm16_r16l_full();break;
            case 4:if(partial)or_w_imm16_r16l_partial();else or_w_imm16_r16l_full();break;
            case 5:if(partial)xor_w_imm16_r16l_partial();else xor_w_imm16_r16l_full();break;
            case 6:if(partial)and_w_imm16_r16l_partial();else and_w_imm16_r16l_full();break;
            }
        } else if((m_inst_state&0xff00)==0x6900) {
            if(m_inst_state&0x80) {if(partial)mov_w_r16l_r32ih_partial();else mov_w_r16l_r32ih_full();}
            else {if(partial)mov_w_r32ih_r16l_partial();else mov_w_r32ih_r16l_full();}
        } else if((m_inst_state&0xff08)==0x7a00 && (m_inst_state&0xf0)<=0x60) {
            switch((m_inst_state>>4)&7) {
            case 0:if(partial)mov_l_imm32_r32l_partial();else mov_l_imm32_r32l_full();break;
            case 1:if(partial)add_l_imm32_r32l_partial();else add_l_imm32_r32l_full();break;
            case 2:if(partial)cmp_l_imm32_r32l_partial();else cmp_l_imm32_r32l_full();break;
            case 3:if(partial)sub_l_imm32_r32l_partial();else sub_l_imm32_r32l_full();break;
            case 4:if(partial)or_l_imm32_r32l_partial();else or_l_imm32_r32l_full();break;
            case 5:if(partial)xor_l_imm32_r32l_partial();else xor_l_imm32_r32l_full();break;
            case 6:if(partial)and_l_imm32_r32l_partial();else and_l_imm32_r32l_full();break;
            }
        } else if((m_inst_state&0xff00)==0x6800) {
            if(m_inst_state&0x80) {if(partial)mov_b_r8l_r32ih_partial();else mov_b_r8l_r32ih_full();}
            else {if(partial)mov_b_r32ih_r8l_partial();else mov_b_r32ih_r8l_full();}
        } else throw std::runtime_error("Unsupported first-H8 instruction");
    }
    uint16_t read16i(uint32_t address){return bus_.read16i(address);}
    uint8_t read8(uint32_t address){return bus_.read8(address);}
    void write8(uint32_t address,uint8_t data){bus_.write8(address,data);}
    uint16_t read16(uint32_t address){return bus_.read16(address);}
    void write16(uint32_t address,uint16_t data){bus_.write16(address,data);}
    bool access_to_be_redone()const{return false;}
    void update_irq_filter() {} // IRQ entry is not supported by this fixed profile.
    void prefetch_done(){m_inst_state=m_IR[0]=m_PIR;}
    void prefetch_done_noirq(){prefetch_done();}
    void prefetch_switch(uint32_t pc,uint16_t ir){m_NPC=pc&0xffffff;m_PC=pc+2;m_PIR=ir;}
    void set_nzv8(u8 v){m_CCR&=~(F_N|F_V|F_Z);if(!v)m_CCR|=F_Z;else if(s8(v)<0)m_CCR|=F_N;}
    void r8_w(int reg,u8 val){if(reg&8)m_R[reg&7]=(m_R[reg&7]&0xff00)|val;else m_R[reg&7]=(m_R[reg&7]&0xff)|(uint16_t(val)<<8);}
    uint16_t r16_r(int reg)const{return m_R[reg&15];}
    void r16_w(int reg,uint16_t value){m_R[reg&15]=value;}
    void set_nzv16(uint16_t value){m_CCR&=~(F_N|F_V|F_Z);if(!value)m_CCR|=F_Z;else if(value&0x8000)m_CCR|=F_N;}
    uint8_t r8_r(int reg)const{return reg&8?uint8_t(m_R[reg&7]):uint8_t(m_R[reg&7]>>8);}
    uint32_t r32_r(int reg)const{return uint32_t(m_R[reg&7])|(uint32_t(m_R[(reg&7)|8])<<16);}
    void r32_w(int reg,uint32_t value){m_R[reg&7]=uint16_t(value);m_R[(reg&7)|8]=uint16_t(value>>16);}
    void set_nzv32(uint32_t value){m_CCR&=~(F_N|F_V|F_Z);if(!value)m_CCR|=F_Z;else if(s32(value)<0)m_CCR|=F_N;}
    // Fixed H8H half-carry boundary: bits 4/12/28 for 8/16/32-bit operations.
    template<unsigned Bits> uint32_t arithmetic(uint32_t left,uint32_t right,bool subtract) {
        static_assert(Bits==8 || Bits==16 || Bits==32);
        const uint64_t mask=(uint64_t{1}<<Bits)-1,sign=uint64_t{1}<<(Bits-1);
        const uint64_t half_mask=(uint64_t{1}<<(Bits-4))-1;
        const uint64_t a=left&mask,b=right&mask;
        const uint64_t wide=subtract?a-b:a+b;
        const uint64_t result=wide&mask;
        const bool half=subtract?((a&half_mask)<(b&half_mask)):((a&half_mask)+(b&half_mask)>half_mask);
        m_CCR&=~(F_H|F_N|F_Z|F_V|F_C);
        if(!result)m_CCR|=F_Z;
        if(result&sign)m_CCR|=F_N;
        if((subtract?(a^b):~(a^b))&(a^result)&sign)m_CCR|=F_V;
        if(subtract?a<b:wide>mask)m_CCR|=F_C;
        if(half)m_CCR|=F_H;
        return uint32_t(result);
    }
    uint8_t do_add8(uint8_t a,uint8_t b){return uint8_t(arithmetic<8>(a,b,false));}
    uint8_t do_sub8(uint8_t a,uint8_t b){return uint8_t(arithmetic<8>(a,b,true));}
    uint16_t do_add16(uint16_t a,uint16_t b){return uint16_t(arithmetic<16>(a,b,false));}
    uint16_t do_sub16(uint16_t a,uint16_t b){return uint16_t(arithmetic<16>(a,b,true));}
    uint32_t do_add32(uint32_t a,uint32_t b){return arithmetic<32>(a,b,false);}
    uint32_t do_sub32(uint32_t a,uint32_t b){return arithmetic<32>(a,b,true);}
    uint8_t extended_byte(uint8_t a,uint8_t b,bool subtract) {
        const unsigned carry=(m_CCR&F_C)?1:0;
        const uint16_t wide=subtract?uint16_t(int(a)-int(b)-int(carry)):uint16_t(unsigned(a)+b+carry);
        const uint8_t result=uint8_t(wide);
        const bool half=subtract?((a&15)<(b&15)+carry):((a&15)+(b&15)+carry>=16);
        // Z is sticky: a zero result preserves it; any nonzero byte clears it.
        m_CCR&=~(F_H|F_N|F_V|F_C);
        if(result)m_CCR&=~F_Z;
        if(result&0x80)m_CCR|=F_N;
        if((subtract?(a^b):~(a^b))&(a^result)&0x80)m_CCR|=F_V;
        if(subtract?unsigned(a)<unsigned(b)+carry:unsigned(a)+b+carry>255)m_CCR|=F_C;
        if(half)m_CCR|=F_H;
        return result;
    }
    uint8_t do_addx8(uint8_t a,uint8_t b){return extended_byte(a,b,false);}
    uint8_t do_subx8(uint8_t a,uint8_t b){return extended_byte(a,b,true);}
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
