#ifndef TDICT_H
#define TDICT_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "TPatricia.h"
#include "serialization.h"

enum {ERR_READ, ERR_DICT_TMP, ERR_DICT_LOAD,
      ERR_DICT_SAVE, ERR_FILE_OPEN,
      ERR_MEM, KEY_NOT_EXISTS, KEY_EXISTS,
      KEY_ADDED, KEY_REMOVED, IO_SUCCESS};

typedef TPatricia Dict;

Dict DictCreate(void);
int DictSearch(Dict dict, TKey key, TValue *val);
int DictAdd(Dict dict, TKey key, TValue val);
int DictRemove(Dict dict, TKey key);
int DictSave(Dict dict, const char *filename);
int DictLoad(Dict *dict, const char *filename);

#endif