#ifndef _OWNER_H_
#define _OWNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"

typedef struct _nodeowner{
    char name[STR_SIZE];
    struct _nodeowner *next;
} NodeOwner;

typedef struct {
    NodeOwner* head;
    //NodeOwner* last;
} ListOwner;

ListOwner *list_create();
void list_add(ListOwner *next_owner, Computer *computer);
//void list_delete(NodeOwner *after_node);
//bool list_is_empty(ListOwner *list);
void list_destroy(ListOwner *list);
void list_print(ListOwner *list);

#endif