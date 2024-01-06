#define NO_DEBUG 0

#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mcc_arch.h"
#include "mcc/string.h"
#include "mcc/token.h"

#include "test.h"

__BEGIN_DECLS

void test_parse_preprocess()
{
	Token t;
	char *s = "#include <stdio.h>\n";
	char *p = s;

	TEST_BEGIN;

	do {
		t = str_next_token(&p);
		if (!t)
			break;
		
		token_dump(t);
	} while (1);
	TEST_END;
}

void test_parse_tokens()
{
	Token t;
	char *s = "int i 0;";
	char *p = s;

	TEST_BEGIN;

	do {
		t = str_next_token(&p);
		if (!t)
			break;
		
		token_dump(t);

		//p += t->len;
	} while (1);
	TEST_END;
}

void test_read_chars()
{
#if 0
	do {
		c = next_char(f);
		debug_nofl("%c", c);
	} while (c != EOF);
#endif
}

DynArray parse_tokens_from_file(const char *source)
{
	File f;
	Token t;
	DynArray a;

	f = file_open(source);
	assert(f);
	a = dynamic_array_create(1);

	do {
		t = next(f);
		if (t != NULL) {
			dynamic_array_push(a, t);
			token_dump(t);
		}
		// sleep(1);
	} while (t != NULL);

	file_close(f);
	return a;
}

int verify_token(DynArray a, const char *expect_file)
{
	FILE *f = fopen(expect_file, "r");
	Token t;
	int i = 0;
	int ret = 1;
	char buf[8192 + 128];
	int size;

	if (!f) {
		error("failed to open file: %s\n", expect_file);
		return -1;
	}

	size = dynamic_array_size(a);
	while (i < size) {
		t = dynamic_array_get(a, i);
		
		if (fgets(buf, sizeof(buf), f) == NULL) {
			error("failed to read file: %s", expect_file);
			ret = -2;
			break;
		}
		
		always("Read line: |%s|\n", buf);
		token_get_name(t);
		always("Token name: |%s|\n", t->name);
		if (t->type >= TOK_AUTO && t->type <= TOK_TILDE) {
			char name[128];
			sscanf(buf, "%s", name);
			if (!STR_EQL(name, t->name))
				return -3;
		} else if (t->type == TOK_IDENTIFIER) {
			char type[128], id_name[4096];
			int len;
			sscanf(buf, "%s %s %d", type, id_name, &len);
			always("Type: %s, id_name: %s, len: %d\n", type, id_name, len);
			always("Token id: %s, len: %d\n", t->name, t->len);
			if (!STR_EQL(t->name, id_name) || t->len != len)
				return -4;
		} else if (t->type == TOK_LITERAL) {
			char buf_type[128], buf_sub_type[128], buf_literal_name[4096], buf_literal_type_name[128];
			int buf_len;
			Cstr cstr;
			const char *type_name, *detail_type_name;
		
			sscanf(buf, "%s %s %s %d %s", buf_type, buf_sub_type, buf_literal_name, &buf_len, buf_literal_type_name);
			always("buf_type: %s, sub_type: %s, literal_name: %s, buf_len: %d, buf_literal_type_name: %s\n",
			       buf_type, buf_sub_type, buf_literal_name, buf_len, buf_literal_type_name);

			type_name = token_enum_to_name(t->type);
			cstr = token_sub_type_enum_to_name(t->sub_type);
			detail_type_name = token_get_type_name(t);
			always("Token type: %s, sub_type: %s, name: %s, len: %d, detail_type_name: %s\n",
			       type_name, cstr->str, t->name, t->len, detail_type_name);
			if (!STR_EQL(type_name, buf_type) || !STR_EQL(cstr->str, buf_sub_type) || !STR_EQL(t->name, buf_literal_name) 
			    || !STR_EQL(detail_type_name, buf_literal_type_name) || t->len != buf_len)
				return -5;
		} else {
			fatal("unknown token type\n");
		}
		++i;
		
	}
	fclose(f);
	return ret;
}

void test_chars_from_file(const char *file)
{
	const char *s = file; // "./test/data/lex_int_const"
	Cstr expect_file;
	DynArray a;
	int r;

	TEST_BEGIN;
	expect_file = cstr_alloc(128);
	assert(expect_file);
	r = cstr_append(expect_file, s, strlen(s));
	assert(r == OK);
	r = cstr_append(expect_file, ".expect", strlen(".expect"));
	assert(r == OK);

	debug("File full path: %s, expect file: %s\n", file, expect_file->str);
	a = parse_tokens_from_file(s);
	r = verify_token(a, expect_file->str);
	debug("ret: %d\n", r);
	assert(r > 0);
	dynamic_array_destroy(a);
	TEST_END;
}

void test_lex_by_folder(const char *folder, const char *file_prefix)
{
	DIR *dir = opendir(folder);
	struct dirent *entry;
	Cstr path;
	int r;

	if (!dir)
		assert("Empty directory\n");

	while ((entry = readdir(dir)) != NULL) {
		if (STR_EQL(entry->d_name, ".") || STR_EQL(entry->d_name, ".."))
			continue;
		if (strcmp(entry->d_name + strlen(entry->d_name) - strlen(".expect"), ".expect") == 0)
			continue;
		if (file_prefix != NULL) {
			if(strstr(entry->d_name, file_prefix) != entry->d_name) {
				continue;
			}
		}
		debug("File: %s\n", entry->d_name);

		path = cstr_alloc(128);
		assert(path);
		r = cstr_append(path, folder, strlen(folder));
		assert(r == OK);
		r = cstr_append(path, "/", strlen("/"));
		assert(r == OK);
		r = cstr_append(path, entry->d_name, strlen(entry->d_name));
		assert(r == OK);

		test_chars_from_file(path->str);
	}
}

void test_lex()
{
	TEST_BEGIN;
	// test_parse_preprocess();
	// test_parse_tokens();
	// test_chars_from_file("./test/data/lex/lex_int_const1");
	test_lex_by_folder("./test/data/lex", NULL);
	TEST_END;
	return;
}

__END_DECLS