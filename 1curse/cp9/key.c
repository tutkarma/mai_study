#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "key.h"


int key_len(void)
{
    return KEY_LEN;
}

Key key_create(void)
{
    Key key = (Key) calloc(1, sizeof(*key));
    if(!key) {
        fprintf(stderr, "Error:no memory\n");
        exit(1);
    }
    return key;
}

void key_destroy(Key *key)
{
    free(*key);
    *key = NULL;
}

void key_insert(Key key)
{
    fscanf(stdin, "%s", key->value);
}

void key_print(Key key)
{
    printf("%s", key->value);
}


int key_cmp(Key key1, Key key2)
{
    for (uint32_t i = 0; i < KEY_LEN; ++i) {
        if (key1->value[i] > key2->value[i])
            return 1;
        if (key1->value[i] < key2->value[i])
            return -1;
    }
    return 0;
}
