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
#include "mcc/mem_buf.h"
#include "mcc/prepare.h"
#include "mcc/section.h"
#include "mcc/symbol.h"

__BEGIN_DECLS

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

int coff_gen_obj(Coff coff, const char *name)
{
	int sec_cnt = coff->sec_cnt;
	FILE *f;
	int r = OK;
	unsigned int file_hdr_size, data_offset;
	void *file_hdr;

	f = fopen(name, "wb");
	NULL_RETURN(f, ERR_FAIL, "open file %s failed\n", name);

	data_offset = coff_get_data_offset(coff);
	fseek(f, data_offset, SEEK_SET);
	// write section data
	for (int i = 0; i < sec_cnt; i++) {
		Section s = SECTION_AT(i);
		unsigned int written_size;

		written_size = coff_write_section_data(coff, s, f, data_offset);
		data_offset += written_size;
	}

	// prepare file header
	file_hdr = coff_alloc_file_header(coff);
	assert(file_hdr != NULL);
	file_hdr_size = coff_get_file_header_size(coff);
	(void)file_hdr_size;

	// write file header
	fseek(f, 0, SEEK_SET);
	coff_write_file_header(coff);

	return r;
}

unsigned int coff_write_section_data(Coff coff, Section s, FILE *f, unsigned int data_offset)
{
	size_t r;

	(void)coff;
	if (!s || s->data == NULL || s->offset == 0)
		return 0;

	r = fwrite(s->data, s->offset, 1, f);
	assert(r == 1);

	section_update_header_pointer(s, data_offset);

	return s->offset;
}

__END_DECLS