#ifndef LEX_H
#define LEX_H

#include "mcc/mcc_base.h"
#include "mcc/token.h"

__BEGIN_DECLS

Token next_token(char **s);

int parse_identifier(char **s, Token *pt);
int parse_number(char **s, Token *pt);

void lex_cal_tok_type(Token t);

__END_DECLS

#endif