#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/log.h"
#include "mcc/section.h"
#include "mcc/type.h"

__BEGIN_DECLS

void gen_byte(unsigned char c)
{
	Section s = Secs[SEC_TYPE_TEXT];
	int orig_offset;

	orig_offset = s->offset;
	section_realloc(s, orig_offset + 1);
	s->data[orig_offset] = c;
	s->offset = orig_offset + 1;
}

inline void gen_prefix(unsigned char opcode)
{
	gen_byte(opcode);
}

inline void gen_opcode1(unsigned char opcode)
{
	gen_byte(opcode);
}

void gen_opcode2(unsigned char first, unsigned char second)
{
	gen_byte(first);
	gen_byte(second);
}

void gen_dword(unsigned int c)
{
	gen_byte(c);
	gen_byte(c >> 8);
	gen_byte(c >> 16);
	gen_byte(c >> 24);
}

__END_DECLS