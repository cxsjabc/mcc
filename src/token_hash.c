#include <stdio.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/hash.h"
#include "mcc/id.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/string.h"
#include "mcc/token.h"
#include "mcc/token_hash.h"

__BEGIN_DECLS

static Token TkHashTable[TOKEN_HASH_SIZE];

int token_hash_insert(Token t)
{
	char *s;
	unsigned int hash;
	int index;
	Token ht;

	if (!t)
		return ERR_INVALID_PARAM;

	s = t->name;
	if (STR_EMPTY(s))
		return ERR_EMPTY_PARAM;

	hash = ELFHash(s);
	index = hash % TOKEN_HASH_SIZE;
	ht = TkHashTable[index];
	LHD;
	while (ht) {
		if (ht->name && strcmp(ht->name, s) == 0) {
			info("Already exists in hash table[%d]: %s\n", index, ht->name);
			return ERR_EXIST;
		}
		ht = ht->next;
	}

	t->next = TkHashTable[index];
	TkHashTable[index] = t;
	info("Added to hash table[%d]%p: %s\n", index, TkHashTable[index], s);
	return OK;
}

int token_hash_exist(Token t)
{
	if (!t)
		return ERR_INVALID_PARAM;
	return token_hash_exist_str(t->name);
}

int token_hash_exist_str(const char *s)
{
	unsigned int hash;
	int index;
	Token t;

	if (STR_EMPTY(s))
		return ERR_EMPTY_PARAM;

	hash = ELFHash(s);
	index = hash % TOKEN_HASH_SIZE;
	t = TkHashTable[index];
	while (t) {
		if (t->name && strcmp(t->name, s) == 0) {
			return TRUE;
		}
		t = t->next;
	}
	return FALSE;
}

void token_hash_dump(int index)
{
	Token t;
	int dump_all = index == -1;
	int dump_start, dump_end;
	int i;

	LHD;
	if (dump_all) {
		dump_start = 0;
		dump_end = TOKEN_HASH_SIZE - 1;
	} else
		dump_start = dump_end = index;

	for (i = dump_start; i <= dump_end; i++) {
		t =  TkHashTable[i];
		if (!t)
			continue;
		debug_nofl("[%d] ", i);
		while (t) {
			debug_nofl("%s ", t->name);
			t = t->next;
		}
		debug_nofl("\n");
	}
	debug_nofl("\n");
	LHD;
}

__END_DECLS