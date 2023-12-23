#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/size.h"
#include "mcc/id.h"

__BEGIN_DECLS

const static char *Char_type_str[] = { "digit", "alpha", "other" };

int is_identifier(const char *str)
{
	char c;

	if (!is_id_start(*str))
		return 0;

	++str;
	while ((c = *str)) {
		if (!is_id(c))
			return 0;
		++str;
	}
	return 1;
}

static CHAR_TYPE char_type_table[CHAR_CNT];

__need_init void init_char_type_table()
{
	int i;

	for (i = 0; i < CHAR_CNT; i++) {
		char c = (char)i;
		if (isalpha(c))
			char_type_table[i] = CHAR_TYPE_ALPHA;
		else if (isdigit(c))
			char_type_table[i] = CHAR_TYPE_DIGIT;
		else
			char_type_table[i] = CHAR_TYPE_OTHER;
	}
}

CHAR_TYPE get_char_type(char c)
{
	return char_type_table[(unsigned char)c];
}

const char *get_char_type_str(CHAR_TYPE type)
{
	return Char_type_str[type];
}

__END_DECLS