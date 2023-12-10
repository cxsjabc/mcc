#ifndef MCC_STATE_H
#define MCC_STATE_H

#include "mcc/array.h"
#include "mcc/features.h"
#include "mcc/file.h"
#include "mcc/mem.h"
#include "mcc/mem_buf.h"

__BEGIN_DECLS

// global mcc_state
// this is used when program initialization, if multi threaded, passing the state to each thread
extern struct mcc_state MS;

typedef struct mcc_state
{
	// mem buf
	struct mem_buf global_buf;  // buffers for all compiled files
	struct mem_buf compile_buf; // buffers for the current compiled file

	// header or lib pathes
	DynArray include_paths; // header file pathes
	DynArray lib_paths;	 // library pathes

	DynArray src_files;	 // files to compile
	DynArray obj_files;	 // objects to compile

} *MccState;

MccState create_mcc_state();
void destroy_mcc_state(MccState ms);

void setup_mcc_state(MccState ms);
void clean_mcc_state(MccState ms);
void dump_mcc_state(MccState ms);

// if type is -1, get file type from path
int mcc_state_add_files(MccState ms, const char *path, FileType type);

__END_DECLS

#define INC_PATH(ms, index) ((char *)((ms)->include_paths->data[index]))
#define LIB_PATH(ms, index) ((char *)((ms)->lib_paths->data[index]))
#define SRC_FILE(ms, index) ((char *)((ms)->src_files->data[index]))
#define OBJ_FILE(ms, index) ((char *)((ms)->obj_files->data[index]))

#endif