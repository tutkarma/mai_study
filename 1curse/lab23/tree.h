#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int TreeItem;
typedef struct _tree *Tree;

Tree tree_create(TreeItem value);
void tree_add_node(Tree tree, TreeItem parent, TreeItem value);
void tree_print(Tree tree);
void tree_print_node(Tree tree, int indent);
void tree_destroy(Tree tree);
void tree_del_node(Tree tree, TreeItem value);
Tree tree_find(Tree tree, TreeItem c);

int max_level(Tree tree, int deep);
void counting_nodes_on_the_lvls(Tree tree, int level, int *mat);
void check_monotonicity_of_decreaset(Tree tree);

#endif // _TREE_H_