#include <stdio.h>

#include "mcc/id.h"
#include "mcc/lex.h"
#include "mcc/string.h"

char *next_token(char *s, char **pe)
{
    char *ps;
    char c;

    skip_blanks(&s);
    ps = s;

    c = *ps;
    if (is_id(c))
        parse_identifier(ps, pe);
    else if (is_digit(c))
        parse_number(ps , pe);

    return ps;
}

void parse_identifier(char *s, char **pe)
{
    // TODO
}

void parse_number(char *s, char **pe)
{
    // TODO
}