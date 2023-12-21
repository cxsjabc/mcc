#ifndef HASH_H
#define HASH_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

typedef struct hash_table
{
	char *s;
	int len;

	struct hash_table *next;
} *HashTable;

// From ELF hash algorithms
unsigned int ELFHash(const char *str);

int hash_add_str(const char *s);
int hash_is_exists(const char *s);
void hash_dump(int index);

__END_DECLS

#endif