#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/string.h"
#include "mcc/token.h"

#include "test.h"

__BEGIN_DECLS

void test_parse_tokens()
{
	Token t;
	char *s = "int i 0;";
	char *p = s;

	TEST_BEGIN;

	do {
		t = next_token(&p);
		if (!t)
			break;
		
		token_dump(t);

		//p += t->len;
	} while (1);
	TEST_END;
}

void test_chars_from_file()
{
	File f;
	int c = 0;
	char *s = "./test/data/2.source";

	TEST_BEGIN;
	f = file_open(s);
	assert(f);

	do {
		c = next_char(f);
		debug_nofl("%c", c);
	} while (c != EOF);

	file_close(f);
	TEST_END;
}

void test_lex()
{
	TEST_BEGIN;
	test_parse_tokens();
	test_chars_from_file();
	TEST_END;
	return;
}

__END_DECLS