#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dict.h"


enum cmdTypeTag { FINAL, INSERT, REMOVE, SEARCH, SAVE, LOAD, ERROR };
typedef enum cmdTypeTag cmdType;

void WordToLower(char *word)
{
    int len = strlen(word);
    for (int i = 0; i < len; ++i) {
        word[i] = tolower(word[i]);
    }
}

cmdType ReadLine(FILE *in, TKey key, TValue *val)
{
    int c = fgetc(in);

    while (isspace(c) && (c != EOF))
        c = fgetc(in);

    if (c == EOF)
        return FINAL;

    if (isalnum(c))
        ungetc(c, in);

    if (fscanf(in, "%256s", key) != 1)
        return ERROR;

    if (c == '!') {
        if (strcmp(key, "Save") == 0 || strcmp(key, "Load") == 0) {
            if (strcmp(key, "Save") == 0) {
                if (fscanf(in, "%256s", key) != 1)
                    return ERROR;
                return SAVE;
            } else {
                if (fscanf(in, "%256s", key) != 1)
                    return ERROR;
                return LOAD;
            }
        }
        return ERROR;
    } else if (c == '+') {
        if (fscanf(in, "%lu", val) != 1)
            return ERROR;
        return INSERT;
    } else if (c == '-')
        return REMOVE;

    return SEARCH;
}

int main(void)
{
    Dict dict = DictCreate();

    char key[257] = { '\0' };
    TValue val = 0;
    int cmdRes = 0;
    cmdType cmd;

    while ((cmd = ReadLine(stdin, key, &val)) != FINAL) {
        WordToLower(key);

        switch (cmd) {
            case INSERT:
                cmdRes = DictAdd(dict, key, val);
                //TreeDebugPrint(dict);

                if (cmdRes == KEY_ADDED)
                    printf("OK\n");
                else if (cmdRes == KEY_EXISTS)
                    printf("Exist\n");
                else if (cmdRes == ERR_MEM)
                    printf("Error: insufficient memory\n");
                else
                    printf("Error: unclassified error\n");
                break;
           case REMOVE:
                cmdRes = DictRemove(dict, key);
                //TreeDebugPrint(dict);

                if(cmdRes == KEY_REMOVED)
                    printf("OK\n");
                else if (cmdRes == KEY_NOT_EXISTS)
                    printf("NoSuchWord\n");
                else if (cmdRes == ERR_MEM)
                    printf("Error: insufficient memory\n");
                else
                    printf("Error: unclassified error\n");
                break;
            case SAVE:
                cmdRes = DictSave(dict, key);

                if (cmdRes == IO_SUCCESS) {
                    printf("OK\n");
                    //TreeDebugPrint(dict);
                }
                else if (cmdRes == ERR_FILE_OPEN)
                    printf("Error: unable to open file\n");
                else if (cmdRes == ERR_DICT_SAVE)
                    printf("Error: can't save dictionary\n");
                else
                    printf("Error: unclassified error\n");
                break;
            case LOAD:
                cmdRes = DictLoad(&dict, key);
                if (cmdRes == IO_SUCCESS) {
                    printf("OK\n");
                    //TreeDebugPrint(dict);
                }
                else if (cmdRes == ERR_FILE_OPEN)
                    printf("Error: unable to open file\n");
                else if (cmdRes == ERR_DICT_TMP)
                    printf("Error: can't creare dictionary\n");
                else if (cmdRes == ERR_READ)
                    printf("Error: can't read from file\n");
                else
                    printf("Error: unclassified error\n");
                break;
            case SEARCH:
                cmdRes = DictSearch(dict, key, &val);
                if (cmdRes == KEY_EXISTS)
                    printf("OK: %lu\n", val);
                else
                    printf("NoSuchWord\n");
                break;
            default:
                printf("Error: parse error\n");
            break;
        }
    }

    TreeDestroy(&dict);
    return 0;
}