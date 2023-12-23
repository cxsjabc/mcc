#ifndef TOKEN_H
#define TOKEN_H

#include "mcc/mcc_base.h"

enum token_enum
{
	#define DEF_TOK(t, s) t,
	#include "mcc/token_internal.h"
	#undef DEF_TOK
};

typedef struct token_value
{
	union
	{
		char *str;
		int i;
		double d;
		void* p;
	} v;
	BOOL is_str;
	BOOL is_pointer;
	BOOL is_integer;
	BOOL is_real;
} TokenValue;

typedef struct token
{
	enum token_enum type;
	TokenValue val;
} Token;

__BEGIN_DECLS

void show_all_tokens();

__END_DECLS

#endif