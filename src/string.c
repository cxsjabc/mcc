#include "mcc/string.h"

void replace_string_with(char *s, const char orig, const char dest)
{
    char cur;
    while (cur = *s)
    {
        if (cur == orig)
            *s = dest;
        s++;
    }
}

int get_string_until_char(const char *s, char **pstart, char delim)
{
    char *p = (char *)s;
    int len;

    if (*p != delim)
        *pstart = p, len = 1, ++p;
    else
        return -1;
    while (*p != delim && *p != '\0')
    {
        ++len;
        ++p;
    }
    return len;
}