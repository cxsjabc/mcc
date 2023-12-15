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

// TODO
const static CHAR_TYPE char_type_table[] = 
{
	CHAR_TYPE_DIGIT, 
};

CHAR_TYPE get_char_type(char c)
{
	return char_type_table[(unsigned char)c];
}

__END_DECLS