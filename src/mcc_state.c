#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"

#define DEFAULT_MS_CNT 16

__BEGIN_DECLS

// default global mcc state
struct mcc_state MS;
// global mcc state pointers
static MccState _Ms_static[DEFAULT_MS_CNT], *_Pms; // if thread_num <= 16, use this
static int _Ms_thread_num;

// thread TLS key
static pthread_key_t _Mcc_thread_key;

MccState create_mcc_state()
{
	MccState ms = (MccState)mcc_calloc(1, sizeof(struct mcc_state));
	NULL_RETURN(ms, "Alloc mcc_state failed.\n");

	setup_mcc_state(ms);

	return ms;
}

int init_from_exist_mcc_state(MccState ms)
{
	memset(ms, 0, sizeof(*ms));

	setup_mcc_state(ms);

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

	ms->global_buf = init_mem_buf(4096); // default 4KB buffer
	assert(ms->global_buf);

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

	debug("Mcc state: %p, tid: %ld, global_buf: %p\n", ms, PTHREAD_T_TO_LONG(ms->tid), ms->global_buf);
	dump_mem_buf(ms->global_buf);
	dump_dynamic_array(ms->include_paths);
	dump_dynamic_array(ms->src_files);
	dump_dynamic_array(ms->lib_paths);
	dump_dynamic_array(ms->obj_files);
}

int mcc_state_add_files(MccState ms, const char *path, FileType type)
{
	int r = ERR_FAIL;

	assert(ms);

	if (type == FILE_TYPE_UNK)
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

MccState *mcc_state_multi_thread_create(int thread_num, pthread_func func)
{
	MccState ms;
	int i, r;

	r = pthread_key_create(&_Mcc_thread_key, NULL);
	assert(r == 0);

	assert(thread_num > 0);

	if (thread_num > DEFAULT_MS_CNT) {
		_Pms = (MccState *)mcc_calloc(1, sizeof(MccState) * thread_num);
		assert(_Pms);
	} else {
		_Pms = _Ms_static;
	}
	for (i = 0; i < thread_num; i++) {
		ms = create_mcc_state();
		assert(ms);
		_Pms[i] = ms;
	}
	_Ms_thread_num = thread_num;

	for (i = 0; i < thread_num; i++) {
		r = pthread_create(&_Pms[i]->tid, NULL, func, _Pms[i]); // passing mcc_state to thread
		assert(r == 0);
		debug("(loop %d)Created thread %ld, tls: %p, pid: %ld\n", i, PTHREAD_T_TO_LONG(_Pms[i]->tid), _Pms[i], (unsigned long)getpid());
	}

	for (i = 0; i < thread_num; i++) {
		r = pthread_join(_Pms[i]->tid, NULL);
		assert(r == 0);
	}
	return _Pms;
}

void mcc_state_multi_thread_destroy()
{
	int i, r;

	for (i = 0; i < _Ms_thread_num; i++) {
		destroy_mcc_state(_Pms[i]);
	}

	if (_Ms_thread_num > DEFAULT_MS_CNT) {
		mcc_free(_Pms);
	}
	_Pms = NULL;
	_Ms_thread_num = 0;

	r = pthread_key_delete(_Mcc_thread_key);
	assert(r == 0);
	_Mcc_thread_key = 0;
}

pthread_key_t get_mcc_thread_key()
{
	return _Mcc_thread_key;
}

__END_DECLS