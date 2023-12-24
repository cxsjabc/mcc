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
	Token t;
	char *s = "int i = 10;";
	char *p = s;

	TEST_BEGIN;

	do {
		t = next_token(p);
		if (!t)
			break;
		
		token_dump(t);

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