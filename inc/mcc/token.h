#ifndef TOKEN_H
#define TOKEN_H

enum Token 
{
    #define DEF_TOK(t, s) t,
    #include "token_internal.h"
    #undef DEF_TOK
};

#endif