import argparse
import numpy as np

from matrix import TridiagonalMatrix, Vector
from utils import read_triagonal_matrix


def tma(mat, D):
    x = Vector(mat.size)
    p, q = [], []
    p.append(-mat.c[0] / mat.b[0])
    q.append(D[0] / mat.b[0])

    for i in range(1, mat.size):
        p_i = 0 if i == mat.size - 1 else (-mat.c[i] / (mat.b[i] + mat.a[i] * p[i - 1]))
        q_i = (D[i] - mat.a[i] * q[i - 1]) / (mat.b[i] + mat.a[i] * p[i - 1])
        p.append(p_i)
        q.append(q_i)

    x[mat.size - 1] = q[mat.size - 1]
    for i in range(mat.size - 2, -1, -1):
        x[i] = p[i] * x[i + 1] + q[i]

    return x


def benchmark():
    pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    #parser.add_argument('--log', help='Logging')

    args = parser.parse_args()

    mat, D = TridiagonalMatrix(), Vector()
    read_triagonal_matrix(args.input, mat, D)
    x = tma(mat, D)
    x.save_to_file(args.output)
