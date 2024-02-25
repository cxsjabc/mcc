#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/compile_log.h"
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

DynArray opstack;
Operand optop;

__need_init void operand_global_init()
{
	opstack = dynamic_array_create(8);
	optop = vec_at(opstack, -1);
}

void operand_push(Type *type, int r, int value)
{
	dynamic_array_push(opstack, allocmz(sizeof(struct operand)));
	optop = (Operand)vec_at(opstack, vec_size(opstack) - 1);
	optop->type = *type;
	optop->r = r;
	optop->value = value;
}

void operand_pop()
{
	dynamic_array_pop(opstack);
	optop = (Operand)vec_at(opstack, vec_size(opstack) - 1);
}

void operand_swap()
{
	Operand tmp, tmp1;

	tmp = (Operand)vec_at(opstack, vec_size(opstack) - 1);
	tmp1 = (Operand)vec_at(opstack, vec_size(opstack) - 2);
	
	dynamic_array_set(opstack, vec_size(opstack) - 2, tmp);
	dynamic_array_set(opstack, vec_size(opstack) - 1, tmp1);
}

__END_DECLS