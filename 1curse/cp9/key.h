#ifndef _KEY_H_
#define _KEY_H_

#include <stdbool.h>
#include <string.h>

#define KEY_LEN 6

typedef struct _key {
    char value[KEY_LEN];
} *Key;

Key key_create(void);
void key_destroy(Key *key);
void key_insert(Key key);
void key_print(Key key);
int key_cmp(Key key1, Key key2);
int key_len(void);

#endif