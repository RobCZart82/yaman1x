// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Selected pinned generated h8h.hxx methods; only class qualifiers removed.
// Included inside H8FirstInstructions. See research/h8-first-instructions-provenance.json.
void mov_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = (m_IR[1] << 16) | m_IR[2];
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void mov_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	m_TMP1 = (m_IR[1] << 16) | m_IR[2];
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_r32ih_r8l_full()
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
	m_TMP1 = read8(r32_r(m_IR[0] >> 4));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nzv8(m_TMP1);
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void mov_b_r32ih_r8l_partial()
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
	m_TMP1 = read8(r32_r(m_IR[0] >> 4));
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
	r8_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_b_r8l_r32ih_full()
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
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	write8(r32_r(m_IR[0] >> 4), m_TMP1);
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

void mov_b_r8l_r32ih_partial()
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
	m_TMP1 = r8_r(m_IR[0]);
	set_nzv8(m_TMP1);
	[[fallthrough]];
case 3:;
	write8(r32_r(m_IR[0] >> 4), m_TMP1);
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

void mov_w_r32ih_r16l_full()
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
	m_TMP1 = read16(r32_r(m_IR[0] >> 4));
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
}

void mov_w_r32ih_r16l_partial()
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
	m_TMP1 = read16(r32_r(m_IR[0] >> 4));
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
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_w_r16l_r32ih_full()
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
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	write16(r32_r(m_IR[0] >> 4), m_TMP1);
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

void mov_w_r16l_r32ih_partial()
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
	m_TMP1 = r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	[[fallthrough]];
case 3:;
	write16(r32_r(m_IR[0] >> 4), m_TMP1);
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

void mov_l_r32ih_r32l_full()
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
	m_TMP2 = r32_r(m_IR[1] >> 4);
	m_TMP1 = read16(m_TMP2) << 16;
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP1 |= read16(m_TMP2+2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
	prefetch_done();
}

void mov_l_r32ih_r32l_partial()
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
	m_TMP2 = r32_r(m_IR[1] >> 4);
	[[fallthrough]];
case 3:;
	m_TMP1 = read16(m_TMP2) << 16;
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
	[[fallthrough]];
case 5:;
	m_TMP1 |= read16(m_TMP2+2);
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
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void mov_l_r32l_r32ih_full()
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
	m_TMP2 = r32_r(m_IR[1] >> 4);
	m_TMP1 = r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	write16(m_TMP2, m_TMP1 >> 16);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write16(m_TMP2+2, m_TMP1);
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

void mov_l_r32l_r32ih_partial()
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
	m_TMP2 = r32_r(m_IR[1] >> 4);
	m_TMP1 = r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	[[fallthrough]];
case 3:;
	write16(m_TMP2, m_TMP1 >> 16);
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
	[[fallthrough]];
case 5:;
	write16(m_TMP2+2, m_TMP1);
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

void add_w_imm16_r16l_full()
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
	r16_w(m_IR[0], do_add16(r16_r(m_IR[0]), m_IR[1]));
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

void add_w_imm16_r16l_partial()
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
	r16_w(m_IR[0], do_add16(r16_r(m_IR[0]), m_IR[1]));
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

void sub_w_imm16_r16l_full()
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
	r16_w(m_IR[0], do_sub16(r16_r(m_IR[0]), m_IR[1]));
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

void sub_w_imm16_r16l_partial()
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
	r16_w(m_IR[0], do_sub16(r16_r(m_IR[0]), m_IR[1]));
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

void cmp_w_imm16_r16l_full()
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
	do_sub16(r16_r(m_IR[0]), m_IR[1]);
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

void cmp_w_imm16_r16l_partial()
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
	do_sub16(r16_r(m_IR[0]), m_IR[1]);
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

