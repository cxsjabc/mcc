#include <stdio.h>

#include "mcc/compile.h"
#include "mcc/compile_log.h"
#include "mcc/decl.h"
#include "mcc/error.h"
#include "mcc/lex.h"
#include "mcc/symbol.h"
#include "mcc/token.h"
#include "mcc/type.h"

// refer to c standards

__BEGIN_DECLS

void expr()
{
	assignment_expr();
	while (Tok->type == TOK_COMMA) 
	{
		NEXT;
		assignment_expr();
		store0_1();
	}
}

void assignment_expr()
{
	cond_expr();
	if (Tok->type == TOK_ASSIGN) 
	{
		check_lvalue();
		NEXT;
		assignment_expr();
		store0_1();
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

void equality_expr()
{
	Token t;

	relational_expr();
	while (Tok->type == TOK_EQUAL || Tok->type == TOK_NEQ) 
	{
		t = Tok;
		NEXT;
		relational_expr();
		gen_op(t);
	}
}

void relational_expr()
{
	int t;
	additive_expr();
	while ((token == TK_LT || token == TK_LEQ) ||
		token == TK_GT || token == TK_GEQ) 
	{
		t = token;
		get_token();
		additive_expr();
		gen_op(t);
	}
}

__END_DECLS