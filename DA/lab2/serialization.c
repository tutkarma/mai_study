#include <string.h>

#include "TPatricia.h"
#include "serialization.h"


int DictWriteRec(FILE *f, TPatricia tree, TPatriciaNode node)
{
    size_t l = strlen(node->key);
    if (fprintf(f, "%lu %d %s %lu ", l, node->skip, node->key, node->value) < 0)
        return -1;
    if ((node->left->skip >= node->skip) &&
        (node->left != node) &&
        (node->left != tree->root))
        DictWriteRec(f, tree, node->left);

    if ((node->right->skip >= node->skip) &&
        (node->right != node) &&
        (node->right != tree->root))
        DictWriteRec(f, tree, node->right);

    return 0;
}

int DictWrite(FILE *f, TPatricia tree)
{
    char *magic = "DICT ";
    if (fprintf(f, "%s", magic) < 0) {
        return -1;
    }
    size_t kek = 6;
    if (tree->root->right == tree->root) {
        if (fprintf(f, "%lu -1 empty 0", kek) < 0)
            return -1;
        return 0;
    }
    return DictWriteRec(f, tree, tree->root->right);
}


int DictReadRec(FILE *f, TPatricia tree)
{
    size_t l;
    int32_t skip;
    TValue val;
    char *key;
    if (fscanf(f, "%lu", &l) != 1) {
        return -2;
    }
    key = (char *) malloc(l + 1);
    if (key == NULL)
        return -2;
    if (fscanf(f, "%d %s %lu", &skip, key, &val) != 3) {
        return -2;
    }

    if ((skip == -1) && (!strcmp(key, "empty")) && (val == 0))
        return 0;

    tree->root->right = NodeCreate();
    NodeFill(tree->root->right, skip, key, val, tree->root, tree->root->right);

    TPatriciaNode parent = tree->root;
    TPatriciaNode child = parent->left;


    while(fscanf(f, "%lu", &l) == 1) {
        key = (char *) malloc(l + 1);
        if (key == NULL)
            return -2;
        if (fscanf(f, "%d %s %lu ", &skip, key, &val) != 3)
            return -2;
        while((parent->skip < child->skip) && (child->skip < skip)) {
            parent = child;
            child = GetBit(key, child->skip) ? child->right : child->left;
        }

        TPatriciaNode temp = NodeCreate();
        NodeFill(temp, skip, key, val, GetBit(key, skip) ? child : temp, GetBit(key, skip) ? temp : child);

        if (GetBit(key, parent->skip)) {
            parent->right = temp;
        } else {
            parent->left = temp;
        }

        parent = tree->root;
        child = parent->right;
    }
    return 0;
}

int DictRead(FILE *f, TPatricia tree)
{
    char magic[4];
    if (fscanf(f, "%s", magic) != 1 || strncmp(magic, "DICT", 4) != 0) {
        return -2;
    }
    int err = DictReadRec(f, tree);
    if (err)
        return err;
    return 0;
}