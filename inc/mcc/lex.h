#ifndef LEX_H
#define LEX_H

char *next_token(char *s, char **p_end);

void parse_identifier(char *s, char **pe);
void parse_number(char *s, char **pe);

#endif