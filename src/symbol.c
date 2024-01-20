#include <stdio.h>

#include "mcc/array.h"
#include "mcc/compile_log.h"
#include "mcc/error.h"
#include "mcc/lex.h"
#include "mcc/mem.h"
#include "mcc/symbol.h"
#include "mcc/stack.h"
#include "mcc/token.h"

__BEGIN_DECLS

DynArray global_sym_arr;
Stack curr_scope_sym_stk;

int sym_global_init()
{
	global_sym_arr = dynamic_array_create(8);
	NULL_ABORT(global_sym_arr, "global_symbol_arr created failed");
	curr_scope_sym_stk = stack_create(8);
	NULL_ABORT(curr_scope_sym_stk, "curr_scope_sym_stk created failed");

	return OK;
}

void sym_global_destroy()
{
	stack_destroy(curr_scope_sym_stk);
	dynamic_array_destroy(global_sym_arr);
}

Sym sym_create(int tok_index, PType pt)
{
	Sym s = allocmz(sizeof(*s));
	NULL_ABORT(s, "allocmz symbol failed");
	s->tk_index = tok_index;
	s->t = *pt;

	return s;
}

Sym sym_push(int tok_index, PType pt, int storage_type)
{
	Sym s = sym_create(tok_index, pt);
	NULL_ABORT(s, "sym_create failed");

	if (storage_type == STORE_GLOBAL)
		dynamic_array_push(global_sym_arr, s);
	else if (storage_type == STORE_LOCAL)
		stack_push(curr_scope_sym_stk, s);
	else
		fatal("Unsupported storage type: %d\n", storage_type);

	return s;
}

// only local scope symbols need to be popped
void sym_pop_to(Stack stk, Sym s)
{
	Sym top;

	top = (Sym)stack_top(stk);
	while (top != s) {
		stack_pop(stk);
		top = (Sym)stack_top(stk);
	}
}

Sym sym_push_func(int tok_index, PType pt)
{
	return sym_push(tok_index, pt, STORE_GLOBAL);
}

Sym sym_push_var(int tok_index, PType pt, int st)
{
	Sym exist;
	const char *s;

	exist = sym_find_identifier(tok_index);
	if (exist) {
		s = token_get_name_by_index(tok_index);
		cerror("Re-definition variable: %s\n", s);
		return NULL;
	}
	return sym_push(tok_index, pt, st);
}

Sym sym_find_identifier(int tok_index)
{
	Token t;

	t = (Token)dynamic_array_get(TokenArray, tok_index);
	return t->identifier;
}

__END_DECLS