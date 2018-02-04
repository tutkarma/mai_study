#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

const static int KEY_SIZE = 32;
const static int MEM_STEP = 2;
const static int NUMERAL_SYSTEM = 16;
const static int CHAR_TO_INT = 10;

struct item {
    char key[33];
    TVal value;
}; //*TVectorItem

struct vector {
    uint32_t size;
    uint32_t freespace;
    TVectorItem array;
}; //*TVector;

TVector VectorCreate(void)
{
    TVector vec = (TVector) malloc(sizeof(*vec));
    if (!vec) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(EXIT_SUCCESS);
    }
    vec->array = (TVectorItem) malloc(sizeof(*(vec->array)));
    vec->size = 0;
    vec->freespace = 1;
    return vec;
}
void VectorResize(TVector vec)
{
    vec->array = realloc(vec->array, MEM_STEP * vec->size * sizeof(*(vec->array)));
    if (!vec->array) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(EXIT_SUCCESS);
    }
    vec->freespace = vec->size;
}

void VectorInsert(TVector vec, char *key, TVal val)
{
    if (!vec->freespace) {
        VectorResize(vec);
    }
    strcpy(vec->array[vec->size].key, key);
    vec->array[vec->size].value = val;
    vec->size++;
    vec->freespace--;
}

void VectorPrint(TVector vec)
{
    if (vec) {
        for (uint32_t i = 0; i < vec->size; ++i) {
            printf("%s\t", vec->array[i].key);
            printf("%lu\n", vec->array[i].value);
        }
    }
}

void VectorDestroy(TVector *vec)
{
    free((*vec)->array);
    (*vec)->array = NULL;
    free(*vec);
    *vec = NULL;
}

void VectorRadixSort(TVector vec)
{
    uint32_t size = vec->size;
    TVectorItem array = vec->array;
    int counts[NUMERAL_SYSTEM];
    int i, j;
    TVectorItem out = malloc(sizeof(*out) * size);
    if (!out) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(EXIT_SUCCESS);
    }
    for (i = KEY_SIZE - 1; i >= 0; --i) {
        for (j = 0; j < NUMERAL_SYSTEM; ++j) {
            counts[j] = 0;
        }
        for (j = 0; j < size; ++j) {
            if (isdigit(array[j].key[i])) {
                ++counts[array[j].key[i] - '0'];
            } else {
                ++counts[array[j].key[i] - 'a' + CHAR_TO_INT];
            }
        }
        for (j = 1; j < NUMERAL_SYSTEM; ++j) {
            counts[j] += counts[j - 1];
        }
        for (j = size - 1; j >= 0; --j) {
            if (isdigit(array[j].key[i])) {
                out[--counts[array[j].key[i] - '0']] = array[j];
            } else {
                out[--counts[array[j].key[i] - 'a' + CHAR_TO_INT]] = array[j];
            }
        }
        for (j = 0; j < size; ++j) {
            array[j] = out[j];
        }
    }
    free(out);
}

void ReadItem(TVector vec)
{
    char key[KEY_SIZE + 1];
    TVal value;
    while (scanf("%s", key) != EOF) {
        scanf("%lu", &value);
        VectorInsert(vec, key, value);
    }
}

