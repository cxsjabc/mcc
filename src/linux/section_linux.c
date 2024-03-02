#include <stdio.h>

#include "mcc/mcc_base.h"

#include "mcc/log.h"
#include "mcc/section.h"

__BEGIN_DECLS

void __need_init global_section_init()
{
	// TODO
	Secs[SECTION_TYPE_TEXT] = section_alloc(".text", 0);
	Secs[SECTION_TYPE_DATA] = section_alloc(".data", 0);
	Secs[SECTION_TYPE_BSS] = section_alloc(".bss", 0);
	Secs[SECTION_TYPE_RDATA] = section_alloc(".rdata", 0);
	Secs[SECTION_TYPE_IDATA] = section_alloc(".idata", 0);
}

void section_update_header(Section s, const char *name, int charactics)
{
	// TODO
}

void section_update_header_pointer(Section s, unsigned int data_offset)
{
	
}

__END_DECLS