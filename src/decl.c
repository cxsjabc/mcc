#include <stdio.h>

#include "mcc/compile.h"
#include "mcc/compile_log.h"
#include "mcc/decl.h"
#include "mcc/error.h"
#include "mcc/lex.h"
#include "mcc/token.h"
#include "mcc/type.h"

__BEGIN_DECLS

int find_type_specifier(Type *t)
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
			b |= MT_INT;
			type_found = 1;
			break;

		case TOK_SIGNED:
			if (b & MT_SIGNED || b & MT_UNSIGNED)
				cerror("Multiple signed or unsigned specifier\n");
			b |= MT_SIGNED;
			type_found = 1;
			break;
		case TOK_UNSIGNED:
			if (b & MT_SIGNED || b & MT_UNSIGNED)
				cerror("Multiple signed or unsigned specifier\n");
			b |= MT_UNSIGNED;
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

int parse_global_decl()
{
	int r = OK;
	Type t;

	r = find_type_specifier(&t);
	debug("type specifier found: %d, type: 0x%x\n", r, t.t);

	return r;
}

__END_DECLS