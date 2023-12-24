#ifndef ID_H
#define ID_H

#include "mcc/mcc_base.h"

#define CHAR_CNT 128

typedef enum {
	CHAR_TYPE_DIGIT = 0x1,
	CHAR_TYPE_ALPHA = 0x2,
	CHAR_TYPE_ALDIG = 0x4,
	CHAR_TYPE_BLANK = 0x8,
	CHAR_TYPE_OTHER = 0x80,
} CHAR_TYPE;

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define is_alpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define is_alnum(c) (is_digit(c) || is_alpha(c))
#define is_xdigit(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))

#define is_octal(c) ((c) >= '0' && (c) <= '7')
#define is_hex(c) (is_xdigit(c))

#define is_newline(c) ((c) == '\n' || (c) == '\r')
#define is_whitespace(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r' || (c) == '\f')

#if CONFIG_USE_CHAR_MAP_TABLE
#define is_id_start(c) (get_char_type(c) & CHAR_TYPE_ALPHA)
#else
#define is_id_start(c) (is_alpha(c) || (c) == '_')
#endif

#if CONFIG_USE_CHAR_MAP_TABLE
#define is_id(c) (get_char_type(c) & CHAR_TYPE_ALDIG)
#else
#define is_id(c) (is_alnum(c) || (c) == '_')
#endif

__BEGIN_DECLS

int is_identifier(const char *str);
__need_init void init_char_type_table();

CHAR_TYPE get_char_type(char c);
const char *get_char_type_str(CHAR_TYPE type);

__END_DECLS

#endif