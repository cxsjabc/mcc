#include <assert.h>
#include <stdio.h>

#include "mcc/args.h"
#include "mcc/array.h"
#include "mcc/compile.h"
#include "mcc/compile_log.h"
#include "mcc/decl.h"
#include "mcc/error.h"
#include "mcc/help.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/string.h"
#include "mcc/token.h"

File Cfile;

__BEGIN_DECLS

int comp_source_file_internal(MccState ms, char *name)
{
	int r = OK;
	File f;
	Token t;

	(void)t;
	// TODO: ignore ms now, for global processing
	ms = &MS;

	f = file_open(name);
	assert(f);
	Cfile = f;

#if 0
	do {
		t = next(f);
		if (t != NULL) {
			token_dump(t);
		}
		// sleep(1);
	} while (t != NULL);
#endif

	next(f);
	while (Tok->type != TOK_EOF) {
		r = parse_global_decl();
		if (r < OK)
			break;
	}

	file_close(f);

	return r;
}

int comp_source_file(MccState ms, char *name)
{
	Args_option opt;

	opt = ms->opt;
	if (ARG_VAR_PURE_EXE(opt)) {
		ms->target_file = TARGET_EXE;
	} else if (ARG_VAR_PREPROCESS_ONLY(opt)) {
		debug_nofl("only preprocess\n");
		ms->target_file = TARGET_PPFILE;
	} else if (ARG_VAR_COMPILE_ONLY(opt)) {
		debug_nofl("only compile\n");
		ms->target_file = TARGET_OBJ;
	} else if (ARG_VAR_ASSEMBLE_ONLY(opt)) {
		debug_nofl("only assemble\n");
		ms->target_file = TARGET_ASM;
	}

	cdebug("output file: \"%s\"\n", opt->out_file.str);

	return comp_source_file_internal(ms, name);
}

int comp_obj_file(MccState ms, char *name)
{
	int r = OK;

	return r;
}

int comp_source(MccState ms, int idx)
{
	char *f;

	f = (char *)vec_at(ms->src_files, idx);
	ms->curr_idx = idx;
	debug("compile file: %s\n", STR(f));

	return comp_source_file(ms, f);
}

int comp_obj(MccState ms, int idx)
{
	char *f;

	f = (char *)vec_at(ms->obj_files, idx);
	ms->curr_obj_idx = idx;
	debug("compile obj: %s\n", STR(f));

	return comp_obj_file(ms, f);
}

int compile(MccState ms)
{
	int r = OK;
	int i;
	int srcs_cnt, objs_cnt;

	if (!ms)
		ms = &MS;
	srcs_cnt = vec_size(ms->src_files);
	objs_cnt = vec_size(ms->obj_files);

	for (i = 0; i < srcs_cnt; i++) {
		r = comp_source(ms, i);
		if (r < OK)
			return r;
	}

	for (i = 0; i < objs_cnt; i++) {
		r = comp_obj(ms, i);
		if (r < OK)
			return r;
	}
	return r;
}

__END_DECLS