#!/usr/bin/python3

import numpy as np
import getopt
import sys
from math import sqrt


USAGE = """
Syntax: main.py [--methods=#]

  Flags:
    methods=#
        Numbers of methods to use. By default all method are used.

        1 -- Classical method
        2 -- Gradient descent method
        3 -- Quickest descent method

    Example:
        --methods=1,3
"""

CNT_METHODS = 3


def get_info(method, coefs, x0=None, h=None, eps=None):
    print(method)
    print("f(x) = {0}x1^2 - x1x2 + {1}x2^2 + {2}x1 + {3}x2 + {4}".format(
        coefs[2], coefs[3], coefs[0], coefs[1], coefs[4]))
    if x0:
        print("x_0 = ({0} {1})^T".format(x0[0], x0[1]))
    if h:
        print("h = {0}".format(h))
    if eps:
        print("eps = {0}".format(eps))


def f(x, coefs):
    return coefs[2] * x[0]**2 - x[0] * x[1] + coefs[3] * x[1]**2 + coefs[0] * x[0] + coefs[1] * x[1] + coefs[4]


def grad(x, coefs):
    dx1 = coefs[2] * 2 * x[0] - x[1] + coefs[0]
    dx2 = coefs[3] * 2 * x[1] - x[0] + coefs[1]
    return [dx1, dx2]


def grad_val(x, coefs):
    dx = grad(x, coefs)
    return sqrt(dx[0] ** 2 + dx[1] ** 2)


def finish_iter_proccess(x, coefs, eps):
    print("Сhecking the condition of the end iterative process:")
    sign = "<" if grad_val(x, coefs) < eps else ">="
    print("{0} {1} {2}". format(grad_val(x, coefs), sign, eps))
    return grad_val(x, coefs) < eps


def get_step(k, x, coefs, gr):
    print("Find h")
    print("x^({0}) = {1} - h * {2}".format(k, x, gr))
    coef0 = coefs[2] * x[0] ** 2 - x[0] * x[1] + coefs[3] * x[1] ** 2 + \
            coefs[0] * x[0] + coefs[1] * x[1] + coefs[4]

    coef1 = -2 * coefs[2] * x[0] * gr[0] + x[0] * gr[1] + x[1] * gr[0] - \
            2 * coefs[3] * x[1] * gr[1] - coefs[0] * gr[0] - coefs[1] * gr[1] # h

    coef2 = coefs[2] * gr[0] ** 2 - gr[0] * gr[1] + coefs[3] * gr[1] ** 2 # h^2

    print("f(x) = {0}h^2 + {1}h + {2}".format(coef2, coef1, coef0))
    print("df = {0}h + {1}".format(coef2 * 2, coef1))

    return (-coef1) / (coef2 * 2)


def sylvester_criterion(hess):
    det = hess[0][0] * hess[1][1] - hess[0][1] * hess[1][0]
    sign = ">" if hess[0][0] > 0 else "<" if hess[0][0] < 0 else "="
    print("det1 = {0} {1} 0".format(hess[0][0], sign))
    sign = ">" if det > 0 else "<" if det < 0 else "="
    print("det2 = {0} {1} 0".format(det, sign))
    if hess[0][0] > 0 and det > 0:
        return 1
    elif hess[0][0] < 0 and det > 0:
        return -1
    return 0


def classical_method(coefs):
    get_info("Calssical method", coefs)

    coefs_dx1 = [coefs[2] * 2, coefs[0]]
    coefs_dx2 = [coefs[3] * 2, coefs[1]]
    print("df/dx1 = {0}x1 - x2 + {1}".format(coefs_dx1[0], coefs_dx1[1]))
    print("df/dx2 = {0}x2 - x1 + {1}".format(coefs_dx2[0], coefs_dx2[1]))

    print("System of equations:")
    print("{0}x1 - x2 + {1} = 0".format(coefs_dx1[0], coefs_dx1[1]))
    print("{0}x2 - x1 + {1} = 0".format(coefs_dx2[0], coefs_dx2[1]))

    a = np.array([[coefs_dx1[0], -1], [-1, coefs_dx2[0]]])
    b = np.array([-coefs_dx1[1], -coefs_dx2[1]])
    stationary_root = np.linalg.solve(a, b)
    print("Stationary root: {0}".format(stationary_root))

    hess = [[coefs_dx1[0], -1], [-1, coefs_dx2[0]]]
    print("Hessian matrix:")
    for i in hess:
        print(*i)

    check = sylvester_criterion(hess)
    if check == 1:
        print("min -- {0}".format(stationary_root))
    elif check == -1:
        print("max -- {0}".format(stationary_root))
    else:
        print("No solution")
    print("\n\n")


