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

void assignment_expression()
{
	equality_expression();
	if (Tok->type == TOK_ASSIGN) 
	{
		check_lvalue();
		NEXT;
		assignment_expression();
		store0_1();
	}
}

void equality_expression()
{
	Token t;

	relational_expression();
	while (Tok->type == TOK_EQUAL || Tok->type == TOK_NEQ) 
	{
		t = Tok;
		NEXT;
		relational_expression();
		gen_op(t);
	}
}

void relational_expression()
{
	int t;
	additive_expression();
	while ((token == TK_LT || token == TK_LEQ) ||
		token == TK_GT || token == TK_GEQ) 
	{
		t = token;
		get_token();
		additive_expression();
		gen_op(t);
	}
}

__END_DECLS