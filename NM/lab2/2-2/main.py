import argparse
import logging
from math import exp, log


import lu
from matrix import Matrix, Vector


EPS = 0.01


def sign_str(a, b):
    return " > " if a > b else " <= "


def f1(x):
    return x[0] ** 2 + x[1] ** 2 - 4


def f2(x):
    return x[0] - exp(x[1]) + 2


def phi1(x):
    return (4 - x[1] ** 2) ** 0.5


def phi2(x):
    return log(x[0] + 2)


def derivative(x, f1=False, f2=False, phi1=False, phi2=False, x1=False, x2=False):
    if f1 and x1:
        return 2 * x[0]
    elif f1 and x2:
        return 2 * x[1]
    elif f2 and x1:
        return 1
    elif f2 and x2:
        return -exp(x[1])

    elif (phi1 and x1) or (phi2 and x2):
        return 0
    elif phi1 and x2:
        return -x[1] / ((4 - x[1] ** 2) ** 0.5)
    elif phi2 and x1:
        return 1 / (x[0] + 2)


def get_q(x):
    max_phi1 = (abs(derivative(x, phi1=True, x1=True)) + 
                abs(derivative(x, phi1=True, x2=True)))
    max_phi2 = (abs(derivative(x, phi2=True, x1=True)) + 
                abs(derivative(x, phi2=True, x2=True)))
    return max(max_phi1, max_phi2)


def A1(x):
    return [[f1(x), derivative(x, f1=True, x2=True)],
            [f2(x), derivative(x, f2=True, x2=True)]]


def A2(x):
    return [[derivative(x, f1=True, x1=True), f1(x)],
            [derivative(x, f2=True, x1=True), f2(x)]]


def jacobi(x):
    return [[derivative(x, f1=True, x1=True), derivative(x, f1=True, x2=True)],
            [derivative(x, f2=True, x1=True), derivative(x, f2=True, x2=True)]]


def det(mat):
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]


def delta_x(x):
    mat = Matrix.from_list(jacobi(x))
    B = Vector.from_list([-f1(x), -f2(x)])
    LU, P = lu.LU_decomposition(mat)
    new_B = lu.get_new_B(B, P)
    delta = lu.LU_solve(LU, new_B)
    return delta.get_data()


def iteration_method(init_dict):
    logging.info("Iteration method")
    interval, eps = init_dict['interval'], init_dict['eps']
    cnt_iter = 0

    inter_x1, inter_x2 = interval[0], interval[1]
    logging.info("Eps: {0}".format(eps))
    logging.info("Intervals: [{0}, {1}]".format(inter_x1, inter_x2))

    x_prev = [(inter_x1[1] - inter_x1[0]) / 2,
              (inter_x2[1] - inter_x2[0]) / 2]
    q = get_q(x_prev)
    logging.info("x0 = {0}".format(x_prev))
    logging.info("q = {0}".format(q))

    while True:
        cnt_iter += 1
        logging.info("Iter #{0}".format(cnt_iter))
        x = [phi1(x_prev), phi2(x_prev)]
        logging.info("x = {0}".format(x))

        finish_iter = max([abs(i - j) for i, j in zip(x, x_prev)]) * q / (1 - q)
        sign = sign_str(finish_iter, eps)
        logging.info("{0}{1}{2}".format(finish_iter, sign, eps))
        if finish_iter <= eps:
            break

        x_prev = x

    return tuple(x), cnt_iter


def newton_method(init_dict, LU=False):
    logging.info("Newton method")
    interval, eps = init_dict['interval'], init_dict['eps']
    cnt_iter = 0

    inter_x1, inter_x2 = interval[0], interval[1]
    logging.info("Eps: {0}".format(eps))
    logging.info("Intervals: [{0}, {1}]".format(inter_x1, inter_x2))

    x_prev = [(inter_x1[1] - inter_x1[0]) / 2,
              (inter_x2[1] - inter_x2[0]) / 2]
    logging.info("x0 = {0}".format(x_prev))

    while True:
        cnt_iter += 1
        logging.info("Iter #{0}".format(cnt_iter))

        if LU is True:
            delta = delta_x(x_prev)
            x = [x_prev[0] + delta[0],
                 x_prev[1] + delta[1]]
        else:
            x = [x_prev[0] - det(A1(x_prev)) / det(jacobi(x_prev)),
                 x_prev[1] - det(A2(x_prev)) / det(jacobi(x_prev))]

        logging.info("x = {0}".format(x))

        finish_iter = max([abs(i - j) for i, j in zip(x, x_prev)])
        sign = sign_str(finish_iter, eps)
        logging.info("{0}{1}{2}".format(finish_iter, sign, eps))
        if finish_iter <= eps:
            break

        x_prev = x

    return tuple(x), cnt_iter


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--eps', help='Accuracy')
    args = parser.parse_args()

    logging.basicConfig(filename="2-2.log", level=logging.INFO)

    init_dict = {
                    'eps': float(args.eps) if args.eps else EPS,
                    'interval': [(1.0, 2.0), (0.5, 1.5)]
                }

    x, cnt_iter = iteration_method(init_dict)
    print("Iteration method:")
    print("x = {0}".format(x))
    print("Count iteration: {0}".format(cnt_iter))

    x, cnt_iter = newton_method(init_dict)
    print("Newton method:")
    print("x = {0}".format(x))
    print("Count iteration: {0}".format(cnt_iter))

    x, cnt_iter = newton_method(init_dict, LU=True)
    print("Newton method with lu decomposition:")
    print("x = {0}".format(x))
    print("Count iteration: {0}".format(cnt_iter))
