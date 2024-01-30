#ifndef SECTION_H
#define SECTION_H

#include "mcc/mcc_base.h"

typedef struct section_header
{
#ifdef _WINDOWS || defined(_MSC_VER)
	IMAGE_SECTION_HEADER hdr;
#else
	Elf64_Shdr hdr;
#endif
} *Sec_Hdr;

typedef struct section
{
	char *data;
	unsigned int max_size;
	unsigned int offset;

	struct section_header hdr;
} *Section;

__BEGIN_DECLS

__END_DECLS

#endif