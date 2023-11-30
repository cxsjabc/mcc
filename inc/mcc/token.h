#ifndef TOKEN_H
#define TOKEN_H

enum Token 
{
    #define DEF_TOK(t, s) t,
    #include "mcc/token_internal.h"
    #undef DEF_TOK
};

void show_all_tokens();

#endif