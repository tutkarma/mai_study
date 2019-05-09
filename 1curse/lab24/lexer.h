#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdbool.h>
#include <inttypes.h>

#define TOKEN_SIZE 256
#define ERROR_INPUT 1
#define ERROR_OPERATOR 2

typedef enum {
    INTEGER,
    FLOATING,
    VARIABLE,
    BRACKET,
    OPERATOR,
    FINAL
} TokenType;

typedef struct {
    TokenType type;
    union {
        uint32_t   value_int;
        float      value_float;
        char       variable_name;
        bool       is_left_bracket;
        char       operator_name;
    } data;
} Token; // Именно они будут в узлах дерева выражений

void token_print(Token *t);
void token_next(Token *t); // Считать в *t следующий "кусок" входной строки

#endif