#include <assert.h>
#include <stdio.h>

#include "mcc/error.h"
#include "mcc/mem.h"
#include "mcc/size.h"
#include "mcc/string.h"
#include "mcc/token.h"

__BEGIN_DECLS

const char *TokenNames[] =
{
	#define DEF_TOK(t, s) s,
	#include "mcc/token_internal.h"
	#undef DEF_TOK
};

const char *TokenSubTypeNames[] =
{
	"None", "Number", "String", "Pointer", "Real",
	"Varaible", "Function", "Label", "Const",
};

const char **get_token_arr()
{
	return TokenNames;
}

const int TokenSize = ARRAY_SIZE(TokenNames);

const char *token_enum_to_name(Token_enum t)
{
	if (t >= TokenSize)
		return NULL;
	return TokenNames[t];
}

// ** This will generate new string, so, caller should free the memory! **
Cstr token_sub_type_enum_to_name(Token_sub_type sub_type)
{
	int i, r;
	Cstr cstr;
	
	cstr = cstr_alloc(128);
	assert(cstr);
	for (i = 0; i < TK_SUB_TYPE_MAX_CNT; ++i) {
		silence("token sub type: 0x%x 0x%x\n", sub_type, (1 << i));
		if (sub_type & (1 << i)) {
			r = cstr_append(cstr, TokenSubTypeNames[i + 1], strlen(TokenSubTypeNames[i + 1]));
			if (r != OK) {
				cstr_free(cstr);
				return NULL;
			}
		}
	}
	return cstr;
}

Token token_alloc()
{
	return (Token)allocmz(sizeof(struct token));
}

void token_set_pointer(Token pt, void *p)
{
	pt->val.v.p = p;
	pt->sub_type |= TK_SUB_TYPE_POINTER;
}

void token_set_str(Token pt, char *s)
{
	token_set_pointer(pt, s);
	pt->sub_type |= TK_SUB_TYPE_STRING;
}

void token_free()
{
	// Do nothing because all token memory is allocated on the memory buffer,
	// it will be freed after the whole compilation is done.
}

void token_dump(Token t)
{
	Cstr str;
	char *sub_type_name = NULL;
	char *name;

	if (!t) {
		always("Token: NULL.\n");
		return;
	}
	str = token_sub_type_enum_to_name(t->sub_type);
	if (str)
		sub_type_name = str->str;
	name = token_get_name(t);
	always("Token: type(%s), subtype(%s), len(%d)",
		token_enum_to_name(t->type), 
		sub_type_name,
		t->len);
	if (name)
		str_dump_with_len(name, t->len, ", name: "), always("\n");
	else if (t->sub_type & TK_SUB_TYPE_NUMBER)
		always(" number: 0x%lx", t->val.v.i), always("\n");

	cstr_free(str);
}

char *token_get_name(Token pt)
{
	if (pt->type == TOK_IDENTIFIER || (pt->type >= TOK_AUTO && pt->type <= TOK_WHILE))
		return (char *)pt->val.v.p;
	return NULL;
}

void show_all_tokens()
{
	unsigned int i;

	printf("TokenSize: %d\n", TokenSize);
	for (i = 0; i < ARRAY_SIZE(TokenNames); i++)
	{
		printf("%s ", TokenNames[i]);
		if ((i + 1) % 5 == 0)
			printf("\n");
	}
	printf("\n");
}

__END_DECLS