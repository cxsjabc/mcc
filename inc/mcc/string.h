#ifndef STRING_H
#define STRING_H

#include "mcc/mcc_base.h"

typedef struct cstr
{
	char *str;
	int len;
	int maxlen;
} *Cstr;

__BEGIN_DECLS

Cstr cstr_alloc(const int maxlen);
int cstr_init(Cstr p, const char *src, int len);
int cstr_append(Cstr p, const char *src, int len);
void cstr_free(Cstr str);

Cstr cstr_alloc_with(const char *src, int len);

int cstr_len(Cstr str);
int cstr_maxlen(Cstr str);
//int cstr_append(Cstr str, const char *src, int len);

void cstr_show(Cstr p);

void str_dump_with_len(char *str, int len, const char *prefix);

void str_replace_with(char *s, const char orig, const char dest);
int str_get_until_char(const char *s, char **pstart, char delim);

int str_copy_ignore_ch(char *d, const char *s, const int size, const char ignore);

void str_skip_blanks(char **pp);

char *mcc_strdup(char *s, int len);

__END_DECLS

#define STR_EQL(s1, s2) (strcmp((s1), (s2)) == 0)

#endif