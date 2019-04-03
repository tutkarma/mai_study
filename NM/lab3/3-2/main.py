import argparse
import logging

from tma import tma
from matrix import TridiagonalMatrix, Vector
from utils import read_data, save_to_file


def get_a(f):
    return [0] + [f[i] for i in range(0, len(f) - 1)]


def get_b(f, h, c):
    b = [0]
    n = len(f) - 1
    for i in range(1, n):
        b.append((f[i] - f[i - 1]) / h[i] - 1/3 * h[i] * (c[i + 1] + 2 * c[i]))
    b.append(f[n] - f[n - 1] / h[n] - 2/3 * h[n] * c[n])
    return b


def get_c(x, f, h):
    n = len(f)
    a = [0] + [h[i - 1] for i in range(3, n)]
    b = [2 * (h[i - 1] + h[i]) for i in range(2, n)]
    c = [h[i] for i in range(2, n - 1)] + [0]
    d = [3 * ((f[i] - f[i - 1]) / h[i] - ((f[i - 1] - f[i - 2]) / h[i - 1])) for i in range(2, n)]
    x = tma(TridiagonalMatrix.from_lists(a, b, c), Vector.from_list(d))
    res = [0, 0] + x.get_data()
    return res


def get_d(h, c):
    d = [0]
    n = len(c) - 1
    for i in range(1, n):
        d.append((c[i + 1] - c[i]) / (3 * h[i]))
    d.append(-c[n] / (3 * h[n]))
    return d


def find_interval(points, x):
    for i in range(0, len(points) - 1):
        if points[i] <= x and x <= points[i + 1]:
            return i


def spline_interpolation(points, values, x):
    h = [0] + [points[i] - points[i - 1] for i in range(1, len(points))]
    c = get_c(points, values, h)
    a = get_a(values)
    b = get_b(values, h, c)
    d = get_d(h, c)
    logging.info("h: {0}".format(h))
    logging.info("a: {0}".format(a))
    logging.info("b: {0}".format(b))
    logging.info("c: {0}".format(c))
    logging.info("d: {0}".format(d))

    i = find_interval(points, x)
    logging.info("i = {0}: {1} <= {2} <= {3}".format(i, points[i], x, points[i + 1]))
    res = a[i + 1] + b[i + 1] * (x - points[i]) + c[i + 1] * ((x - points[i]) ** 2) + d[i + 1] * ((x - points[i]) ** 3)
    return res


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="3-2.log", level=logging.INFO)

    need_args = ('points', 'values','x')
    init_dict = read_data(args.input, need_args)
    points, vals, x = init_dict['points'], init_dict['values'], init_dict['x']

    val = spline_interpolation(points, vals, x)
    logging.info("Value: {0}".format(val))

    save_to_file(args.output, Spline=[val])