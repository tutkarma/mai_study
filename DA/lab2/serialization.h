#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stdio.h>

static const int KEY_LIMIT = 256;

int DictWrite(FILE *f, TPatricia tree);
int DictRead(FILE *f, TPatricia tree);

#endif