#ifndef OPERAND_H
#define OPERAND_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"

__BEGIN_DECLS

typedef struct operand
{
	Type type;
	unsigned short r;
	int value;
	struct Symbol *sym;
} *Operand;

void operand_push(Type *type, int r, int value);
void operand_pop();

void operand_swap();
void operand_assign(Operand *opd, int t, int r, int v);

void cancel_lvalue();
void indirection();

__END_DECLS

#endif