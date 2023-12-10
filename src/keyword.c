#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/size.h"

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

static const char *KeywordsA[] = {
    "auto", NULL,
};

static const char *KeywordsB[] = {
    "break", NULL,
};

static const char *KeywordsC[] = {
    "case", "char", "const", "continue", NULL,
};

static const char *KeywordsD[] = {
    "default", "do", "double", NULL,
};

static const char *KeywordsE[] = {
    "else", "enum", "extern", NULL,
};

static const char *KeywordsF[] = {
    "float", "for", NULL,
};

static const char *KeywordsG[] = {
    "goto", NULL,
};

static const char *KeywordsI[] = {
    "if", "int", NULL,
};

static const char *KeywordsL[] = {
    "long", NULL,
};

static const char *KeywordsR[] = {
    "register", "return", NULL,
};

static const char *KeywordsS[] = {
    "short", "signed", "sizeof", "static", "struct", "switch", NULL,
};

static const char *KeywordsT[] = {
    "typedef", NULL,
};

static const char *KeywordsU[] = {
    "union", "unsigned", NULL,
};

static const char *KeywordsV[] = {
    "void", "volatile", NULL,
};

static const char *KeywordsW[] = {
    "while", NULL,
};

static const char **KeywordsBeginArr[] = {
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
    const char **arr;
    int i;
    char c;

    assert(str != NULL);
    
    c = str[0];
    i = tolower((int)c) - 'a'; // use int cast to fix clang build error
    arr = KeywordsBeginArr[i];
    while (arr && *arr) {
        debug("str: %s, arr_begin: %s\n", str, *arr);

        if (strcmp(*arr, str) == 0) {
            debug("Found keyword: %s\n", *arr);
            return 1;
        }
        debug("Not found keyword: %s, continue\n", *arr);
        ++arr;
    }

    return 0;
}

__END_DECLS