void add_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	r32_w(m_IR[0], do_add32(r32_r(m_IR[0]), (m_IR[1] << 16) | m_IR[2]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void add_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	r32_w(m_IR[0], do_add32(r32_r(m_IR[0]), (m_IR[1] << 16) | m_IR[2]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void sub_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	r32_w(m_IR[0], do_sub32(r32_r(m_IR[0]), (m_IR[1] << 16) | m_IR[2]));
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void sub_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	r32_w(m_IR[0], do_sub32(r32_r(m_IR[0]), (m_IR[1] << 16) | m_IR[2]));
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void cmp_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	do_sub32(r32_r(m_IR[0]), (m_IR[1] << 16) | m_IR[2]);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void cmp_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	do_sub32(r32_r(m_IR[0]), (m_IR[1] << 16) | m_IR[2]);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void add_l_r32h_r32l_full()
{
	r32_w(m_IR[0], do_add32(r32_r(m_IR[0]), r32_r(m_IR[0] >> 4)));
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

void add_l_r32h_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_add32(r32_r(m_IR[0]), r32_r(m_IR[0] >> 4)));
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

void sub_l_r32h_r32l_full()
{
	r32_w(m_IR[0], do_sub32(r32_r(m_IR[0]), r32_r(m_IR[0] >> 4)));
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

void sub_l_r32h_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_sub32(r32_r(m_IR[0]), r32_r(m_IR[0] >> 4)));
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

void cmp_l_r32h_r32l_full()
{
	do_sub32(r32_r(m_IR[0]), r32_r(m_IR[0] >> 4));
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

void cmp_l_r32h_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	do_sub32(r32_r(m_IR[0]), r32_r(m_IR[0] >> 4));
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

void and_w_imm16_r16l_full()
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
	m_TMP1 = m_IR[1] & r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void and_w_imm16_r16l_partial()
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
	m_TMP1 = m_IR[1] & r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void or_w_imm16_r16l_full()
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
	m_TMP1 = m_IR[1] | r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void or_w_imm16_r16l_partial()
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
	m_TMP1 = m_IR[1] | r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void xor_w_imm16_r16l_full()
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
	m_TMP1 = m_IR[1] ^ r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void xor_w_imm16_r16l_partial()
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
	m_TMP1 = m_IR[1] ^ r16_r(m_IR[0]);
	set_nzv16(m_TMP1);
	r16_w(m_IR[0], m_TMP1);
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

void and_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r32_r(m_IR[0]) & ((m_IR[1] << 16) | m_IR[2]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void and_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	m_TMP1 = r32_r(m_IR[0]) & ((m_IR[1] << 16) | m_IR[2]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void or_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r32_r(m_IR[0]) | ((m_IR[1] << 16) | m_IR[2]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void or_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	m_TMP1 = r32_r(m_IR[0]) | ((m_IR[1] << 16) | m_IR[2]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void xor_l_imm32_r32l_full()
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
	m_IR[2] = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC += 2;
	m_TMP1 = r32_r(m_IR[0]) ^ ((m_IR[1] << 16) | m_IR[2]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	m_PIR = read16i(m_PC);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC += 2;
	prefetch_done();
}

void xor_l_imm32_r32l_partial()
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
	[[fallthrough]];
case 3:;
	m_IR[2] = read16i(m_PC);
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
	m_TMP1 = r32_r(m_IR[0]) ^ ((m_IR[1] << 16) | m_IR[2]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[0], m_TMP1);
	m_NPC = m_PC & 0xffffff;
	[[fallthrough]];
case 5:;
	m_PIR = read16i(m_PC);
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
	m_PC += 2;
	prefetch_done();
	break;
}
	m_inst_substate = 0;
}

void or_w_r16h_r16l_full()
{
	m_TMP1 = r16_r(m_IR[0] >> 4) | r16_r(m_IR[0]);
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

void or_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r16_r(m_IR[0] >> 4) | r16_r(m_IR[0]);
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

void xor_w_r16h_r16l_full()
{
	m_TMP1 = r16_r(m_IR[0] >> 4) ^ r16_r(m_IR[0]);
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

void xor_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r16_r(m_IR[0] >> 4) ^ r16_r(m_IR[0]);
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

void and_w_r16h_r16l_full()
{
	m_TMP1 = r16_r(m_IR[0] >> 4) & r16_r(m_IR[0]);
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

void and_w_r16h_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r16_r(m_IR[0] >> 4) & r16_r(m_IR[0]);
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

void or_l_r32h_r32l_full()
{
	m_TMP1 = r32_r(m_IR[1] >> 4) | r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
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

void or_l_r32h_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r32_r(m_IR[1] >> 4) | r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
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

void xor_l_r32h_r32l_full()
{
	m_TMP1 = r32_r(m_IR[1] >> 4) ^ r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
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

void xor_l_r32h_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r32_r(m_IR[1] >> 4) ^ r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
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

void and_l_r32h_r32l_full()
{
	m_TMP1 = r32_r(m_IR[1] >> 4) & r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
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

void and_l_r32h_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP1 = r32_r(m_IR[1] >> 4) & r32_r(m_IR[1]);
	set_nzv32(m_TMP1);
	r32_w(m_IR[1], m_TMP1);
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

void shll_w_r16l_full()
{
	r16_w(m_IR[0], do_shll16(r16_r(m_IR[0])));
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

void shll_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_shll16(r16_r(m_IR[0])));
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

void shll_l_r32l_full()
{
	r32_w(m_IR[0], do_shll32(r32_r(m_IR[0])));
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

void shll_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_shll32(r32_r(m_IR[0])));
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

void shal_w_r16l_full()
{
	r16_w(m_IR[0], do_shal16(r16_r(m_IR[0])));
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

void shal_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_shal16(r16_r(m_IR[0])));
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

void shal_l_r32l_full()
{
	r32_w(m_IR[0], do_shal32(r32_r(m_IR[0])));
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

void shal_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_shal32(r32_r(m_IR[0])));
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

void shlr_w_r16l_full()
{
	r16_w(m_IR[0], do_shlr16(r16_r(m_IR[0])));
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

void shlr_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_shlr16(r16_r(m_IR[0])));
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

void shlr_l_r32l_full()
{
	r32_w(m_IR[0], do_shlr32(r32_r(m_IR[0])));
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

void shlr_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_shlr32(r32_r(m_IR[0])));
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

void shar_w_r16l_full()
{
	r16_w(m_IR[0], do_shar16(r16_r(m_IR[0])));
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

void shar_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_shar16(r16_r(m_IR[0])));
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

void shar_l_r32l_full()
{
	r32_w(m_IR[0], do_shar32(r32_r(m_IR[0])));
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

void shar_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_shar32(r32_r(m_IR[0])));
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

void rotxl_w_r16l_full()
{
	r16_w(m_IR[0], do_rotxl16(r16_r(m_IR[0])));
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

void rotxl_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_rotxl16(r16_r(m_IR[0])));
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

void rotxl_l_r32l_full()
{
	r32_w(m_IR[0], do_rotxl32(r32_r(m_IR[0])));
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

void rotxl_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_rotxl32(r32_r(m_IR[0])));
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

void rotl_w_r16l_full()
{
	r16_w(m_IR[0], do_rotl16(r16_r(m_IR[0])));
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

void rotl_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_rotl16(r16_r(m_IR[0])));
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

void rotl_l_r32l_full()
{
	r32_w(m_IR[0], do_rotl32(r32_r(m_IR[0])));
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

void rotl_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_rotl32(r32_r(m_IR[0])));
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

void rotxr_w_r16l_full()
{
	r16_w(m_IR[0], do_rotxr16(r16_r(m_IR[0])));
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

void rotxr_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_rotxr16(r16_r(m_IR[0])));
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

void rotxr_l_r32l_full()
{
	r32_w(m_IR[0], do_rotxr32(r32_r(m_IR[0])));
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

void rotxr_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_rotxr32(r32_r(m_IR[0])));
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

void rotr_w_r16l_full()
{
	r16_w(m_IR[0], do_rotr16(r16_r(m_IR[0])));
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

void rotr_w_r16l_partial()
{
switch(m_inst_substate) {
case 0:
	r16_w(m_IR[0], do_rotr16(r16_r(m_IR[0])));
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

void rotr_l_r32l_full()
{
	r32_w(m_IR[0], do_rotr32(r32_r(m_IR[0])));
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

void rotr_l_r32l_partial()
{
switch(m_inst_substate) {
case 0:
	r32_w(m_IR[0], do_rotr32(r32_r(m_IR[0])));
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

void bset_r8h_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bset_r8h_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bnot_r8h_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bnot_r8h_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bclr_r8h_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bclr_r8h_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void btst_r8h_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void btst_r8h_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bset_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
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

void bset_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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
	m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
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

void bnot_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
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

void bnot_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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
	m_TMP1 ^= 1 << ((m_IR[1] >> 4) & 7);
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

void bclr_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
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

void bclr_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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
	m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
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

void btst_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void btst_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bst_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
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

void bst_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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
	if(m_CCR & F_C)
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
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

void bist_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
	m_TMP1 = read8(m_TMP2);
	if(m_icount <= m_bcount) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
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

void bist_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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
	if(!(m_CCR & F_C))
		m_TMP1 |= 1 << ((m_IR[1] >> 4) & 7);
	else
		m_TMP1 &= ~(1 << ((m_IR[1] >> 4) & 7));
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

void bor_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bor_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bior_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bior_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bxor_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bxor_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bixor_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bixor_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void band_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void band_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void biand_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void biand_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bld_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bld_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bild_imm3_r32ihh_full()
{
	m_TMP2 = r32_r(m_IR[0] >> 4);
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

void bild_imm3_r32ihh_partial()
{
switch(m_inst_substate) {
case 0:
	m_TMP2 = r32_r(m_IR[0] >> 4);
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
