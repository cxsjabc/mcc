#include <stdio.h>
#include <windows.h>

#include "mcc/mcc_base.h"

#include "mcc/log.h"
#include "mcc/section.h"

__BEGIN_DECLS

void __need_init global_section_init()
{
	Secs[SECTION_TYPE_TEXT] = section_alloc(".text", IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE);
	Secs[SECTION_TYPE_DATA] = section_alloc(".data", IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	Secs[SECTION_TYPE_BSS] = section_alloc(".bss", IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	Secs[SECTION_TYPE_RDATA] = section_alloc(".rdata", IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ);
	Secs[SECTION_TYPE_IDATA] = section_alloc(".idata", IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
}

void section_update_header(Section s, const char *name, int charactics)
{
	if (strlen(name) >= IMAGE_SIZEOF_SHORT_NAME - 1)
		error("section name(%s) is too long", name);

	strcpy((char *)s->hdr.Name, name);
	s->hdr.Characteristics = charactics;
}

void section_update_header_pointer(Section s, unsigned int data_offset)
{
	IMAGE_SECTION_HEADER hdr;

	(void)hdr;
	hdr = s->hdr;
	hdr.PointerToRawData = data_offset;
	hdr.SizeOfRawData = s->offset;
}

__END_DECLS