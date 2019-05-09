#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"

#define DEEP 0
#define max(x,y) ((x) > (y) ? (x) : (y))


struct _tree {
    TreeItem value;
    Tree child;
    Tree sibling;
}; //*Tree

Tree tree_create(TreeItem value)
{
    Tree tree = (Tree) malloc(sizeof(*tree));
    if(!tree) {
        fprintf(stderr, "%s\n", "Error: no memory");
        exit(1);
    }
    tree->value = value;
    tree->child = NULL;
    tree->sibling = NULL;
    
    return tree;
}

void tree_add_node(Tree tree, TreeItem parent, TreeItem value)
{
    Tree parent_node = tree_find(tree, parent);
    if(parent_node) {
        if(!parent_node->child) {
            parent_node->child = tree_create(value);
        } else {
            parent_node = parent_node->child;
            while(parent_node->sibling) {
                parent_node = parent_node->sibling;
            }
            parent_node->sibling = tree_create(value);
        }
    } else {
        printf("Указанная родительская вершина не найдена.\n");
    }
}

Tree tree_find(Tree tree, TreeItem c)
{
    if(!tree) {
        return NULL;
    }

    if(tree->value == c) {
        return tree;
    }

    Tree result = NULL;
    if(tree->child) {
        result = tree_find(tree->child, c);
        if(result) return result;
    }

    if(tree->sibling) {
        result = tree_find(tree->sibling, c);
        if(result) return result;
    }

}


void tree_print_node(Tree tree, int indent)
{
    for(int i = 0; i < indent; ++i) {
        printf("\t");
    }
    printf("%d\n", tree->value);
    if(tree->child) {
        tree_print_node(tree->child, indent + 1);
    }
    if(tree->sibling) {
        tree_print_node(tree->sibling, indent);
    }
}

void tree_print(Tree tree)
{
    tree_print_node(tree, 0);
}

void tree_destroy(Tree tree)
{

    if(tree->child) {
        tree_destroy(tree->child);
    }
    if(tree->sibling) {
        tree_destroy(tree->sibling);
    }
    free(tree);
    tree = NULL;
}


void tree_del_node(Tree tree, TreeItem value)
{
    if(tree->child) {
        if(tree->child->value == value) {
            Tree tmp = tree->child;
            tree->child = tree->child->sibling;
            if (tmp->child) {
                tree_destroy(tmp->child);
            }
            free(tmp);
            tmp = NULL;
            return;
        } else {
            tree_del_node(tree->child, value);
        }
    }


    if(tree->sibling) {
        if(tree->sibling->value == value) {
            Tree tmp = tree->sibling;
            tree->sibling = tree->sibling->sibling;
            if(tmp->child) {
                tree_destroy(tmp->child);
            }
            free(tmp);
            tmp = NULL;
            return;
        } else {
            tree_del_node(tree->sibling, value);
        }
    }
}

int max_level(Tree tree, int deep)
{
    if(!tree) return deep - 1;
    return max(max_level(tree->child, deep + 1), max_level(tree->sibling, deep));
}

void couting_nodes_on_the_lvls(Tree tree, int level, int *mat)
{
    mat[level] += 1;
    if(tree->child) {
        couting_nodes_on_the_lvls(tree->child, level + 1, mat);
    }
    if(tree->sibling) {
        couting_nodes_on_the_lvls(tree->sibling, level, mat);
    }
}

void check_monotonicity_of_decreaset(Tree tree)
{
    bool more_one_lvl = false;
    bool decreasing = true;
    if(tree->child == NULL) {
        printf("Дерево содержит только корень, этого недостаточно для определения монотонности.\n");
    } else {
        if(tree->child->child) {
            more_one_lvl = true;
        }
        for(Tree tmp = tree->child; tmp->sibling; tmp = tmp->sibling) {
            if(tmp->child != NULL) {
                more_one_lvl = true;
                break;
            }
        }
        if(more_one_lvl) {
            int deep = max_level(tree, DEEP);
            int level = 0;
            int mat[deep];
            for(int i = 0; i <= deep; ++i) {
                mat[i] = 0;
            }
            couting_nodes_on_the_lvls(tree, level, mat);
            for(int i = 1; i < deep; ++i) {
                if(mat[i] <= mat[i + 1]) {
                    printf("Дерево не убывает.\n");
                    decreasing = false;
                    break;
                }
            }
            if(decreasing) printf("Дерево убывает.\n");
        } else printf("Дерево содержит только 1 уровень, этого недостаточно для определения монотонности\n");
    }
}