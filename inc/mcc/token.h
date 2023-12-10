#ifndef TOKEN_H
#define TOKEN_H

#include "mcc/features.h"

enum Token 
{
	#define DEF_TOK(t, s) t,
	#include "mcc/token_internal.h"
	#undef DEF_TOK
};

__BEGIN_DECLS

void show_all_tokens();

__END_DECLS

#endif