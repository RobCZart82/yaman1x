// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Selected pinned generated h8.hxx methods; only class qualifiers removed.
// Included inside H8FirstInstructions. See research/h8-first-instructions-provenance.json.
void divxu_b_r8h_r16l_full()
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
	internal(11);
	m_TMP1 = r16_r(m_IR[0]);
	m_TMP2 = r8_r(m_IR[0] >> 4);
	m_CCR &= ~(F_Z|F_N);
	if(m_TMP2 & 0x80)
		m_CCR |= F_N;
	if(!m_TMP2) {
		m_CCR |= F_Z;
	} else {
		int q = m_TMP1 / m_TMP2;
		int r = m_TMP1 % m_TMP2;
		r16_w(m_IR[0], (q & 0xff) | ((r & 0xff) << 8));
	}
	prefetch_done();
}

void divxu_b_r8h_r16l_partial()
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
	internal(11);
	m_TMP1 = r16_r(m_IR[0]);
	m_TMP2 = r8_r(m_IR[0] >> 4);
	m_CCR &= ~(F_Z|F_N);
	if(m_TMP2 & 0x80)
		m_CCR |= F_N;
	if(!m_TMP2) {
		m_CCR |= F_Z;
	} else {
		int q = m_TMP1 / m_TMP2;
		int r = m_TMP1 % m_TMP2;
		r16_w(m_IR[0], (q & 0xff) | ((r & 0xff) << 8));
	}
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void dispatch_01c0_full()
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
	m_inst_state = 0xe0000 | m_IR[1];
}

void dispatch_01c0_partial()
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
	m_inst_state = 0xe0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void mulxu_b_r8h_r16l_full()
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
	r16_w(m_IR[0], u8(r16_r(m_IR[0])) * r8_r(m_IR[0] >> 4));
	internal(m_has_mac ? 2 : 11);
	prefetch_done();
}

void mulxu_b_r8h_r16l_partial()
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
	r16_w(m_IR[0], u8(r16_r(m_IR[0])) * r8_r(m_IR[0] >> 4));
	internal(m_has_mac ? 2 : 11);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_abs8_r8u_full()
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
	m_TMP1 = read8(0xffffff00 | m_IR[0]);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	prefetch_done();
}

void mov_b_abs8_r8u_partial()
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
	[[fallthrough]];
