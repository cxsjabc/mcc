#ifndef FILE_H
#define FILE_H

#include "mcc/mcc_base.h"

#define IO_SIZE 10

typedef enum file_type
{
	FILE_TYPE_UNKNOWN = -1,

	FILE_TYPE_C,
	FILE_TYPE_HEADER,
	FILE_TYPE_ASM,

	FILE_TYPE_OBJECT,
	FILE_TYPE_EXE,

	FILE_TYPE_HEADER_PATH,
	FILE_TYPE_LIB_PATH,	
} FileType;

typedef struct file
{
	char *buf;
	char *buf_end;

	int fd;
	char *name;

	struct file *prev;
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

__END_DECLS

#endif