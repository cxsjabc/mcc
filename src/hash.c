#include <stdio.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/hash.h"
#include "mcc/id.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/string.h"

__BEGIN_DECLS

static HashTable _StringHashTable[4096];

// From ELF hash algorithms
unsigned int ELFHash(const char *str)
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

int hash_add_str(const char *s)
{
	unsigned int hash = ELFHash(s);
	int index = hash % 4096;
	HashTable ht = _StringHashTable[index];

	LHD;
	while (ht) {
		if (strcmp(ht->s, s) == 0) {
			warn("Already exists in hash table[%d]: %s", index, ht->s);
			return 0;
		}
		ht = ht->next;
	}
	LHD;
	ht = (HashTable)mcc_malloc(sizeof(HashTable));
	if (!ht)
		return ERR_NO_MEM;
	ht->s = (char *)s;
	ht->len = strlen(s);
	ht->next = NULL;
	LHD;
	ht->next = _StringHashTable[index];
	_StringHashTable[index] = ht;
	info("Added to hash table[%d]: %s", index, s);
	return OK;
}

int hash_is_exists(const char *s)
{
	unsigned int hash = ELFHash(s);
	int index = hash % 4096;
	HashTable ht = _StringHashTable[index];

	while (ht) {
		if (strcmp(ht->s, s) == 0) {
			return TRUE;
		}
		ht = ht->next;
	}
	return FALSE;
}

void hash_dump(int index)
{
	HashTable ht;
	int dump_all = index == -1;
	int dump_start, dump_end;
	int i;

	if (dump_all) {
		dump_start = 0;
		dump_end = 4095;
	} else
		dump_start = dump_end = index;

	for (i = dump_start; i <= dump_end; i++) {
		ht =  _StringHashTable[i];
		if (!ht)
			continue;
		debug_nofl("[%d] ", i);
		while (ht) {
			debug_nofl("%s ", ht->s);
			ht = ht->next;
		}
		debug_nofl("\n");
	}
	debug_nofl("\n");
}

__END_DECLS