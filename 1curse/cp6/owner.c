#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "owner.h"

ListOwner *list_create()
{
    ListOwner *list = (ListOwner*) malloc(sizeof(ListOwner));

    list->head = (NodeOwner*) malloc(sizeof(NodeOwner));
    list->head->next = NULL;
    //list->last->next = NULL;

    return list;
}


void list_add(ListOwner *next_owner, Computer *computer)
{
        NodeOwner *tmp = (NodeOwner *) malloc(sizeof(NodeOwner));
        strcpy(tmp->name, computer->owner);
        tmp->next = next_owner->head;
        next_owner->head = tmp;
}


void list_destroy(ListOwner *list)
{
    NodeOwner *n = list->head->next;
    while (list->head != NULL)
    {
        n = list->head->next;
        free(list->head);
        list->head = n;
    }
    list->head = NULL;
}


void list_print(ListOwner *list)
{
    for(NodeOwner* tmp = list->head; tmp->next; tmp = tmp->next)
        printf("%s\n", tmp->name);
}
