#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "stack.h"

struct _stack {
    int ptr;
    Item *data;
}; //Stack


Stack *stack_create(size_t stack_size)
{
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if(!s) {
        fprintf(stderr, "%s\n", "Error: no memory");
        exit(MEMORY_LIMIT);
    }

    s->data = NULL;
    s->ptr  = 0;

    return s;
}

void stack_push(Stack *s, Item value)
{
    s->data = (Item *) realloc(s->data, s->ptr * sizeof(Item) + sizeof(Item));
    if(!s->data) {
        fprintf(stderr, "%s\n", "Error: no memory\n");
        exit(MEMORY_LIMIT);
    }
    s->data[s->ptr++] = value;
}

Item stack_pop(Stack *s)
{
    if(stack_is_empty(s)) {
        fprintf(stderr, "%s\n", "Error: stack underflow\n");
        exit(STACK_UNDERFLOW);
    } else {
        return s->data[--s->ptr];
    }
}

bool stack_is_empty(Stack *s)
{
    return (s->ptr <= 0);
}

void stack_destroy(Stack **s)
{
    free((*s)->data);
    (*s)->data = NULL;
    (*s)->ptr = 0;
    free(*s);
    *s == NULL;
}

Item stack_top(Stack *s)
{
    if(s->ptr <= 0 || s->data == NULL) {
        return 0;
    } else {
        return s->data[(s->ptr) - 1];
    }
}
