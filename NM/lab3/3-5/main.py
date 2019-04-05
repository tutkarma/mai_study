import argparse
import logging

import numpy as np

from utils import read_data, save_to_file


def f(x):
    return 1 / (3 * x ** 2 + 4 * x + 2)


def get_points(x0, x, step):
    return [i for i in np.arange(x0, x + step, step)]


def get_values(points):
    return [f(x) for x in points]


def rectangle_method(x, h):
    return h * sum([f((x[i] + x[i + 1]) / 2) for i in range(len(x) - 1)])


def trapeze_method(y, h):
    n = len(y) - 1
    return h * (y[0] / 2 + sum([y[i] for i in range(1, n - 1)]) + y[n] / 2)


def Simpson_method(y, h):
    n = len(y) - 1
    return h / 3 * (y[0] + sum([4 * y[i] for i in range(1, n, 2)]) + 
                           sum([2 * y[i] for i in range(2, n - 1, 2)]) + y[n])


def Runge_Romberg_method(res):
    k = res[0]['h'] / res[1]['h']
    err_rec = abs(res[0]['Rectangle'] - res[1]['Rectangle']) / (k ** 2 - 1)
    err_trap = abs(res[0]['Trapeze'] - res[1]['Trapeze']) / (k ** 2 - 1)
    err_sim = abs(res[0]['Simpson'] - res[1]['Simpson']) / (k ** 4 - 1)
    return {'Rectangle': err_rec, 'Trapeze': err_trap, 'Simpson': err_sim}

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="3-5.log", level=logging.INFO)

    need_args = ('x0', 'x', 'h')
    init_dict = read_data(args.input, need_args)
    x0, x, steps = init_dict['x0'], init_dict['x'], init_dict['h']

    save_res = []
    for h in steps:
        logging.info("a: {0}".format(x0))
        logging.info("b: {0}".format(x))
        logging.info("h: {0}".format(h))
        points = get_points(x0, x, h)
        logging.info("x: {0}".format(points))
        values = get_values(points)
        logging.info("y(x): {0}".format(values))

        logging.info("Rectangle method")
        res_rec = rectangle_method(points, h)
        logging.info("Value: {0}".format(res_rec))

        logging.info("Trapeze method")
        res_trap = trapeze_method(values, h)
        logging.info("Value: {0}".format(res_trap))

        logging.info("Simpson method")
        res_sim = Simpson_method(values, h)
        logging.info("Value: {0}".format(res_sim))

        save_res.append({
                        "h": h,
                        "Rectangle": res_rec,
                        "Trapeze": res_trap,
                        "Simpson": res_sim
                        })

    errors = Runge_Romberg_method(save_res)
    logging.info("Errors")
    logging.info("Rectangle error: {0}".format(errors['Rectangle']))
    logging.info("Trapeze error: {0}".format(errors['Trapeze']))
    logging.info("Simpson error: {0}".format(errors['Simpson']))

    save_to_file(args.output, h1=save_res[0], h2=save_res[1], errors=errors)