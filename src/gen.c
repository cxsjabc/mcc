#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/log.h"
#include "mcc/type.h"

__BEGIN_DECLS

int ind = 0;

void gen_byte(char c)
{
	int ind1;
	ind1 = ind + 1;
	if (ind1 > sec_text->data_allocated)
		section_realloc(sec_text, ind1);
	sec_text->data[ind] = c;
	ind = ind1;
}

void gen_prefix(char opcode)
{
	gen_byte(opcode);
}

void gen_opcode1(char opcode)
{
	gen_byte(opcode);
}

void gen_opcode2(char first, char second)
{
	gen_byte(first);
	gen_byte(second);
}

void gen_dword(unsigned int c)
{
	gen_byte(c);
	gen_byte(c >> 8);
	gen_byte(c >> 16);
	gen_byte(c >> 24);
}

void backpatch(int t, int a)
{
	int n, *ptr;
	while (t)
	{
		ptr = (int *)(sec_text->data + t);
		n = *ptr;
		*ptr = a - t - 4;
		t = n;
	}
}

int makelist(int s)
{
	int ind1;
	ind1 = ind + 4;
	if (ind1 > sec_text->data_allocated)
		section_realloc(sec_text, ind1);
	*(int *)(sec_text->data + ind) = s;
	s = ind;
	ind = ind1;
	return s;
}

void gen_addr32(int r, Symbol *sym, int c)
{
	if (r & SC_SYM)
		coffreloc_add(sec_text, sym, ind, IMAGE_REL_I386_DIR32);
	gen_dword(c);
}

void gen_modrm(int mod, int reg_opcode, int r_m, Symbol *sym, int c)
{
	mod <<= 6;
	reg_opcode <<= 3;
	if (mod == 0xc0)
	{
		gen_byte(mod | reg_opcode | (r_m & SC_VALMASK));
	}
	else if ((r_m & SC_VALMASK) == SC_GLOBAL)
	{
		gen_byte(0x05 | reg_opcode);
		gen_addr32(r_m, sym, c);
	}
	else if ((r_m & SC_VALMASK) == SC_LOCAL)
	{
		if (c == (char)c)
		{
			gen_byte(0x45 | reg_opcode);
			gen_byte(c);
		}
		else
		{
			gen_byte(0x85 | reg_opcode);
			gen_dword(c);
		}
	}
	else
	{
		gen_byte(0x00 | reg_opcode | (r_m & SC_VALMASK));
	}
}

void load(int r, Operand *opd)
{
	int v, ft, fc, fr;

	fr = opd->r;
	ft = opd->type.t;
	fc = opd->value;

	v = fr & SC_VALMASK;
	if (fr & SC_LVAL)
	{
		if ((ft & T_BTYPE) == T_CHAR)
		{
			gen_opcode2(0x0f, 0xbe);
		}
		else if ((ft & T_BTYPE) == T_SHORT)
		{
			gen_opcode2(0x0f, 0xbf);
		}
		else
		{
			gen_opcode1(0x8b);
		}
		gen_modrm(ADDR_OTHER, r, fr, opd->sym, fc);
	}
	else
	{
		if (v == SC_GLOBAL)
		{
			gen_opcode1(0xb8 + r);
			gen_addr32(fr, opd->sym, fc);
		}
		else if (v == SC_LOCAL)
		{
			gen_opcode1(0x8d);
			gen_modrm(ADDR_OTHER, r, SC_LOCAL, opd->sym, fc);
		}
		else if (v == SC_CMP)
		{
			gen_opcode1(0xb8 + r);
			gen_dword(0);

			gen_opcode2(0x0f, fc + 16);
			gen_modrm(ADDR_REG, 0, r, NULL, 0);
		}
		else if (v != r)
		{
			gen_opcode1(0x89);
			gen_modrm(ADDR_REG, v, r, NULL, 0);
		}
	}
}

void store(int r, Operand *opd)
{
	int fr, bt;

	fr = opd->r & SC_VALMASK;
	bt = opd->type.t & T_BTYPE;
	if (bt == T_SHORT)
		gen_prefix(0x66);
	if (bt == T_CHAR)
		gen_opcode1(0x88);
	else
		gen_opcode1(0x89);

	if (fr == SC_GLOBAL ||
	    fr == SC_LOCAL ||
	    (opd->r & SC_LVAL))
	{
		gen_modrm(ADDR_OTHER, r, opd->r, opd->sym, opd->value);
	}
}

int load_1(int rc, Operand *opd)
{
	int r;
	r = opd->r & SC_VALMASK;
	if (r >= SC_GLOBAL ||
	    (opd->r & SC_LVAL))
	{
		r = allocate_reg(rc);
		load(r, opd);
	}
	opd->r = r;
	return r;
}

