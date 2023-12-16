#include <assert.h>
#include <stdio.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"

__BEGIN_DECLS

struct mcc_state MS;

MccState create_mcc_state()
{
	MccState ms = (MccState)mcc_malloc(sizeof(struct mcc_state));
	NULL_RETURN(ms, "Alloc mcc_state failed.\n");

	setup_mcc_state(ms);

	return ms;
}

int init_from_exist_mcc_state(MccState ms)
{
	memset(ms, 0, sizeof(*ms));

	setup_mcc_state(ms);
	ms->global_buf = init_mem_buf(0);

	return 1;
}

void destroy_mcc_state(MccState ms)
{
	if (ms) {
		clean_mcc_state(ms);
		mcc_free(ms);
	}
}

void setup_mcc_state(MccState ms)
{
	assert(ms);

	ms->include_paths = create_dynamic_array(32);
	assert(ms->include_paths);
	ms->include_paths->to_string = NULL;
	
	ms->src_files = create_dynamic_array(2);
	assert(ms->src_files);

	ms->lib_paths = create_dynamic_array(0);
	assert(ms->lib_paths);

	ms->obj_files = create_dynamic_array(0);
	assert(ms->obj_files);
}

void clean_mcc_state(MccState ms)
{
	assert(ms);

	destroy_dynamic_array(ms->include_paths);
	destroy_dynamic_array(ms->src_files);
	destroy_dynamic_array(ms->lib_paths);
	destroy_dynamic_array(ms->obj_files);
}

void dump_mcc_state(MccState ms)
{
	assert(ms);

	dump_dynamic_array(ms->include_paths);
	dump_dynamic_array(ms->src_files);
	dump_dynamic_array(ms->lib_paths);
	dump_dynamic_array(ms->obj_files);
}

int mcc_state_add_files(MccState ms, const char *path, FileType type)
{
	int r = ERR_FAIL;

	assert(ms);

	if (type == FILE_TYPE_UNKNOWN)
		type = (FileType)get_file_type(path);

	switch (type)
	{
	case FILE_TYPE_C:
		r = dynamic_array_push(ms->src_files, (void *)path);
		break;
	case FILE_TYPE_HEADER_PATH:
		r = dynamic_array_push(ms->include_paths, (void *)path);
		break;
	case FILE_TYPE_OBJECT:
		r = dynamic_array_push(ms->obj_files, (void *)path);
		break;
	case FILE_TYPE_LIB_PATH:
		r = dynamic_array_push(ms->lib_paths, (void *)path);
		break;
	default:
		error("Unknown file type, %d\n", type);
		break;
	}

	return r;
}

__END_DECLS