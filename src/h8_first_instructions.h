// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Minimal advanced-H8H execution host adapted from pinned MAME helpers.
// See research/h8-first-instructions-provenance.json. NOT a complete CPU.
#pragma once
#include "h8_execution_runner.h"
#include "h8h_bus_access.h"
#include "h8h_irq_controller.h"
#include <array>

namespace yamanix {
// Host: read_instruction_word, read/write_byte, read/write_word, update(cycle).
// Fixed profile: advanced H8H, reset-SYSCR TRAPA; optional external IRQ/NMI controller; no DMA/DTC/trace/retry/standby support.
// Bus accesses complete synchronously; the host owns address mapping.
// Both host and immutable code memory must outlive the instance; no reentry.
template<class Host> class H8FirstInstructions {
public:
    struct Snapshot {
        std::array<uint16_t,16> registers;
        uint32_t pc, ppc, npc, state, substate, tmp1, tmp2;
        std::array<uint16_t,4> ir;
        uint16_t pir;
        uint8_t ccr, exr;
        int remaining, debt, taken_vector, taken_level;
        bool operator==(const Snapshot&) const = default;
    };
    // Deterministic research cold state, not verified hardware power-on values.
    explicit H8FirstInstructions(Host& host, uint8_t initial_ccr=0, H8hIrqController* irq=nullptr)
        : host_(host), irq_(irq), m_icount(runner_.counter()), bus_(host,m_icount), m_CCR(initial_ccr) {}
    H8FirstInstructions(const H8FirstInstructions&)=delete;
    H8FirstInstructions& operator=(const H8FirstInstructions&)=delete;
    std::size_t run(uint64_t start,int cycles,std::size_t limit) {
        Owner owner{*this};return runner_.run(start,cycles,limit,owner);
    }
    bool failed() const {return runner_.failed();}
    Snapshot snapshot() const {
        return {m_R,m_PC,m_PPC,m_NPC,m_inst_state,m_inst_substate,m_TMP1,m_TMP2,
                m_IR,m_PIR,m_CCR,m_EXR,m_icount,runner_.debt(),m_taken_irq_vector,m_taken_irq_level};
    }
private:
    using u8=uint8_t; using u16=uint16_t; using u32=uint32_t; using s8=int8_t; using s16=int16_t; using s32=int32_t;
    static constexpr u8 F_I=0x80,F_UI=0x40,F_H=0x20,F_N=0x08,F_Z=0x04,F_V=0x02,F_C=0x01;
    static constexpr u8 EXR_I=0x07,EXR_NC=0x78;
    static constexpr bool m_has_hc=true,m_mode_advanced=true,m_has_mac=false;
    struct Owner {
        H8FirstInstructions& cpu;
        uint64_t update(uint64_t cycle){cpu.bus_.set_slice_end(cpu.runner_.slice_end());return cpu.host_.update(cycle);}
        bool has_partial()const{return cpu.m_inst_substate!=0;}
        void full(H8ExecutionBudget& b){
            cpu.m_bcount=b.boundary();
            if(cpu.m_inst_state<0x10000)cpu.m_PPC=cpu.m_NPC;
            cpu.dispatch(false);
        }
        void partial(H8ExecutionBudget& b){cpu.m_bcount=b.boundary();cpu.dispatch(true);}
    };
    void dispatch(bool partial) {
        // Flat first-match exits avoid MSVC C1061 from a deep else-if chain.
        if(m_inst_state==0x10000) {if(partial)state_reset_partial();else state_reset_full(); return;
        }
        if(m_inst_state==0x10001) {if(partial)state_irq_partial();else state_irq_full(); return;
        }
        if((m_inst_state&0xffff0000)==0x20000) {
            if((m_inst_state&0xff08)==0x6900) {
                if(m_inst_state&0x80) {if(partial)mov_l_r32l_r32ih_partial();else mov_l_r32l_r32ih_full();}
                else {if(partial)mov_l_r32ih_r32l_partial();else mov_l_r32ih_r32l_full();}
            } else if((m_inst_state&0xff08)==0x6d00) {
                if(m_inst_state&0x80) {if(partial)mov_l_r32l_pr32h_partial();else mov_l_r32l_pr32h_full();}
                else {if(partial)mov_l_r32ph_r32l_partial();else mov_l_r32ph_r32l_full();}
            } else if((m_inst_state&0xfff8)==0x6b00) {if(partial)mov_l_abs16_r32l_partial();else mov_l_abs16_r32l_full();
            } else if((m_inst_state&0xfff8)==0x6b80) {if(partial)mov_l_r32l_abs16_partial();else mov_l_r32l_abs16_full();
            } else if((m_inst_state&0xfff8)==0x6b20) {if(partial)mov_l_abs32_r32l_partial();else mov_l_abs32_r32l_full();
            } else if((m_inst_state&0xfff8)==0x6ba0) {if(partial)mov_l_r32l_abs32_partial();else mov_l_r32l_abs32_full();
            } else if((m_inst_state&0xff08)==0x6f00) {
                if(m_inst_state&0x80) {if(partial)mov_l_r32l_r32d16h_partial();else mov_l_r32l_r32d16h_full();}
                else {if(partial)mov_l_r32d16h_r32l_partial();else mov_l_r32d16h_r32l_full();}
            } else throw std::runtime_error("Unsupported H8 long-memory extension"); return;
        }
        if(m_inst_state==0x01c0) {if(partial)dispatch_01c0_partial();else dispatch_01c0_full();return;}
        if((m_inst_state&0xffff0000)==0xe0000) {
            if((m_inst_state&0xff00)==0x5000) {if(partial)mulxs_b_r8h_r16l_partial();else mulxs_b_r8h_r16l_full();}
            else if((m_inst_state&0xff08)==0x5200) {if(partial)mulxs_w_r16h_r32l_partial();else mulxs_w_r16h_r32l_full();}
            else throw std::runtime_error("Unsupported H8 signed multiply extension");return;
        }
        if((m_inst_state&0xffff0000)==0x110000) {
            const auto code=m_inst_state&0xff88;
            if(code==0x6400) {if(partial)or_l_r32h_r32l_partial();else or_l_r32h_r32l_full();}
            else if(code==0x6500) {if(partial)xor_l_r32h_r32l_partial();else xor_l_r32h_r32l_full();}
            else if(code==0x6600) {if(partial)and_l_r32h_r32l_partial();else and_l_r32h_r32l_full();}
            else throw std::runtime_error("Unsupported H8 long-logic extension"); return;
        }
        if(m_inst_state==0x01f0) {if(partial)dispatch_01f0_partial();else dispatch_01f0_full(); return;
        }
        if((m_inst_state&0xffff0000)==0x190000) {
            if((m_inst_state&0xff0f)==0x6300) {if(partial)btst_r8h_r32ihh_partial();else btst_r8h_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7300) {if(partial)btst_imm3_r32ihh_partial();else btst_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7400) {if(partial)bor_imm3_r32ihh_partial();else bor_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7480) {if(partial)bior_imm3_r32ihh_partial();else bior_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7500) {if(partial)bxor_imm3_r32ihh_partial();else bxor_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7580) {if(partial)bixor_imm3_r32ihh_partial();else bixor_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7600) {if(partial)band_imm3_r32ihh_partial();else band_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7680) {if(partial)biand_imm3_r32ihh_partial();else biand_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7700) {if(partial)bld_imm3_r32ihh_partial();else bld_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7780) {if(partial)bild_imm3_r32ihh_partial();else bild_imm3_r32ihh_full();}
            else throw std::runtime_error("Unsupported H8 indirect-bit extension"); return;
        }
        if((m_inst_state&0xffff0000)==0x1a0000) {
            if((m_inst_state&0xff0f)==0x6000) {if(partial)bset_r8h_r32ihh_partial();else bset_r8h_r32ihh_full();}
            else if((m_inst_state&0xff0f)==0x6100) {if(partial)bnot_r8h_r32ihh_partial();else bnot_r8h_r32ihh_full();}
            else if((m_inst_state&0xff0f)==0x6200) {if(partial)bclr_r8h_r32ihh_partial();else bclr_r8h_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7000) {if(partial)bset_imm3_r32ihh_partial();else bset_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7100) {if(partial)bnot_imm3_r32ihh_partial();else bnot_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x7200) {if(partial)bclr_imm3_r32ihh_partial();else bclr_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x6700) {if(partial)bst_imm3_r32ihh_partial();else bst_imm3_r32ihh_full();}
            else if((m_inst_state&0xff8f)==0x6780) {if(partial)bist_imm3_r32ihh_partial();else bist_imm3_r32ihh_full();}
            else throw std::runtime_error("Unsupported H8 indirect-bit extension"); return;
        }
        if((m_inst_state&0xffff0000)==0x1b0000) {
            if((m_inst_state&0xff0f)==0x6300) {if(partial)btst_r8h_abs8_partial();else btst_r8h_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7300) {if(partial)btst_imm3_abs8_partial();else btst_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7400) {if(partial)bor_imm3_abs8_partial();else bor_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7480) {if(partial)bior_imm3_abs8_partial();else bior_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7500) {if(partial)bxor_imm3_abs8_partial();else bxor_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7580) {if(partial)bixor_imm3_abs8_partial();else bixor_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7600) {if(partial)band_imm3_abs8_partial();else band_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7680) {if(partial)biand_imm3_abs8_partial();else biand_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7700) {if(partial)bld_imm3_abs8_partial();else bld_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7780) {if(partial)bild_imm3_abs8_partial();else bild_imm3_abs8_full();}
            else throw std::runtime_error("Unsupported H8 absolute-bit extension"); return;
        }
        if((m_inst_state&0xffff0000)==0x1c0000) {
            if((m_inst_state&0xff0f)==0x6000) {if(partial)bset_r8h_abs8_partial();else bset_r8h_abs8_full();}
            else if((m_inst_state&0xff0f)==0x6100) {if(partial)bnot_r8h_abs8_partial();else bnot_r8h_abs8_full();}
            else if((m_inst_state&0xff0f)==0x6200) {if(partial)bclr_r8h_abs8_partial();else bclr_r8h_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7000) {if(partial)bset_imm3_abs8_partial();else bset_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7100) {if(partial)bnot_imm3_abs8_partial();else bnot_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x7200) {if(partial)bclr_imm3_abs8_partial();else bclr_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x6700) {if(partial)bst_imm3_abs8_partial();else bst_imm3_abs8_full();}
            else if((m_inst_state&0xff8f)==0x6780) {if(partial)bist_imm3_abs8_partial();else bist_imm3_abs8_full();}
            else throw std::runtime_error("Unsupported H8 absolute-bit extension"); return;
        }
        if(m_inst_state>0xffff) {throw std::runtime_error("Unsupported H8 internal instruction state"); return;
        }
        if((m_inst_state&0xff08)==0x5200) {if(partial)mulxu_w_r16h_r32l_partial();else mulxu_w_r16h_r32l_full();return;}
        if((m_inst_state&0xff00)==0x5100) {if(partial)divxu_b_r8h_r16l_partial();else divxu_b_r8h_r16l_full();return;}
        if((m_inst_state&0xff00)==0x5000) {if(partial)mulxu_b_r8h_r16l_partial();else mulxu_b_r8h_r16l_full();return;}
        if((m_inst_state&0xff80)==0x6e00) {if(partial)mov_b_r32d16h_r8l_partial();else mov_b_r32d16h_r8l_full();return;}
        if((m_inst_state&0xff80)==0x6e80) {if(partial)mov_b_r8l_r32d16h_partial();else mov_b_r8l_r32d16h_full();return;}
        if((m_inst_state&0xff80)==0x6f00) {if(partial)mov_w_r32d16h_r16l_partial();else mov_w_r32d16h_r16l_full();return;}
        if((m_inst_state&0xff80)==0x6f80) {if(partial)mov_w_r16l_r32d16h_partial();else mov_w_r16l_r32d16h_full();return;}
        if((m_inst_state&0xf000)==0x2000) {if(partial)mov_b_abs8_r8u_partial();else mov_b_abs8_r8u_full();return;}
        if((m_inst_state&0xf000)==0x3000) {if(partial)mov_b_r8u_abs8_partial();else mov_b_r8u_abs8_full();return;}
        if((m_inst_state&0xfff0)==0x6a00) {if(partial)mov_b_abs16_r8l_partial();else mov_b_abs16_r8l_full();return;}
        if((m_inst_state&0xfff0)==0x6a80) {if(partial)mov_b_r8l_abs16_partial();else mov_b_r8l_abs16_full();return;}
        if((m_inst_state&0xfff0)==0x6b00) {if(partial)mov_w_abs16_r16l_partial();else mov_w_abs16_r16l_full();return;}
        if((m_inst_state&0xfff0)==0x6b80) {if(partial)mov_w_r16l_abs16_partial();else mov_w_r16l_abs16_full();return;}
        if((m_inst_state&0xfff0)==0x6a20) {if(partial)mov_b_abs32_r8l_partial();else mov_b_abs32_r8l_full();return;}
        if((m_inst_state&0xfff0)==0x6aa0) {if(partial)mov_b_r8l_abs32_partial();else mov_b_r8l_abs32_full();return;}
        if((m_inst_state&0xfff0)==0x6b20) {if(partial)mov_w_abs32_r16l_partial();else mov_w_abs32_r16l_full();return;}
        if((m_inst_state&0xfff0)==0x6ba0) {if(partial)mov_w_r16l_abs32_partial();else mov_w_r16l_abs32_full();return;}
        if((m_inst_state&0xff00)==0x6400) {if(partial)or_w_r16h_r16l_partial();else or_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xff00)==0x6500) {if(partial)xor_w_r16h_r16l_partial();else xor_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xff00)==0x6600) {if(partial)and_w_r16h_r16l_partial();else and_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1000) {if(partial)shll_b_r8l_partial();else shll_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1010) {if(partial)shll_w_r16l_partial();else shll_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1030) {if(partial)shll_l_r32l_partial();else shll_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1080) {if(partial)shal_b_r8l_partial();else shal_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1090) {if(partial)shal_w_r16l_partial();else shal_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x10b0) {if(partial)shal_l_r32l_partial();else shal_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1100) {if(partial)shlr_b_r8l_partial();else shlr_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1110) {if(partial)shlr_w_r16l_partial();else shlr_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1130) {if(partial)shlr_l_r32l_partial();else shlr_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1180) {if(partial)shar_b_r8l_partial();else shar_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1190) {if(partial)shar_w_r16l_partial();else shar_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x11b0) {if(partial)shar_l_r32l_partial();else shar_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1200) {if(partial)rotxl_b_r8l_partial();else rotxl_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1210) {if(partial)rotxl_w_r16l_partial();else rotxl_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1230) {if(partial)rotxl_l_r32l_partial();else rotxl_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1280) {if(partial)rotl_b_r8l_partial();else rotl_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1290) {if(partial)rotl_w_r16l_partial();else rotl_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x12b0) {if(partial)rotl_l_r32l_partial();else rotl_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1300) {if(partial)rotxr_b_r8l_partial();else rotxr_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1310) {if(partial)rotxr_w_r16l_partial();else rotxr_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1330) {if(partial)rotxr_l_r32l_partial();else rotxr_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1380) {if(partial)rotr_b_r8l_partial();else rotr_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1390) {if(partial)rotr_w_r16l_partial();else rotr_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x13b0) {if(partial)rotr_l_r32l_partial();else rotr_l_r32l_full(); return;
        }
        if((m_inst_state&0xff00)==0x6000) {if(partial)bset_r8h_r8l_partial();else bset_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x6100) {if(partial)bnot_r8h_r8l_partial();else bnot_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x6200) {if(partial)bclr_r8h_r8l_partial();else bclr_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x6300) {if(partial)btst_r8h_r8l_partial();else btst_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7000) {if(partial)bset_imm3_r8l_partial();else bset_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7100) {if(partial)bnot_imm3_r8l_partial();else bnot_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7200) {if(partial)bclr_imm3_r8l_partial();else bclr_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7300) {if(partial)btst_imm3_r8l_partial();else btst_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x6700) {if(partial)bst_imm3_r8l_partial();else bst_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x6780) {if(partial)bist_imm3_r8l_partial();else bist_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7400) {if(partial)bor_imm3_r8l_partial();else bor_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7480) {if(partial)bior_imm3_r8l_partial();else bior_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7500) {if(partial)bxor_imm3_r8l_partial();else bxor_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7580) {if(partial)bixor_imm3_r8l_partial();else bixor_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7600) {if(partial)band_imm3_r8l_partial();else band_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7680) {if(partial)biand_imm3_r8l_partial();else biand_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7700) {if(partial)bld_imm3_r8l_partial();else bld_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x7780) {if(partial)bild_imm3_r8l_partial();else bild_imm3_r8l_full(); return;
        }
        if((m_inst_state&0xff8f)==0x7c00) {if(partial)dispatch_7c00_partial();else dispatch_7c00_full(); return;
        }
        if((m_inst_state&0xff8f)==0x7d00) {if(partial)dispatch_7d00_partial();else dispatch_7d00_full(); return;
        }
        if((m_inst_state&0xff00)==0x7e00) {if(partial)dispatch_7e00_partial();else dispatch_7e00_full(); return;
        }
        if((m_inst_state&0xff00)==0x7f00) {if(partial)dispatch_7f00_partial();else dispatch_7f00_full(); return;
        }
        if((m_inst_state&0xffff)==0x5470) {if(partial)rts_partial();else rts_full(); return;
        }
        if((m_inst_state&0xffff)==0x5670) {if(partial)rte_partial();else rte_full(); return;
        }
        if((m_inst_state&0xff00)==0x5500) {if(partial)bsr_rel8_partial();else bsr_rel8_full(); return;
        }
        if((m_inst_state&0xffff)==0x5c00) {if(partial)bsr_rel16_partial();else bsr_rel16_full(); return;
        }
        if((m_inst_state&0xff8f)==0x5900) {if(partial)jmp_r32h_partial();else jmp_r32h_full(); return;
        }
        if((m_inst_state&0xff00)==0x5a00) {if(partial)jmp_abs24e_partial();else jmp_abs24e_full(); return;
        }
        if((m_inst_state&0xff00)==0x5b00) {if(partial)jmp_abs8i_partial();else jmp_abs8i_full(); return;
        }
        if((m_inst_state&0xff8f)==0x5d00) {if(partial)jsr_r32h_partial();else jsr_r32h_full(); return;
        }
        if((m_inst_state&0xff00)==0x5e00) {if(partial)jsr_abs24e_partial();else jsr_abs24e_full(); return;
        }
        if((m_inst_state&0xff00)==0x5f00) {if(partial)jsr_abs8i_partial();else jsr_abs8i_full(); return;
        }
        if((m_inst_state&0xff80)==0x6c00) {if(partial)mov_b_r32ph_r8l_partial();else mov_b_r32ph_r8l_full(); return;
        }
        if((m_inst_state&0xff80)==0x6c80) {if(partial)mov_b_r8l_pr32h_partial();else mov_b_r8l_pr32h_full(); return;
        }
        if((m_inst_state&0xff80)==0x6d00) {if(partial)mov_w_r32ph_r16l_partial();else mov_w_r32ph_r16l_full(); return;
        }
        if((m_inst_state&0xff80)==0x6d80) {if(partial)mov_w_r16l_pr32h_partial();else mov_w_r16l_pr32h_full(); return;
        }
        if((m_inst_state&0xff0f)==0x5800) {
            switch((m_inst_state>>4)&15) {
            case 0:if(partial)bt_rel16_partial();else bt_rel16_full();break;
            case 1:if(partial)bf_rel16_partial();else bf_rel16_full();break;
            case 2:if(partial)bhi_rel16_partial();else bhi_rel16_full();break;
            case 3:if(partial)bls_rel16_partial();else bls_rel16_full();break;
            case 4:if(partial)bcc_rel16_partial();else bcc_rel16_full();break;
            case 5:if(partial)bcs_rel16_partial();else bcs_rel16_full();break;
            case 6:if(partial)bne_rel16_partial();else bne_rel16_full();break;
            case 7:if(partial)beq_rel16_partial();else beq_rel16_full();break;
            case 8:if(partial)bvc_rel16_partial();else bvc_rel16_full();break;
            case 9:if(partial)bvs_rel16_partial();else bvs_rel16_full();break;
            case 10:if(partial)bpl_rel16_partial();else bpl_rel16_full();break;
            case 11:if(partial)bmi_rel16_partial();else bmi_rel16_full();break;
            case 12:if(partial)bge_rel16_partial();else bge_rel16_full();break;
            case 13:if(partial)blt_rel16_partial();else blt_rel16_full();break;
            case 14:if(partial)bgt_rel16_partial();else bgt_rel16_full();break;
            case 15:if(partial)ble_rel16_partial();else ble_rel16_full();break;
            } return;
        }
        if(m_inst_state==0x0100) {if(partial)dispatch_0100_partial();else dispatch_0100_full(); return;
        }
        if((m_inst_state&0xffcf)==0x5700) {if(partial)trapa_imm2_partial();else trapa_imm2_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1700) {if(partial)not_b_r8l_partial();else not_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1710) {if(partial)not_w_r16l_partial();else not_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1730) {if(partial)not_l_r32l_partial();else not_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1750) {if(partial)extu_w_r16l_partial();else extu_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1770) {if(partial)extu_l_r32l_partial();else extu_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1780) {if(partial)neg_b_r8l_partial();else neg_b_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1790) {if(partial)neg_w_r16l_partial();else neg_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x17b0) {if(partial)neg_l_r32l_partial();else neg_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x17d0) {if(partial)exts_w_r16l_partial();else exts_w_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x17f0) {if(partial)exts_l_r32l_partial();else exts_l_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x0a00) {if(partial)inc_b_one_r8l_partial();else inc_b_one_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1a00) {if(partial)dec_b_one_r8l_partial();else dec_b_one_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x0b50) {if(partial)inc_w_one_r16l_partial();else inc_w_one_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x0b70) {if(partial)inc_l_one_r32l_partial();else inc_l_one_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x0bd0) {if(partial)inc_w_two_r16l_partial();else inc_w_two_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x0bf0) {if(partial)inc_l_two_r32l_partial();else inc_l_two_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1b50) {if(partial)dec_w_one_r16l_partial();else dec_w_one_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1b70) {if(partial)dec_l_one_r32l_partial();else dec_l_one_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x1bd0) {if(partial)dec_w_two_r16l_partial();else dec_w_two_r16l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1bf0) {if(partial)dec_l_two_r32l_partial();else dec_l_two_r32l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x0b00) {if(partial)adds_l_one_r32l_partial();else adds_l_one_r32l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x0b80) {if(partial)adds_l_two_r32l_partial();else adds_l_two_r32l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x0b90) {if(partial)adds_l_four_r32l_partial();else adds_l_four_r32l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1b00) {if(partial)subs_l_one_r32l_partial();else subs_l_one_r32l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1b80) {if(partial)subs_l_two_r32l_partial();else subs_l_two_r32l_full(); return;
        }
        if((m_inst_state&0xfff8)==0x1b90) {if(partial)subs_l_four_r32l_partial();else subs_l_four_r32l_full(); return;
        }
        if((m_inst_state&0xff00)==0x0c00) {if(partial)mov_b_r8h_r8l_partial();else mov_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x0d00) {if(partial)mov_w_r16h_r16l_partial();else mov_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xff88)==0x0f80) {if(partial)mov_l_r32h_r32l_partial();else mov_l_r32h_r32l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x0200) {if(partial)stc_ccr_r8l_partial();else stc_ccr_r8l_full(); return;
        }
        if((m_inst_state&0xfff0)==0x0300) {if(partial)ldc_r8l_ccr_partial();else ldc_r8l_ccr_full(); return;
        }
        if((m_inst_state&0xff00)==0x0400) {if(partial)orc_imm8_ccr_partial();else orc_imm8_ccr_full(); return;
        }
        if((m_inst_state&0xff00)==0x0500) {if(partial)xorc_imm8_ccr_partial();else xorc_imm8_ccr_full(); return;
        }
        if((m_inst_state&0xff00)==0x0600) {if(partial)andc_imm8_ccr_partial();else andc_imm8_ccr_full(); return;
        }
        if((m_inst_state&0xff00)==0x0700) {if(partial)ldc_imm8_ccr_partial();else ldc_imm8_ccr_full(); return;
        }
        if((m_inst_state&0xff00)==0x0900) {if(partial)add_w_r16h_r16l_partial();else add_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1900) {if(partial)sub_w_r16h_r16l_partial();else sub_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1d00) {if(partial)cmp_w_r16h_r16l_partial();else cmp_w_r16h_r16l_full(); return;
        }
        if((m_inst_state&0xff88)==0x0a80) {if(partial)add_l_r32h_r32l_partial();else add_l_r32h_r32l_full(); return;
        }
        if((m_inst_state&0xff88)==0x1a80) {if(partial)sub_l_r32h_r32l_partial();else sub_l_r32h_r32l_full(); return;
        }
        if((m_inst_state&0xff88)==0x1f80) {if(partial)cmp_l_r32h_r32l_partial();else cmp_l_r32h_r32l_full(); return;
        }
        if((m_inst_state&0xf000)==0x9000) {if(partial)addx_b_imm8_r8u_partial();else addx_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xf000)==0xb000) {if(partial)subx_b_imm8_r8u_partial();else subx_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xff00)==0x0e00) {if(partial)addx_b_r8h_r8l_partial();else addx_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1e00) {if(partial)subx_b_r8h_r8l_partial();else subx_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xf000)==0xc000) {if(partial)or_b_imm8_r8u_partial();else or_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xf000)==0xd000) {if(partial)xor_b_imm8_r8u_partial();else xor_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xf000)==0xe000) {if(partial)and_b_imm8_r8u_partial();else and_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xff00)==0x1400) {if(partial)or_b_r8h_r8l_partial();else or_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1500) {if(partial)xor_b_r8h_r8l_partial();else xor_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1600) {if(partial)and_b_r8h_r8l_partial();else and_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xf000)==0x8000) {if(partial)add_b_imm8_r8u_partial();else add_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xf000)==0xa000) {if(partial)cmp_b_imm8_r8u_partial();else cmp_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xff00)==0x0800) {if(partial)add_b_r8h_r8l_partial();else add_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1800) {if(partial)sub_b_r8h_r8l_partial();else sub_b_r8h_r8l_full(); return;
        }
        if((m_inst_state&0xff00)==0x1c00) {if(partial)cmp_b_r8h_r8l_partial();else cmp_b_r8h_r8l_full(); return;
        }
        if(m_inst_state==0) {if(partial)nop_partial();else nop_full(); return;
        }
        if((m_inst_state&0xf000)==0xf000 && m_inst_state<=0xffff) {
            if(partial)mov_b_imm8_r8u_partial();else mov_b_imm8_r8u_full(); return;
        }
        if((m_inst_state&0xf000)==0x4000 && m_inst_state<=0x4fff) {
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
            } return;
        }
        if((m_inst_state&0xff00)==0x7900 && (m_inst_state&0xf0)<=0x60) {
            switch((m_inst_state>>4)&7) {
            case 0:if(partial)mov_w_imm16_r16l_partial();else mov_w_imm16_r16l_full();break;
            case 1:if(partial)add_w_imm16_r16l_partial();else add_w_imm16_r16l_full();break;
            case 2:if(partial)cmp_w_imm16_r16l_partial();else cmp_w_imm16_r16l_full();break;
            case 3:if(partial)sub_w_imm16_r16l_partial();else sub_w_imm16_r16l_full();break;
            case 4:if(partial)or_w_imm16_r16l_partial();else or_w_imm16_r16l_full();break;
            case 5:if(partial)xor_w_imm16_r16l_partial();else xor_w_imm16_r16l_full();break;
            case 6:if(partial)and_w_imm16_r16l_partial();else and_w_imm16_r16l_full();break;
            } return;
        }
        if((m_inst_state&0xff00)==0x6900) {
            if(m_inst_state&0x80) {if(partial)mov_w_r16l_r32ih_partial();else mov_w_r16l_r32ih_full();}
            else {if(partial)mov_w_r32ih_r16l_partial();else mov_w_r32ih_r16l_full();} return;
        }
        if((m_inst_state&0xff08)==0x7a00 && (m_inst_state&0xf0)<=0x60) {
            switch((m_inst_state>>4)&7) {
            case 0:if(partial)mov_l_imm32_r32l_partial();else mov_l_imm32_r32l_full();break;
            case 1:if(partial)add_l_imm32_r32l_partial();else add_l_imm32_r32l_full();break;
            case 2:if(partial)cmp_l_imm32_r32l_partial();else cmp_l_imm32_r32l_full();break;
            case 3:if(partial)sub_l_imm32_r32l_partial();else sub_l_imm32_r32l_full();break;
            case 4:if(partial)or_l_imm32_r32l_partial();else or_l_imm32_r32l_full();break;
            case 5:if(partial)xor_l_imm32_r32l_partial();else xor_l_imm32_r32l_full();break;
            case 6:if(partial)and_l_imm32_r32l_partial();else and_l_imm32_r32l_full();break;
            } return;
        }
        if((m_inst_state&0xff00)==0x6800) {
            if(m_inst_state&0x80) {if(partial)mov_b_r8l_r32ih_partial();else mov_b_r8l_r32ih_full();}
            else {if(partial)mov_b_r32ih_r8l_partial();else mov_b_r32ih_r8l_full();} return;
        }
        throw std::runtime_error("Unsupported first-H8 instruction");
    }
    void refresh_bus_deadline(){
        if constexpr(requires{host_.next_bus_event();})
            m_bcount=runner_.reschedule(host_.next_bus_event());
    }
    uint16_t read16i(uint32_t address){auto v=bus_.read16i(address);refresh_bus_deadline();return v;}
    uint8_t read8(uint32_t address){auto v=bus_.read8(address);refresh_bus_deadline();return v;}
    void write8(uint32_t address,uint8_t data){bus_.write8(address,data);refresh_bus_deadline();}
    uint16_t read16(uint32_t address){auto v=bus_.read16(address);refresh_bus_deadline();return v;}
    void write16(uint32_t address,uint16_t data){bus_.write16(address,data);refresh_bus_deadline();}
    bool access_to_be_redone()const{return false;}
    void update_irq_filter(){if(irq_)irq_->set_filter((m_CCR&F_I)?2:0,-1);}
    void prefetch_done(){
        if(irq_){const auto selected=irq_->current();if(selected.vector>0 && selected.level>=0){
            m_taken_irq_vector=selected.vector;m_taken_irq_level=selected.level;m_inst_state=0x10001;return;
        }}
        m_inst_state=m_IR[0]=m_PIR;
    }
    void prefetch_done_noirq(){m_inst_state=m_IR[0]=m_PIR;}
    void prefetch_done_notrace(){prefetch_done();}
    // Fixed reset SYSCR=0x0b: H83002 trapa_setup sets I, vector base 8.
    int trapa_setup(){m_CCR|=F_I;return 8;}
    void irq_setup(){m_CCR|=F_I;}
    void interrupt_taken(){if(!irq_)throw std::logic_error("Missing IRQ owner");irq_->taken(m_taken_irq_vector);}
    void debugger_exception_hook(int vector){if constexpr(requires{host_.exception_hook(vector);})host_.exception_hook(vector);}
    bool exr_in_stack()const{return false;}
    void internal(int cycles){m_icount-=cycles+1;}
    void prefetch_switch(uint32_t pc,uint16_t ir){m_NPC=pc&0xffffff;m_PC=pc+2;m_PIR=ir;}
