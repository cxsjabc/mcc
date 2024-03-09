#ifndef SEC_HEADER_H
#define SEC_HEADER_H

#include "mcc/mcc_base.h"

typedef struct section
{
	int offset; // the first element, because it is used more frequently

	void *hdr;
	unsigned char *data;
	unsigned int size;

	struct section *str_sec;
	int sec_type;
} *Section;

enum {
	SEC_TYPE_TEXT,
	SEC_TYPE_DATA,
	SEC_TYPE_BSS,
	SEC_TYPE_RDATA,
	SEC_TYPE_IDATA,
	SEC_TYPE_SYMBOL,
	SEC_TYPE_SYMBOL_STR,
	SEC_TYPE_RELOC,
	// the above sections are used for object file
	// together with the below sections are used for final executable file
	SEC_TYPE_LINKSYM,
	SEC_TYPE_LINKSYM_STR,

	SEC_TYPE_MAX,
};

#define SEC_DATA_NEED_INIT(sec) 1 // TODO

__BEGIN_DECLS

__END_DECLS

#endif