#include <stdio.h>

#include "mcc/token.h"
#include "mcc/size.h"

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