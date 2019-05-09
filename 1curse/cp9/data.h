#ifndef _DATA_H_
#define _DATA_H_

#include <stdbool.h>
#include <string.h>

#define DATA_LEN 100

typedef struct _data {
    char value[DATA_LEN];
} *Data;

Data data_create(void);
void data_destroy(Data *data);
void data_insert(Data data);
void data_print(Data data);
int data_len(void);

#endif