def gradient_descent(coefs, x_prev, h, eps):
    get_info("Gradient descent method", coefs, x0=x_prev, h=h, eps=eps)

    k = 0
    while not finish_iter_proccess(x_prev, coefs, eps):
        print("---------")
        print("Iteration #{}".format(k))
        print("grad(f(x^{0})) = {1}". format(k, grad(x_prev, coefs)))

        gr = grad(x_prev, coefs)
        x = [i - h * j for i, j in zip(x_prev, gr)]
        print("x = {0} - {1} * {2} = {3}".format(x_prev, h, gr, x))

        while f(x, coefs) >= f(x_prev, coefs):
            print("Checking the condition f(x^(k+1)) < f(x^(k)):")
            print("{0} >= {1}".format(f(x, coefs), f(x_prev, coefs)))
            print("h = h / 2 = {0}".format(h / 2))
            h /= 2
            gr = grad(x_prev, coefs)
            print("x = {0} - {1} * {2}".format(x_prev, h, gr))
            x = [i - h * j for i, j in zip(x_prev, gr)]

        if f(x, coefs) < f(x_prev, coefs):
            print("Checking the condition f(x^(k+1)) < f(x^(k)):")
            print("{0} < {1}".format(f(x, coefs), f(x_prev, coefs)))

        k += 1
        x_prev = x
    print("\n\n")


def quickest_descent(coefs, x_prev, h, eps):
    get_info("Quickest descent method", coefs, x0=x_prev, h=h, eps=eps)

    k = 0
    while not finish_iter_proccess(x_prev, coefs, eps):
        print("---------")
        print("Iteration #{}".format(k))
        gr = grad(x_prev, coefs)
        print("grad(f(x^({0}))) = {1}". format(k, gr))

        h = get_step(k, x_prev, coefs, gr)
        print("h^({0}) = {1}".format(k, h))
        x = [i - h * j for i, j in zip(x_prev, gr)]
        print("x^({0}) = {1}".format(k, x))
        x_prev = x
        k += 1
    print("\n\n")


def print_usage(message):
    print(USAGE)
    if message:
        sys.exit('\nFATAL ERROR: ' + message)
    else:
        sys.exit(1)


def parse_args(args):
    try:
        opts, args = getopt.getopt(args, '', ['help', 'methods='])
    except getopt.GetoptError:
        print_usage('Invalid arguments.')

    methods = [i for i in range(1, CNT_METHODS + 1)]

    for (opt, val) in opts:
        if opt == '--help':
            print_usage(None)
        elif opt == '--methods':
            try:
                methods = set(map(int, val.split(',')))
            except ValueError:
                print_usage('Methods must be comma separated list.')

            for v in methods:
                if v not in range(1, CNT_METHODS + 1):
                    print_usage('Incorrect methods')

    return methods


if __name__ == '__main__':
    methods = parse_args(sys.argv[1:])
    print("Enter coefs of the function (x1, x2, x1^2, x2^2, free coef)")
    print("Example:\n14 -7 7 7 13")
    coefs = list(map(int, input().split()))
    #coefs = [14, -7, 7, 7, 13] # x1, x2, x1^2, x2^2, свободный коэф
    x_0 = [2, 3]
    h = 0.1
    eps = 0.9

    for i in methods:
        if i == 1:
            classical_method(coefs)
        elif i == 2:
            gradient_descent(coefs, x_0, h, eps)
        elif i == 3:
            quickest_descent(coefs, x_0, h, eps)
