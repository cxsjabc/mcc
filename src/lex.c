#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "mcc/compile_log.h"
#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/id.h"
#include "mcc/keyword.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/preprocess.h"
#include "mcc/string.h"

__BEGIN_DECLS

int process_eob(File f)
{
	int len;

	if (f->buf == f->buf_end) {
		if (f->buf == NULL) {
			f->buf = allocm(IO_SIZE);
			assert(f->buf);
		}
		len = read(f->fd, f->buf, IO_SIZE - 1);
		silence("len: %d\n", len);
		if (len < 0) {
			perror("read file error");
			return EOF;
		} else if (len == 0)
			return EOF;
		// str_dump_with_len(f->buf, len, "");
		f->buf_end = f->buf + len;
		f->buf_end[0] = EOB;
		return *f->buf++;
	}
	return EOF;
}

int next_char(File f)
{
	if (f->buf != NULL && f->buf < f->buf_end) {
		return *f->buf++;
	}
	else {
		if (f->buf == NULL || *f->buf == EOB)
			return process_eob(f);
		else
			fatal("Why so?\n");
	}
	return EOF;
}

Token next_token(char **ps)
{
	char *s;
	char c;
	int r;
	Token pt = NULL;

	s = *ps;
	debug("token start: %s\n", *ps);
	str_skip_blanks(&s);
	debug("after skip blanks: token start: %s\n", s);
	
	LHD;
	c = *s;
	if (is_id_start(c))
		r = parse_identifier(&s, &pt);
	else if (is_digit(c))
		r = parse_number(&s, &pt);
	else
		r = parse_other_token(&s, &pt);

	LHD;
	if (r != OK)
		return NULL;
	*ps = s;
	LHD;
	return pt;
}

Token next(File f)
{
	return next_token(&f->buf);
}

int skip_blanks(File f)
{
	int c;

	do {
		c = next_char(f);
	} while (is_whitespace(c));
	return c;
}

int parse_other_token(char **ps, Token *pt)
{
	Token t = *pt;
	char *s = *ps;
	char *b = s;
	int tok;

	LHD;
	t = token_alloc();
	assert(t);
	LHD;
	switch (*s) {
	case '#':
		tok = TOK_PREP;
		++s;
		pp_process_str(&s);
		break;
	case '(':
		tok = TOK_LPAREN;
		break;
	case ')':
		tok = TOK_RPAREN;
		break;
	case '{':
		tok = TOK_LBRACE;
		break;
	case '}':
		tok = TOK_RBRACE;
		break;
	case '[':
		tok = TOK_LBRACKET;
		break;
	case ']':
		tok = TOK_RBRACKET;
		break;
	case ';':
		tok = TOK_SEMICOLON;
		break;
	case ',':
		tok = TOK_COMMA;
		break;
	case '=':
		tok = TOK_ASSIGN;
		break;
	case '+':
		tok = TOK_PLUS;
		break;
	case '-':
		tok = TOK_MINUS;
		break;
	case '*':
		tok = TOK_STAR;
		break;
	case '/':
		tok = TOK_DIV;
		break;
	case '%':
		tok = TOK_MOD;
		break;
	case '!':
		tok = TOK_NOT;
		break;
	case '<':
		tok = TOK_LESS;
		break;
	case '>':
		tok = TOK_GREATER;
		break;
	case '&':
		tok = TOK_AND;
		break;
	case '|':
		tok = TOK_OR;
		break;
	case '^':
		tok = TOK_XOR;
		break;
	case '?':
		tok = TOK_QUESTION;
		break;
	case ':':
		tok = TOK_COLON;
		break;
	case '~':
		tok = TOK_TILDE;
		break;
	case '.':
		tok = TOK_DOT;
		break;
	case '"':
		tok = TOK_QUOTE;
		break;
	default:
		tok = TOK_RESERVE;
		break;
	}

	t->len = 1; // TODO
	t->type = tok;
	debug("Parse \"%s\", len: %d\n", b, t->len);
	*ps = s;

	return OK;
}

