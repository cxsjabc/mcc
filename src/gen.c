#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/log.h"
#include "mcc/type.h"

__BEGIN_DECLS

int ind = 0;

void gen_byte(char c)
{

}

void gen_prefix(char opcode)
{
	gen_byte(opcode);
}

void gen_opcode1(char opcode)
{
	gen_byte(opcode);
}

void gen_opcode2(char first, char second)
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