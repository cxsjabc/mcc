#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/env.h"
#include "mcc/id.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/mem_buf.h"
#include "mcc/operand.h"
#include "mcc/symbol.h"
#include "mcc/type.h"

__BEGIN_DECLS

void operand_push(Type *type, int r, int value)
{
	if (optop >= opstack + (OPSTACK_SIZE - 1))
		error("内存分配失败");
	optop++;
	optop->type = *type;
	optop->r = r;
	optop->value = value;
}

void operand_pop()
{
	optop--;
}

void operand_swap()
{
	Operand tmp;

	tmp = optop[0];
	optop[0] = optop[-1];
	optop[-1] = tmp;
}

void check_lvalue()
{
	if (!(optop->r & SC_LVAL))
		expect("左值");
}

void operand_assign(Operand *opd, int t, int r, int value)
{
	opd->type.t = t;
	opd->r = r;
	opd->value = value;
}

void cancel_lvalue()
{
	check_lvalue();
	optop->r &= ~SC_LVAL;
}

void indirection()
{
	if ((optop->type.t & T_BTYPE) != T_PTR)
	{
		if ((optop->type.t & T_BTYPE) == T_FUNC)
			return;
		expect("指针");
	}
	if ((optop->r & SC_LVAL))
		load_1(REG_ANY, optop);
	optop->type = *pointed_type(&optop->type);

	// 数组与函数不能为左值
	if (!(optop->type.t & T_ARRAY) && (optop->type.t & T_BTYPE) != T_FUNC)
	{
		optop->r |= SC_LVAL;
	}
}

__END_DECLS