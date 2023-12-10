#ifndef LEX_H
#define LEX_H

__BEGIN_DECLS

char *next_token(char *s, char **p_end);

void parse_identifier(char *s, char **pe);
void parse_number(char *s, char **pe);

__END_DECLS

#endif