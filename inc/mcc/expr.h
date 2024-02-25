#ifndef EXPR_H
#define EXPR_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"

__BEGIN_DECLS

void expr();
void assignment_expr();
void constant_expr();
void cond_expr();
void logic_or_expr();
void logic_and_expr();
void or_expr();
void xor_expr();
void and_expr();
void equality_expr();
void relational_expr();
void shift_expr();
void additive_expr();
void multiple_expr();
void unary_expr();
void sizeof_expr();
void postfix_expr();
void prim_expr();
void args_list();

__END_DECLS

#endif