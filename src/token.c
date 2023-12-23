#include <stdio.h>

#include "mcc/mem.h"
#include "mcc/size.h"
#include "mcc/token.h"

__BEGIN_DECLS

const char *TokenNames[] =
{
	#define DEF_TOK(t, s) s,
	#include "mcc/token_internal.h"
	#undef DEF_TOK
};

const char **get_token_arr()
{
	return TokenNames;
}

const int TokenSize = ARRAY_SIZE(TokenNames);

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