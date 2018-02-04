#ifndef VECTOR_H
#define VECTOR_H

#include <ctype.h>
#include <inttypes.h>

typedef struct item *TVectorItem;
typedef struct vector *TVector;

typedef uint64_t TVal;

TVector VectorCreate(void);
void VectorDestroy(TVector *vec);
void VectorInsert(TVector vec, char *key, TVal val);
void VectorResize(TVector vec);
void VectorPrint(TVector vec);

void ReadItem(TVector vec);
void VectorRadixSort(TVector vec);

#endif