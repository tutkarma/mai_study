#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"


int main(void)
{
    char s[8];

    Tree tree = NULL;
    int root = 0, ver = 0, parent = 0;

    printf("\nДля получения помощи в использовании программы напишите 'help' или 'h'.\n\n");
    while (1) {
        scanf("%7s", s);
        if (!strcmp(s, "insert") || !strcmp(s, "ins")) {
            if(!tree) {
                printf("Введите значение корня дерева:\n");
                scanf("%d", &root);
                tree = tree_create(root);
            }
            while (scanf("%d%d", &parent, &ver)) {
                tree_add_node(tree, parent, ver);
            }
        } else if (!strcmp(s, "delete") || !strcmp(s, "del")) {
            if(!tree) printf("Дерева не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                scanf("%d", &ver);
                tree_del_node(tree, ver);
            }
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            if (tree) tree_destroy(tree);
            break;
        } else if (!strcmp(s, "run") || !strcmp(s, "r")) {
            if(!tree) printf("Дерева не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                check_monotonicity_of_decrease(tree);
            }
        } else if (!strcmp(s, "print") || !strcmp(s, "p")) {
            if (!tree) printf("Дерева не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                printf("\n\n");
                tree_print(tree);
                printf("\n\n");
            }
        } else if (!strcmp(s, "destroy") || !strcmp(s, "des")) {
            if (!tree) printf("Дерева не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                tree_destroy(tree);
                tree = NULL;
            }
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            printf("\n\nКоманды 'insert' и 'ins', если дерево не создано -- создают дерево, если создано -- добавляют вершины в дерево.\n\n");
            printf("Команды 'delete num' и 'del num' удаляют вершину и всех ее детей.\n\n");
            printf("Команды 'print' и 'p' печатают вершины дерева.\n\n");
            printf("Команды 'run' и 'r' проверяют монотонность убывания ширины уровня дерева.\n\n");
            printf("Команды 'quit', 'q' и 'exit' заканчивают работу программы.\n\n");
            printf("Команды 'destroy' и 'des' удаляют все дерево.\n\n");
        } else {
            printf("\n\nТакой команды не существует, воспользуйтесь командами 'help' или 'h'. \n\n");
        }
    }
    return 0;
}