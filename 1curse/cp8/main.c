#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "list.h"

uint32_t main(void)
{
    double x_val = 0.0, y_val = 0.0;
    double x_node = 0.0, y_node = 0.0;

    //elem_type val = 0;
    //elem_type node = 0;
    uint32_t k = 0;
    List list = list_create();

    char s[8];

    printf("\nДля получения помощи в использовании программы напишите 'help' или 'h'.\n\n");
    while (1) {
        scanf("%7s", s);
        if (!strcmp(s, "insert") || !strcmp(s, "ins")) {
            if(list == NULL) {
                list = list_create();
            }
            scanf("%lf %lf %lf %lf", &x_node, &y_node, &x_val, &y_val);
            elem_type node = x_node + y_node * I;
            elem_type val = x_val + y_val * I;
            Node *after_node = NULL;
            after_node = node_search(list, node);
            //if(after_node != NULL) printf("lol\n");
            list_insert(list, after_node, val);
    
        } else if (!strcmp(s, "delete") || !strcmp(s, "del")) {
            if(list == NULL) printf("Списка не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else if (list_length(list) == 0) {
                printf("Список пуст.\n");
            } else {
                scanf("%lf %lf", &x_node, &y_node);
                elem_type node = x_node + y_node * I;
                Node *after_node = NULL;
                after_node = node_search(list, node);
                list_node_delete(list, after_node);
            
            }
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            if (list != NULL) list_destroy(&list);
            break;
        } else if (!strcmp(s, "run") || !strcmp(s, "r")) {
            if(list == NULL) printf("Списка не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                scanf("%d", &k);
                delete_last_k_elements(list, k);
            }
        } else if (!strcmp(s, "print") || !strcmp(s, "p")) {
            if (list == NULL) printf("Списка не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                printf("\n");
                list_print(list);
                printf("\n");
            }
        } else if (!strcmp(s, "length") || !strcmp(s, "len")) {
            if (list == NULL) printf("Списка не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                printf("%d\n", list_length(list));
            }
        } else if (!strcmp(s, "destroy") || !strcmp(s, "des")) {
            if (list == NULL) printf("Спиcка не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                list_destroy(&list);
            }
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            printf("\n\nКоманды 'insert after_node new_node' и 'ins after_node new_node' добавляют значения в список, где after_node -- значение, после которого вставляем, new_node -- вставляемое значение.\n\n");
            printf("\n\nКоманды 'print' и 'p' печатают список.\n\n");
            printf("\n\nКоманды 'run k' и 'r k' удаляют последние k записей, если k больше длины списка, то список остается без изменений.\n\n");
            printf("\n\nКоманды 'length' и 'len' возвращают длину списка.\n\n");
            printf("\n\nКоманды 'delete' и 'del' удаляют вершину из списка.\n\n");
            printf("\n\nКоманды 'destroy' и 'des' удаляют список.\n\n");
            printf("\n\nКоманды 'quit' и 'q' выходят из программы.\n\n");
        }
    }
    return 0;
}