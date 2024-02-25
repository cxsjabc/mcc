#include <stdio.h>

#include "mcc/compile.h"
#include "mcc/compile_log.h"
#include "mcc/decl.h"
#include "mcc/error.h"
#include "mcc/lex.h"
#include "mcc/symbol.h"
#include "mcc/token.h"
#include "mcc/type.h"

__BEGIN_DECLS

void skip(int tok)
{
	if (Tok->type != tok)
		expect("%s", token_enum_to_name(tok));
	debug("skip %s\n", token_enum_to_name(tok));
	NEXT;
}

int parse_type_specifier(Type *t)
{
	int type_found = 0;
	int tok_type;
	int b = 0;

	while (1) {
		if (!Tok)
			goto end;
		tok_type = Tok->type;
		switch (tok_type) {
		case TOK_CHAR:
			if (BASIC_TYPE(b))
				cerror("Multiple type specifier\n");
			b |= MT_CHAR;
			type_found = 1;
			break;
		case TOK_INT:
			if (BASIC_TYPE(b))
				cerror("Multiple type specifier\n");
			LHD;
			b |= MT_INT;
			type_found = 1;
			break;

		case TOK_SIGNED:
			if (b & MT_SIGNED || b & MT_UNSIGNED)
				cerror("Multiple signed or unsigned specifier\n");
			b |= MT_SIGNED;
			b |= MT_INT;
			type_found = 1;
			break;
		case TOK_UNSIGNED:
			if (b & MT_SIGNED || b & MT_UNSIGNED)
				cerror("Multiple signed or unsigned specifier\n");
			b |= MT_UNSIGNED;
			b |= MT_INT;
			type_found = 1;
			break;

		case TOK_EXTERN:
			if (b & MT_EXTERN || b & MT_STATIC)
				cerror("Multiple storage specifier\n");
			b |= MT_EXTERN;
			break;
		case TOK_STATIC:
			if (b & MT_EXTERN || b & MT_STATIC)
				cerror("Multiple storage specifier\n");
			b |= MT_STATIC;
			break;
		default:
			if (!type_found)
				cerror("No type specifier\n");
			goto end;
		}
		NEXT;
	}

end:	
	if (!(b & MT_SIGNED) && !(b & MT_UNSIGNED))
		b |= MT_SIGNED;

	t->t = b;
	return type_found;
}

void parse_parameter(Type *t)
{
	Type lt;
	int val;
	Sym s;
	Sym *last, temp = NULL;

	last = &temp;
	while (1) {
		LHD;
		if (!parse_type_specifier(&lt))
			expect("type specifier");
		LHD;
		parse_declarator(&lt, &val);
		LHD;
		s = sym_push(val, &lt, STORE_LOCAL);
		LHD;
		*last = s;
		last = &s->slib;

		LHD;
		if (Tok->type == TOK_RPAREN)
			break;
		skip(TOK_COMMA);
		LHD;
	}
	skip(TOK_RPAREN);

	// add the function symbol
	s = sym_push(STORE_FUNC_TEMP, t, STORE_GLOBAL);
	s->slib = temp;
	t->t = MT_FUNC;
	t->sym = s;
	sym_dump(s);
}

int parse_declarator(Type *t, int *val)
{
	if (Tok->type == TOK_IDENTIFIER) {
		*val = Tok->tk_index;
		NEXT;
	} else
		expect("identifier");

	if (Tok->type == TOK_LPAREN) {
		NEXT;
		parse_parameter(t);
	}
	return OK;
}

void parse_function_body()
{

}

int parse_global_decl()
{
	int r = OK;
	int val;
	Type t;
	Sym sym;

	LHD;
	r = parse_type_specifier(&t);
	debug("type specifier found: %d, type: 0x%x\n", r, t.t);
	if (!r)
		expect("type specifier");

	while (1) {
		LHD;
		r = parse_declarator(&t, &val);
		LHD;
		if (Tok->type == TOK_LBRACE) { // function definition
			LHD;
			sym = sym_find_identifier(val);
			if (!sym) {
				sym = sym_push_func(val, &t);
				sym->slib = t.sym;
			} else {
				if ((t.t & 0xF) == MT_FUNC)
					cerror("duplicate function define\n");
				else
					sym->slib = t.sym;
			}
			// dump function parameter list
			sym_dump(sym);
			parse_function_body();
			break;
		} else { // variable or function decalration
			int need_init;
			LHD;
			if (IS_FUNC(t.t)) {
				LHD;
				if (sym_find_identifier(val) == NULL)
					sym_push(val, &t, STORE_GLOBAL);
			} else { // variable decalration or initialization
				need_init = (Tok->type == TOK_ASSIGN);
				if (need_init)
					NEXT;
			}

			if (Tok->type == TOK_SEMICOLON) { // declaration ends with ';'
				LHD;
				skip(TOK_SEMICOLON);
				LHD;
				break;
			} else if (Tok->type == TOK_COMMA) { // declaration list
				LHD;
				NEXT;
				continue;
			} else
				expect(";");
		}
	}

	return r;
}

__END_DECLS