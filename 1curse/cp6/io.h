#ifndef _IO_H_
#define _IO_H_
#include <stdio.h>
#include <inttypes.h>

#include "structure.h"

uint32_t computer_read_txt(Computer *computer, FILE *in);
uint32_t computer_read_bin(Computer *computer, FILE *in);
void computer_write_bin(Computer *computer, FILE *out);
void computer_print(Computer *computer);

uint32_t reference_read_txt(Reference *reference, FILE *in);

#endif

