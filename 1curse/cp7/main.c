#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

#include "matrix.h"

int main(void)
{
    int n, m;
    double x = 0.0, y = 0.0;
    printf("Enter the size of the input matrix:\n");
    scanf("%d %d", &n, &m);
    Matrix mat = matrix_create(n, m);
    Matrix column = matrix_create(m, 1);

    printf("Enter the matrix:\n");
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            scanf("%lf %lf", &x, &y);
            elem_type elem = x + y * I;
            elem_set(mat, i, j, elem);
        }
    }
    printf("Enter the matrix-сolumn:\n");
    for (int i = 1; i <= m; ++i) {
        scanf("%lf %lf", &x, &y);
        elem_type elem = x + y * I;
        elem_set(column, i, 1, elem);
    }

    Matrix mult = matrix_create(n, 1);
    printf("Column:\n");
    matrix_print_outside(column);
    printf("Internal representation:\n");
    matrix_print_inside(column);
    printf("Matrix:\n");
    matrix_print_outside(mat);
    printf("Internal representation:\n");
    matrix_print_inside(mat);
    matrix_multiply(mat, column, mult);
    printf("Result:\n");
    matrix_print_outside(mult);
    printf("Internal representation:\n");
    matrix_print_inside(mult);
    printf("Nonzero elements: %d\n", nonzero(mult));
    matrix_destroy(&mult);
    matrix_destroy(&column);
    matrix_destroy(&mat);
    return 0;
}