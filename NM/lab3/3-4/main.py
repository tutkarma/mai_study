import argparse
import logging

from utils import read_data, save_to_file


def find_interval(points, x):
    for i in range(0, len(points) - 1):
        if points[i] <= x and x <= points[i + 1]:
            return i


def first_derivative(x, y, x0):
    i = find_interval(x, x0)
    addend1 = (y[i + 1] - y[i]) / (x[i + 1] - x[i])
    addend2 = ((y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1]) - addend1) / \
              (x[i + 2] - x[i]) * (2 * x0 - x[i] - x[i + 1])
    return addend1 + addend2


def second_derivative(x, y, x0):
    i = find_interval(x, x0)
    num1 = (y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1])
    num2 = (y[i + 1] - y[i]) / (x[i+ 1] - x[i])
    return 2 * (num1 - num2) / (x[i + 2] - x[i])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    logging.basicConfig(filename="3-4.log", level=logging.INFO)

    need_args = ('points', 'values', 'x')
    init_dict = read_data(args.input, need_args)
    points, values, x = init_dict['points'], init_dict['values'], init_dict['x']

    logging.info("Numerical differentiation")
    logging.info("Points: {0}".format(points))
    logging.info("Values: {0}".format(values))
    logging.info("x: {0}".format(x))

    first_der = first_derivative(points, values, x)
    second_der = second_derivative(points, values, x)
    logging.info("First derivative: {0}".format(first_der))
    logging.info("Second derivative: {0}".format(second_der))

    save_to_file(args.output, first_der=first_der, second_der=second_der)