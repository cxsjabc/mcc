#ifndef LEX_H
#define LEX_H

#include "mcc/file.h"
#include "mcc/mcc_base.h"
#include "mcc/token.h"

__BEGIN_DECLS

extern DynArray TokenArray;

Token str_next_token(char **s);

int str_parse_identifier(char **ps, Token *pt);
int str_parse_number(char **ps, Token *pt);
int str_parse_other_token(char **ps, Token *pt);

void lex_cal_tok_type(Token t);

int lex_init();

int process_eob(File f);
int next_char(File f);
Token next(File f);

int parse_identifier(File f, Token *pt);
int parse_number(File f, Token *pt);
int parse_string(File f, Token *ppt);
int parse_other_token(File f, Token *pt);

void lex_dump_token_array();

int skip_blanks(File f);

__END_DECLS

#endif