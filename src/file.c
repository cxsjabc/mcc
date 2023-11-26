#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"

int is_absolute_path(const char *path)
{
    if (path[0] == '/')
        return 1;
    else
        return 0;
}

int is_relative_path(const char *path)
{
    return !is_absolute_path(path);
}

int is_directory(const char *path)
{
    return 0;
}

const char *get_file_name(const char *path)
{
    const char *file = strrchr(path, '/');
    if (file == NULL)
        return path;
    else
        return file + 1;
}

const char *get_file_extension(const char *path)
{
    const char *ext = strrchr(path, '.');
    if (ext == NULL)
        return "";
    else
        return ext + 1;
}