void load_2(int rc1, int rc2)
{
	load_1(rc2, optop);
	load_1(rc1, &optop[-1]);
}

void store0_1()
{
	int r, t;
	r = load_1(REG_ANY, optop);

	if ((optop[-1].r & SC_VALMASK) == SC_LLOCAL)
	{
		Operand opd;
		t = allocate_reg(REG_ANY);
		operand_assign(&opd, T_INT, SC_LOCAL | SC_LVAL, optop[-1].value);
		load(t, &opd);
		optop[-1].r = t | SC_LVAL;
	}
	store(r, optop - 1);
	operand_swap();
	operand_pop();
}

void gen_addsp(int val)
{
	int opc = 0;
	if (val == (char)val)
	{
		gen_opcode1(0x83);
		gen_modrm(ADDR_REG, opc, REG_ESP, NULL, 0);
		gen_byte(val);
	}
	else
	{
		gen_opcode1(81);
		gen_modrm(ADDR_REG, opc, REG_ESP, NULL, 0);
		gen_dword(val);
	}
}

void gen_call()
{
	int r;
	if ((optop->r & (SC_VALMASK | SC_LVAL)) == SC_GLOBAL)
	{
		coffreloc_add(sec_text, optop->sym, ind + 1, IMAGE_REL_I386_REL32);

		gen_opcode1(0xe8);
		gen_dword(optop->value - 4);
	}
	else
	{
		r = load_1(REG_ANY, optop);
		gen_opcode1(0xff);
		gen_opcode1(0xd0 + r);
	}
}

void gen_invoke(int nb_args)
{
	int size, r, args_size, i, func_call;

	args_size = 0;

	for (i = 0; i < nb_args; i++)
	{
		r = load_1(REG_ANY, optop);
		size = 4;

		gen_opcode1(0x50 + r);
		args_size += size;
		operand_pop();
	}
	spill_regs();
	func_call = optop->type.ref->r;
	gen_call();
	if (args_size && func_call != KW_STDCALL)
		gen_addsp(args_size);
	operand_pop();
}

void gen_opi2(int opc, int op)
{
	int r, fr, c;
	if ((optop->r & (SC_VALMASK | SC_LVAL | SC_SYM)) == SC_GLOBAL)
	{
		r = load_1(REG_ANY, &optop[-1]);
		c = optop->value;
		if (c == (char)c)
		{
			gen_opcode1(0x83);
			gen_modrm(ADDR_REG, opc, r, NULL, 0);
			gen_byte(c);
		}
		else
		{
			gen_opcode1(0x81);
			gen_modrm(ADDR_REG, opc, r, NULL, 0);
			gen_byte(c);
		}
	}
	else
	{
		load_2(REG_ANY, REG_ANY);
		r = optop[-1].r;
		fr = optop[0].r;

		gen_opcode1((opc << 3) | 0x01);
		gen_modrm(ADDR_REG, fr, r, NULL, 0);
	}
	operand_pop();
	if (op >= TK_EQ && op <= TK_GEQ)
	{
		optop->r = SC_CMP;
		switch (op)
		{
		case TK_EQ:
			optop->value = 0x84;
			break;
		case TK_NEQ:
			optop->value = 0x85;
			break;
		case TK_LT:
			optop->value = 0x8c;
			break;
		case TK_LEQ:
			optop->value = 0x8e;
			break;
		case TK_GT:
			optop->value = 0x8f;
			break;
		case TK_GEQ:
			optop->value = 0x8d;
			break;
		}
	}
}

void gen_opi(int op)
{
	int r, fr, opc;

	switch (op)
	{
	case TK_PLUS:
		opc = 0;
		gen_opi2(opc, op);
		break;
	case TK_MINUS:
		opc = 5;
		gen_opi2(opc, op);
		break;
	case TK_STAR:
		load_2(REG_ANY, REG_ANY);
		r = optop[-1].r;
		fr = optop[0].r;
		operand_pop();

		gen_opcode2(0x0f, 0xaf);
		gen_modrm(ADDR_REG, r, fr, NULL, 0);
		break;
	case TK_DIVIDE:
	case TK_MOD:
		opc = 7;
		load_2(REG_EAX, REG_ECX);
		r = optop[-1].r;
		fr = optop[0].r;
		operand_pop();
		spill_reg(REG_EDX);

		gen_opcode1(0x99);

		gen_opcode1(0xf7);
		gen_modrm(ADDR_REG, opc, fr, NULL, 0);

		if (op == TK_MOD)
			r = REG_EDX;
		else
			r = REG_EAX;
		optop->r = r;
		break;
	default:
		opc = 7;
		gen_opi2(opc, op);
		break;
	}
}

