#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

struct _node_A {
    int column;
    elem_type elem;
    struct _node_A *next;
}; //Node_A

struct _node_M {
    Node_A *begin_row; // = NULL, если в строке только 0
    struct _node_M *next;
}; //Node_M

struct _matrix {
    int n;
    int m;
    int entries;
    Node_M *head_M;
}; //*Matrix;

Node_A *node_A_create(void);
Node_M *node_M_create(void);
void node_A_add_last(Node_M *node_M, Node_A *new_node);
void node_A_add_before(Node_M *node_M, Node_A *after_node_A, Node_A *new_node);
void node_M_add_last(Matrix mat, Node_M *new_node);


Matrix matrix_create(int n, int m)
{
    Matrix mat = (Matrix) calloc(1, sizeof(*mat));
    if (mat == NULL) {
        fprintf(stderr, "Error: no memory\n");
        exit(MEMORY_LIMIT);
    }

    mat->n = n;
    mat->m = m;
    mat->entries = 0;

    Node_M *node_M;
    for(int i = 0; i < n; ++i) {
        node_M = node_M_create();
        node_M_add_last(mat, node_M);
        node_M = node_M->next;
    }


    return mat; //матрица из одних нулей
}

Node_A *node_A_create(void)
{
    Node_A *new_node = (Node_A *) calloc(1, sizeof(Node_A));
    if (new_node == NULL) {
        fprintf(stderr, "Error: no memory\n");
        exit(MEMORY_LIMIT);
    }
    new_node->next = NULL;
    return new_node;
}

Node_M *node_M_create(void)
{
    Node_M *new_node = (Node_M *) calloc(1, sizeof(Node_M));
    if (new_node == NULL) {
        fprintf(stderr, "Error: no memory\n");
        exit(MEMORY_LIMIT);
    }
    new_node->next = NULL;
    new_node->begin_row = NULL;
    return new_node;
}


void node_A_add_last(Node_M *node_M, Node_A *new_node)
{
    Node_A *tmp = node_M->begin_row;
    if(tmp == NULL) {
        node_M->begin_row = new_node;
        new_node->next = NULL;
        return;
    }
    while(tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = new_node;
    new_node->next = NULL;
}

void node_A_add_before(Node_M *node_M, Node_A *after_node_A, Node_A *new_node)
{
    Node_A *tmp = node_M->begin_row;
    if(node_M->begin_row == after_node_A) {
        new_node->next = node_M->begin_row;
        node_M->begin_row = new_node;
        return;
    }
    while(tmp && (tmp->next != after_node_A)) {
        tmp = tmp->next;
    }
    if(tmp) {
        new_node->next = tmp->next;
        tmp->next = new_node;
    }
}

void node_M_add_last(Matrix mat, Node_M *new_node)
{
    Node_M *tmp = mat->head_M;
    if(tmp == NULL) {
        mat->head_M = new_node;
        new_node->next = NULL;
        new_node->begin_row = NULL;
        return;
    }
    while(tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = new_node;
    new_node->next = NULL;
    new_node->begin_row = NULL;
}

void matrix_destroy(Matrix *mat)
{
    Node_M *node_M = (*mat)->head_M;
    while(node_M) {
        Node_M *tmp = node_M->next;

        Node_A *node_A = node_M->begin_row;
        while(node_A) {
            Node_A *tmp_A = node_A->next;
            free(node_A);
            node_A = tmp_A;
        }

        free(node_M);
        node_M = tmp;
    }

    free(*mat);
    *mat = NULL;
}

elem_type elem_get(Matrix mat, int i, int j)
{
    if (mat->head_M == NULL) {
        fprintf(stderr, "Error: impossible getting element\n");
        exit(ERROR_GETTING_ELEMENT);
    }

    Node_M *row = mat->head_M;
    while(i > 1) {
        row = row->next;
        --i;
    }

    if(row == NULL) {
        fprintf(stderr, "Error: impossible getting element\n");
        exit(ERROR_GETTING_ELEMENT);
    }

    //получим row->begin_row -- начало нужной строки
    if(row->begin_row == NULL) { //если в строке только 0
        return 0;
    } else {
        for(Node_A *helper = row->begin_row; helper->next; helper = helper->next) { //идем по нужной строке в А
            if(helper->column == j) {
                return helper->elem;
            }
        }
        return 0; //если в А нет нужного столбца, значит элемент == 0
    }
}


void matrix_print_outside(Matrix mat)
{
    for (int i = 1; i <= mat->n; ++i) {
        for (int j = 1; j <= mat->m; ++j) {
            elem_type num = elem_get(mat, i, j);
            printf("(%.2lf + %.2lfi)", creal(num), cimag(num));
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_print_inside(Matrix mat)
{
    Node_M *M_print = mat->head_M;
    for (int i = 0; M_print; ++i, M_print = M_print->next) {
        printf("%d row address: %p\n", i+1, &M_print->begin_row);
        Node_A *A_print = M_print->begin_row;
        if(A_print == NULL) {
            printf("NULL\n");
        } else {
            for (; A_print; A_print = A_print->next) {
                printf("Column: %d, value: (%.2lf + %.2lfi), address next element %p\n", A_print->column, creal(A_print->elem), cimag(A_print->elem), &A_print->next);
            }
        }
    }
    printf("\n");
}


int nonzero(Matrix mat)
{
    return mat->entries;
}


void elem_set(Matrix mat, int i, int j, elem_type value)
{
    if(value != 0) {
        mat->entries++;
        Node_M *node_M = mat->head_M;
        for(; i > 1; --i) {
            node_M = node_M->next;
        }

        Node_A *node_A = node_M->begin_row;
        bool insert_last = true;
        while(node_A) {
            if(node_A->column < j) {
                node_A = node_A->next;
            } else { //column > j, вставка нового узла перед node_A
                Node_A *new_node = node_A_create();
                node_A_add_before(node_M, node_A, new_node);
                new_node->column = j;
                new_node->elem = value;
                insert_last = false;
                break;
            }

            if(insert_last) {
                node_A = node_A_create();
                node_A_add_last(node_M, node_A);
                node_A->column = j;
                node_A->elem = value;
            }
        }
    }
}



void matrix_multiply(Matrix mat1, Matrix mat2, Matrix res)
{
    for (int i = 1; i <= mat1->n; ++i) {
        for (int j = 1; j <= mat2->m; ++j) {
            elem_type s = 0;
            for (int k = 1; k <= mat1->m; ++k) {
                elem_type num_mat1 = elem_get(mat1, i, k);
                elem_type num_mat2 = elem_get(mat2, k, j);
                s += num_mat1 * num_mat2;
            }
            elem_set(res, i, j, s);
        }
    }
}