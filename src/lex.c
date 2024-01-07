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
		debug("len: %d\n", len);
		if (len < 0) {
			perror("read file error");
			return EOF;
		} else if (len == 0)
			return EOF;
		str_dump_with_len(f->buf, len, "");
		f->buf_end = f->buf + len;
		if (len == IO_SIZE - 1)
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
			return EOF;
	}
	return EOF;
}

Token str_next_token(char **ps)
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
		r = str_parse_identifier(&s, &pt);
	else if (is_digit(c))
		r = str_parse_number(&s, &pt);
	else
		r = str_parse_other_token(&s, &pt);

	LHD;
	if (r != OK)
		return NULL;
	*ps = s;
	LHD;
	return pt;
}

Token next(File f)
{
	char *s;
	char c;
	int r;
	Token pt = NULL;

	s = f->buf;
	debug("next: %p\n", s);
	debug("token start: %c\n", s != NULL ? *s : ' ');
	LHD;
	c = skip_blanks(f);
	LHD;
	debug("c: %c(%d)\n", c, c);
	if (c != EOF)
		debug("after skip blanks: token start: %c\n", *(f->buf - 1));
	else
		return NULL;
	(void)s;
	LHD;
	
	LHD;
	if (is_id_start(c))
		r = parse_identifier(f, &pt);
	else if (is_digit(c))
		r = parse_number(f, &pt);
	else if (c == '"')
		r = parse_string(f, &pt);
	else
		r = parse_other_token(f, &pt);

	LHD;
	if (r != OK)
		return NULL;
	LHD;
	return pt;
}

int skip_blanks(File f)
{
	int c;

	do {
		c = next_char(f);
	} while (is_whitespace(c));
	return c;
}

int parse_identifier(File f, Token *ppt)
{
	Token pt = *ppt;
	char *s = f->buf - 1;
	char *b = s;
	int tok;
	int c;
	char *data;

	LHD;
	pt = token_alloc();
	assert(pt);
	LHD;
	c = next_char(f);
	while (is_id(c)) {
		c = next_char(f);
	}

	if (c != EOF)
		--f->buf;
	pt->len = f->buf - s;
	if ((tok = is_keyword_with_len(b, pt->len)))
		pt->type = tok;
	else {
		pt->type = TOK_IDENTIFIER;
	}
	data = allocm(pt->len + 1);
	assert(data);
	memcpy(data, b, pt->len);
	data[pt->len] = '\0';
	pt->val.v.p = data;
	pt->name = data;
	*ppt = pt;
	debug("Identifier/Keyword \"%s\", len: %d\n", (char *)pt->val.v.p, pt->len);
	LHD;
	return OK;
}

