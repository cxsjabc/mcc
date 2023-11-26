#ifndef FILE_H
#define FILE_H

int is_absolute_path(const char *path);
int is_relative_path(const char *path);
int is_directory(const char *path);

const char *get_file_name(const char *path);
const char *get_file_extension(const char *path);

#endif