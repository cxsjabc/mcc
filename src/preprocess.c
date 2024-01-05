#include <stdio.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/preprocess.h"

__BEGIN_DECLS

int pp_process(File f)
{
	return pp_process_str(&f->buf);
}

int pp_parse_include(File f)
{
	return pp_parse_include_str(&f->buf);
}

int pp_process_str(char **ps)
{
	Token t;
	int r = OK;
	LHD;
	t = str_next_token(ps);
	switch (t->type)
	{
	case TOK_INCLUDE:
		LHD;
		r = pp_parse_include_str(ps);
		while(1);
		break;
	
	default:
		break;
	}

	return r;
}

int pp_parse_include_str(char **ps)
{
	LHD;
	return ERR_FAIL;
}

__END_DECLS