#ifndef TOKEN_HASH_H
#define TOKEN_HASH_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

#define TOKEN_HASH_SIZE 4096

int token_hash_insert(Token t);
int token_hash_exist(Token t);
int token_hash_exist_str(const char *s);
void token_hash_dump(int index);

__END_DECLS

#endif