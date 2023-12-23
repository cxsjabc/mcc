#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/string.h"
#include "mcc/token.h"

#include "test.h"

__BEGIN_DECLS

void test_parse_tokens()
{
	struct token *t;
	char *s = "int i = 10;";
	char *p = s;

	TEST_BEGIN;

	do {
		t = next_token(p);
		if (!t)
			break;
		debug("Token: type(%s), subtype(%s), len(%d), name(%s)\n",
		      token_enum_to_name(t->type), 
		      token_sub_type_enum_to_name(t->sub_type),
		      t->len, mcc_strdup(token_get_name(t), t->len));

		p += t->len;
	} while (1);
	TEST_END;
}

void test_lex()
{
	TEST_BEGIN;
	test_parse_tokens();
	TEST_END;
	return;
}

__END_DECLS