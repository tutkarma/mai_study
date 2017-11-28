#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

int main(void)
{
    int act = 0;
    ElemType key = 0;
    BST tree = NULL;
    printf("This is compile-time linking\n\n");
    printf("Choose an operation:\n");
    printf("1) Add key\n");
    printf("2) Remove key\n");
    printf("3) Find key\n");
    printf("4) Print tree\n");
    printf("0) Exit\n");
    while (scanf("%d", &act) && act) {
        switch(act) {
        case 1:
            printf("Enter key: ");
            scanf("%d", &key);
            TreeInsert(&tree, key);
            break;
        case 2:
            printf("Enter key: ");
            scanf("%d", &key);
            if (TreeFind(tree, key)) {
                tree = TreeRemove(tree, key);
            } else {
                printf("This key doesn't exist\n");
            }
            break;
        case 3:
            printf("Enter key: ");
            scanf("%d", &key);
            if (TreeFind(tree, key)) {
                printf("Key found\n");
            } else {
                printf("Key not found\n");
            }
            break;
        case 4:
            if (tree) {
                printf("\n");
                TreePrint(tree);
                printf("\n");
            } else {
                printf("Tree is empty\n");
            }
            break;
        default:
            printf("Incorrect command\n");
            break;
        }
        printf("Choose an operation:\n");
        printf("1) Add key\n");
        printf("2) Remove key\n");
        printf("3) Find key\n");
        printf("4) Print tree\n");
        printf("0) Exit\n");
    }
    TreeDestroy(tree);
    return SUCCESS;
}