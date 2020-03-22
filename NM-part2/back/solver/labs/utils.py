import numpy as np


def tma(a, b, c, d):
    size = len(a)
    p, q = [], []
    p.append(-c[0] / b[0])
    q.append(d[0] / b[0])

    for i in range(1, size):
        p_tmp = -c[i] / (b[i] + a[i] * p[i - 1])
        q_tmp = (d[i] - a[i] * q[i - 1]) / (b[i] + a[i] * p[i - 1])
        p.append(p_tmp)
        q.append(q_tmp)

    x = [0 for _ in range(size)]
    x[size - 1] = q[size - 1]

    for i in range(size - 2, -1, -1):
        x[i] = p[i] * x[i + 1] + q[i]

    return x


def norm_inf(A):
    n = len(A)
    norm = 0
    for i in range(n):
        sum_ = 0
        for j in range(n):
            sum_ += abs(A[i][j])
        norm = sum_ if norm < sum_ else norm
    return norm


def norm_inf_vec(A):
    n = len(A)
    norm = 0
    for i in range(n):
        if abs(A[i]) > norm:
            norm = abs(A[i])
    return norm