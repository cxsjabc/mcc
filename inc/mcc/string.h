#ifndef STRING_H
#define STRING_H

void replace_string_with(char *s, const char orig, const char dest);
int get_string_until_char(const char *s, char **pstart, char delim);

int copy_ignore_char(char *d, const char *s, const int size, const char ignore);

#endif