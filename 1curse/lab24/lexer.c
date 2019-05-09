#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer.h"

void token_next(Token *t)
{
    static bool can_be_unary = true;
    char c;
    
    do { // Избавление от пробельных литер
        c = fgetc(stdin);
    } while (isspace(c));
    
    if (c == EOF) {
        t->type = FINAL;
    }
    
    else if (isalpha(c) || c == '_') { // Переменные
        t->type = VARIABLE;
        t->data.variable_name = c;
        can_be_unary = false;
    }
    
    else if (isdigit(c)) { // Числа
        float result;
        ungetc(c, stdin);
        scanf("%f", &result);
        
        if (result == (uint32_t) result) {
            t->type = INTEGER;
            t->data.value_int = (uint32_t) result;
        } else {
            t->type = FLOATING;
            t->data.value_float = result;
        }
        can_be_unary = false;
    }
    
    else if (c == '(' || c == ')') {
        t->type = BRACKET;
        t->data.is_left_bracket = (c == '(');
        can_be_unary = t->data.is_left_bracket;
    }
    
    else if (can_be_unary && (c == '-' || c == '+')) { // Учёт минуса перед числом
        int m = (c == '+') ? +1 : -1; // Знак
        
        do {
            c = fgetc(stdin);
        } while (isspace(c));
        
        if (isdigit(c)) {
            ungetc(c, stdin);
            token_next(t); // После минуса и т.д. надо число считать
            if (t->type == INTEGER) {
                t->data.value_int *= m;
            } else {
                t->data.value_float *= m;
            }
        } else {
            ungetc(c, stdin);
            t->type = OPERATOR;
            t->data.operator_name = '-';
            can_be_unary = true;
        }
    }
    
    else if (c == '-' || c == '+' || c == '/' || c == '*' || c = '^') {
        t->type = OPERATOR;
        t->data.operator_name = c;
        can_be_unary = true;
    } else {
            fprintf(stderr, "\n");
            exit(ERROR_OPERATOR);
    }
}

void token_print(Token *t)
{
    switch (t->type) {
        case FINAL:
            break;
        case INTEGER:
            printf("%u", t->data.value_int);
            break;
        case FLOATING:
            printf("%lf", t->data.value_float);
            break;
        case VARIABLE:
            printf("%c", t->data.variable_name);
            break;
        case OPERATOR:
            printf("%c", t->data.operator_name);
            break;
        case BRACKET:
            printf("%c", (t->data.is_left_bracket) ? '(' : ')');
            break;
    }
}