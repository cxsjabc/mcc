#ifndef STRING_H
#define STRING_H

typedef struct cstr
{
    char *str;
    int len;
    int maxlen;
} Cstr, *PCstr;

PCstr alloc_cstr(const int maxlen);
int init_cstr(PCstr p, const char *src, int len);
void free_cstr(PCstr str);

PCstr alloc_with_cstr(const char *src, int len);

int cstr_len(PCstr str);
int cstr_maxlen(PCstr str);
//int cstr_append(PCstr str, const char *src, int len);

void cstr_show(PCstr p);

void replace_string_with(char *s, const char orig, const char dest);
int get_string_until_char(const char *s, char **pstart, char delim);

int copy_ignore_char(char *d, const char *s, const int size, const char ignore);

void skip_blanks(char **pp);

#define STR_EQL(s1, s2) (strcmp((s1), (s2)) == 0)

#endif