#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/error.h"
#include "mcc/keyword.h"
#include "mcc/log.h"
#include "mcc/size.h"
#include "mcc/string.h"
#include "mcc/token.h"

__BEGIN_DECLS

static const char *KeywordsAllArr[] = {
	"auto",
	"break",
	"case", "char", "const", "continue",
	"default", "do", "double",
	"else", "enum", "extern",
	"float", "for",
	"goto",
	"if", "int",
	"long",
	"register", "return",
	"short", "signed", "sizeof", "static", "struct", "switch",
	"typedef",
	"union", "unsigned",
	"void", "volatile",
	"while",
};

static struct keyword KeywordsA[] = {
	{ "auto", 4, TOK_AUTO },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsB[] = {
	{ "break", 5, TOK_BREAK },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsC[] = {
	{ "case", 4, TOK_CASE },
	{ "char", 4, TOK_CHAR},
	{ "const", 5, TOK_CONST },
	{ "continue", 8, TOK_CONTINUE },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsD[] = {
	{ "default", 7, TOK_DEFAULT },
	{ "do", 2, TOK_DO },
	{ "double", 6, TOK_DOUBLE },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsE[] = {
	{ "else", 4, TOK_ELSE },
	{ "enum", 4, TOK_ENUM },
	{ "extern", 6, TOK_EXTERN },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsF[] = {
	{ "float", 5, TOK_FLOAT },
	{ "for", 3, TOK_FOR },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsG[] = {
	{ "goto", 4, TOK_GOTO },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsI[] = {
	{ "if", 2, TOK_IF },
	{ "int", 3, TOK_INT },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsL[] = {
	{ "long", 4, TOK_LONG },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsR[] = {
	{ "register", 8, TOK_REGISTER },
	{ "return", 6, TOK_RETURN },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsS[] = {
	{ "short", 5, TOK_SHORT },
	{ "signed", 6, TOK_SIGNED },
	{ "sizeof", 6, TOK_SIZEOF },
	{ "static", 6, TOK_STATIC },
	{ "struct", 6, TOK_STRUCT },
	{ "switch", 6, TOK_SWITCH },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsT[] = {
	{ "typedef", 7, TOK_TYPEDEF },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsU[] = {
	{ "union", 5, TOK_UNION },
	{ "unsigned", 8, TOK_UNSIGNED },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsV[] = {
	{ "void", 4, TOK_VOID },
	{ "volatile", 8, TOK_VOLATILE },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword KeywordsW[] = {
	{ "while", 5, TOK_WHILE },
	{ NULL, 0, TOK_ERROR },
};

static struct keyword *KeywordsBeginArr[] = {
	KeywordsA, KeywordsB, KeywordsC, KeywordsD, KeywordsE,
	KeywordsF, KeywordsG, NULL, KeywordsI, NULL, 
	NULL, KeywordsL, NULL, NULL, NULL, 
	NULL, NULL, KeywordsR, KeywordsS, KeywordsT,
	KeywordsU, KeywordsV, KeywordsW, NULL, NULL,
	NULL,
};

int is_keyword_v1(const char *str) 
{
	assert(str != NULL);
	unsigned int i;
	for (i = 0; i < ARRAY_SIZE(KeywordsAllArr); i++) {
		if (strcmp(KeywordsAllArr[i], str) == 0) {
			return 1;
		}
	}
	return 0;
}

int is_keyword(const char *str) 
{
	struct keyword *key;
	int i;
	char c;

	assert(str != NULL);
	
	c = str[0];
	i = tolower((int)c) - 'a'; // use int cast to fix clang build error
	key = KeywordsBeginArr[i];
	if (!key)
		return 0;

	while (key && key->name) {
		if (strcmp(key->name, str) == 0) {
			debug("Found keyword: %s\n", key->name);
			return key->token;
		}
		silence("str: %s\n", key->name);
		++key;
	}

	return 0;
}

int is_keyword_with_len(const char *str, int len) 
{
	struct keyword *key;
	int i;
	char c;

	assert(str != NULL);
	
	c = str[0];
	i = tolower((int)c) - 'a'; // use int cast to fix clang build error
	key = KeywordsBeginArr[i];
	if (!key)
		return 0;

	while (key && key->name) {
		if (key->len == len && strncmp(key->name, str, len) == 0) {
			silence("Found keyword: %s\n", key->name);
			return key->token;
		}
		silence("str: %s\n", key->name);
		++key;
	}

	return 0;
}

__END_DECLS