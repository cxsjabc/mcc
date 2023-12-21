#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>

#include "mcc/hash.h"
#include "mcc/log.h"

__BEGIN_DECLS

void test_hash()
{
	char *arr[] = {
			"a", "b", "c", "d", "e", "int", "int8", "int16", "int32", "int64",
        		"uint", "uint8", "uint16", "uint32", "uint64", "float", "success",
		      };
	int i;

	for (i = 0; i < sizeof(arr) / sizeof(char *); i++) {
		char *s = arr[i];
		unsigned int hash = ELFHash(s);
		debug("hash(%s) = %u\n", s, hash);
	}
}

__END_DECLS