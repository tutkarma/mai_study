#ifndef _BST_H_
#define _BST_H_

#define SUCCESS 0
#define FAILURE 1

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef int32_t ElemType;

typedef struct _bst {
    struct _bst *left;
    struct _bst *right;
    ElemType key;
} *BST;

extern void TreeInsert(BST *root, ElemType newKey);
extern BST TreeFind(BST root, ElemType key);
extern BST TreeRemove(BST root, ElemType key);
extern void TreePrint(BST root);
extern void TreeDestroy(BST root);
extern bool TreeIsEmpty(BST root);

#endif /* _BST_H */