#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <complex.h>

#include "list.h"

struct _node {
    elem_type elem;
    struct _node *next;
    struct _node *prev;
};

struct _list {
    Node *head;
    Node *iter;
    uint32_t elements;
};


void iterator_next(List list);
void iterator_prev(List list);
void iterator_first(List list);
void iterator_last(List list);
elem_type iterator_get(List list);
void iterator_set(List list, elem_type value);
void iterator_delete(List list);


List list_create()
{
    List list = (List) malloc(sizeof(*list));
    list->head = NULL;
    list->elements = 0;
    iterator_first(list);
    return list;
}


void iterator_next(List list)
{
    list->iter = list->iter->next;
}

void iterator_prev(List list)
{
    list->iter = list->iter->prev;
}

void iterator_first(List list)
{
    list->iter = list->head;
}

void iterator_last(List list)
{
    iterator_first(list);
    iterator_prev(list);
}

void iterator_set(List list, elem_type value)
{
    list->iter->elem = value;
}

elem_type iterator_get(List list)
{
    return list->iter->elem;
}

void iterator_delete(List list)
{
    if(list->iter == NULL && list_length(list) > 0) {
        printf("Указанной вершины нет в списке\n");
    } else {
        if(list->iter->next == list->iter) {
            free(list->iter);
            list->iter = NULL;
            list->head = NULL;
        } else {
            Node *tmp = list->iter->next;
            if(list->iter == list->head) {
                list->head = tmp;
            }
            list->iter->prev->next = tmp;
            tmp->prev = list->iter->prev;
            free(list->iter);
            list->iter = tmp;
        }
        list->elements--;
    }
}


bool list_is_empty(List list)
{
    return (list->elements > 0 ? true : false);
}


void list_insert(List list, Node *after_node, elem_type value)
{
    Node *new_node = (Node *) malloc(sizeof(Node));
    if(!new_node) {
        fprintf(stderr, "Error: no memory\n");
        exit(1);
    }
    list->iter = after_node;
    new_node->elem = value;
    if(list->iter == NULL && list_length(list) == 0) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->iter = new_node;
        list->head = new_node;
        list->elements++;
    } else if(list->iter == NULL && list_length(list) > 0) {
        free(new_node);
        new_node = NULL;
        printf("Указанной вершины нет в списке\n");
    } else {
        new_node->next = list->iter->next;
        new_node->prev = list->iter;
        list->iter->next->prev = new_node;
        list->iter->next = new_node;
        list->elements++;
    }
}


Node *node_search(List list, elem_type value)
{
    iterator_first(list);
    //printf("check2\n");
    for (uint32_t i = 0; i < list_length(list); ++i) {
        //printf("check\n");
        if(value == iterator_get(list)) {
            //printf("kek\n");
            return list->iter;
        }
        iterator_next(list);
    }
    return NULL;
}

uint32_t list_length(List list)
{
    return list->elements;
}

void list_print(List list)
{
    iterator_first(list);
    for (uint32_t i = 0; i < list_length(list); ++i) {
        printf("(%.2lf + %.2lfi) \n", creal(iterator_get(list)), cimag(iterator_get(list)));
        iterator_next(list);
    }
}

void list_node_delete(List list, Node *del_node)
{
    list->iter = del_node;
    iterator_delete(list);
}


void list_destroy(List *list)
{
    if((*list)->iter != NULL) {
        iterator_first(*list);
        while((*list)->head->next != (*list)->head) {
            iterator_delete(*list);
        }
        iterator_delete(*list);
    }
    free(*list);
    *list = NULL;
}


void delete_last_k_elements(List list, uint32_t k)
{
    if (list_length(list) < k) {
        printf("Длина списка меньше, чем k.\n");
    } else {
        for (uint32_t i = 0; i < k; i++) {
            iterator_last(list);
            iterator_delete(list);
        }
    }
}

