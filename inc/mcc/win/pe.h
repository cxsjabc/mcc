#ifndef PE_H
#define PE_H

#include "mcc/mcc_base.h"

#include <windows.h>
#include <winnt.h>

extern IMAGE_DOS_HEADER golden_dos_header;
extern BYTE golden_dos_stub[0x40];
extern IMAGE_NT_HEADERS golden_nt_headers;

__BEGIN_DECLS

void __need_init pe_verify_header();
void __need_init pe_verify_dos_header();
void __need_init pe_verify_nt_header();

__END_DECLS

#endif