case 3:;
	m_TMP1 = read8(0xffffff00 | m_IR[0]);
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
	set_nzv8(m_TMP1);
	r8_w(m_IR[0] >> 8, m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_r8u_abs8_full()
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
	m_TMP1 = r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	write8(0xffffff00 | m_IR[0], m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_done();
}

void mov_b_r8u_abs8_partial()
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
	m_TMP1 = r8_r(m_IR[0] >> 8);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 3:;
	write8(0xffffff00 | m_IR[0], m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_abs16_r8l_full()
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
	m_TMP1 = read8(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void mov_b_abs16_r8l_partial()
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
	[[fallthrough]];
case 5:;
	m_TMP1 = read8(s16(m_IR[1]));
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
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_r8l_abs16_full()
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
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	write8(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void mov_b_r8l_abs16_partial()
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
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 5:;
	write8(s16(m_IR[1]), m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_w_abs16_r16l_full()
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
	m_TMP1 = read16(s16(m_IR[1]));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void mov_w_abs16_r16l_partial()
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
	[[fallthrough]];
case 5:;
	m_TMP1 = read16(s16(m_IR[1]));
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
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_w_r16l_abs16_full()
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
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	write16(s16(m_IR[1]), m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void mov_w_r16l_abs16_partial()
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
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	[[fallthrough]];
case 5:;
	write16(s16(m_IR[1]), m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void not_b_r8l_full()
{
	m_TMP1 = ~r8_r(m_IR[0]);
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

void not_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = ~r8_r(m_IR[0]);
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

void neg_b_r8l_full()
{
	r8_w(m_IR[0], do_sub8(0, r8_r(m_IR[0])));
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

void neg_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_sub8(0, r8_r(m_IR[0])));
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

void inc_b_one_r8l_full()
{
	r8_w(m_IR[0], do_inc8(r8_r(m_IR[0]), 1));
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

void inc_b_one_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_inc8(r8_r(m_IR[0]), 1));
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

void dec_b_one_r8l_full()
{
	r8_w(m_IR[0], do_dec8(r8_r(m_IR[0]), 1));
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

void dec_b_one_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_dec8(r8_r(m_IR[0]), 1));
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

void mov_b_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0] >> 4);
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

void mov_b_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0] >> 4);
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

void mov_w_r16h_r16l_full()
{
	m_TMP1 = r16_r(m_IR[0] >> 4);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void mov_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r16_r(m_IR[0] >> 4);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void stc_ccr_r8l_full()
{
	r8_w(m_IR[0], m_CCR);
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

void stc_ccr_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], m_CCR);
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

void ldc_r8l_ccr_full()
{
	m_CCR = r8_r(m_IR[0]);
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

void ldc_r8l_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR = r8_r(m_IR[0]);
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

void orc_imm8_ccr_full()
{
	m_CCR |= m_IR[0];
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

void orc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR |= m_IR[0];
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

void xorc_imm8_ccr_full()
{
	m_CCR ^= m_IR[0];
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

void xorc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR ^= m_IR[0];
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

void andc_imm8_ccr_full()
{
	m_CCR &= m_IR[0];
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

void andc_imm8_ccr_partial()
{
switch(m_inst_substate) {
case 0:
	m_CCR &= m_IR[0];
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

void dispatch_01f0_full()
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
	m_inst_state = 0x110000 | m_IR[1];
}

void dispatch_01f0_partial()
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
	m_inst_state = 0x110000 | m_IR[1];
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

void shll_b_r8l_full()
{
	r8_w(m_IR[0], do_shll8(r8_r(m_IR[0])));
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

void shll_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shll8(r8_r(m_IR[0])));
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

void shal_b_r8l_full()
{
	r8_w(m_IR[0], do_shal8(r8_r(m_IR[0])));
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

void shal_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shal8(r8_r(m_IR[0])));
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

void shlr_b_r8l_full()
{
	r8_w(m_IR[0], do_shlr8(r8_r(m_IR[0])));
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

void shlr_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shlr8(r8_r(m_IR[0])));
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

void shar_b_r8l_full()
{
	r8_w(m_IR[0], do_shar8(r8_r(m_IR[0])));
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

void shar_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_shar8(r8_r(m_IR[0])));
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

void rotxl_b_r8l_full()
{
	r8_w(m_IR[0], do_rotxl8(r8_r(m_IR[0])));
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

void rotxl_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotxl8(r8_r(m_IR[0])));
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

void rotl_b_r8l_full()
{
	r8_w(m_IR[0], do_rotl8(r8_r(m_IR[0])));
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

void rotl_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotl8(r8_r(m_IR[0])));
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

void rotxr_b_r8l_full()
{
	r8_w(m_IR[0], do_rotxr8(r8_r(m_IR[0])));
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

void rotxr_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotxr8(r8_r(m_IR[0])));
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

void rotr_b_r8l_full()
{
	r8_w(m_IR[0], do_rotr8(r8_r(m_IR[0])));
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

void rotr_b_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	r8_w(m_IR[0], do_rotr8(r8_r(m_IR[0])));
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

void bset_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
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

void bset_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
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

void bnot_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
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

void bnot_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((r8_r(m_IR[0] >> 4)) & 7);
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

void bclr_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((r8_r(m_IR[0] >> 4)) & 7));
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

void bclr_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((r8_r(m_IR[0] >> 4)) & 7));
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

void btst_r8h_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((r8_r(m_IR[0] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void btst_r8h_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((r8_r(m_IR[0] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void bset_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
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

void bset_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
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

void bnot_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((m_IR[0] >> 4) & 7);
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

void bnot_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 ^= 1 << ((m_IR[0] >> 4) & 7);
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

void bclr_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
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

void bclr_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
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

void btst_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void btst_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void bst_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
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

void bst_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
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

void bist_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
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

void bist_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[0] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[0] >> 4) & 7));
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

void bor_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
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

void bor_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
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

void bior_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
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

void bior_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
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

void bxor_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR ^= F_C;
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

void bxor_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR ^= F_C;
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

void bixor_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR ^= F_C;
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

void bixor_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR ^= F_C;
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

void band_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR &= ~F_C;
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

void band_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR &= ~F_C;
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

void biand_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_C;
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

void biand_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR &= ~F_C;
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

void bld_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void bld_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void bild_imm3_r8l_full()
{
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void bild_imm3_r8l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r8_r(m_IR[0]);
	if(!(m_TMP1 & (1 << ((m_IR[0] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void dispatch_7c00_full()
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
	m_inst_state = 0x190000 | m_IR[1];
}

void dispatch_7c00_partial()
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
	m_inst_state = 0x190000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void dispatch_7d00_full()
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
	m_inst_state = 0x1a0000 | m_IR[1];
}

void dispatch_7d00_partial()
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
	m_inst_state = 0x1a0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void dispatch_7e00_full()
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
	m_inst_state = 0x1b0000 | m_IR[1];
}

void dispatch_7e00_partial()
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
	m_inst_state = 0x1b0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void dispatch_7f00_full()
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
	m_inst_state = 0x1c0000 | m_IR[1];
}

void dispatch_7f00_partial()
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
	m_inst_state = 0x1c0000 | m_IR[1];
	break;
}
	m_inst_substate = 0;
}

void bset_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 |= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
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
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bset_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	m_TMP1 |= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
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
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bnot_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 ^= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
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
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bnot_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	m_TMP1 ^= 1 << ((r8_r(m_IR[1] >> 4)) & 7);
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
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bclr_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 &= ~(1 << ((r8_r(m_IR[1] >> 4)) & 7));
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
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bclr_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	m_TMP1 &= ~(1 << ((r8_r(m_IR[1] >> 4)) & 7));
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
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void btst_r8h_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((r8_r(m_IR[1] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void btst_r8h_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_TMP1 & (1 << ((r8_r(m_IR[1] >> 4)) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void bset_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
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
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bset_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bnot_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
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
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bnot_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bclr_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
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
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bclr_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void btst_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void btst_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_Z;
	else
		m_CCR |= F_Z;
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

void bst_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
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
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bst_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bist_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
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
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	write8(m_TMP2, m_TMP1);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_done();
}

void bist_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
	[[fallthrough]];
case 5:;
	write8(m_TMP2, m_TMP1);
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
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void bor_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
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

void bor_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
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

void bior_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
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

void bior_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
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

void bxor_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR ^= F_C;
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

void bxor_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR ^= F_C;
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

void bixor_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR ^= F_C;
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

void bixor_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR ^= F_C;
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

void band_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR &= ~F_C;
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

void band_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR &= ~F_C;
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

void biand_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_C;
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

void biand_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR &= ~F_C;
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

void bld_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void bld_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7)))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void bild_imm3_abs8_full()
{
	m_TMP2 = 0xffffff00 | m_IR[0];
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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

void bild_imm3_abs8_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = 0xffffff00 | m_IR[0];
	[[fallthrough]];
case 1:;
	m_TMP1 = read8(m_TMP2);
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
	if(!(m_TMP1 & (1 << ((m_IR[1] >> 4) & 7))))
		m_CCR |= F_C;
	else
		m_CCR &= ~F_C;
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