int parse_number(File f, Token *pt)
{
	Token t = *pt;
	char *s = f->buf - 1;
	char cb = *s;
	int base = 10;
	unsigned long long v = 0ULL;
	char c;
	char *data;

	assert(sizeof(t->val.v.i) >= sizeof(v));
	c = next_char(f);
	LHD;
	t = token_alloc();
	assert(t);
	LHD;
	if (cb == '0') {
		if (c == 'x' || c == 'X')
			base = 16;
		else if (is_octal(c))
			v = c - '0', base = 8;
		else
			base = 0;
		if (base != 0)
			c = next_char(f);
	}
	debug("Base: %d\n", base);
	if (base == 10) {
		v = cb - '0';
		while (is_digit(c)) {
			v *= 10;
			v += c - '0';
			c = next_char(f);
		}
	reparse:
		debug("parse: %c(is_unsigned: %d, is_long: %d)\n", c, t->t.is_unsigned, t->t.is_long);
		if (c == 'u' || c == 'U') {
			if (t->t.is_unsigned == 0)
				t->t.is_unsigned = 1;
			else {
				cerror("Multi %c character for integer.\n", c);
				return ERR_INVALID_FORMAT;
			}
			c = next_char(f);
			debug("is_unsigned: %d, is_long: %d\n", t->t.is_unsigned, t->t.is_long);
			goto reparse;
		} else if (c == 'l' || c == 'L') {
			if (t->t.is_long == 0)
				t->t.is_long = 1;
			else if (t->t.is_long == 1) {
				if (t->t.is_unsigned && (*(f->buf - 2) == 'u' || *(f->buf - 2) == 'U')) {
					cerror("Wrong %c character for integer.\n", c);
					return ERR_INVALID_FORMAT;
				}
				t->t.is_long = 3;
			} else {
				cerror("Wrong %c character for integer.\n", c);
				return ERR_INVALID_FORMAT;
			}
			c = next_char(f);
			debug("is_unsigned: %d, is_long: %d\n", t->t.is_unsigned, t->t.is_long);
			goto reparse;
		} else if (is_alpha(c)) {
			cerror("Invalid character %c for integer.\n", c);
			return ERR_INVALID_FORMAT;
		} else
			goto int_scan_done;
	} else if (base == 16) {
		v = cb - '0';
		while (is_xdigit(c)) {
			v *= 16;
			v += c - '0';
			c = next_char(f);
		}
		if (is_alpha(c)) {
			cerror("Invalid character %c for hex integer.\n", c);
			return ERR_INVALID_FORMAT;
		} else
			goto hex_scan_done;
	} else if (base == 8) {
		while (is_xdigit(c)) {
			v *= 8;
			v += c - '0';
			c = next_char(f);
		}
		if (is_alpha(c)) {
			cerror("Invalid character %c for hex integer.\n", c);
			return ERR_INVALID_FORMAT;
		} else
			goto hex_scan_done;
	} else if (base == 0) {
		LHD;
		if (is_alpha(c)) {
			cerror("Invalid character %c for integer.\n", c);
			return ERR_INVALID_FORMAT;
		} else
			goto int_scan_done;
	} else {
		fatal("Never should be here!\n");
		return ERR_FAIL;
	}

int_scan_done:
hex_scan_done:
	if (c != EOF)
		--f->buf;
	t->len = f->buf - s;
	lex_cal_tok_type(t);
	t->type = TOK_LITERAL;
	t->sub_type = TK_SUB_TYPE_NUMBER | TK_SUB_TYPE_CONSTANT;
	t->val.v.i = (uint64_t)v;

	data = allocm(t->len + 1);
	assert(data);
	memcpy(data, s, t->len);
	data[t->len] = '\0';
	t->name = data;

	*pt = t;
	debug("Number \"%ld\", len: %d\n", t->val.v.i, t->len);
	LHD;
	return OK;
}

int parse_string(File f, Token *ppt)
{
	Token pt = *ppt;
	char *s = f->buf - 1;
	char *b = s;
	int c;
	char *data;
	Cstr rs; // real string
	int r;

	LHD;
	pt = token_alloc();
	assert(pt);
	LHD;
	rs = cstr_alloc(1);
	assert(rs);

	c = next_char(f);
	while (c != '\"') {
		if ( c == '\\') {
			c = next_char(f);
			if (c == EOF) {
				cerror("Unterminated string.\n");
				return ERR_INVALID_FORMAT;
			}
			if (c == 'n')
				c = '\n';
			else if (c == '\\')
				c = '\\';
			else {
				cerror("Invalid escape sequence \\%c.\n", c);
				return ERR_INVALID_FORMAT;
			}
			r = cstr_append_ch(rs, c);
			assert(r == OK);
		} else {
			r = cstr_append(rs, f->buf - 1, 1);
			assert(r == OK);
		}
		c = next_char(f);
	}
	if (c != '\"') {
		cerror("Unterminated string.\n");
		return ERR_INVALID_FORMAT;
	}

	pt->len = f->buf - s;
	pt->type = TOK_LITERAL;
	pt->sub_type = TK_SUB_TYPE_STRING;

	data = allocm(pt->len + 1);
	assert(data);
	memcpy(data, b, pt->len);
	data[pt->len] = '\0';
	pt->val.v.p = rs->str;
	pt->name = data;

	*ppt = pt;
	debug("String \"%s\", len: %d\n", (char *)pt->name, pt->len);
	str_dump_decimal_with_len(rs->str, rs->len, "\tDump string: ");
	LHD;
	return OK;
}

