#include "mcc/string.h"

void replace_string_with(char *s, const char orig, const char dest)
{
    char cur;
    while ((cur = *s) != '\0')
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

int copy_ignore_char(char *d, const char *s, const int size, const char ignore)
{
    int i = 0;
    char c;

    while (i < size && (c = *s)) {
        if (c != ignore) {
            *d = c;
            ++s;
            ++d;
            ++i;
        } else {
            ++s;
        }

    }
    return i;
}