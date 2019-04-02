import argparse
import logging
from math import log2, log


EPS = 0.01


def sign_str(a, b):
    return " > " if a > b else " <= "


def f(x):
    return 2 ** x + x ** 2 - 2


def df(x):
    return 2 ** x * log(2) + 2 * x


def phi(x):
    res = (2 - 2 ** x) ** 0.5
    logging.info("Phi(x) = {0}".format(res))
    return res


def dphi(x):
    return 2 ** x * log(2) / (2 * (2 - 2 ** x) ** 0.5)


def get_q(a, b):
    return max(abs(dphi(a)), abs(dphi(b)))


def iteration_method(init_dict):
    logging.info("Iteration method")
    interval, eps = init_dict['interval'], init_dict['eps']
    a, b = interval[0], interval[1]
    logging.info("Eps: {0}".format(eps))
    logging.info("Interval: [{0}, {1}]".format(a, b))

    x_prev = (b - a) / 2
    q = get_q(a, b)
    cnt_iter = 0

    logging.info("x0 = {0}".format(x_prev))
    logging.info("q = {0}".format(q))

    while True:
        cnt_iter += 1
        logging.info("Iter #{0}".format(cnt_iter))

        x = phi(x_prev)
        finish_iter = abs(x - x_prev) * q / (1 - q)
        sign = sign_str(finish_iter, eps)
        logging.info("{0}{1}{2}".format(finish_iter, sign, eps))
        if finish_iter <= eps:
            break

        x_prev = x

    return x, cnt_iter


def newton_method(init_dict):
    logging.info("Newton method")
    interval, eps = init_dict['interval'], init_dict['eps']
    a, b = interval[0], interval[1]

    x_prev = b
    cnt_iter = 0

    logging.info("x0 = {0}".format(x_prev))
    while True:
        cnt_iter += 1
        logging.info("Iter #{0}".format(cnt_iter))

        x = x_prev - f(x_prev) / df(x_prev)
        logging.info("x = {0}".format(x))

        finish_iter = abs(f(x) - f(x_prev))
        sign = sign_str(finish_iter, eps)
        logging.info("{0}{1}{2}".format(finish_iter, sign, eps))
        if finish_iter <= eps:
            break

        x_prev = x

    return x, cnt_iter


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--eps', help='Accuracy')
    args = parser.parse_args()

    logging.basicConfig(filename="2-1.log", level=logging.INFO)

    init_dict = {
                    'eps': int(args.eps) if args.eps else EPS,
                    'interval': (0, 0.7)
                }

    x, cnt_iter = iteration_method(init_dict)
    print("Iteration method:")
    print("x = {0}".format(x))
    print("Count iteration: {0}".format(cnt_iter))

    x, cnt_iter = newton_method(init_dict)
    print("Newton method:")
    print("x = {0}".format(x))
    print("Count iteration: {0}".format(cnt_iter))
