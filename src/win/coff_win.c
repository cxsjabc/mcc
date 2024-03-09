#include <stdio.h>
#include <windows.h>

#include "mcc/mcc_base.h"

#include "mcc/coff.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/mem.h"

__BEGIN_DECLS

void *coff_alloc_file_header(Coff coff)
{
	void *hdr;

	hdr = allocmz(sizeof(IMAGE_FILE_HEADER));
	coff->file_hdr = hdr;
	coff->file_hdr_size = sizeof(IMAGE_FILE_HEADER);
	return hdr;
}

inline unsigned int coff_get_file_header_size(Coff coff)
{
	return coff->file_hdr_size;
}

inline unsigned int coff_get_section_header_size(Coff coff)
{
	return sizeof(IMAGE_SECTION_HEADER);
}

inline unsigned int coff_get_data_offset(Coff coff)
{
	return coff_get_file_header_size(coff) + coff_get_section_header_size(coff) * coff->sec_cnt;
}

int coff_write_file_header(Coff coff)
{
	IMAGE_FILE_HEADER *hdr;
	FILE *f;
	int r = OK;

	f = coff->f;
	hdr = (IMAGE_FILE_HEADER *)coff->file_hdr;

	hdr->Machine = (WORD)coff->machine;
	hdr->NumberOfSections = coff->sec_cnt;
	hdr->PointerToSymbolTable = SEC_SYMBOL_OFFSET(coff->sec_sym);
	hdr->NumberOfSymbols = SEC_SYM_CNT(coff->sec_sym);

	fseek(f, 0, SEEK_SET);
	r = fwrite(hdr, coff->file_hdr_size, 1, f);

	return r == 1;
}

__END_DECLS