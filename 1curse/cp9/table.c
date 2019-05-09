#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "table.h"
#include "key.h"
#include "data.h"
#include "stack.h"

#define stack_size 1
#define SWAP(A,B) {Note T = A; A = B; B = T; };

struct _note {
    Key key;
    Data data;
};

struct _table {
    Note *note_arr;
    size_t size;
    bool sorted;
};

uint32_t randomAB(int a, int b);

Table table_create(void)
{
    Table table = (Table) malloc(sizeof(*table));
    if(!table) {
        fprintf(stderr, "Error:no memory\n");
        exit(1);
    }
    table->note_arr = NULL;
    table->size = 0;
    table->sorted = false;
    return table;
}

void table_destroy(Table *table)
{
    int max_size = (*table)->size - 1;
    for(int i = 0; i < max_size; ++i) {
        if((*table)->note_arr[i].key)
            //key_destroy(&((*table)->note_arr[i].key));
            free((*table)->note_arr[i].key);
            (*table)->note_arr[i].key = NULL;
        if((*table)->note_arr[i].data)
            //data_destroy(&((*table)->note_arr[i].data));
            free((*table)->note_arr[i].data);
            (*table)->note_arr[i].data = NULL;
    }
    if((*table)->note_arr) {
        free((*table)->note_arr);
        (*table)->note_arr = NULL;
    }
    if(*table) {
        free(*table);
        *table = NULL;
    }
}

void table_push(Table table, uint32_t rows)
{
    for(uint32_t i = 0; i < rows; ++i) {
        Key tmp_key = key_create();
        Data tmp_data = data_create();
        scanf("%s", tmp_key->value);
        scanf("%s", tmp_data->value);
        size_t pos;
        Note *note_found = table_binary_search(table, tmp_key, &pos);
        if (note_found != NULL) {
            key_destroy(&tmp_key);
            data_destroy(&tmp_data);
            printf("Элемент с таким ключом уже существует.\n");
            return;
        }
        table->size++;
        table->note_arr = (Note *) realloc(table->note_arr, table->size * sizeof(Note));
        table->note_arr[table->size - 1].key = tmp_key;
        table->note_arr[table->size - 1].data = tmp_data;
        table->sorted = false; 
    }
}


void table_note_delete(Table table, Key key)
{
    size_t pos;
    Note *note_found = table_binary_search(table, key, &pos);
    if(note_found == NULL) {
        return;
    }
    key_destroy(&(table->note_arr[pos].key));
    data_destroy(&(table->note_arr[pos].data));
    for (uint32_t i = pos; i < table->size - 1; ++i) {
        table->note_arr[i].key = table->note_arr[i + 1].key;
        table->note_arr[i].data = table->note_arr[i + 1].data;
    }
    table->size--;
    table->note_arr = (Note *) realloc(table->note_arr, table->size * sizeof(Note));
}


void table_qsort(Table table)
{
    if(table->size == 0)
        return;

    size_t i, j;

    Key pivot;

    Stack *lows  = stack_create(stack_size);
    Stack *highs = stack_create(stack_size);
    int32_t low, high;

    stack_push(lows,  0);
    stack_push(highs, table->size - 1);

    while (!stack_is_empty(lows)) {
        low  = stack_pop(lows);
        high = stack_pop(highs);
        i = low;
        j = high;
        pivot = table->note_arr[(low + high)/2].key;
        do {
            while (key_cmp(table->note_arr[i].key, pivot) == -1) {
                ++i;
            }
            while (key_cmp(table->note_arr[j].key, pivot) == 1) {
                --j;
            }
            if (i <= j) {
                if (key_cmp(table->note_arr[i].key, table->note_arr[j].key) == 1) {
                    SWAP(table->note_arr[i], table->note_arr[j]);
                }
                ++i;
                if (j > 0) {
                    --j;
                }
            }
        } while (i <= j);

        if (i < high) {
            stack_push(lows, i);
            stack_push(highs, high);
        }
        if (j > low) {
            stack_push(lows, low);
            stack_push(highs, j);
        }
    }

    stack_destroy(&lows);
    stack_destroy(&highs);
    table->sorted = true;
}


Note *table_binary_search(Table table, Key key, size_t *pos)
{
    if (table->size == 0)
        return NULL;

    if (!table_is_sorted(table)) {
        table_qsort(table);
    }
    size_t start = 0;
    size_t end = table->size;
    size_t mid = (start + end) / 2;

    int lg;
    size_t mid_old = -1;

    while ((lg = key_cmp(key, table->note_arr[mid].key)) != 0) {
        if (lg > 0)
            start = mid;
        else
            end = mid;
        mid_old = mid;
        mid = (start + end) / 2;
        if (mid == mid_old)
            return NULL;
    }
    *pos = mid;
    return table->note_arr + mid;
}


void table_print(Table table)
{
    if (table->size == 0) {
        printf("Таблица пуста.\n");
        return;
    }
    for (uint32_t i = 0; i < table->size; ++i) {
        note_print(table->note_arr + i);
    }
    printf("\n");
}

void note_print(Note *note)
{
    key_print(note->key);
    printf(" ");
    data_print(note->data);
    printf("\n");
}

bool table_is_sorted(Table table)
{
    return table->sorted;
}

void table_reverse(Table table)
{
    if(table->size == 0)
        return;

    Note *tmp;
    for(uint32_t i = 0; i <= (table->size - 1) / 2; ++i) {
        *tmp = table->note_arr[i];
        table->note_arr[i] = table->note_arr[table->size - 1 - i];
        table->note_arr[table->size - 1 - i] = *tmp;
    }
    table->sorted = false;
}

void table_scramble(Table table)
{
    if(table->size == 0)
        return;

    srand((uint32_t)time(0));
    for(uint32_t i = 0; i < table->size - 1; ++i) {
        uint32_t j = randomAB(0, table->size - 1);
        uint32_t k = randomAB(0, table->size - 1);
        SWAP(table->note_arr[j], table->note_arr[k]);
    }
}

uint32_t randomAB(int a, int b)
{
    return a + rand() % (b - a + 1);
}