void set_nz16(u16 v)
{
	m_CCR &= ~(F_N|F_Z);
	if(!v)
		m_CCR |= F_Z;
	else if(s16(v) < 0)
		m_CCR |= F_N;
}
void set_nz32(u32 v)
{
	m_CCR &= ~(F_N|F_Z);
	if(!v)
		m_CCR |= F_Z;
	else if(s32(v) < 0)
		m_CCR |= F_N;
}
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
    template<unsigned Bits> uint32_t increment_decrement(uint32_t value,unsigned amount,bool subtract) {
        const auto preserved=m_CCR&uint8_t(F_H|F_C);
        const auto result=arithmetic<Bits>(value,amount,subtract);
        m_CCR=uint8_t((m_CCR&~(F_H|F_C))|preserved);
        return result;
    }
    uint8_t do_inc8(uint8_t a,uint8_t b){return uint8_t(increment_decrement<8>(a,b,false));}
    uint16_t do_inc16(uint16_t a,uint16_t b){return uint16_t(increment_decrement<16>(a,b,false));}
    uint32_t do_inc32(uint32_t a,uint32_t b){return increment_decrement<32>(a,b,false);}
    uint8_t do_dec8(uint8_t a,uint8_t b){return uint8_t(increment_decrement<8>(a,b,true));}
    uint16_t do_dec16(uint16_t a,uint16_t b){return uint16_t(increment_decrement<16>(a,b,true));}
    uint32_t do_dec32(uint32_t a,uint32_t b){return increment_decrement<32>(a,b,true);}
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
    // Unsigned fixed-width adaptation of pinned MAME do_sh*/do_rot* helpers.
    template<unsigned Bits,unsigned Op> uint32_t shift_rotate(uint32_t input) {
        const uint64_t modulus=uint64_t{1}<<Bits, sign=modulus/2;
        const uint64_t v=input&(modulus-1), carry=(m_CCR&F_C)?1:0;
        const bool left=Op==0 || Op==1 || Op==4 || Op==5;
        const bool outgoing=left?(v&sign)!=0:(v&1)!=0;
        uint64_t result=left?(v<<1):(v>>1);
        if constexpr(Op==3)result|=v&sign;
        if constexpr(Op==4)result|=carry;
        if constexpr(Op==5)result|=outgoing?1:0;
        if constexpr(Op==6)result|=carry?sign:0;
        if constexpr(Op==7)result|=outgoing?sign:0;
        result&=modulus-1;
        m_CCR&=~(F_N|F_Z|F_V|F_C);
        if(!result)m_CCR|=F_Z;
        if(result&sign)m_CCR|=F_N;
        if(outgoing)m_CCR|=F_C;
        if constexpr(Op==1)if((v^result)&sign)m_CCR|=F_V;
        return uint32_t(result);
    }
    uint8_t do_shll8(uint8_t v){return uint8_t(shift_rotate<8,0>(v));}
    uint16_t do_shll16(uint16_t v){return uint16_t(shift_rotate<16,0>(v));}
    uint32_t do_shll32(uint32_t v){return uint32_t(shift_rotate<32,0>(v));}
    uint8_t do_shal8(uint8_t v){return uint8_t(shift_rotate<8,1>(v));}
    uint16_t do_shal16(uint16_t v){return uint16_t(shift_rotate<16,1>(v));}
    uint32_t do_shal32(uint32_t v){return uint32_t(shift_rotate<32,1>(v));}
    uint8_t do_shlr8(uint8_t v){return uint8_t(shift_rotate<8,2>(v));}
    uint16_t do_shlr16(uint16_t v){return uint16_t(shift_rotate<16,2>(v));}
    uint32_t do_shlr32(uint32_t v){return uint32_t(shift_rotate<32,2>(v));}
    uint8_t do_shar8(uint8_t v){return uint8_t(shift_rotate<8,3>(v));}
    uint16_t do_shar16(uint16_t v){return uint16_t(shift_rotate<16,3>(v));}
    uint32_t do_shar32(uint32_t v){return uint32_t(shift_rotate<32,3>(v));}
    uint8_t do_rotxl8(uint8_t v){return uint8_t(shift_rotate<8,4>(v));}
    uint16_t do_rotxl16(uint16_t v){return uint16_t(shift_rotate<16,4>(v));}
    uint32_t do_rotxl32(uint32_t v){return uint32_t(shift_rotate<32,4>(v));}
    uint8_t do_rotl8(uint8_t v){return uint8_t(shift_rotate<8,5>(v));}
    uint16_t do_rotl16(uint16_t v){return uint16_t(shift_rotate<16,5>(v));}
    uint32_t do_rotl32(uint32_t v){return uint32_t(shift_rotate<32,5>(v));}
    uint8_t do_rotxr8(uint8_t v){return uint8_t(shift_rotate<8,6>(v));}
    uint16_t do_rotxr16(uint16_t v){return uint16_t(shift_rotate<16,6>(v));}
    uint32_t do_rotxr32(uint32_t v){return uint32_t(shift_rotate<32,6>(v));}
    uint8_t do_rotr8(uint8_t v){return uint8_t(shift_rotate<8,7>(v));}
    uint16_t do_rotr16(uint16_t v){return uint16_t(shift_rotate<16,7>(v));}
    uint32_t do_rotr32(uint32_t v){return uint32_t(shift_rotate<32,7>(v));}
    Host& host_;
    H8hIrqController* irq_;
    H8ExecutionRunner runner_;
    int& m_icount;
    H8hBusAccess<Host> bus_;
    int m_taken_irq_vector=0,m_taken_irq_level=-1;
    int m_bcount=0;
    std::array<uint16_t,16> m_R{};
    uint32_t m_PC=0,m_PPC=0,m_NPC=0,m_inst_state=0x10000,m_inst_substate=0,m_TMP1=0,m_TMP2=0;
    std::array<uint16_t,4> m_IR{};
    uint16_t m_PIR=0;
    uint8_t m_CCR=0,m_EXR=0;
#include "h8_first_handlers.h"
#include "h8_first_h8h_handlers.h"
};
} // namespace yamanix
