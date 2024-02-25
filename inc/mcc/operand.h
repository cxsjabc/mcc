#ifndef OPERAND_H
#define OPERAND_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"
#include "mcc/symbol.h"

__BEGIN_DECLS

typedef struct operand
{
	Type type;
	unsigned short r;
	int value;
	struct sym *sym;
} *Operand;

void operand_push(Type *type, int r, int value);
void operand_pop();

void operand_swap();

__END_DECLS

#endif