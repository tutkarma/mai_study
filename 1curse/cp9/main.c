#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "data.h"
#include "key.h"
#include "table.h"

uint32_t main(void)
{
    uint32_t rows = 0;
    size_t pos;
    Table table = table_create();
    Key key = key_create();

    char s[9];
    unsigned int search_time;


    printf("\nДля получения помощи в использовании программы напишите 'help' или 'h'.\n\n");
    while (1) {
        scanf("%8s", s);
        if (!strcmp(s, "insert") || !strcmp(s, "ins")) {
            if(table == NULL)
                table = table_create();
            printf("Введите количество строк:\n");
            scanf("%u", &rows);
            table_push(table, rows);
        } else if (!strcmp(s, "delete") || !strcmp(s, "del")) {
            if(table == NULL) printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                key_insert(key);
                table_note_delete(table, key);
            }
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            if (table != NULL) table_destroy(&table);
            if (key != NULL) key_destroy(&key);
            break;
        } else if (!strcmp(s, "run") || !strcmp(s, "r")) {
            if(table == NULL)
                printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else{
                unsigned int start_time = clock();
                table_qsort(table);
                unsigned int end_time = clock();
                search_time = end_time - start_time;
                //printf("%u\n", search_time);
            }
        } else if (!strcmp(s, "print") || !strcmp(s, "p")) {
            if (table == NULL)
                printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else
                table_print(table);
        } else if (!strcmp(s, "find") || !strcmp(s, "f")) {
            if (table == NULL) printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                key_insert(key);
                Note *note_found = table_binary_search(table, key, &pos);
                if (note_found == NULL) {
                    printf("Записи с таким ключом нет в таблице.\n");
                } else {
                    note_print(note_found);
                }
            }
        } else if (!strcmp(s, "destroy") || !strcmp(s, "des")) {
            if (table == NULL) printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                table_destroy(&table);
            }
        } else if (!strcmp(s, "reverse") || !strcmp(s, "rev")) {
            if (table == NULL) printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                table_reverse(table);
            }
        } else if (!strcmp(s, "scramble") || !strcmp(s, "scr")) {
            if (table == NULL) printf("Таблицы не существует, воспользуйтесь командами 'help' или 'h'.\n");
            else {
                table_scramble(table);
            }
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            printf("\n\nКоманды 'insert' и 'ins' добавляют запись в таблицу.\n");
            printf("\nКоманды 'print' и 'p' печатают таблицу.\n");
            printf("\nКоманды 'run' и 'r' сортируют таблицу, используя нерекурсивную сортировку Хоара.\n");
            printf("\nКоманды 'find <key>' и 'f <key>' находят элемент по ключу.\n");
            printf("\nКоманды 'delete <key>' и 'del <key>' удаляют запись из таблицы.\n");
            printf("\nКоманды 'reverse' и 'rev' реверсят таблицу.\n");
            printf("\nКоманды 'scramble' и 'scr' перемешивают записи таблицы.\n");
            printf("\nКоманды 'destroy' и 'des' удаляют таблицу.\n");
            printf("\nКоманды 'quit' и 'q' выходят из программы.\n");
        }
    }
    printf("%u\n", search_time);
    return 0;
}