Type *pointed_type(Type *t)
{
	return &t->ref->type;
}

int pointed_size(Type *t)
{
	int align;
	return type_size(pointed_type(t), &align);
}

void gen_op(int op)
{
	int u, bt1, bt2;
	Type type1;

	bt1 = optop[-1].type.t & T_BTYPE;
	bt2 = optop[0].type.t & T_BTYPE;

	if (bt1 == T_PTR || bt2 == T_PTR)
	{
		if (op >= TK_EQ && op <= TK_GEQ)
		{
			gen_opi(op);
			optop->type.t = T_INT;
		}
		else if (bt1 == T_PTR && bt2 == T_PTR)
		{
			if (op != TK_MINUS)
				error("两个指针只能进行关系或减法运算");
			u = pointed_size(&optop[-1].type);
			gen_opi(op);
			optop->type.t = T_INT;
			operand_push(&int_type, SC_GLOBAL, u);
			gen_op(TK_DIVIDE);
		}
		else
		{
			if (op != TK_MINUS && op != TK_PLUS)
				error("指针只能进行关系或加减运算");

			if (bt2 == T_PTR)
			{
				operand_swap();
			}
			type1 = optop[-1].type;
			operand_push(&int_type, SC_GLOBAL, pointed_size(&optop[-1].type));
			gen_op(TK_STAR);

			gen_opi(op);
			optop->type = type1;
		}
	}
	else
	{
		gen_opi(op);
		if (op >= TK_EQ && op <= TK_GEQ)
		{
			optop->type.t = T_INT;
		}
	}
}

int allocate_reg(int rc)
{
	int r;
	Operand *p;
	int used;

	for (r = 0; r <= REG_EBX; r++)
	{
		if (rc & REG_ANY || r == rc)
		{
			used = 0;
			for (p = opstack; p <= optop; p++)
			{
				if ((p->r & SC_VALMASK) == r)
					used = 1;
			}
			if (used == 0)
				return r;
		}
	}

	for (p = opstack; p <= optop; p++)
	{
		r = p->r & SC_VALMASK;
		if (r < SC_GLOBAL && (rc & REG_ANY || r == rc))
		{
			spill_reg(r);
			return r;
		}
	}

	return -1;
}

void spill_reg(int r)
{
	int size, align;
	Operand *p, opd;
	Type *type;

	for (p = opstack; p <= optop; p++)
	{
		if ((p->r & SC_VALMASK) == r)
		{
			r = p->r & SC_VALMASK;
			type = &p->type;
			if (p->r & SC_LVAL)
				type = &int_type;
			size = type_size(type, &align);
			loc = calc_align(loc - size, align);
			operand_assign(&opd, type->t, SC_LOCAL | SC_LVAL, loc);
			store(r, &opd);
			if (p->r & SC_LVAL)
			{
				p->r = (p->r & ~(SC_VALMASK)) | SC_LLOCAL;
			}
			else
			{
				p->r = SC_LOCAL | SC_LVAL;
			}
			p->value = loc;
			break;
		}
	}
}

void spill_regs()
{
	int r;
	Operand *p;
	for (p = opstack; p <= optop; p++)
	{
		r = p->r & SC_VALMASK;
		if (r < SC_GLOBAL)
		{
			spill_reg(r);
		}
	}
}

int gen_jmpforward(int t)
{
	gen_opcode1(0xe9);
	return makelist(t);
	;
}

void gen_jmpbackword(int a)
{
	int r;
	r = a - ind - 2;
	if (r == (char)r)
	{
		gen_opcode1(0xeb);
		gen_byte(r);
	}
	else
	{
		gen_opcode1(0xe9);
		gen_dword(a - ind - 4);
	}
}

int gen_jcc(int t)
{
	int v;
	int inv = 1;

	v = optop->r & SC_VALMASK;
	if (v == SC_CMP)
	{
		gen_opcode2(0x0f, optop->value ^ inv);
		t = makelist(t);
	}
	else
	{
		if ((optop->r & (SC_VALMASK | SC_LVAL | SC_SYM)) == SC_GLOBAL)
		{

			t = gen_jmpforward(t);
		}
		else
		{
			v = load_1(REG_ANY, optop);

			gen_opcode1(0x85);
			gen_modrm(ADDR_REG, v, v, NULL, 0);

			gen_opcode2(0x0f, 0x85 ^ inv);
			t = makelist(t);
		}
	}
	operand_pop();
	return t;
}

