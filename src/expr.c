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
	while (1) 
	{
		assignment_expr();
		if (Tok->type != TOK_COMMA)
			break;
		operand_pop();
		NEXT;
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
		NEXT;
		relational_expr();
		gen_op(t);
	}
}

void relational_expr()
{
	int t;

	shift_expr();
	while ((Tok->type == TOK_LESS || Tok->type == TOK_GREATER) ||
		Tok->type == TOK_LESS_EQ || Tok->type == TOK_GREATER_EQ) 
	{
		t = token;
		NEXT;
		shift_expr();
		gen_op(t);
	}
}

void shift_expr()
{
	Token t;

	additive_expr();
	while (Tok->type == TOK_LSHIFT || Tok->type == TOK_RSHIFT) 
	{
		t = Tok;
		NEXT;
		additive_expr();
		gen_op(t);
	}
}

void additive_expr()
{
	Token t;

	multiple_expr();
	while (Tok->type == TOK_PLUS || Tok->type == TOK_MINUS) 
	{
		t = Tok;
		NEXT;
		multiple_expr();
		gen_op(t);
	}
}

void multiple_expr()
{
	Token t;

	unary_expr();
	while (Tok->type == TOK_STAR || Tok->type == TOK_DIV || Tok->type == TOK_MOD) 
	{
		t = Tok;
		NEXT;
		unary_expr();
		gen_op(t);
	}
}

void unary_expr()
{
	switch (Tok->type) {
	case TOK_MINUS:
		NEXT;
		operand_push();
		unary_expr();
		gen_op(TOK_MINUS);
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
	operand_push();
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
		operand_push();
		NEXT;
	} else if (Tok->sub_type == TK_SUB_TYPE_STRING) {
		int t = MT_CHAR;
		type.t = t;
		mk_pointer(&type);
		type.t |= MT_ARRAY;
		
		allocate_storage(&type, SC_GLOBAL, 2, 0, &addr);
		var_sym_put(&type, SC_GLOBAL, 0, addr);
		initializer(&type, addr, sec);
	} else {
		if (Tok->type != TOK_IDENTIFIER)
			expect("identifier");
	}
}

void args_list()
{
	Operand ret;
	Sym *s,*sa;
	int nb_args;

	s = optop->type.ref;
	NEXT;
	sa = s->next; 
	nb_args = 0;
	ret.type = s->type;
	ret.r = REG_IRET;
	ret.value = 0;

	if (Tok->type != TOK_RPAREN) 
	{
		for( ; ; ) 
		{
			assignment_expr();
			nb_args++;
			if (sa)
				sa = sa->next;
			if (token == TOK_RPAREN)
				break;
			skip(TOK_COMMA);
		}
	}   
	if (sa)
		cerror("arguments is less than paramters\n");
	skip(TOK_RPAREN);
	gen_invoke(nb_args);

	operand_push(&ret.type, ret.r, ret.value);
}

__END_DECLS