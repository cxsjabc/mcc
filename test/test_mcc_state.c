#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mcc/args.h"
#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/size.h"
#include "mcc/string.h"

__BEGIN_DECLS

#define TEST_THREAD_NUM 32

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static void *saved_buffer_addr[TEST_THREAD_NUM];
static int saved_index;
static const int size = 128 * 2;
static int unit;

void verify_mem_buf(MccState *pms, int num)
{
	int i;

	for (i = 0; i < num; i++) {
		unsigned long *p = (unsigned long *)saved_buffer_addr[i];
		unsigned long *p1 = (unsigned long *)saved_buffer_addr[i + 1];

		array_dump(p, size);
		assert(is_array_element_equal(p, size, unit));
		if (i < num - 1)
			assert(*p != *p1);
	}
}

void test_mem_buf_data(MccState ms)
{
	MemBuf mb = ms->global_buf;
	unsigned long *p;
	int i;

	(void)mb;

	p = (unsigned long *)allocm(size);
	//p = (unsigned long *)alloc_from_mem_buf(mb, size);
	assert(p);

	memset(p, 0, size);
	i = size / unit - 1;

	while (i >= 0) {
		assert(unit == sizeof(unsigned long));
		p[i] = PTHREAD_T_TO_LONG(ms->tid);
		// debug("p[%d] = %lx\n", i, p[i]);
		--i;
	}

	pthread_mutex_lock(&mutex);
	saved_buffer_addr[saved_index++] = p;
	pthread_mutex_unlock(&mutex);
}

void *thread_func(void *arg)
{
	int r;
#if 0
	const char *argv[] = {"-I", ".", "-I/usr/xxx", "3.c", "-L./lib", "-L", "abc", "-Iabc/d", "-I", "..", "1.c", "2.c"};
	int argc = ARRAY_SIZE(argv);
#endif
	MccState ms;
	void *tls;

	ms = (MccState)arg;
	r = pthread_setspecific(get_mcc_thread_key(), (void *)ms);
	assert(r == 0);
	tls = pthread_getspecific(get_mcc_thread_key());
	assert(tls == ms);
	debug("Thread %lx TLS: %p, pid: %lx\n", PTHREAD_T_TO_LONG(pthread_self()), ms, (unsigned long)getpid());

#if 0
	r = parse_args(argc, (char **)argv, ms);
	assert(r == ERR_NONE);

	assert(ms->include_paths->size == 4);
	assert(ms->src_files->size == 3);
	assert(ms->lib_paths->size == 2);

	assert(STR_EQL(INC_PATH(ms, 0), "."));
	assert(STR_EQL(INC_PATH(ms, 1), "/usr/xxx"));
	assert(STR_EQL(INC_PATH(ms, 2), "abc/d"));
	assert(STR_EQL(INC_PATH(ms, 3), ".."));

	assert(STR_EQL(LIB_PATH(ms, 0), "./lib"));
	assert(STR_EQL(LIB_PATH(ms, 1), "abc"));

	assert(STR_EQL(SRC_FILE(ms, 0), "3.c"));
	assert(STR_EQL(SRC_FILE(ms, 1), "1.c"));
	assert(STR_EQL(SRC_FILE(ms, 2), "2.c"));

	dump_mcc_state(ms);
#endif
	test_mem_buf_data(ms);

	return NULL;
}

void test_mcc_state()
{
	MccState *ms;

	unit = sizeof(unsigned long);

	ms = mcc_state_multi_thread_create(TEST_THREAD_NUM, thread_func);
	assert(ms);
	debug("Wait for sub thread to finish...\n");

	verify_mem_buf(ms, TEST_THREAD_NUM);
	// dump_mcc_state(ms);
	mcc_state_multi_thread_destroy();

	pthread_mutex_destroy(&mutex);
	always("%s pass.\n", __func__);
	return;
}

__END_DECLS