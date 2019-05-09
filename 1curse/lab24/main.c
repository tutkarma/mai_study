#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexer.h"
#include "tree.h"
#include "transform.h"

int main(void)
{
    bool trans = false;

    Token tokens[TOKEN_SIZE];
    uint32_t ind = 0;

    Token token;
    token_next(&token);

    while (token.type != FINAL) {
        tokens[ind++] = token;
        token_next(&token);
    }

    if(ind == 0) {
        fprintf(stderr, "Выражение не введено, преобразование невозможно\n");
        exit(ERROR_INPUT);
    }
    printf("Введите добавляемое выражение:\n");

    Token exprs[TOKEN_SIZE];
    uint32_t ind_expr = 0;

    Token expr;
    token_next(&expr);

    while(expr.type != FINAL) {
        exprs[ind_expr++] = expr;
        token_next(&expr);
    }

    if(ind_expr == 0) {
        fprintf(stderr, "Добавляемое выражение не введено, замена невозможна.\n");
        exit(ERROR_INPUT);
    }

    printf("\nВыражение в линейном представлении:\n");
    for (uint32_t i = 0; i < ind; ++i) {
        token_print(&tokens[i]);
    }

    printf("\nДобавляемое выражение в линейном представлении:\n");
    for (uint32_t i = 0; i < ind_expr; ++i) {
        token_print(&exprs[i]);
    }

    printf("\nДерево выражения до преобразования:\n");
    Tree tree;
    tree = tree_create(&tokens[0], 0, ind - 1);
    tree_print(tree, 0);

    char var;
    printf("\nВведите переменную, которую будем менять:\n");
    scanf("%c", &var);
    if(!variable_is_in_tree(tree, var)) {
        printf("Этой переменной нет в выражении...\n");
    } else {
        tree_transform(tree, var, tree_expr);
        trans = true;
    }
    printf("\nДерево выражения после преобразования:\n");
    //tree_transform(tree, var, tree_expr);
    tree_print(tree, 0);

    printf("\nВыражение в линейном представлении:\n");
    tree_infix(tree);
    printf("\n");

    if(trans) {
        tree_retransform(tree, tree_expr);
    }
    tree_destroy(&tree_expr);
    tree_destroy(&tree);

    return 0;
}