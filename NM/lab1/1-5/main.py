import argparse
import logging
import math
import enum

import numpy as np
from numpy.linalg import norm

from matrix import Matrix, Vector
from utils import read_data, save_to_file, complex_to_list
from benchmark import numpy_eig


def sign(x):
    return -1 if x < 0 else 1 if x > 0 else 0


def householder(a, sz, k):
    v = np.zeros(sz)
    a = np.array(a.get_data())
    v[k] = a[k] + sign(a[k]) * norm(a[k:])
    for i in range(k + 1, sz):
        v[i] = a[i]
    v = v[:, np.newaxis]
    H = np.eye(sz) - (2 / (v.T @ v)) * (v @ v.T)
    return Matrix.from_list(H.tolist())


def get_QR(A):
    sz = len(A)
    Q = Matrix.identity(sz)
    A_i = Matrix(A)

    for i in range(sz - 1):
        col = A_i.get_column(i)
        H = householder(col, len(A_i), i)
        Q = Q.multiply(H)
        A_i = H.multiply(A_i)

    return Q, A_i


def get_roots(A, i):
    sz = len(A)
    a11 = A[i][i]
    a12 = A[i][i + 1] if i + 1 < sz else 0
    a21 = A[i + 1][i] if i + 1 < sz else 0
    a22 = A[i + 1][i + 1] if i + 1 < sz else 0
    return np.roots((1, -a11 - a22, a11 * a22 - a12 * a21))


def finish_iter_for_complex(A, eps, i):
    Q, R = get_QR(A)
    A_next = R.multiply(Q)
    lambda1 = get_roots(A, i)
    lambda2 = get_roots(A_next, i)
    return True if abs(lambda1[0] - lambda2[0]) <= eps and \
                abs(lambda1[1] - lambda2[1]) <= eps else False


def get_eigenvalue(A, eps, i):
    A_i = Matrix(A)
    while True:
        Q, R = get_QR(A_i)
        A_i = R.multiply(Q)
        a = np.array(A_i.get_data())
        if norm(a[i + 1:, i]) <= eps:
            res = (a[i][i], False, A_i)
            break
        elif norm(a[i + 2:, i]) <= eps and finish_iter_for_complex(A_i, eps, i):
            res = (get_roots(A_i, i), True, A_i)
            break
    return res


def QR_method(A, eps):
    res = Vector()
    i = 0
    A_i = Matrix(A)
    while i < len(A):
        eigenval = get_eigenvalue(A_i, eps, i)
        if eigenval[1]:
            res.extend(eigenval[0])
            i += 2
        else:
            res.append(eigenval[0])
            i += 1
        A_i = eigenval[2]
    return res, i


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="1-5.log", level=logging.INFO)

    need_args = ('matrix', 'eps')
    init_dict = read_data(args.input, need_args)
    A, eps = init_dict['matrix'], init_dict['eps']
    logging.info("Input matrix:")
    logging.info(A)

    tmp, count_iter = QR_method(A, eps)
    logging.info("Eigenvalues:")
    logging.info(tmp)
    logging.info("Count iteration:")
    logging.info(count_iter)
    numpy_eig(A, tmp)
    x = complex_to_list(tmp)
    save_to_file(args.output, x=x)