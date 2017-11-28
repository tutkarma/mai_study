#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "bst.h"

int main(void)
{
    void (*TreeInsert)(BST *root, ElemType newKey);
    BST (*TreeFind)(BST root, ElemType key);
    BST (*TreeRemove)(BST root, ElemType key);
    void (*TreePrint)(BST root);
    void (*TreeDestroy)(BST root);
    char *err;

    void *libHandle;
    libHandle = dlopen("libbst.so", RTLD_LAZY);
    if (!libHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(FAILURE);
    }

    TreeInsert = dlsym(libHandle, "TreeInsert");
    TreeRemove = dlsym(libHandle, "TreeRemove");
    TreeFind = dlsym(libHandle, "TreeFind");
    TreePrint = dlsym(libHandle, "TreePrint");
    TreeDestroy = dlsym(libHandle, "TreeDestroy");

    if(err = dlerror()) {
        fprintf(stderr, "%s\n", err);
        exit(FAILURE);
    }

    int act = 0;
    ElemType key = 0;
    BST tree = NULL;
    printf("This is runtime linking\n\n");
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
            (*TreeInsert)(&tree, key);
            break;
        case 2:
            printf("Enter key: ");
            scanf("%d", &key);
            if ((*TreeFind)(tree, key)) {
                tree = (*TreeRemove)(tree, key);
            } else {
                printf("This key doesn't exist\n");
            }
            break;
        case 3:
            printf("Enter key: ");
            scanf("%d", &key);
            if ((*TreeFind)(tree, key)) {
                printf("Key found\n");
            } else {
                printf("Key not found\n");
            }
            break;
        case 4:
            if (tree) {
                printf("\n");
                (*TreePrint)(tree);
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
    (*TreeDestroy)(tree);
    dlclose(libHandle);
    return SUCCESS;
}