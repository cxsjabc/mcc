#include <stdio.h>
#include <windows.h>

#include "mcc/mcc_base.h"

#include "mcc/log.h"
#include "mcc/section.h"

__BEGIN_DECLS

void __need_init global_section_init()
{
	Secs[SEC_TYPE_TEXT] = section_alloc(".text", SEC_TYPE_TEXT, IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE);
	Secs[SEC_TYPE_DATA] = section_alloc(".data", SEC_TYPE_DATA, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	Secs[SEC_TYPE_BSS] = section_alloc(".bss", SEC_TYPE_BSS, IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
	Secs[SEC_TYPE_RDATA] = section_alloc(".rodata", SEC_TYPE_RDATA, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ);
	Secs[SEC_TYPE_IDATA] = section_alloc(".idata", SEC_TYPE_IDATA, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);

	Secs[SEC_TYPE_RELOC] = section_alloc(".reloc", SEC_TYPE_RELOC, IMAGE_SCN_MEM_READ | IMAGE_SCN_LNK_REMOVE);

	Secs[SEC_TYPE_SYMBOL] = section_alloc(".symbol", SEC_TYPE_SYMBOL, IMAGE_SCN_MEM_READ | IMAGE_SCN_LNK_REMOVE);
	Secs[SEC_TYPE_SYMBOL_STR] = section_alloc(".symstr", SEC_TYPE_SYMBOL_STR, IMAGE_SCN_MEM_READ | IMAGE_SCN_LNK_REMOVE);
	Secs[SEC_TYPE_SYMBOL]->str_sec = Secs[SEC_TYPE_SYMBOL_STR];

	Secs[SEC_TYPE_LINKSYM] = section_alloc(".link", SEC_TYPE_LINKSYM, IMAGE_SCN_MEM_READ | IMAGE_SCN_LNK_REMOVE);
	Secs[SEC_TYPE_LINKSYM_STR] = section_alloc(".lnkstr", SEC_TYPE_LINKSYM_STR, IMAGE_SCN_MEM_READ | IMAGE_SCN_LNK_REMOVE);
	Secs[SEC_TYPE_LINKSYM]->str_sec = Secs[SEC_TYPE_LINKSYM_STR];
}

void section_update_header(Section s, const char *name, int charactics)
{
	if (strlen(name) > IMAGE_SIZEOF_SHORT_NAME - 1)
		error("section name(%s) is too long\n", name);

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

void section_update_virt_addr_size(Section s, unsigned int virt_addr, unsigned int size)
{
	IMAGE_SECTION_HEADER *hdr;

	hdr = &s->hdr;
	hdr->VirtualAddress = virt_addr;
	hdr->Misc.VirtualSize = size;
}

__END_DECLS