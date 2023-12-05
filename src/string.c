#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/log.h"
#include "mcc/string.h"
#include "mcc/mem.h"
#include "mcc/error.h"
#include "mcc/id.h"

PCstr alloc_cstr(const int maxlen)
{
    PCstr p;

    p = (PCstr)mcc_malloc(sizeof(Cstr));
    if (!p) {
        error("No enough memory!\n");
        return NULL;
    }

    p->str = (char *)mcc_malloc(maxlen);
    if (!p->str) {
        error("No enough memory!\n");
        mcc_free(p);
        return NULL;
    }

    p->maxlen = maxlen;
    p->len = 0;
    p->str[0] = '\0';
    return p;
}

int init_cstr(PCstr p, const char *src, int len)
{
    if (!p || !src || len <= 0)
        fatal("passing NULL parameter or length <= 0.\n");

    if (!p->str) {
        p->str = (char *)mcc_malloc(len);
        if (!p->str) {
            error("No enough memory!\n");
            return ERR_NO_MEM;
        }
        p->maxlen = len;
    }

    strncpy(p->str, src, len);
    p->str[len] = '\0';
    p->len = len - 1;

    return ERR_NONE;
}

void free_cstr(PCstr p)
{
    if (p->str) {
        mcc_free(p->str);
        p->str = NULL;
    }
    if (p)
        mcc_free(p);
}

PCstr alloc_with_cstr(const char *src, int len)
{
    PCstr p;
    
    p = alloc_cstr(len);
    if (!p) {
        error("No enough memory!\n");
        return NULL;
    }
    if (init_cstr(p, src, len) != ERR_NONE)
        return NULL;
    return p;
}

int cstr_len(PCstr str)
{
    return str->len;
}

int cstr_maxlen(PCstr str)
{
    return str->maxlen;
}

void cstr_show(PCstr p)
{
    always("Cstr(%p): %s, len: %d, maxlen: %d\n", p, p->str ? p->str : "NULL", p->len, p->maxlen);
}

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
    char *p = (char *)s, *p1 = (char *)s;
    int len;

    skip_blanks(&p1);
    p = p1;
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

void skip_blanks(char **pp)
{
    char *s = *pp;
    while (is_whitespace(*s))
        ++s;
    *pp = s;
}