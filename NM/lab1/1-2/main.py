import argparse

import numpy as np


class TridiagonalMatrix:
    def __init__(self):
        self.size = 0
        self.a = []
        self.b = []
        self.c = []
        self.d = []

    def read_matrix(self, filename):
        with open(filename, 'r') as f:
            self.size = int(f.readline())
            for i in range(self.size):
                if i == 0:
                    a, b, c, d = [0] + list(map(int, f.readline().split()))
                elif i == self.size - 1:
                    a, b, d, c = list(map(int, f.readline().split())) + [0]
                else:
                    a, b, c, d = list(map(int, f.readline().split()))
                self.a.append(a)
                self.b.append(b)
                self.c.append(c)
                self.d.append(d)

    def debug_print(self):
        for i in range(self.size):
            print("{0} {1} {2} {3}".format(self.a[i], 
                self.b[i], self.c[i], self.d[i]))


def tma(mat):
    p, q, x = [], [], [0] * (mat.size)
    p.append(-mat.c[0] / mat.b[0])
    q.append(mat.d[0] / mat.b[0])

    for i in range(1, mat.size):
        p_i = 0 if i == mat.size - 1 else (-mat.c[i] / (mat.b[i] + mat.a[i] * p[i - 1]))
        q_i = (mat.d[i] - mat.a[i] * q[i - 1]) / (mat.b[i] + mat.a[i] * p[i - 1])
        p.append(p_i)
        q.append(q_i)

    x[mat.size - 1] = q[mat.size - 1]
    for i in range(mat.size - 2, -1, -1):
        x[i] = p[i] * x[i + 1] + q[i]

    return x


def print_answer(filename, x):
    with open(filename, 'w') as f:
        for i, el in enumerate(x):
            f.write('x{0} = {1}\n'.format(i + 1, el))


def benchmark():
    pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')

    args = parser.parse_args()

    mat = TridiagonalMatrix()
    mat.read_matrix(args.input)
    x = tma(mat)
    print_answer(args.output, x)