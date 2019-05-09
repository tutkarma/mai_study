#ifndef _LIST_H_
#define _LIST_H_

#include <stdbool.h>
#include <inttypes.h>
#include <complex.h>

typedef complex double elem_type;
//typedef int elem_type;

typedef struct _node Node;
typedef struct _list *List;

List list_create();

bool list_is_empty(List list);
void list_insert(List list, Node *after_node, elem_type elem);
uint32_t list_length(List list);
void list_print(List list);
void list_node_delete(List list, Node *del_node);
void list_destroy(List *list);
Node *node_search(List list, elem_type value);

void delete_last_k_elements(List list, uint32_t k);


#endif