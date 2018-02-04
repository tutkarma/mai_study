#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "dict.h"

Dict DictCreate(void)
{
    return TreeCreate();
}

int DictSearch(Dict dict, TKey key, TValue *val)
{
    TPatriciaNode tmp = NodeSearch(dict, key);
    if (tmp != NULL) {
        *val = tmp->value;
        return KEY_EXISTS;
    }
    val = NULL;
    return KEY_NOT_EXISTS;
}

int DictAdd(Dict dict, TKey key, TValue val)
{
    TKey s = (TKey) malloc(strlen(key) + 1);
    if (!s) {
        return ERR_MEM;
    }
    strcpy(s, key);
    if (NodeSearch(dict, key) == NULL) {
        int err = TreeInsert(dict, s, val);
        if (err) {
            free(s);
            return ERR_MEM;
        } else {
            //free(s);
            return KEY_ADDED;
        }
    } else {
        free(s);
        return KEY_EXISTS;
    }
}

int DictRemove(Dict dict, TKey key)
{
    if (NodeSearch(dict, key)) {
        TreeDelete(dict, key);
        return KEY_REMOVED;
    } else {
        return KEY_NOT_EXISTS;
    }
}

int DictSave(Dict dict, const char *filename)
{
    FILE *f = fopen(filename, "w+");
    if (!f) {
        return ERR_FILE_OPEN;
    }
    int err = DictWrite(f, dict);
    if (err) {
        fclose(f);
        return ERR_DICT_SAVE;
    }
    fclose(f);
    return IO_SUCCESS;
}

int DictLoad(Dict *dict, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) {
        return ERR_FILE_OPEN;
    }
    Dict newdict = DictCreate();
    if (!newdict) {
        fclose(f);
        return ERR_DICT_TMP;
    }
    int err = DictRead(f, newdict);
    //printf("err: %d\n", err);
    if (err == -2) {
        fclose(f);
        return ERR_READ;
    }
    fclose(f);
    TreeDestroy(dict);
    *dict = newdict;
    if (err && err != -2)
        return ERR_READ;
    return IO_SUCCESS;
}