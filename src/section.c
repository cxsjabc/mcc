#include <stdio.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/mem.h"
#include "mcc/section.h"

#define SECTION_INIT_SIZE 64

__BEGIN_DECLS

Section Secs[SECTION_TYPE_MAX];

void __need_init global_section_init()
{
	Secs[SECTION_TYPE_TEXT] = section_alloc(".text", IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE);
	Secs[SECTION_TYPE_DATA] = section_alloc(".data", IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	Secs[SECTION_TYPE_BSS] = section_alloc(".bss", IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	Secs[SECTION_TYPE_RDATA] = section_alloc(".rdata", IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ);
	Secs[SECTION_TYPE_IDATA] = section_alloc(".idata", IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
}

void global_section_free()
{
	int i;

	for (i = 0; i < SECTION_TYPE_MAX; i++) {
		section_free(Secs[i]);
	}
}

Section section_alloc(const char *name, int charactics)
{
	Section s = allocmz(sizeof(struct section));
	NULL_RETURN(s, "alloc section failed");

	s->data = mcc_malloc(SECTION_INIT_SIZE);
	NULL_RETURN(s->data, "alloc section data failed");
	s->size = SECTION_INIT_SIZE;

	section_update_header(s, name, charactics);
	
	return s;
}

void section_update_header(Section s, const char *name, int charactics)
{
	if (strlen(name) >= IMAGE_SIZEOF_SHORT_NAME - 1)
		error("section name(%s) is too long", name);

	strcpy((char *)s->hdr.Name, name);
	s->hdr.Characteristics = charactics;
}

void section_free(Section sec)
{
	mcc_free(sec->data);
	freem(sec);
}

__END_DECLS