#include <stdio.h>

#include "mcc/mcc_base.h"

#include "mcc/coff.h"
#include "mcc/error.h"
#include "mcc/log.h"

__BEGIN_DECLS

void *coff_alloc_file_header(Coff coff)
{
	return NULL;
}

inline unsigned int coff_get_file_header_size(Coff coff)
{
	return 0;
}

inline unsigned int coff_get_section_header_size(Coff coff)
{
	return 0;
}

inline unsigned int coff_get_data_offset(Coff coff)
{
	return 0;
}

int coff_write_file_header(Coff coff)
{
	return ERR_FAIL;
}

__END_DECLS