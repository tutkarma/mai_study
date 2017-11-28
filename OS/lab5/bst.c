#include "bst.h"

void TreeInsert(BST *root, ElemType newKey)
{
    if (!(*root)) {
        BST newNode = (BST) malloc(sizeof(*newNode));
        if (!newNode) {
            printf("Error: no memory\n");
            exit(FAILURE);
        }
        newNode->left = newNode->right = NULL;
        newNode->key = newKey;
        *root = newNode;
        return;
    }

    if (newKey <= (*root)->key) {
        TreeInsert(&(*root)->left, newKey);
    } else {
        TreeInsert(&(*root)->right, newKey);
    }
}

BST TreeFind(BST root, ElemType key)
{
    if (!root) {
        return root;
    }

    if (key < root->key) {
        return TreeFind(root->left, key);
    } else if (key > root->key) {
        return TreeFind(root->right, key);
    } else {
        return root;
    }
}

BST minValueNode(BST root)
{
    BST cur = root;
    while (cur->left)
        cur = cur->left;
    return cur;
}

BST TreeRemove(BST root, ElemType key)
{
    if (!root)
        return root;

    if (key < root->key) {
        root->left = TreeRemove(root->left, key);
    } else if (key > root->key) {
        root->right = TreeRemove(root->right, key);
    } else {
        if (!root->left) {
            BST tmp = root->right;
            free(root);
            root = NULL;
            return tmp;
        } else if (!root->right) {
            BST tmp = root->left;
            free(root);
            root = NULL;
            return tmp;
        }

        BST tmp = minValueNode(root->right);
        root->key = tmp->key;
        root->right = TreeRemove(root->right, tmp->key);
    }
    return root;
}

void TreeNodePrint(BST node, int idx)
{
    if (node) {
        TreeNodePrint(node->left, idx + 1);
        for (int j = 0; j < idx; ++j)
            putchar('\t');
        printf("%d\n", node->key);
        TreeNodePrint(node->right, idx + 1);
    }
}

void TreePrint(BST root)
{
    if (root) {
        TreeNodePrint(root, 0);
    } else {
        printf("BST died\n");
    }
}

void TreeDestroy(BST root)
{
    if (root) {
        TreeDestroy(root->right);
        TreeDestroy(root->left);
    }
    free(root);
    root = NULL;
}

bool TreeIsEmpty(BST root)
{
    return !root;
}
