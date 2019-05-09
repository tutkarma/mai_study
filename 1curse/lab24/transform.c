#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "tree.h"
#include "transform.h"


uint32_t variable_match(Tree t, char c)
{
    return (t != NULL) && (t->node.type == VARIABLE) && (t->node.data.variable_name == c);
}

void tree_transform(Tree t, char c, Tree expr)
{
    if (t != NULL) {
        tree_transform(t->left, c, expr);
        tree_transform(t->right, c, expr);

        if(t->right) {
            if(variable_match(t->right, c)) {
                free(t->right);
                t->right = NULL;
                t->right = expr;
            }
        }
        if(t->left) {
            if(variable_match(t->left, c)) {
                free(t->left);
                t->left = NULL;
                t->left = expr;
            }
        }
    }
}

uint32_t variable_is_in_tree(Tree t, char c)
{
    if(!t)
        return 0;
    if(variable_match(t, c))
        return 1;
    if(!variable_is_in_tree(t->left, c))
        return variable_is_in_tree(t->right, c);
}

void tree_retransform(Tree t, Tree expr)
{
    if (t != NULL) {
        tree_retransform(t->left, expr);
        tree_retransform(t->right, expr);

        if(t->right == expr)
            t->right = NULL;
        if(t->left == expr)
            t->left == NULL;
    }
}