int parse_identifier(char **ps, Token *ppt)
{
	Token pt = *ppt;
	char *s = *ps;
	char *b = s;
	int tok;

	++s;
	LHD;
	pt = token_alloc();
	assert(pt);
	LHD;
	while (is_id(*s)) {
		++s;
	}

	pt->len = s - b;
	debug("Parse \"%s\", len: %d\n", b, pt->len);
	if ((tok = is_keyword_with_len(b, pt->len)))
		pt->type = tok;
	else {
		pt->type = TOK_IDENTIFIER;
		pt->sub_type = TK_SUB_TYPE_VARIABLE;
	}
	pt->val.v.p = b;
	*ppt = pt;
	*ps = s;
	LHD;
	return OK;
}

int parse_number(char **ps, Token *pt)
{
	Token t = *pt;
	char *s = *ps;
	char *b = s;
	int base = 10;
	unsigned long long v = 0ULL;

	assert(sizeof(t->val.v.i) >= sizeof(v));
	++s;
	LHD;
	t = token_alloc();
	assert(t);
	LHD;
	if (*b == '0') {
		if (*s == 'x' || *s == 'X')
			base = 16;
		else if (is_octal(*s))
			v = *s - '0', base = 8;
		else
			base = 0;
		if (base != 0)
			++s;
	}
	debug("Base: %d\n", base);
	if (base == 10) {
		v = *b - '0';
		while (is_digit(*s)) {
			v *= 10;
			v += *s - '0';
			++s;
		}
	reparse:
		if (*s == 'u' || *s == 'U') {
			if (!t->t.is_unsigned)
				t->t.is_unsigned = 1;
			else {
				cerror("Multi %c character for integer.\n", *s);
				return ERR_INVALID_FORMAT;
			}
			++s;
			goto reparse;
		} else if (*s == 'l' || *s == 'L') {
			if (t->t.is_long == 0)
				t->t.is_long = 1;
			else if (t->t.is_long == 1) {
				if (t->t.is_unsigned && (*(s - 1) == 'u' || *(s - 1) == 'U')) {
					cerror("Wrong %c character for integer.\n", *s);
					return ERR_INVALID_FORMAT;
				}
				t->t.is_long = 3;
			} else {
				cerror("Wrong %c character for integer.\n", *s);
				return ERR_INVALID_FORMAT;
			}
			++s;
			goto reparse;
		} else if (is_alpha(*s)) {
			cerror("Invalid character %c for integer.\n", *s);
			return ERR_INVALID_FORMAT;
		} else
			goto int_scan_done;
	} else if (base == 16) {
		v = *b - '0';
		while (is_xdigit(*s)) {
			v *= 16;
			v += *s - '0';
			++s;
		}
		if (is_alpha(*s)) {
			cerror("Invalid character %c for hex integer.\n", *s);
			return ERR_INVALID_FORMAT;
		} else
			goto hex_scan_done;
	} else if (base == 8) {
		while (is_xdigit(*s)) {
			v *= 8;
			v += *s - '0';
			++s;
		}
		if (is_alpha(*s)) {
			cerror("Invalid character %c for hex integer.\n", *s);
			return ERR_INVALID_FORMAT;
		} else
			goto hex_scan_done;
	} else if (base == 0) {
		LHD;
		if (is_alpha(*s)) {
			cerror("Invalid character %c for integer.\n", *s);
			return ERR_INVALID_FORMAT;
		} else
			goto int_scan_done;
	} else {
		fatal("Never should be here!\n");
		return ERR_FAIL;
	}

int_scan_done:
hex_scan_done:
	t->len = s - b;
	debug("Parse \"%s\", len: %d\n", b, t->len);
	lex_cal_tok_type(t);
	t->type = TOK_LITERAL;
	t->sub_type = TK_SUB_TYPE_NUMBER | TK_SUB_TYPE_CONSTANT;
	t->val.v.i = (uint64_t)v;
	*pt = t;
	*ps = s;
	LHD;
	return OK;
}

void lex_cal_tok_type(Token t)
{
	assert(t->t.is_long == 0 || t->t.is_long == 1 || t->t.is_long == 3);

	if (t->t.is_unsigned == 1) {
		if (t->t.is_long == 1)
			t->t.category = ULONG;
		else if (t->t.is_long == 3)
			t->t.category = ULLONG;
	} else {
		if (t->t.is_long == 1)
			t->t.category = LONG;
		else if (t->t.is_long == 3)
			t->t.category = LLONG;
	}
}

__END_DECLS