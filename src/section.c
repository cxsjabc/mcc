#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "mcc/align.h"
#include "mcc/error.h"
#include "mcc/mem.h"
#include "mcc/section.h"
#include "mcc/symbol.h"
#include "mcc/type.h"

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

unsigned int section_cal_type_offset(Section s, Type *t)
{
        unsigned int align;
	unsigned int offset;

	align = type_align(t);

	offset = ALIGN(s->offset, align);
	return offset;
}

int section_realloc(Section s, unsigned int new_size)
{
	void *p;

	if (s->size >= new_size)
		return OK;

	p = mcc_realloc_safe(s->data, s->size, new_size);
	if (p == NULL)
		return ERR_NO_MEM;
	s->data = p;
	s->size = new_size;

	return OK;
}

Section section_alloc_global_space(Type *t, int section_type, unsigned int *addr)
{
	Section s;
	unsigned int size, new_offset;
	int r;

	size = type_size(t);
	assert(size > 0);

	assert(section_type < SECTION_TYPE_MAX);
	s = Secs[section_type];

	new_offset = section_cal_type_offset(s, t);
	s->offset = new_offset;
	if (SEC_DATA_NEED_INIT(s)) {
		r = section_realloc(s, new_offset + size);
		assert(r == OK);
	}

	*addr = s->offset;
	s->offset += size;

	return s;
}

Section section_alloc_data_space(Type *t, int store_type, int need_init, unsigned int *addr)
{
	int sec_type = SECTION_TYPE_BSS;

	if (need_init)
		sec_type = SECTION_TYPE_DATA;

	if (LOCAL_VARIABLE(store_type))
		; // TODO
	return section_alloc_global_space(t, sec_type, addr);
}

__END_DECLS