#ifndef FILE_H
#define FILE_H

typedef enum file_type
{
    FILE_TYPE_C,
    FILE_TYPE_H,
    FILE_TYPE_ASM,
    FILE_TYPE_O,
    FILE_TYPE_EXE,
} FileType;

int is_absolute_path(const char *path);
int is_relative_path(const char *path);
int is_directory(const char *path);

const char *get_file_name(const char *path);
const char *get_file_extension(const char *path);

char *read_file(const char *path);

#endif