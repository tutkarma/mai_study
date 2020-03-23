import argparse
import logging
import math
import profile

from matrix import Matrix, Vector
from utils import read_data, save_to_file

from numpy.linalg import solve, eig
import numpy as np
#from benchmark import numpy_eig

def numpy_eigs(matrix, my_values, my_vectors):
    print("My eigenvalues:")
    print(my_values)
    print("My eigenvectors:")
    print(my_vectors)

    a = np.array(matrix.get_data())
    eig_np = eig(a)
    print("Numpy eigenvalues:")
    print(eig_np[0].round(3))
    print("Numpy eigenvectors:")
    print(eig_np[1].round(3))


def t(A):
    return math.sqrt(sum([A[i][j] ** 2 for i in range(len(A)) 
        for j in range(i + 1, len(A))]))


def indexes_max_elem(A):
    i_max = j_max = 0
    a_max = A[0][0]
    for i in range(len(A)):
        for j in range(i + 1, len(A)):
            if abs(A[i][j]) > a_max:
                a_max = abs(A[i][j])
                i_max, j_max = i, j
    return i_max, j_max


def get_phi(a_ii, a_jj, a_ij):
    return math.pi / 4 if a_ii == a_jj else \
            0.5 * math.atan(2 * a_ij / (a_ii - a_jj))


def jacobi_eigenvalue(A, eps):
    sz = len(A)
    A_i = Matrix(A)
    eigenvectors = Matrix.identity(sz)

    a = 0
    while True:
        U = Matrix.identity(sz)
        i, j = indexes_max_elem(A_i)
        phi = get_phi(A_i[i][i], A_i[j][j], A_i[i][j])
        U[i][j] = -math.sin(phi)
        U[j][i] = math.sin(phi)
        U[i][i] = U[j][j] = math.cos(phi)

        U_T = Matrix(U)
        U_T.transpose()
        A_i = U_T.multiply(A_i.multiply(U))

        eigenvectors = eigenvectors.multiply(U)

        if t(A_i) < eps:
            break
        a += 1

    eigenvalues = A_i.diag()
    return eigenvalues, eigenvectors


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="1-4.log", level=logging.INFO)

    need_args = ('matrix', 'eps')
    init_dict = read_data(args.input, need_args)
    A, eps = init_dict['matrix'], init_dict['eps']

    values, vectors = jacobi_eigenvalue(A, eps)
    numpy_eigs(A, values, vectors)
    save_to_file(args.output, eigenvalues=values, eigenvectors=vectors)

if __name__ == '__main__':
    profile.run('main()')