void gen_prolog(Type *func_type)
{
	int addr, align, size, func_call;
	int param_addr;
	Symbol *sym;
	Type *type;

	sym = func_type->ref;
	func_call = sym->r;
	addr = 8;
	loc = 0;

	func_begin_ind = ind;
	ind += FUNC_PROLOG_SIZE;
	if (sym->type.t == T_STRUCT)
		error("不支持返回结构体，可以返回结构体指针");

	while ((sym = sym->next) != NULL)
	{
		type = &sym->type;
		size = type_size(type, &align);
		size = calc_align(size, 4);

		if ((type->t & T_BTYPE) == T_STRUCT)
		{
			size = 4;
		}

		param_addr = addr;
		addr += size;

		sym_push(sym->v & ~SC_PARAMS, type,
			 SC_LOCAL | SC_LVAL, param_addr);
	}
	func_ret_sub = 0;

	if (func_call == KW_STDCALL)
		func_ret_sub = addr - 8;
}

void gen_epilog()
{
	int v, saved_ind, opc;

	gen_opcode1(0x8b);
	gen_modrm(ADDR_REG, REG_ESP, REG_EBP, NULL, 0);

	gen_opcode1(0x58 + REG_EBP);

	if (func_ret_sub == 0)
	{
		gen_opcode1(0xc3);
	}
	else
	{
		gen_opcode1(0xc2);
		gen_byte(func_ret_sub);
		gen_byte(func_ret_sub >> 8);
	}

	v = calc_align(-loc, 4);
	saved_ind = ind;
	ind = func_begin_ind;

	gen_opcode1(0x50 + REG_EBP);

	gen_opcode1(0x89);
	gen_modrm(ADDR_REG, REG_ESP, REG_EBP, NULL, 0);

	gen_opcode1(0x81);
	opc = 5;
	gen_modrm(ADDR_REG, opc, REG_ESP, NULL, 0);
	gen_dword(v);
	ind = saved_ind;
}

void init_variable(Type *type, Section *sec, int c, int v)
{
	int bt;
	void *ptr;

	if (sec)
	{
		if ((optop->r & (SC_VALMASK | SC_LVAL)) != SC_GLOBAL)
			error("全局变量必须用常量初始化");

		bt = type->t & T_BTYPE;
		ptr = sec->data + c;

		switch (bt)
		{
		case T_CHAR:
			*(char *)ptr = optop->value;
			break;
		case T_SHORT:
			*(short *)ptr = optop->value;
			break;
		default:
			if (optop->r & SC_SYM)
			{
				coffreloc_add(sec, optop->sym, c, IMAGE_REL_I386_DIR32);
			}
			*(int *)ptr = optop->value;
			break;
		}
		operand_pop();
	}
	else
	{
		if (type->t & T_ARRAY)
		{
			operand_push(type, SC_LOCAL | SC_LVAL, c);
			operand_swap();
			spill_reg(REG_ECX);

			gen_opcode1(0xB8 + REG_ECX);
			gen_dword(optop->type.ref->c);
			gen_opcode1(0xB8 + REG_ESI);
			gen_addr32(optop->r, optop->sym, optop->value);
			operand_swap();

			gen_opcode1(0x8D);
			gen_modrm(ADDR_OTHER, REG_EDI, SC_LOCAL, optop->sym, optop->value);

			gen_prefix(0xf3);

			gen_opcode1(0xA4);
			optop -= 2;
		}
		else
		{
			operand_push(type, SC_LOCAL | SC_LVAL, c);
			operand_swap();
			store0_1();
			operand_pop();
		}
	}
}

Section *allocate_storage(Type *type, int r, int has_init, int v, int *addr)
{
	int size, align;
	Section *sec = NULL;
	size = type_size(type, &align);

	if (size < 0)
	{
		if (type->t & T_ARRAY && type->ref->type.t == T_CHAR)
		{
			type->ref->c = strlen((char *)tkstr.data) + 1;
			size = type_size(type, &align);
		}
		else
			error("类型尺寸未知");
	}

	if ((r & SC_VALMASK) == SC_LOCAL)
	{
		loc = calc_align(loc - size, align);
		*addr = loc;
	}
	else
	{

		if (has_init == 1)
			sec = sec_data;
		else if (has_init == 2)
			sec = sec_rdata;
		else
			sec = sec_bss;

		sec->data_offset = calc_align(sec->data_offset, align);
		*addr = sec->data_offset;
		sec->data_offset += size;

		if (sec->sh.Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA &&
		    sec->data_offset > sec->data_allocated)
			section_realloc(sec, sec->data_offset);

		if (v == 0)
		{
			operand_push(type, SC_GLOBAL | SC_SYM, *addr);
			optop->sym = sym_sec_rdata;
		}
	}
	return sec;
}

__END_DECLS