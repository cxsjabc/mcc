#ifndef KEYWORD_H
#define KEYWORD_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

typedef struct keyword
{
	char *name;
	int len;
	int token;
} *Keyword;

int is_keyword(const char *str);
int is_keyword_with_len(const char *str, int len);

__END_DECLS

#endif