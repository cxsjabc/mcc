#ifndef LEX_H
#define LEX_H

#include "mcc/mcc_base.h"
#include "mcc/token.h"

__BEGIN_DECLS

Token next_token(char **s);

int parse_identifier(char **ps, Token *pt);
int parse_number(char **ps, Token *pt);
int parse_other_token(char **ps, Token *pt);

void lex_cal_tok_type(Token t);

int process_eob(File f);
int next_char(File f);

__END_DECLS

#endif