import argparse
import logging
import math
import enum

import numpy as np
from numpy.linalg import norm

from matrix import Matrix, Vector
from utils import read_data, save_to_file, complex_to_list
from benchmark import numpy_eig


class TypeEig(enum.Enum):
    real = 1
    img = 2


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
    R = Matrix(A)

    for i in range(sz - 1):
        col = A.get_column(i)
        H = householder(col, len(A), i)
        Q = Q.multiply(H)
        R = H.multiply(R)

    return Q, R


def finish_iter_process(A, eps, type_eig):
    sz = len(A)
    i = 0
    a = np.array(A.get_data())
    while i < sz:
        if not type_eig[i]:
            if norm(a[i + 1:, i]) <= eps:
                type_eig[i] = TypeEig.real
            elif norm(a[i + 2:, i]) <= eps:
                if i + 1 < sz and not type_eig[i + 1]:
                    type_eig[i] = TypeEig.img
                    type_eig[i + 1] = TypeEig.img
                    i += 1
        i += 1
    return type_eig


def get_eigenvalues(A, type_eig):
    res = Vector()
    sz = len(A)
    i = 0
    while i < len(A):
        if type_eig[i] == TypeEig.real:
            res.append(A[i][i])
        else:
            a11 = A[i][i]
            a12 = A[i][i + 1] if i + 1 < sz else 0
            a21 = A[i + 1][i] if i + 1 < sz else 0
            a22 = A[i + 1][i + 1] if i + 1 < sz else 0
            res.extend(np.roots((1, -a11 - a22, a11 * a22 - a12 * a21)))
            i += 1
        i += 1
    return res


def QR_method(A, eps):
    A_i = Matrix(A)
    type_eig = [0] * len(A)
    i = 0
    while True:
        Q, R = get_QR(A_i)
        A_i = R.multiply(Q)
        type_eig = finish_iter_process(A_i, eps, type_eig)
        if all(type_eig):
            return get_eigenvalues(A_i, type_eig)
        i += 1


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="1-5.log", level=logging.INFO)

    need_args = ('matrix', 'eps')
    init_dict = read_data(args.input, need_args)
    A, eps = init_dict['matrix'], init_dict['eps']

    tmp = QR_method(A, eps)
    numpy_eig(A, tmp)
    x = complex_to_list(tmp)
    save_to_file(args.output, x=x)