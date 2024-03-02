#ifndef COFF_H
#define COFF_H

#include <stdio.h>

#include "mcc/mcc_base.h"

#include "mcc/section.h"

typedef struct coff
{
	int machine;
	int sec_cnt;
	void *file_hdr;
	unsigned int file_hdr_size;

	Section sec_sym;
} *Coff;

__BEGIN_DECLS

Coff coff_alloc();
void coff_free(Coff c);

void coff_set_machine(Coff coff, int machine);
int coff_get_machine(Coff coff);
void coff_set_sec_cnt(Coff coff, int sec_cnt);
int coff_get_sec_cnt(Coff coff);

void coff_set_sec_sym(Coff coff, Section sec_sym);
Section coff_get_sec_sym(Coff coff);

void *coff_alloc_file_header(Coff coff);

unsigned int coff_get_file_header_size(Coff coff);
unsigned int coff_get_section_header_size(Coff coff);
unsigned int coff_get_data_offset(Coff coff);

void coff_write_file_header(Coff coff);
unsigned int coff_write_section_data(Coff coff, Section s, FILE *f, unsigned int data_offset);

__END_DECLS

#endif