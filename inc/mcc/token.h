#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

#include "mcc/mcc_base.h"
#include "mcc/string.h"
#include "mcc/type.h"

#define EOB '\\'
#ifndef EOF
#define EOF (-1)
#endif

typedef enum token_enum
{
	#define DEF_TOK(t, s) t,
	#include "mcc/token_internal.h"
	#undef DEF_TOK
} Token_enum;

typedef enum token_sub_type
{
	TK_SUB_TYPE_NONE = 0,

	TK_SUB_TYPE_NUMBER = 0x1,
	TK_SUB_TYPE_STRING = 0x2,
	TK_SUB_TYPE_POINTER = 0x4,
	TK_SUB_TYPE_REAL = 0x8,
	TK_SUB_TYPE_VARIABLE = 0x10,
	TK_SUB_TYPE_FUNCTION = 0x20,
	TK_SUB_TYPE_LABEL = 0x40,
	TK_SUB_TYPE_CONSTANT = 0x80,

	TK_SUB_TYPE_MAX_CNT = 8,  // ** Needs to update if change sub type enums **
} Token_sub_type;

typedef struct token_value
{
	union
	{
		uint64_t i;
		long double d;
		void *p;
	} v;
} *TokenValue;

typedef struct token
{
	Type t;
	Token_enum type; // TOKEN_INT, TOKEN_ID ...
	Token_sub_type sub_type;

	unsigned int len; // the token's length
	struct token_value val;
} *Token;

#define APPEND_TYPE(pt, sub_type) ((pt)->sub_type |= (sub_type))
#define SET_TYPE(pt, sub_type) ((pt)->sub_type = (sub_type))

__BEGIN_DECLS

Token token_alloc();
void token_set_pointer(Token pt, void *p);
void token_set_str(Token pt, char *s);
void token_free();

void token_dump(Token pt);

const char *token_enum_to_name(Token_enum t);
Cstr token_sub_type_enum_to_name(Token_sub_type sub_type);

char *token_get_name(Token pt);

void show_all_tokens();

__END_DECLS

#endif