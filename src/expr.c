#include <stdio.h>

#include "mcc/compile.h"
#include "mcc/compile_log.h"
#include "mcc/decl.h"
#include "mcc/error.h"
#include "mcc/expr.h"
#include "mcc/lex.h"
#include "mcc/operand.h"
#include "mcc/symbol.h"
#include "mcc/token.h"
#include "mcc/type.h"

// refer to c standards

__BEGIN_DECLS

void expr()
{
	while (1) 
	{
		assignment_expr();
		if (Tok->type != TOK_COMMA)
			break;
		NEXT;
	}
}

void assignment_expr()
{
	cond_expr();
	if (Tok->type == TOK_ASSIGN) 
	{
		NEXT;
		assignment_expr();
	}
}

void constant_expr()
{
	cond_expr();
}

void cond_expr()
{
	logic_or_expr();
	if (Tok->type == TOK_QUESTION) {
		expr();
		if (Tok->type == TOK_COLON) {
			NEXT;
			cond_expr();
		} else
			expect(":");
	}
}

void logic_or_expr()
{
	logic_and_expr();
	if (Tok->type == TOK_LOR) {
		NEXT;
		logic_and_expr();
	}
}

void logic_and_expr()
{
	or_expr();
	if (Tok->type == TOK_LAND) {
		NEXT;
		or_expr();
	}
}

void or_expr()
{
	xor_expr();
	while (Tok->type == TOK_OR) {
		NEXT;
		xor_expr();
	}
}

void xor_expr()
{
	and_expr();
	while (Tok->type == TOK_XOR) {
		NEXT;
		and_expr();
	}
}

void and_expr()
{
	equality_expr();
	while (Tok->type == TOK_AND) {
		NEXT;
		equality_expr();
	}
}

void equality_expr()
{
	Token t;

	relational_expr();
	while (Tok->type == TOK_EQUAL || Tok->type == TOK_NEQ) 
	{
		t = Tok;
		(void)t;
		NEXT;
		relational_expr();
	}
}

void relational_expr()
{
	shift_expr();
	while ((Tok->type == TOK_LESS || Tok->type == TOK_GREATER) ||
		Tok->type == TOK_LESS_EQ || Tok->type == TOK_GREATER_EQ) 
	{
		NEXT;
		shift_expr();
	}
}

void shift_expr()
{
	additive_expr();
	while (Tok->type == TOK_LSHIFT || Tok->type == TOK_RSHIFT) 
	{
		NEXT;
		additive_expr();
	}
}

void additive_expr()
{
	multiple_expr();
	while (Tok->type == TOK_PLUS || Tok->type == TOK_MINUS) 
	{
		NEXT;
		multiple_expr();
	}
}

void multiple_expr()
{
	unary_expr();
	while (Tok->type == TOK_STAR || Tok->type == TOK_DIV || Tok->type == TOK_MOD) 
	{
		NEXT;
		unary_expr();
	}
}

void unary_expr()
{
	switch (Tok->type) {
	case TOK_MINUS:
		NEXT;
		unary_expr();
		break;
	case TOK_SIZEOF:
		sizeof_expr();
		break;
	default:
		postfix_expr();
		break;
	}
}

void sizeof_expr()
{
	Type t;
	int size;

	NEXT;
	skip(TOK_LPAREN);
	parse_type_specifier(&t);
	skip(TOK_RPAREN);
	
	size = type_size(&t);
	if (size < 0)
		cerror("sizeof size is negative\n");
}

void postfix_expr()
{
	prim_expr();
	while (1) {
		if (Tok->type == TOK_LPAREN) {
			args_list();
		} else
			break;
	}
}

void prim_expr()
{
	Type type;

	type = Tok->t;
	if (IS_INT(type.t)) {
		NEXT;
	} else if (Tok->sub_type == TK_SUB_TYPE_STRING) {
		int t = MT_CHAR;
		type.t = t;
		// TODO
	} else {
		if (Tok->type != TOK_IDENTIFIER)
			expect("identifier");
	}
}

void args_list()
{
	Operand ret = NULL, optop = NULL; // TODO
	Sym s, sa;
	int nb_args;

	s = optop->type.sym;
	NEXT;
	sa = s->slib; 
	nb_args = 0;
	ret->type = s->t;
	ret->r = 0;
	ret->value = 0;

	(void) ret;

	if (Tok->type != TOK_RPAREN) 
	{
		for( ; ; ) 
		{
			assignment_expr();
			nb_args++;
			if (sa)
				sa = sa->slib;
			if (Tok->type == TOK_RPAREN)
				break;
			skip(TOK_COMMA);
		}
	}   
	if (sa)
		cerror("arguments is less than paramters\n");
	skip(TOK_RPAREN);
}

__END_DECLS