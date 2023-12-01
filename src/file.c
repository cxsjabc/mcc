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
    // TODO
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

char *read_file(const char *path)
{
    FILE *file;
    size_t r;
    long size;

    file = fopen(path, "r");
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (buffer == NULL)
    {
        fclose(file);
        return NULL;
    }

    r = fread(buffer, 1, size, file);
    if (r != size) {
        //error("Read file buffer error, fread return the wrong size: %ul(expect: %ld)\n", r, size);
        free(buffer);
        fclose(file);
        return NULL;
    }
    fclose(file);
    buffer[size] = '\0';
    return buffer;
}