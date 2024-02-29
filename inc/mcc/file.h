#ifndef FILE_H
#define FILE_H

#include "mcc/mcc_base.h"

struct mcc_state;

#define IO_SIZE 4096

typedef enum file_type
{
	FILE_TYPE_UNK = -1,

	FILE_TYPE_C = 0x10000000,
	FILE_TYPE_HEADER = 0x11000000,
	FILE_TYPE_ASM = 0x12000000,

	FILE_TYPE_OBJECT = 0x13000000,
	FILE_TYPE_EXE = 0x14000000,

	FILE_TYPE_HEADER_PATH = 0x15000000,
	FILE_TYPE_LIB_PATH = 0x16000000,	
} FileType;

#define FILE_TYPE_MASK 0xFF000000
#define FILE_TYPE_SHIFT 24
#define FILE_TYPE_BASE 0x10
#define FILE_TYPE_GET(x) ((x) & FILE_TYPE_MASK)
#define FILE_TYPE_IDX(x) ((FILE_TYPE_GET(x) >> FILE_TYPE_SHIFT) - FILE_TYPE_BASE)

typedef struct file
{
	char *buf;
	char *buf_end;

	int fd;
	char *name;

	struct file *prev;
	struct mcc_state *ms;
} *File;

__BEGIN_DECLS

File file_open(const char *name);
void file_close(File f);

extern const char *FileTypeNames[];

int is_absolute_path(const char *path);
int is_relative_path(const char *path);
int is_directory(const char *path);

const char *get_file_name(const char *path);
const char *get_file_extension(const char *path);

int get_preprocess_ext_len();
const char *get_preprocessed_ext();

char *alloc_preprocessed_file_name(const char *path);

char *read_file(const char *path);

int get_file_type(const char *path);

// fill the file from start to end: [start, end]
void file_fill(FILE *f, unsigned char c, int start, int end);
void file_fill_zero(FILE *f, int start, int end);

__END_DECLS

#endif