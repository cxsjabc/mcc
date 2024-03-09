#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/env.h"
#include "mcc/error.h"
#include "mcc/exe.h"
#include "mcc/id.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/mem.h"
#include "mcc/mem_buf.h"
#include "mcc/prepare.h"
#include "mcc/section.h"
#include "mcc/symbol.h"

__BEGIN_DECLS

static int _exe_write_section_data(Exe exe, Section s, unsigned int data_offset);
int _exe_write_file_header(Exe exe);

Exe exe_alloc()
{
	int r;
	Exe exe;
	DynArray arr;

	exe = (Exe)mcc_mallocz(sizeof(struct exe));
	NULL_RETURN(exe, NULL, "failed to allocate Exe memory");

	EXE_SEC_CNT(exe) = EXE_SECTION_COUNT;
	arr = dynamic_array_create(8);
	COND_GOTO(!arr, fail_alloc_array, "failed to allocate dynamic array memory");
	exe->secs = arr;

	r = dynamic_array_push_arr(arr, (void **)Secs, EXE_SEC_CNT(exe));
	COND_GOTO(r != OK, fail_push_array, "failed to push exe sections array");

	return exe;
fail_push_array:
	dynamic_array_destroy(exe->secs);
fail_alloc_array:
	mcc_free(exe);
	return NULL;
}

void exe_free(Exe c)
{
	if (c->f) {
		fclose(c->f);
		c->f = NULL;
	}
	mcc_free(c);
}

int exe_gen(const char *name)
{
	Exe exe;
	FILE *f;
	int r = OK;

	exe = exe_alloc();
	NULL_RETURN(exe, ERR_FAIL, "exe_alloc failed\n");

	f = fopen(name, "wb");
	COND_GOTO(!f, fail_fopen, "open file %s failed\n", name);
	exe->f = f;

	exe->file_align = exe_get_file_align(exe);

        // write section data
	r = exe_write_section_data(exe);
	COND_GOTO(r != OK, fail_write_section_data, "failed to write file header\n");

	// write file header
	r = exe_write_file_header(exe);
	COND_GOTO(r != OK, fail_write_header, "failed to write file header\n");

	r = exe_write_section_header(exe);
	COND_GOTO(r != OK, fail_write_section_header, "failed to write section header\n");
	goto success;

fail_write_section_header:
fail_write_header:
fail_write_section_data:
	fclose(exe->f);
	exe->f = NULL;
fail_fopen:
success:
	exe_free(exe);
	return r;
}

int exe_write_file_header(Exe exe)
{
	return _exe_write_file_header(exe);
}

int exe_write_section_header(Exe exe)
{
	FILE *f;
	int r;

	f = exe->f;

	fseek(f, EXE_FILE_HDR_SIZE(exe), SEEK_SET);
	for (int i = 0; i < exe->sec_cnt; i++) {
		Section s = SECTION_AT(i);
		r = fwrite(&s->hdr, EXE_SEC_HDR_SIZE(exe), 1, f);
		if (r != 1)
			return ERR_FAIL;
	}

	return OK;
}

int exe_write_section_data(Exe exe)
{
	unsigned int data_offset;

	data_offset = EXE_DATA_OFFSET(exe);
	fseek(exe->f, data_offset, SEEK_SET);
	// write section data
	for (int i = 0; i < exe->sec_cnt; i++) {
		Section s = SECTION_AT(i);
		int written_size;

		written_size = _exe_write_section_data(exe, s, data_offset);
		if (written_size < 0)
			return ERR_FAIL;

		data_offset += written_size;
	}

	return OK;
}

int _exe_write_section_data(Exe exe, Section s, unsigned int data_offset)
{
	size_t r;
	FILE *f;

	f = exe->f;
	if (!s || s->data == NULL || s->offset == 0)
		return 0;

	r = fwrite(s->data, s->offset, 1, f);
	if (r != 1)
		return ERR_FAIL;

	return (int)s->offset;
}

__END_DECLS