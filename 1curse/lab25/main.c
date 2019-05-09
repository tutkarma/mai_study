#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"

#define stack_size 1

void stack_print(Stack *stack)
{
    Stack *helper_stack = stack_create(stack_size);
    while(!stack_is_empty(stack)) {
        printf("%d\n", stack_top(stack));
        stack_push(helper_stack, stack_pop(stack));
    }
    while(!stack_is_empty(helper_stack)) {
        stack_push(stack, stack_pop(helper_stack));
    }
    stack_destroy(&helper_stack);
}

void search_and_swap(Stack *orig, Stack *helper)
{
    bool swap = false;
    Item first_elem = stack_pop(orig);
    Item second_elem = stack_top(helper);
    while(first_elem > second_elem && !stack_is_empty(helper)) {
        second_elem = stack_pop(helper);
        stack_push(orig, second_elem);
        second_elem = stack_top(helper);
        swap = true;
    }
    if(swap) {
        stack_push(helper, first_elem);
        swap = false;
    }
}


void bubble_sort(Stack *orig_stack)
{
    Stack *helper_stack = stack_create(stack_size);
    while(!stack_is_empty(orig_stack)) {
        Item elem = stack_pop(orig_stack);
        if(stack_is_empty(helper_stack) || (stack_top(helper_stack) >= elem)) {
            stack_push(helper_stack, elem);
        } else {
            stack_push(orig_stack, elem);
            search_and_swap(orig_stack, helper_stack);
    }
    while(!stack_is_empty(helper_stack)) {
        stack_push(orig_stack, stack_pop(helper_stack));
    }
    stack_destroy(&helper_stack);
}


int main(int argc, char **argv)
{
    char s[8];

    Stack *stack = stack_create(stack_size);

    Item elem = 0;

    printf("\nДля получения помощи в использовании программы напишите 'help' или 'h'.\n\n");
    while (1) {
        scanf("%7s", s);
        if (!strcmp(s, "insert") || !strcmp(s, "ins")) {
            if(stack == NULL) {
                stack = stack_create(stack_size);
            }
            while (scanf("%d", &elem)) {
                stack_push(stack, elem);
            }
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            if(stack == NULL) {
                break;
            }
            stack_destroy(&stack);
            break;
        } else if (!strcmp(s, "run") || !strcmp(s, "r")) {
            if(stack == NULL) {
                printf("Стека не существует.\n");
            } else if (stack_is_empty(stack)) {
                printf("Стек пуст.\n");
            } else {
                bubble_sort(stack);
            }
        } else if (!strcmp(s, "print") || !strcmp(s, "p")) {
            if(stack == NULL) {
                printf("Стека не существует.\n");
            } else if (stack_is_empty(stack)) {
                printf("Стек пуст.\n");
            } else {
                stack_print(stack);
            }
        } else if (!strcmp(s, "destroy") || !strcmp(s, "des")) {
            stack_destroy(&stack);
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            printf("\n\nКоманды 'insert num1 num2...' и 'ins num1 num2...' добавляют элементы в стек.\n\n");
            printf("Команды 'print' и 'p' печатают стек.\n\n");
            printf("Команды 'run' и 'r' сортируют элементы стека.\n\n");
            printf("Команды 'destroy' и 'des' удаляют стек.\n\n");
            printf("Команды 'quit', 'q' и 'exit' заканчивают работу программы.\n\n");
        } else {
            printf("\n\nТакой команды не существует, воспользуйтесь командами 'help' или 'h'. \n\n");
        }
    }

    return 0;
}
