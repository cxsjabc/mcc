#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/coff.h"
#include "mcc/env.h"
#include "mcc/error.h"
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

static int _coff_write_section_data(Coff coff, Section s, unsigned int data_offset);

Coff coff_alloc()
{
	void *hdr;
	Coff coff = (Coff)mcc_mallocz(sizeof(struct coff));
	NULL_RETURN(coff, NULL, "failed to allocate Coff memory");

	hdr = coff_alloc_file_header(coff);
	if (!hdr) {
		error("failed to allocate file header\n");
		mcc_free(coff);
		return NULL;
	}
	return coff;
}

void coff_free(Coff c)
{
	freem(c->file_hdr);
	if (c->f)
		fclose(c->f);
	mcc_free(c);
}

inline void coff_set_machine(Coff coff, int machine)
{
	coff->machine = machine;
}

inline int coff_get_machine(Coff coff)
{
	return coff->machine;
}

inline void coff_set_sec_cnt(Coff coff, int sec_cnt)
{
	coff->sec_cnt = sec_cnt;
}

inline int coff_get_sec_cnt(Coff coff)
{
	return coff->sec_cnt;
}

inline void coff_set_sec_sym(Coff coff, Section sec_sym)
{
	coff->sec_sym = sec_sym;
}

inline Section coff_get_sec_sym(Coff coff)
{
	return coff->sec_sym;
}

int coff_gen_obj(const char *name)
{
	Coff coff;
	FILE *f;
	int r = OK;

	coff = coff_alloc();
	NULL_RETURN(coff, ERR_FAIL, "coff_alloc failed\n");
	coff_set_sec_cnt(coff, OBJ_SECTION_COUNT);

	f = fopen(name, "wb");
	COND_GOTO(!f, fail_fopen, "open file %s failed\n", name);
	coff->f = f;

        // write section data
	r = coff_write_section_data(coff);
	COND_GOTO(r != OK, fail_write_section_data, "failed to write file header\n");

	// write file header
	r = coff_write_file_header(coff);
	COND_GOTO(r != OK, fail_write_header, "failed to write file header\n");

	r = coff_write_section_header(coff);
	COND_GOTO(r != OK, fail_write_section_header, "failed to write section header\n");

fail_write_section_header:
fail_write_header:
fail_write_section_data:
	fclose(coff->f);
	coff->f = NULL;
fail_fopen:
	coff_free(coff);
	return r;
}

int coff_write_section_header(Coff coff)
{
	FILE *f;
	int r;

	f = coff->f;

	fseek(f, coff_get_file_header_size(coff), SEEK_SET);
	for (int i = 0; i < coff->sec_cnt; i++) {
		Section s = SECTION_AT(i);
		r = fwrite(&s->hdr, coff_get_section_header_size(coff), 1, f);
		if (r != 1)
			return ERR_FAIL;
	}

	return OK;
}

int coff_write_section_data(Coff coff)
{
	unsigned int data_offset;

	data_offset = coff_get_data_offset(coff);
	fseek(coff->f, data_offset, SEEK_SET);
	// write section data
	for (int i = 0; i < coff->sec_cnt; i++) {
		Section s = SECTION_AT(i);
		int written_size;

		written_size = _coff_write_section_data(coff, s, data_offset);
		if (written_size < 0)
			return ERR_FAIL;

		data_offset += written_size;
	}

	return OK;
}

int _coff_write_section_data(Coff coff, Section s, unsigned int data_offset)
{
	size_t r;
	FILE *f;

	f = coff->f;
	if (!s || s->data == NULL || s->offset == 0)
		return 0;

	r = fwrite(s->data, s->offset, 1, f);
	if (r != 1)
		return ERR_FAIL;

	section_update_header_pointer(s, data_offset);

	return (int)s->offset;
}

__END_DECLS