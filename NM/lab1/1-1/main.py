import argparse
from functools import reduce

from matrix import Matrix, Vector
from utils import read_matrix


def get_P(mat):
    p = Matrix.identity(mat.size)
    for i in range(mat.size):
        row = max(range(i, mat.size), key=lambda j: abs(mat[j][i]))
        if i != row:
            p[i], p[row] = p[row], p[i]
    return p


def get_new_B(B, P):
    perm = [row.index(1) for row in P]
    new_B = Vector(len(B))
    for i in range(len(B)):
        new_B[i] = B[perm[i]]
    return new_B


def LU_decomposition(mat):
    P = get_P(mat)
    PA = P.multiply(mat)
    LU = Matrix(PA)

    for i in range(mat.size):
        for j in range(i + 1, mat.size):
            LU[j][i] /= LU[i][i]
            for k in range(i + 1, mat.size):
                LU[j][k] -= LU[j][i] * LU[i][k]

    return LU, P


def LU_solve(LU, B):
    z = Vector(LU.size)
    x = Vector(LU.size)

    z[0] = B[0]
    for i in range(1, LU.size):
        s = sum([LU[i][j] * z[j] for j in range(i)])
        z[i] = B[i] - s

    x[-1] = z[-1] / LU[-1][-1]
    for i in reversed(range(LU.size - 1)):
        s = sum([LU[i][j] * x[j]
                      for j in range(i + 1, LU.size)])
        x[i] = (z[i] - s) / LU[i][i]

    return x


def LU_inverse(LU):
    E = Matrix.identity(LU.size)
    x = []
    for row in E:
        x.append(LU_solve(LU, row))
    res = Matrix.from_list(x)
    res.transpose()
    return res


def LU_determinant(LU):
    return reduce(lambda x, y: x * y, [LU[i][i] for i in range(LU.size)])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')

    args = parser.parse_args()
    mat, B = Matrix(), Vector()
    read_matrix(args.input, mat, B)
    LU, P = LU_decomposition(mat)
    new_B = get_new_B(B, P)
    x = LU_solve(LU, new_B)
    x.save_to_file(args.output)
    inv = LU_inverse(LU)
    inv.debug_print()
    print(LU_determinant(LU))