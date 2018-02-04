#ifndef TPATRICIA_H
#define TPATRICIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef char *TKey;
typedef uint64_t TValue;

typedef struct patriciaNode *TPatriciaNode;

struct patriciaNode {
    int32_t skip;
    TKey key;
    TValue value;
    TPatriciaNode left;
    TPatriciaNode right;
};

typedef struct {
    TPatriciaNode root;
} *TPatricia;


TPatricia TreeCreate(void);
TPatriciaNode NodeCreate(void);
void NodeFill(TPatriciaNode node, int32_t skip, TKey key, TValue val, TPatriciaNode left, TPatriciaNode right);
void TreeDebugPrint(TPatricia tree);
int32_t GetBit(TKey key, int32_t idx);
TPatriciaNode NodeSearch(TPatricia tree, TKey key);
int TreeInsert(TPatricia tree, TKey key, TValue val);
int TreeDelete(TPatricia tree, TKey key);
void NodeDestroy(TPatriciaNode node, TPatricia *tree);
void TreeDestroy(TPatricia *tree);


#endif