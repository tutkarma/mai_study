from math import sqrt


def get_info(method, coefs, x0, h, eps):
    print(method)
    print("f(x) = {0}x1^2 - x1x2 + {1}x2^2 + {2}x1 + {3}x2 + 13".format(
        coefs[2], coefs[3], coefs[0], coefs[1]))
    print("x_0 = ({0} {1})^T".format(x0[0], x0[1]))
    print("h = {0}".format(h))
    print("eps = {0}".format(eps))


def grad(x, coefs):
    dx1 = coefs[2] * 2 * x[0] + x[1] + coefs[0]
    dx2 = coefs[3] * 2 * x[1] + x[0] + coefs[1]
    return [dx1, dx2]


def grad_val(x, coefs):
    dx = grad(x, coefs)
    return sqrt(dx[0] ** 2 + dx[1] ** 2)


def f(x, coefs):
    return coefs[2] * x[0]**2 - x[0] * x[1] + coefs[3] * x[1]**2 + coefs[0] * x[0] + coefs[1] * x[1] + 13


def finish_iter_proccess(x, coefs, eps):
    print("Сhecking the condition of the end iterative process:")
    sign = "<" if abs(grad_val(x, coefs)) < eps else ">="
    print("{0} {1} {2}". format(grad_val(x, coefs), sign, eps))
    return abs(grad_val(x, coefs)) < eps


def classical_method(coefs, x_0, h, eps):
    get_info("Calssical method", coefs, x_prev, h, eps)


def gradient_descent(coefs, x_prev, h, eps):
    get_info("Gradient descent method", coefs, x_prev, h, eps)

    k = 0
    while not finish_iter_proccess(x_prev, coefs, eps):
        print("---------")
        print("Iteration #{}".format(k))

        gr = grad(x_prev, coefs)
        print("x = {0} - {1} * {2}".format(x_prev, h, gr))
        x = [i - h * j for i, j in zip(x_prev, gr)]

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


def quickest_descent(coefs, x_0, h, eps):
    get_info("Quickest descent method", coefs, x_prev, h, eps)


if __name__ == '__main__':
    '''x1 = int(input("Coefficient x1 = "))
    x2 = int(input("Coefficient x2 = "))
    x12 = int(input("Coefficient x1^2 = "))
    x22 = int(input("Coefficient x2^2 = "))
    coefs = (x1, x2, x12, x22)
    x0 = list(map(int, input("Vector x_0 (two values) = ")))
    h = int(input("h = "))
    eps = int(input("eps = "))'''
    coefs = [14, -7, 7, 7] # x1, x2, x1^2, x2^2
    x_0 = [2, 3]
    h = 0.1
    eps = 0.9
    #classical_method(coefs, x_0, h, eps)
    gradient_descent(coefs, x_0, h, eps)
    #quickest_descent(coefs, x_0, h, eps)