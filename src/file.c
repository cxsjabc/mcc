#define NO_DEBUG 1

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/string.h"

__BEGIN_DECLS

File file_open(const char *name)
{
	int fd;
	File f;

	f = mcc_malloc(sizeof(struct file));
	NULL_RETURN(f, NULL, "open file failed\n");

	fd = open(name, O_RDONLY);
	if (fd < 0) {
		error("open file %s failed\n", name);
		freem(f);
		return NULL;
	}

	f->fd = fd;
	f->name = (char *)name;
	f->buf = f->buf_end = NULL;

	return f;
}

void file_close(File f)
{
	if (f) {
		if (f->fd >= 0)
			close(f->fd);
		mcc_free(f);
	}
}

// **Should be matched with FileType(starting from FILE_TYPE_C) **
const char *FileTypeNames[] = {
	"C File",
	"Header File",
	"ASM File",

	"Object File",
	"Executable File",

	"Header Path",
	"Library Path",
};

int get_file_type(const char *path)
{
	const char *ext = get_file_extension(path);

	LHD;
	if (strcmp(ext, "c") == 0)
		return FILE_TYPE_C;
	else if (strcmp(ext, "h") == 0)
		return FILE_TYPE_HEADER;
	else if (strcmp(ext, "o") == 0 || strcmp(ext, "obj") == 0)
		return FILE_TYPE_OBJECT;
	else if (strcmp(ext, "s") == 0 || strcmp(ext, "S") == 0)
		return FILE_TYPE_ASM;
	else if (strcmp(ext, "exe") == 0)
		return FILE_TYPE_EXE;
	else
		return FILE_TYPE_UNK;
}

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
		return NULL;
	else
		return *(ext + 1) != '\0' ? ext + 1 : NULL;
}

int get_preprocess_ext_len()
{
	return strlen(get_preprocessed_ext());
}

const char *get_preprocessed_ext()
{
	return "i";
}

char *alloc_preprocessed_file_name(const char *path)
{
	const char *ext = get_file_extension(path);
	int non_ext_len = strlen(path) - strlen(ext);
	int pp_ext_len = get_preprocess_ext_len();
	char *pf;

	pf = (char *)allocm(non_ext_len + pp_ext_len + 1);
	NULL_RETURN(pf, NULL, "Alloc preprocessed file name failed");
	
	strncpy(pf, path, non_ext_len);
	strncpy(pf + non_ext_len, get_preprocessed_ext(), pp_ext_len);
	pf[non_ext_len + pp_ext_len] = '\0';

	return pf;
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

	char *buffer = (char *)mcc_malloc(size + 1);
	if (buffer == NULL)
	{
		fclose(file);
		return NULL;
	}

	r = fread(buffer, 1, size, file);
	if ((long)r != size) {
		error("Read file buffer error, fread return the wrong size: %zu(expect: %ld)\n", r, size);
		mcc_free(buffer);
		fclose(file);
		return NULL;
	}
	fclose(file);
	buffer[size] = '\0';
	return buffer;
}

void file_fill(FILE *f, unsigned char c, int start, int end)
{
	assert(end >= start);
        fseek(f, start, SEEK_SET);
	while (start++ <= end)
		fputc(c, f);
}

void file_fill_zero(FILE *f, int start, int end)
{
	file_fill(f, 0, start, end);
}

__END_DECLS