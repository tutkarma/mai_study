#include "TPatricia.h"

void NodeDebugPrint(TPatricia tree, TPatriciaNode node, int32_t idx);
int KeysIsEqual(TKey key1, TKey key2);
int KeysCopy(TPatriciaNode tmp, TPatriciaNode parent);

const static int BITS = 8;


TPatricia TreeCreate(void)
{
    TPatricia tree = (TPatricia) malloc(sizeof(*tree));
    if (!tree) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(EXIT_SUCCESS);
    }
    tree->root = NodeCreate();
    return tree;
}

TPatriciaNode NodeCreate(void)
{
    TPatriciaNode node = (TPatriciaNode) malloc(sizeof(*node));
    if (!node) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(EXIT_SUCCESS);
    }
    node->skip = -1;
    node->key = NULL;
    node->value = 0;
    node->left = node;
    node->right = node;
    return node;
}

void NodeFill(TPatriciaNode node, int32_t skip, TKey key, TValue val, TPatriciaNode left, TPatriciaNode right)
{
    //заполнение полей новосозданного узла
    node->skip = skip;
    node->key = key;
    node->value = val;
    node->left = left;
    node->right = right;
}

int32_t GetBit(TKey key, int32_t idx)
{
    if (idx < 0) {
        return -1;
    }
    uint32_t byteNum = idx / BITS;
    uint8_t bitNum = idx % BITS;
    uint8_t sym = key[byteNum];
    uint8_t bit = (sym >> bitNum) % 2;

    return bit;
}

void TreeDebugPrint(TPatricia tree)
{
    NodeDebugPrint(tree, tree->root, 0);
}

void NodeDebugPrint(TPatricia tree, TPatriciaNode node, int32_t idx)
{
    if ((node->left->skip >= node->skip) && 
        (node->left != node) && 
        (node->left != tree->root))
        NodeDebugPrint(tree, node->left, idx + 1);
    for (int j = 0; j < idx; ++j)
        putchar('\t');
    printf("%s, %d | left is %s right is %s\n", node->key, node->skip, node->left->key, node->right->key);
    if ((node->right->skip >= node->skip) && 
        (node->right != node) && (node->right != tree->root))
        NodeDebugPrint(tree, node->right, idx + 1);
}

void NodeDestroy(TPatriciaNode node, TPatricia *tree)
{
    //рекурсивное удаление дерева
    if (!node)
        return;
    if ((node->left->skip >= node->skip) &&
      (node->left != node) &&
      (node->right != (*tree)->root))
        NodeDestroy(node->left, tree);
    if ((node->right->skip >= node->skip) &&
      (node->right != node) &&
      (node->right != (*tree)->root))
        NodeDestroy(node->right, tree);

    free(node->key);
    node->key = NULL;
    free(node);
    node = NULL;
}

void TreeDestroy(TPatricia *tree)
{
    NodeDestroy((*tree)->root, tree);
    //(*tree)->root = NULL;
    free(*tree);
    *tree = NULL;
}


TPatriciaNode NodeSearch(TPatricia tree, TKey s)
{
    //поиск по ключу
    TPatriciaNode parent = tree->root;
    TPatriciaNode child = parent->right;

    while (parent->skip < child->skip) {
        parent = child;
        child = GetBit(s, child->skip) ? child->right : child->left;
    }

    return (KeysIsEqual(s, child->key) ? child : NULL);
}


int TreeInsert(TPatricia tree, TKey key, TValue val)
{
    TPatriciaNode parent = tree->root;
    TPatriciaNode child = parent->right;

    while (parent->skip < child->skip) {
        parent = child;
        child = GetBit(key, child->skip) ? child->right : child->left;
    }

    if (KeysIsEqual(key, child->key)) {
        return -1; // запись существует
    }

    int32_t resBit = 0;
    for (;child->key != NULL && key[resBit] == child->key[resBit]; ++resBit);
    resBit *= BITS;
    for (;child->key != NULL && GetBit(key, resBit) == GetBit(child->key, resBit); ++resBit);


    parent = tree->root;
    
    TPatriciaNode childTmp = parent->right;
    while ((parent->skip < childTmp->skip) && (childTmp->skip < resBit)) {
        parent = childTmp;
        childTmp = GetBit(key, childTmp->skip) ? childTmp->right : childTmp->left;
    }


    child = NodeCreate();
    NodeFill(child, resBit, key, val, GetBit(key, resBit) ? childTmp : child, GetBit(key, resBit) ? child : childTmp);
    if (GetBit(key, parent->skip)) {
        parent->right = child;
    } else {
        parent->left = child;
    }
    return 0;
}


int TreeDelete(TPatricia tree, TKey key)
{
    //удаление вершины по ключу
    TPatriciaNode grandpa;
    TPatriciaNode parent = tree->root;
    TPatriciaNode tmp = parent->right;

    while (parent->skip < tmp->skip) {
        grandpa = parent;
        parent = tmp;
        tmp = GetBit(key, tmp->skip) ? tmp->right : tmp->left;
    }
    if (!KeysIsEqual(tmp->key, key)) {
        return -1;
    }

    if (tmp != parent) {
        //strcpy(tmp->key, parent->key);
        KeysCopy(tmp, parent);
        tmp->value = parent->value;
    }


    if ((parent->left->skip > parent->skip) || (parent->right->skip > parent->skip)) {
        if (parent != tmp) {
            TPatriciaNode leaf = parent;
            TPatriciaNode parentTmp = GetBit(parent->key, parent->skip) ? parent->right : parent->left;

            while (leaf->skip < parentTmp->skip) {
                leaf = parentTmp;
                parentTmp = GetBit(parent->key, parentTmp->skip) ? parentTmp->right : parentTmp->left; 
            }

            if (!KeysIsEqual(parent->key, parentTmp->key)) {
                return -1;
            }

            if (GetBit(parent->key, leaf->skip)) {
                leaf->right = tmp;
            } else {
                leaf->left = tmp;
            }
        }

        if (grandpa != parent) {
            TPatriciaNode child = GetBit(key, parent->skip) ? parent->left : parent->right;

            if (GetBit(key, grandpa->skip)) {
                grandpa->right = child;
            } else {
                grandpa->left = child;
            }
        }
    } else {
        if (grandpa != parent) {
            TPatriciaNode item = ((parent->left == parent->right) && (parent->left == parent)) ? grandpa : ((parent->left == parent) ? parent->right : parent->left);

            if (GetBit(key, grandpa->skip)) {
                grandpa->right = item;
            } else {
                grandpa->left = item;
            }
        }
    }

    if (parent->key) {
        free(parent->key);
        parent->key = NULL;
    }
    if (parent) {
        free(parent);
        parent = NULL;
    }
    return 0;
}

int KeysCopy(TPatriciaNode tmp, TPatriciaNode parent)
{
    if (parent->key == NULL) {
        tmp->key = NULL;
        return 0;
    }
    if (strlen(parent->key) > strlen(tmp->key)) {
        tmp->key = realloc(tmp->key, strlen(parent->key) + 1);
    }
    strcpy(tmp->key, parent->key);
    return 0;
}

int KeysIsEqual(TKey key1, TKey key2)
{
    if (key1 == NULL || key2 == NULL)
        return 0;
    return !strcmp(key1, key2);
}