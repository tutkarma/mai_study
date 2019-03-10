from random import randint
from math import gcd, pi
from functools import reduce
from datetime import datetime

CHECK_NUM = 1598756544210860812002683252504666631284038535154979340910964824673923578639226397918134429192737005854188177977059177858243855990803981275665690912975534091041361701843465578101733863479781680791655959578320442108371634048374313524202193198694894536452471646868825144743014452957912743920239954473534374422647748020165306769379396190044599513110393062461302839244356754741065320775011514774723155863731595182892822790709843296375075272651902641460504103291775361


def f(x, a, b):
    return a * x ** 2 + b


def is_prime(N):
    if N in (0, 1):
        return False
    if N == 2:
        return True
    if N % 2 == 0:
        return False
    s = N - 1
    while s % 2 == 0:
        s //= 2
    for i in range(50):
        a = randint(1, N - 1)
        exp = s
        mod = pow(a, exp, N)
        while exp != N - 1 and mod != 1 and mod != N - 1:
            mod = mod * mod % N
            exp *= 2
        if mod != N - 1 and exp % 2 == 0:
            return False
    return True


def find_factor(n):
    maxiterssq = pi / 4 * n
    x = randint(1, n - 1)
    y = x
    d = 1
    iters = 0
    a = randint(1, n - 1)
    b = randint(1, n - 1)
    while d in (1, n):
        if iters ** 2 > maxiterssq:
            a = randint(1, n - 1)
            b = randint(1, n - 1)
            x = randint(1, n - 1)
            y = x
            iters = 0
        x = f(x, a, b) % n
        y = f(f(y, a, b), a, b) % n
        d = gcd(abs(x - y), n)
        iters += 1
    return d


def find_prime_factor(n, factors):
    if is_prime(n):
        factors.append(n)
    else:
        tmp = n // find_factor(n)
        find_prime_factor(tmp, factors)


def factor(n, factors):
    while n % 2 == 0:
        factors.append(2)
        n //= 2
    while n % 3 == 0:
        factors.append(3)
        n //= 3
    while n > 1:
        find_prime_factor(n, factors)
        n //= factors[-1]


def find_all_factors(prime_factors, all_factors):
    all_factors.append(1)
    all_factors.append(prime_factors[0])
    for i in range(1, len(prime_factors)):
        tmp = []
        for f in all_factors:
            if f * prime_factors[i] not in all_factors:
                tmp.append(f * prime_factors[i])
        all_factors += tmp
    all_factors.sort()


def pollard_rho(n):
    factors = []
    factor(n, factors)
    factors.sort()
    all_factors = []
    find_all_factors(factors, all_factors)
    return all_factors


def get_info(num, factors, start_time):
    print("Original number: {0}".format(num))
    print("Factors:")
    print(*factors, sep='\n')
    print("Time: {0}".format(datetime.now() - start_time))


if __name__ == '__main__':
    n = 0
    with open('test1', 'r') as file:
        n = int(file.read())
    start_time = datetime.now()
    factors = pollard_rho(n)
    factors = factors[1:len(factors) - 2]
    get_info(n, factors, start_time)

    factors.clear()
    with open('test2', 'r') as file:
        n = int(file.read())
    start_time = datetime.now()
    factors.append(gcd(n, CHECK_NUM))
    factors.append(n / factors[0])
    get_info(n, factors, start_time)
