#ifndef SYMBOL_H
#define SYMBOL_H

#include "mcc/mcc_base.h"
#include "mcc/stack.h"
#include "mcc/type.h"

__BEGIN_DECLS

enum
{
	STORE_GLOBAL = 0x0,
	STORE_LOCAL = 0x1,
	STORE_MASK = 0xF,

	STORE_LEFT_VAL = 0x10,

	STORE_FUNC_TEMP = 0x10000000,
	STORE_PARAM_TEMP = 0x20000000,
	STORE_TEMP = 0x40000000,
};

#define LOCAL_VARIABLE(store_type) (((store_type) & STORE_MASK) == STORE_LOCAL)
#define GLOBAL_VARIABLE(store_type) (((store_type) & STORE_MASK) == STORE_GLOBAL)

typedef struct sym
{
	Type t;
	int tk_index; // the token index of the symbol, can be easily got from token array

	struct sym *slib;
	struct sym *parent;
} *Sym;

int sym_global_init();
void sym_global_destroy();

Sym sym_create(int tok_index, PType pt);
Sym sym_push(int tok_index, PType pt, int storage_type);
// only local scope symbols need to be popped
void sym_pop_to(Stack stk, Sym s);
Sym sym_push_func(int tok_index, PType pt);
Sym sym_push_var(int tok_index, PType pt, int st);
Sym sym_find_identifier(int tok_index);

void sym_dump(Sym s);

__END_DECLS

#endif