int parse_other_token(File f, Token *pt)
{
	Token t = *pt;
	int tok;
	int c;
	char *s = f->buf - 1;
	char *data;

	LHD;
	t = token_alloc();
	assert(t);
	LHD;
	c = *s;
	debug("Token start: %c\n", c);
	switch (c) {
	case '#':
		tok = TOK_PREP;
		c = next_char(f);
		break;
	case '(':
		tok = TOK_LPAREN;
		c = next_char(f);
		break;
	case ')':
		tok = TOK_RPAREN;
		c = next_char(f);
		break;
	case '{':
		tok = TOK_LBRACE;
		c = next_char(f);
		break;
	case '}':
		tok = TOK_RBRACE;
		c = next_char(f);
		break;
	case '[':
		tok = TOK_LBRACKET;
		c = next_char(f);
		break;
	case ']':
		tok = TOK_RBRACKET;
		c = next_char(f);
		break;
	case ';':
		LHD;
		tok = TOK_SEMICOLON;
		c = next_char(f);
		debug("file buf: %c\n", *(f->buf));
		break;
	case ',':
		tok = TOK_COMMA;
		c = next_char(f);
		break;
	case '=':
		c = next_char(f);
		if (c == '=')
			tok = TOK_EQUAL, c = next_char(f);
		else
			--f->buf, tok = TOK_ASSIGN;
		break;
	case '+':
		c = next_char(f);
		if (c == '=')
			tok = TOK_ADD_ASSIGN, c = next_char(f);
		else if (c == '+')
			tok = TOK_INC, c = next_char(f);
		else
			--f->buf, tok = TOK_PLUS;
		break;
	case '-':
		tok = TOK_MINUS;
		c = next_char(f);
		break;
	case '*':
		c = next_char(f);
		tok = TOK_STAR;
		break;
	case '/':
		c = next_char(f);
		tok = TOK_DIV;
		break;
	case '%':
		c = next_char(f);
		tok = TOK_MOD;
		break;
	case '!':
		c = next_char(f);
		tok = TOK_NOT;
		break;
	case '<':
		c = next_char(f);
		tok = TOK_LESS;
		break;
	case '>':
		c = next_char(f);
		tok = TOK_GREATER;
		break;
	case '&':
		c = next_char(f);
		tok = TOK_AND;
		break;
	case '|':
		c = next_char(f);
		tok = TOK_OR;
		break;
	case '^':
		c = next_char(f);
		tok = TOK_XOR;
		break;
	case '?':
		c = next_char(f);
		tok = TOK_QUESTION;
		break;
	case ':':
		c = next_char(f);
		tok = TOK_COLON;
		break;
	case '~':
		c = next_char(f);
		tok = TOK_TILDE;
		break;
	case '.':
		c = next_char(f);
		tok = TOK_DOT;
		break;
	case '"':
		c = next_char(f);
		tok = TOK_QUOTE;
		break;
	case EOF:
		LHD;
		tok = TOK_EOF;
		*pt = NULL;
		return EOF;
	default:
		LHD;
		tok = TOK_RESERVE;
		c = next_char(f);
		break;
	}

	if (c != EOF)
		--f->buf;
	t->len = f->buf - s; // TODO
	t->type = tok;

	data = allocm(t->len + 1);
	assert(data);
	memcpy(data, s, t->len);
	data[t->len] = '\0';
	t->name = data;

	*pt = t;
	debug("Token \"%s(%d)\", len: %d\n", token_enum_to_name(tok), tok, t->len);

	return OK;
}

int str_parse_other_token(char **ps, Token *pt)
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

int str_parse_identifier(char **ps, Token *ppt)
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

int str_parse_number(char **ps, Token *pt)
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
	debug("is long: %d\n", t->t.is_long);
	assert(t->t.is_long == 0 || t->t.is_long == 1 || t->t.is_long == 3);

	if (t->t.is_unsigned == 1) {
		if (t->t.is_long == 1)
			t->t.category = ULONG_T;
		else if (t->t.is_long == 3)
			t->t.category = ULLONG_T;
		else
			t->t.category = UINT_T;
	} else {
		if (t->t.is_long == 1)
			t->t.category = LONG_T;
		else if (t->t.is_long == 3)
			t->t.category = LLONG_T;
		else
			t->t.category = INT_T;
	}
}

__END_DECLS