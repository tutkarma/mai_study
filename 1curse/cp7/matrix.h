#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <complex.h>

#define MEMORY_LIMIT 1
#define ERROR_GETTING_ELEMENT 2

typedef complex double elem_type;
//typedef int elem_type;

typedef struct _node_A Node_A;
typedef struct _node_M Node_M;
typedef struct _matrix *Matrix;


Matrix matrix_create(int n, int m);
void matrix_destroy(Matrix *mat);
elem_type elem_get(Matrix mat, int i, int j);
void matrix_print_inside(Matrix mat);
void matrix_print_outside(Matrix mat);
int nonzero(Matrix mat);
void elem_set(Matrix mat, int i, int j, elem_type value);
void matrix_multiply(Matrix mat1, Matrix mat2, Matrix res);

#endif // __MATRIX_H__