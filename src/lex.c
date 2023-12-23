#include <assert.h>
#include <stdio.h>

#include "mcc/error.h"
#include "mcc/id.h"
#include "mcc/keyword.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/string.h"

__BEGIN_DECLS

Token *next_token(char *s)
{
	char *ps;
	char c;
	int r;
	Token *pt = NULL;

	skip_blanks(&s);
	ps = s;
	LHD;
	c = *ps;
	if (is_id(c))
		r = parse_identifier(ps + 1, &pt);
	else if (is_digit(c))
		r = parse_number(ps + 1, &pt);
	else
		r = ERR_UNKNOWN;

	LHD;
	if (r != OK)
		return NULL;
	LHD;
	return pt;
}

int parse_identifier(char *s, Token **ppt)
{
	Token *pt = *ppt;
	char *b = s - 1;
	int tok;

	LHD;
	pt = token_alloc();
	assert(pt);
	LHD;
	while (is_id(*s)) {
		++s;
	}

	pt->len = s - b;
	debug("Parse \"%s\", len: %d\n", b, pt->len);
	if ((tok = is_keyword_with_len(b, pt->len)))
		pt->type = tok;
	else {
		pt->type = TOK_IDENTIFIER;
		pt->sub_type = TK_SUB_TYPE_VARIABLE;
	}
	pt->val.v.p = b;
	*ppt = pt;
	LHD;
	return OK;
}

int parse_number(char *s, Token **pt)
{
	return OK;
}

__END_DECLS