#ifndef COMPILE_H
#define COMPILE_H

#include "mcc/mcc_base.h"

#include "mcc/mcc_state.h"

enum {
	TARGET_EXE,
	TARGET_PPFILE,
	TARGET_OBJ,
	TARGET_ASM,
};

extern File Cfile;

__BEGIN_DECLS

int compile(MccState ms);

int comp_source(MccState ms, int idx);
int comp_obj(MccState ms, int idx);
int comp_source_file(MccState ms, char *f);
int comp_obj_file(MccState ms, char *f);

__END_DECLS

#endif