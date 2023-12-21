#include <stdio.h>

#include "mcc/id.h"
#include "mcc/hash.h"
#include "mcc/string.h"

__BEGIN_DECLS

// From ELF hash algorithms
unsigned int ELFHash(char *str)
{
	unsigned int hash = 0;
	unsigned int x = 0;

	while (*str)
	{
		hash = (hash << 4) + (*str++);
		if ((x=hash & 0xf0000000)!=0)
			hash ^= x >> 24;
		hash &= ~x;
	}
	return (hash & 0x7fffffff);
}

__END_DECLS