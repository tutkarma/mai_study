#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "data.h"


Data data_create(void)
{
    Data data = (Data) calloc(1, sizeof(*data));
    if(!data) {
        fprintf(stderr, "Error:no memory\n");
        exit(1);
    }
    return data;
}
void data_destroy(Data *data)
{
    free(*data);
    *data = NULL;
}

void data_insert(Data data)
{
    fscanf(stdin, "%s", data->value);
}


void data_print(Data data)
{
    printf("%s", data->value);

}

int data_len(void)
{
    return DATA_LEN;
}