import argparse
import logging
from math import exp

import matplotlib.pyplot as plt
import numpy as np

from utils import read_data, save_to_file
from tma import tma
from matrix import TridiagonalMatrix, Vector
from ERK import Runge_Kutta_method


def func(x, y, y_der):
    return (4 * y - 4 * x * y_der) / (2 * x + 1)


def g(x, y, z):
    return z


def p(x):
    return 4 * x / (2 * x + 1)


def q(x):
    return -4 / (2 * x + 1)


def f(x):
    return 0


def exact_func(x):
    return x + exp(-2 * x)


def first_der(x, y, x0):
    i = 0
    while i < len(x) - 1 and x[i + 1] < x0:
        i += 1
    return (y[i + 1] - y[i]) / (x[i + 1] - x[i])


def get_n(n_prev, n, ans_prev, ans, b, delta, gamma, y1):
    x, y = ans_prev[0], ans_prev[1]
    y_der = first_der(x, y, b)
    phi_n_prev = delta * y[-1] + gamma * y_der - y1
    x, y = ans[0], ans[1]
    y_der = first_der(x, y, b)
    phi_n = delta * y[-1] + gamma * y_der - y1
    return n - (n - n_prev) / (phi_n - phi_n_prev) * phi_n


def check_finish(x, y, b, delta, gamma, y1, eps):
    y_der = first_der(x, y, b)
    return abs(delta * y[-1] + gamma * y_der - y1) > eps


def shooting_method(f, g, a, b, alpha, beta, delta, gamma, y0, y1, h, eps):
    n_prev, n = 1.0, 0.8
    y_der = (y0 - alpha * n_prev) / beta
    ans_prev = Runge_Kutta_method(f, g, a, b, h, n_prev, y_der)[:2]
    y_der = (y0 - alpha * n) / beta
    ans = Runge_Kutta_method(f, g, a, b, h, n, y_der)[:2]

    while check_finish(ans[0], ans[1], b, delta, gamma, y1, eps):
        n, n_prev = get_n(n_prev, n, ans_prev, ans, b, delta, gamma, y1), n
        ans_prev = ans
        y_der = (y0 - alpha * n) / beta
        ans = Runge_Kutta_method(f, g, a, b, h, n, y_der)[:2]

    return ans


def finite_difference_method(f, p, q, a, b, alpha, beta, delta, gamma, y0, y1, h):
    n = int((b - a) / h)
    x = [i for i in np.arange(a, b + h, h)]
    A = [0] + [1 - p(x[i]) * h / 2 for i in range(0, n - 1)] + [-gamma]
    B = [alpha * h - beta] + [q(x[i]) * h ** 2 - 2 for i in range(0, n - 1)] + [delta * h + gamma]

    C = [beta] + [1 + p(x[i]) * h / 2 for i in range(0, n - 1)] + [0]
    D = [y0 * h] + [f(x[i]) * h ** 2 for i in range(0, n - 1)] + [y1 * h]

    y = tma(TridiagonalMatrix.from_lists(A, B, C), Vector.from_list(D))
    return x, y.get_data()


def draw_plot(res, res2):
    plt.scatter(res[0], res[1], color='r', alpha=0.6, label='Shooting method')
    plt.scatter(res2[0], res2[1], color='b', alpha=0.6, label='Finite difference method')
    plt.legend()
    plt.grid(True)
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    #parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="4-2.log", level=logging.INFO)

    need_args = ('a', 'b', 'alpha', 'beta', 'delta', 'gamma', 'y0', 'y1', 'h', 'eps')
    init_dict = read_data(args.input, need_args)
    a, b, alpha, beta = init_dict['a'], init_dict['b'], init_dict['alpha'], init_dict['beta']
    delta, gamma, y0, y1 = init_dict['delta'], init_dict['gamma'], init_dict['y0'], init_dict['y1']
    h, eps = init_dict['h'], init_dict['eps']

    res = shooting_method(func, g, a, b, alpha, beta, delta, gamma, y0, y1, h, eps)
    res2 = finite_difference_method(f, p, q, a, b, alpha, beta, delta, gamma, y0, y1, h)

    draw_plot(res, res2)