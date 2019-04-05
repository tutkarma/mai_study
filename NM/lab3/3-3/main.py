import argparse
import logging

import matplotlib.pyplot as plt

import lu
from matrix import Matrix, Vector
from utils import read_data, save_to_file


def F(coefs, x):
    return sum([x ** i * c for i, c in enumerate(coefs)])


def F_to_print(coefs):
    n = len(coefs)
    f = 'F{0}(x) = '.format(n - 1)
    for i in range(n):
        f += '{0}x^{1} + '.format(coefs[i], i)
    f = f[:-2]
    return f


def sum_squared_errors(F, y):
    return sum([(f_i - y_i) ** 2 for f_i, y_i in zip(F, y)])


def mls(n, x, y):
    N = len(x)
    mat = [[sum([x_j ** (i + j) for x_j in x]) for i in range(n + 1)] for j in range(n + 1)]
    mat[0][0] = N + 1
    b = [sum([x_j ** i * y_j for x_j, y_j in zip(x, y)]) for i in range(n + 1)]

    mat = Matrix.from_list(mat)
    B = Vector.from_list(b)
    LU, P = lu.LU_decomposition(mat)
    new_B = lu.get_new_B(B, P)
    coefs = lu.LU_solve(LU, new_B)
    return coefs.get_data()



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="3-3.log", level=logging.INFO)

    need_args = ('x', 'y')
    init_dict = read_data(args.input, need_args)
    x, y = init_dict['x'], init_dict['y']

    logging.info("Least squares method")
    logging.info("x: {0}".format(x))
    logging.info("y: {0}".format(y))
    coefs1 = mls(1, x, y)
    logging.info(F_to_print(coefs1))
    F1 = [F(coefs1, i) for i in x]
    err1 = sum_squared_errors(F1, y)
    logging.info("Error: {0}".format(err1))

    coefs2 = mls(2, x, y)
    logging.info(F_to_print(coefs2))
    F2 = [F(coefs2, i) for i in x]
    err2 = sum_squared_errors(F2, y)
    logging.info("Error: {0}".format(err2))


    plt.scatter(x, y, color='r')
    plt.plot(x, F1, color='b')
    plt.plot(x, F2, color='g')
    plt.show()

    save_to_file(args.output, F1=[F1, err1], F2=[F2, err2])