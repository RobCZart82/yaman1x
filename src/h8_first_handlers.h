// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Selected pinned generated h8.hxx methods; only class qualifiers removed.
// Included inside H8FirstInstructions. See research/h8-first-instructions-provenance.json.
void state_reset_full()
{
	m_CCR |= (m_has_hc ? F_I : F_H);
	m_EXR = EXR_I | EXR_NC;
	if(m_mode_advanced) {
		m_IR[0] = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
		m_IR[1] = read16i(2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		m_PC = (m_IR[0] << 16) | m_IR[1];
	} else {
		m_PC = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void state_reset_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR |= (m_has_hc ? F_I : F_H);
	m_EXR = EXR_I | EXR_NC;
	if(m_mode_advanced) {
	[[fallthrough]];
case 1:;
		m_IR[0] = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:;
		m_IR[1] = read16i(2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		m_PC = (m_IR[0] << 16) | m_IR[1];
	} else {
	[[fallthrough]];
case 5:;
		m_PC = read16i(0);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 7:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void nop_full()
{
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void nop_partial()
{
switch(m_inst_substate) {
case 0:
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_imm8_r8u_full()
{
	set_nzv8(m_IR[0]);
	r8_w(m_IR[0] >> 8, m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void mov_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	set_nzv8(m_IR[0]);
	r8_w(m_IR[0] >> 8, m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_w_imm16_r16l_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	set_nzv16(m_IR[1]);
	r16_w(m_IR[0], m_IR[1]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void mov_w_imm16_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	set_nzv16(m_IR[1]);
	r16_w(m_IR[0], m_IR[1]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 3:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void dispatch_0100_full()
{
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_inst_state = 0x20000 | m_IR[1];
}

void dispatch_0100_partial()
{
switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:;
	m_IR[1] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	m_inst_state = 0x20000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void add_b_imm8_r8u_full()
{
	r8_w(m_IR[0] >> 8, do_add8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void add_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0] >> 8, do_add8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void cmp_b_imm8_r8u_full()
{
	do_sub8(r8_r(m_IR[0] >> 8), m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void cmp_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub8(r8_r(m_IR[0] >> 8), m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void add_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_add8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void add_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_add8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void sub_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void sub_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void cmp_b_r8h_r8l_full()
{
	do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void cmp_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void ldc_imm8_ccr_full()
{
	m_CCR = m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done_noirq();
}

void ldc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR = m_IR[0];
	update_irq_filter();
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done_noirq();
	break;
}
	m_inst_substate = 0;
}

void add_w_r16h_r16l_full()
{
	r16_w(m_IR[0], do_add16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void add_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_add16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void sub_w_r16h_r16l_full()
{
	r16_w(m_IR[0], do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void sub_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void cmp_w_r16h_r16l_full()
{
	do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void cmp_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub16(r16_r(m_IR[0]), r16_r(m_IR[0] >> 4));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void addx_b_imm8_r8u_full()
{
	r8_w(m_IR[0] >> 8, do_addx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void addx_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0] >> 8, do_addx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void subx_b_imm8_r8u_full()
{
	r8_w(m_IR[0] >> 8, do_subx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void subx_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0] >> 8, do_subx8(r8_r(m_IR[0] >> 8), m_IR[0]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void addx_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_addx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void addx_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_addx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void subx_b_r8h_r8l_full()
{
	r8_w(m_IR[0], do_subx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void subx_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_subx8(r8_r(m_IR[0]), r8_r(m_IR[0] >> 4)));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void and_b_imm8_r8u_full()
{
	m_TMP1 = m_IR[0] & r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void and_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_IR[0] & r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void or_b_imm8_r8u_full()
{
	m_TMP1 = m_IR[0] | r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void or_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_IR[0] | r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void xor_b_imm8_r8u_full()
{
	m_TMP1 = m_IR[0] ^ r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void xor_b_imm8_r8u_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_IR[0] ^ r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void and_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4) & r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void and_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4) & r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void or_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4) | r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void or_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4) | r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void xor_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4) ^ r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void xor_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4) ^ r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bt_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(true)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bt_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(true)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bf_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(false)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bf_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(false)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bhi_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & (F_C|F_Z)))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bhi_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & (F_C|F_Z)))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bls_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & (F_C|F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bls_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & (F_C|F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bcc_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_C))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bcc_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_C))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bcs_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_C)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bcs_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_C)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bne_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bne_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_Z))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void beq_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_Z)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void beq_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_Z)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bvc_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bvc_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bvs_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bvs_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bpl_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!(m_CCR & F_N))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bpl_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!(m_CCR & F_N))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bmi_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(m_CCR & F_N)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bmi_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(m_CCR & F_N)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bge_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bge_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void blt_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void blt_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if((m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bgt_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if(!((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void bgt_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if(!((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V))
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void ble_rel8_full()
{
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC += 2;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	if((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
}

void ble_rel8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = m_PC + s8(m_IR[0]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 1:;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC += 2;
	[[fallthrough]];
case 3:;
	m_TMP2 = read16i(m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	if((m_CCR & F_Z) || (m_CCR & (F_N|F_V)) == F_N || (m_CCR & (F_N|F_V)) == F_V)
		prefetch_switch(m_TMP1, m_TMP2);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}
