import argparse
import logging
from math import exp

import numpy as np

from utils import read_data, save_to_file


def Cauchy_problem(x, y, y_der):
    return 4 * x * y_der - (4 * x ** 2 + 2) * y


def g(x, y, z):
    return z


def exact_func(x):
    return (1 + x) * exp(-x**2)


def analytical_solution(f, a, b, h):
    x = [i for i in np.arange(a, b + h, h)]
    y = [f(i) for i in x]
    return x, y


def Euler_method(f, a, b, h, y0, y_der):
    n = int((b - a) / h)
    x = [i for i in np.arange(a, b + h, h)]
    y = [y0]
    z = y_der
    for i in range(n):
        z += h * f(x[i], y[i], z)
        y_i = y[i] + h * z
        y.append(y_i)
    return x, y


def Runge_Kutta_method(f, g, a, b, h, y0, y_der):
    n = int((b - a) / h)
    x = [i for i in np.arange(a, b + h, h)]
    y = [y0]
    z = [y_der]
    for i in range(n):
        K1 = h * g(x[i], y[i], z[i])
        L1 = h * f(x[i], y[i], z[i])
        K2 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1)
        L2 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1)
        K3 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2)
        L3 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2)
        K4 = h * g(x[i] + h, y[i] + K3, z[i] + L3)
        L4 = h * f(x[i] + h, y[i] + K3, z[i] + L3)
        delta_y = (K1 + 2 * K2 + 2 * K3 + K4) / 6
        delta_z = (L1 + 2 * L2 + 2 * L3 + L4) / 6
        y.append(y[i] + delta_y)
        z.append(z[i] + delta_z)
    return x, y, z


def Adams_method(f, g, x, y, z, h):
    n = len(x)
    x = x[:4]
    y = y[:4]
    z = z[:4]
    for i in range(3, n):
        z_i = z[i] + h * (55 * f(x[i], y[i], z[i]) -
                          59 * f(x[i - 1], y[i - 1], z[i - 1]) +
                          37 * f(x[i - 2], y[i - 2], z[i - 2]) -
                           9 * f(x[i - 3], y[i - 3], z[i - 3])) /24
        z.append(z_i)
        y_i = y[i] + h * (55 * g(x[i], y[i], z[i]) -
                          59 * g(x[i - 1], y[i - 1], z[i - 1]) +
                          37 * g(x[i - 2], y[i - 2], z[i - 2]) -
                           9 * g(x[i - 3], y[i - 3], z[i - 3])) / 24
        y.append(y_i)
        x.append(x[i] + h)
    return x, y


def Runge_Romberg_method(res):
    k = res[0]['h'] / res[1]['h']
    err_euler = []
    for i in range(len(res[0]['Euler']['y'])):
        err_euler.append(abs(res[0]['Euler']['y'][i] - res[1]['Euler']['y'][i]) / (k ** 1 - 1))

    err_runge = []
    for i in range(len(res[0]['Runge']['y'])):
        err_runge.append(abs(res[0]['Runge']['y'][i] - res[1]['Runge']['y'][i]) / (k ** 4 - 1))

    err_adams = []
    for i in range(len(res[0]['Adams']['y'])):
        err_adams.append(abs(res[0]['Adams']['y'][i] - res[1]['Adams']['y'][i]) / (k ** 4 - 1))

    return {'Euler': err_euler, 'Runge': err_runge, 'Adams': err_adams}


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="4-1.log", level=logging.INFO)

    need_args = ('a', 'b', 'h', 'y0', 'y_der')
    init_dict = read_data(args.input, need_args)
    a, b, st = init_dict['a'], init_dict['b'], init_dict['h']
    y0, y_der = init_dict['y0'], init_dict['y_der']

    save_res = []
    steps = [st, st / 2]
    for h in steps:
        logging.info("Euler method")
        euler_x, euler_y = Euler_method(Cauchy_problem, a, b, h, y0, y_der)
        for x, y in zip(euler_x, euler_y):
            logging.info("x: {0}, y: {1}".format(round(x, 3), y))

        logging.info("Runge-Kutta method")
        runge_x, runge_y, runge_z = Runge_Kutta_method(Cauchy_problem, g, a, b, h, y0, y_der)
        for x, y in zip(runge_x, runge_y):
            logging.info("x: {0}, y: {1}".format(round(x, 3), y))

        logging.info("Adams method")
        adams_x, adams_y = Adams_method(Cauchy_problem, g, runge_x, runge_y, runge_z, h)
        for x, y in zip(adams_x, adams_y):
            logging.info("x: {0}, y: {1}".format(round(x, 3), y))

        logging.info("Analytical solution")
        anal_x, anal_y = analytical_solution(exact_func, a, b, h)
        for x_i, y_i in zip(anal_x, anal_y):
            logging.info("x: {0}, y: {1}".format(round(x_i, 3), y_i))

        save_res.append({
                        "h": h,
                        "Euler": {'x': euler_x, 'y': euler_y},
                        "Runge": {'x': runge_x, 'y': runge_y},
                        "Adams": {'x': adams_x, 'y': adams_y},
                        })

    errors = Runge_Romberg_method(save_res)
    logging.info("Errors")
    logging.info("Euler error: {0}".format(errors['Euler']))
    logging.info("Runge error: {0}".format(errors['Runge']))
    logging.info("Adams error: {0}".format(errors['Adams']))

    save_to_file(args.output, h1=save_res[0], h2=save_res[1], errors=errors)