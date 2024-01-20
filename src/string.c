#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/log.h"
#include "mcc/string.h"
#include "mcc/mem.h"
#include "mcc/error.h"
#include "mcc/id.h"

__BEGIN_DECLS

Cstr cstr_alloc(const int maxlen)
{
	Cstr p;

	p = (Cstr)mcc_malloc(sizeof(Cstr));
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

int cstr_init(Cstr p, const char *src, int len)
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

void cstr_free(Cstr p)
{
	if (p && p->str) {
		mcc_free(p->str);
		p->str = NULL;
	}
	if (p)
		mcc_free(p);
}

Cstr cstr_alloc_with(const char *src, int len)
{
	Cstr p;
	
	p = cstr_alloc(len);
	if (!p) {
		error("No enough memory!\n");
		return NULL;
	}
	if (cstr_init(p, src, len) != ERR_NONE)
		return NULL;
	return p;
}

int cstr_append(Cstr p, const char *src, int len)
{
	int new_len;

	if (!p || !src || len <= 0)
		fatal("passing NULL parameter or length <= 0.\n");

	new_len = p->len + len;
	if (new_len + 1 > p->maxlen) {
		char *rs = (char *)mcc_realloc(p->str, new_len + 1);
		if (!rs) {
			error("No enough memory!\n");
			return ERR_NO_MEM;
		}
		p->str = rs;
	}

	strncpy(p->str + p->len, src, len);
	p->str[new_len] = '\0';
	p->len = new_len;
	return OK;
}

int cstr_append_ch(Cstr p, const char c)
{
	int new_len;

	if (!p)
		fatal("passing NULL parameter or length <= 0.\n");

	new_len = p->len + 1;
	if (new_len + 1 > p->maxlen) {
		char *rs = (char *)mcc_realloc(p->str, new_len + 1);
		if (!rs) {
			error("No enough memory!\n");
			return ERR_NO_MEM;
		}
		p->str = rs;
	}

	p->str[p->len] = c;
	p->str[new_len] = '\0';
	p->len = new_len;
	return OK;
}

int cstr_len(Cstr str)
{
	return str->len;
}

int cstr_maxlen(Cstr str)
{
	return str->maxlen;
}

void cstr_show(Cstr p)
{
	always("Cstr(%p): %s, len: %d, maxlen: %d\n", p, p->str ? p->str : "NULL", p->len, p->maxlen);
}

void str_dump_with_len(const char *str, int len, const char *prefix)
{
	int i;

	always("%s", prefix);
	for (i = 0; i < len; i++)
		always("%c", str[i]);
}

void str_dump_decimal_with_len(const char *str, int len, const char *prefix)
{
	int i;

	always("%s", prefix);
	for (i = 0; i < len; i++)
		always("%d ", str[i]);
	always("\n");
}

void str_replace_with(char *s, const char orig, const char dest)
{
	char cur;
	while ((cur = *s) != '\0')
	{
		if (cur == orig)
			*s = dest;
		s++;
	}
}

int str_get_until_char(const char *s, char **pstart, char delim)
{
	char *p = (char *)s, *p1 = (char *)s;
	int len;

	str_skip_blanks(&p1);
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

int str_copy_ignore_ch(char *d, const char *s, const int size, const char ignore)
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

void str_skip_blanks(char **pp)
{
	char *s = *pp;
	while (is_whitespace(*s))
		++s;
	*pp = s;
}

int str_compare_ignore_postfix_whitespace(const char *s1, const char *s2)
{
	while (*s1 && *s2) {
		if (*s1 < *s2)
			return -1;
		else if (*s1 > *s2)
			return 1;
		else
			++s1, ++s2;
	}

	if (is_whitespace(*s1) || is_whitespace(*s2))
		return 0;
	else
		return *s1 < *s2 ? -1 : 1;
}

char *mcc_strdup(char *s, int len)
{
	char *p;

	p = allocm(len + 1);
	assert(p);
	memcpy(p, s, len);
	p[len] = '\0';

	return p;
}

__END_DECLS