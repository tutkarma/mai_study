import argparse
import logging
import operator
from functools import reduce

from utils import read_data, save_to_file


def f(x):
    return x ** 0.5


def w(points, x, i):
    return reduce(operator.mul, [x - points[j] for j in range(len(points)) if i != j])


def w_to_print(points, i):
    w = ''
    for j in range(len(points)):
        if i != j:
            w += '(x - {0})'.format(points[j])
    return w


def get_coefs(x, y):
    cnt = len(x)
    coefs = [y[i] for i in range(cnt)]

    for j in range(1, cnt):
        for i in range(cnt - 1, j - 1, -1):
            coefs[i] = float(coefs[i] - coefs[i - 1]) / float(x[i] - x[i - j])

    return coefs


def Lagrange_interpolation(points, x):
    res = 0
    res_str = 'L(x) = '
    for i in range(len(points)):
        f_w = f(points[i]) / w(points, points[i], i)
        res += f_w * w(points, x, i)

        sign = ' + ' if f_w > 0 else ''
        res_str += '{0} {1}'.format(sign, f_w) + w_to_print(points, i)

    logging.info(res_str)
    return res


def Newton_interpolation(points, x):
    y = [f(i) for i in points]
    coefs = get_coefs(points, y)
    cnt = len(coefs) - 1
    res = coefs[cnt]
    for i in range(cnt - 1, -1, -1):
        res = res * (x - points[i]) + coefs[i]
    return res


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="3-1.log", level=logging.INFO)

    need_args = ('points', 'x')
    init_dict = read_data(args.input, need_args)
    points, x = init_dict['points'], init_dict['x']


    res_lagr = []
    res_newton = []
    for i in range(len(points)):
        logging.info("Points: {0}".format(points[i]))
        logging.info("x: {0}".format(x))

        val = f(x)
        logging.info("Lagrange interpolation")
        val_by_lagrange = Lagrange_interpolation(points[i], x)
        abs_err = abs(val - val_by_lagrange)
        logging.info("Value: {0}".format(val))
        logging.info("Lagrange: {0}".format(val_by_lagrange))
        logging.info("Absolute error: {0}\n".format(abs_err))
        res_lagr.append((val_by_lagrange, abs_err))

        logging.info("Newton interpolation")
        val_by_newton = Newton_interpolation(points[i], x)
        abs_err = abs(val - val_by_newton)
        logging.info("Value: {0}".format(val))
        logging.info("Newton: {0}".format(val_by_newton))
        logging.info("Absolute error: {0}\n".format(abs_err))
        res_newton.append((val_by_newton, abs_err))

    save_to_file(args.output, Lagrange=res_lagr, Newton=res_newton)