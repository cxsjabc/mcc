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
const char *token_sub_type_enum_to_name(Token_sub_type sub_type)
{
	int i, r;
	PCstr cstr;
	
	cstr = alloc_cstr(128);
	assert(cstr);
	for (i = 0; i < TK_SUB_TYPE_MAX_CNT; ++i) {
		debug("token sub type: 0x%x 0x%x\n", sub_type, (1 << i));
		if (sub_type & (1 << i)) {
			r = cstr_append(cstr, TokenSubTypeNames[i + 1], strlen(TokenSubTypeNames[i + 1]));
			if (r != OK) {
				free_cstr(cstr);
				return NULL;
			}
		}
	}
	return cstr->str;
}

Token *token_alloc()
{
	return (Token *)allocmz(sizeof(Token));
}

void token_set_pointer(Token *pt, void *p)
{
	pt->val.v.p = p;
	pt->sub_type |= TK_SUB_TYPE_POINTER;
}

void token_set_str(Token *pt, char *s)
{
	token_set_pointer(pt, s);
	pt->sub_type |= TK_SUB_TYPE_STRING;
}

void token_free()
{
	// Do nothing because all token memory is allocated on the memory buffer,
	// it will be freed after the whole compilation is done.
}

char *token_get_name(Token *pt)
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