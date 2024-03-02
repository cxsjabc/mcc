#ifndef SECTION_HEADER_H
#define SECTION_HEADER_H

#include "mcc/mcc_base.h"

typedef struct section
{
	int offset; // the first element, because it is used more frequently

	void *hdr;
	unsigned char *data;
	unsigned int size;
} *Section;

enum {
	SECTION_TYPE_TEXT,
	SECTION_TYPE_DATA,
	SECTION_TYPE_BSS,
	SECTION_TYPE_RDATA,
	SECTION_TYPE_IDATA,
	SECTION_TYPE_SYMBOL,
	SECTION_TYPE_SYMBOL_STR,
	SECTION_TYPE_RELOC,
	// the above sections are used for object file
	// the below sections are used for final executable file
	SECTION_TYPE_LINKSYM,
	SECTION_TYPE_LINKSYM_STR,

	SECTION_TYPE_MAX,
};

#define SEC_DATA_NEED_INIT(sec) 1 // TODO

__BEGIN_DECLS

__END_DECLS

#endif