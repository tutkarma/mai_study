#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "tree.h"

void tree_transform(Tree t, char c, Tree expr);
uint32_t variable_match(Tree t, char c);
uint32_t variable_is_in_tree(Tree t, char c);
void tree_retransform(Tree t, Tree expr);

#endif