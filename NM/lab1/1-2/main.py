import argparse
import logging

import numpy as np

from matrix import TridiagonalMatrix, Vector
from utils import read_triagonal_matrix, save_to_file


def tma(mat, D):
    sz = len(mat)
    x = Vector(sz)
    p, q = [], []
    p.append(-mat.c[0] / mat.b[0])
    q.append(D[0] / mat.b[0])

    for i in range(1, sz):
        p_i = 0 if i == sz - 1 else (-mat.c[i] / (mat.b[i] + mat.a[i] * p[i - 1]))
        q_i = (D[i] - mat.a[i] * q[i - 1]) / (mat.b[i] + mat.a[i] * p[i - 1])
        p.append(p_i)
        q.append(q_i)

    x[sz - 1] = q[sz - 1]
    for i in range(sz - 2, -1, -1):
        x[i] = p[i] * x[i + 1] + q[i]

    return x


def benchmark():
    pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="1-2.log", level=logging.INFO)

    mat, D = TridiagonalMatrix(), Vector()
    read_triagonal_matrix(args.input, mat, D)
    logging.info("Input matrix:")
    logging.info(mat)
    logging.info("Input vector:")
    logging.info(D)

    x = tma(mat, D)
    logging.info("Answer:")
    logging.info(x)
    save_to_file(args.output, X=x)
