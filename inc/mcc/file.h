#ifndef FILE_H
#define FILE_H

#include "mcc/features.h"

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

__BEGIN_DECLS

extern const char *FileTypeNames[];

int is_absolute_path(const char *path);
int is_relative_path(const char *path);
int is_directory(const char *path);

const char *get_file_name(const char *path);
const char *get_file_extension(const char *path);

char *read_file(const char *path);

int get_file_type(const char *path);

__END_DECLS

#endif