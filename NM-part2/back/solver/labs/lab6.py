import numpy as np

from solver.labs.utils import tma


class EquationData:
    def __init__(self, params):
        self.a = params['a']
        self.b = params['b']
        self.c = params['c']
        self.d = params['d']
        self.l = params['l']
        self.f = params['f']
        self.alpha = params['alpha']
        self.beta = params['beta']
        self.gamma = params['gamma']
        self.delta = params['delta']
        self.psi1 = params['psi1']
        self.psi2 = params['psi2']
        self.psi1_dir1 = params['psi1_dir1']
        self.psi1_dir2 = params['psi1_dir2']
        self.phi0 = params['phi0']
        self.phil = params['phil']
        self.bound_type = params['bound_type']
        self.approximation = params['approximation']
        self.solution = params['solution']


class HyperbolicSolver:
    def __init__(self, params, equation_type):
        self.data = EquationData(params)
        try:
            self.solve_func = getattr(self, f'_{equation_type}_solve')
        except:
            raise Exception("This type does not exist")

    def solve(self, N, K, T):
        self.h = self.data.l / N;
        self.tau = T / K;
        self.sigma = (self.tau ** 2) / (self.h ** 2)
        return self.solve_func(N, K, T)

    def solve_analytic(self, N, K, T):
        self.h = self.data.l / N;
        self.tau = T / K;
        self.sigma = (self.tau ** 2) / (self.h ** 2)
        u = np.zeros((K, N))
        for k in range(K):
            for j in range(N):
                u[k][j] = self.data.solution(j * self.h, k * self.tau)
        return u

    def _implicit_solve(self, N, K, T):
        u = np.zeros((K, N))

        for j in range(N):
            x = j * self.h
            u[0][j] = self.data.psi1(x)

            if self.data.approximation == 'p1':
                u[1][j] = self.data.psi1(x) + self.data.psi2(x) * self.tau + self.data.psi1_dir2(x) * self.tau ** 2 / 2
            elif self.data.approximation == 'p2':
                k = self.tau ** 2 / 2
                u[1][j] = (1 + self.data.c * k) * self.data.psi2(x) + \
                    self.data.a * k * self.data.psi1_dir2(x) + \
                    self.data.b * k * self.data.psi1_dir1(x) + \
                    (self.tau - self.data.d * k) * self.data.psi1(x) + \
                    k * self.data.f()

        a = np.zeros(N)
        b = np.zeros(N)
        c = np.zeros(N)
        d = np.zeros(N)

        for k in range(2, K):
            for j in range(1, N - 1):
                a[j] = self.sigma
                b[j] = -(1 + 2 * self.sigma)
                c[j] = self.sigma
                d[j] = -2 * u[k - 1][j] + u[k - 2][j]

            if self.data.bound_type == 'a1p2':
                b[0] = self.data.alpha / self.h / (self.data.beta - self.data.alpha / self.h)
                c[0] = 1
                d[0] = 1 / (self.data.beta - self.data.alpha / self.h) * self.data.phi0(k * self.tau)
                a[-1] = -self.data.gamma / self.h / (self.data.delta + self.data.gamma / self.h)
                d[-1] = 1 / (self.data.delta + self.data.gamma / self.h) * self.data.phil(k * self.tau)
            elif self.data.bound_type == 'a2p3':
                k1 = 2 * self.h * self.data.beta - 3 * self.data.alpha
                k2 = 2 * self.h * self.data.delt + 3 * self.data.gamma
                omega = self.tau ** 2 * self.data.b / (2 * self.h)
                xi = self.data.d * self.tau / 2

                b[0] = 4 * self.data.alpha - self.data.alpha / (self.sigma + omega) * (1 + xi + 2 * self.sigma - self.data.c * self.tau ** 2)
                c[0] = k1 - self.data.alpha * (omega - self.sigma) / (omega + self.sigma)
                d[0] = 2 * self.h * self.data.phi0(k * self.tau) + self.data.alpha * d[1] / (-sigma - omega)
                a[-1] = -self.data.gamma / (omega - sel.sigma) * (1 + xi + 2 * self.sigma - self.data.c * self.tau ** 2) - 4 * self.data.gamma
                d[-1] = 2 * self.h * self.data.phil(k * self.tau) - self.data.gamma * d[-2] / (omega - self.sigma)
            elif self.data.bound_type == 'a2p2':
                b[0] = 2 * self.data.a / self.h
                c[0] = -2 * self.data.a / self.h + self.h / self.tau ** 2 - self.data.c * self.h + \
                    -self.data.d * self.h / (2 * self.tau) + \
                    self.data.beta / self.data.alpha * (2 * self.data.a + self.data.b * self.h)
                d[0] = self.h / self.tau ** 2 * (u[k - 2][0] - 2 * u[k - 1][0]) - self.h * self.data.f() + \
                    -self.data.d * self.h / (2 * self.tau) * u[k - 2][0] + \
                    (2 * self.data.a - self.data.b * self.h) / self.data.alpha * self.data.phi0(k * self.tau)
                a[-1] =-b[0]
                d[-1] = self.h / self.tau ** 2 * (-u[k - 2][0] + 2 * u[k - 1][0]) + self.h * self.data.f() + \
                    self.data.d * self.h / (2 * self.tau) * u[k - 2][0] + \
                    (2 * self.data.a + self.data.b * self.h) / self.data.alpha * self.data.phil(k * self.tau)

            u[k] = tma(a, b, c, d)

        return u

    def _left_bound_a1p2(self, u, k, t):
        return -(self.data.alpha / self.h) / (self.data.beta - self.data.alpha / self.h) * u[k - 1][1] \
               + self.data.phi0(t) / (self.data.beta - self.data.alpha / self.h)

    def _right_bound_a1p2(self, u, k, t):
        return (self.data.gamma / self.h) / (self.data.delta + self.data.gamma / self.h) * u[k - 1][-2] \
               + self.data.phil(t) / (self.data.delta + self.data.gamma / self.h)

    def _left_bound_a2p3(self, u, k, t):
        denom = 2 * self.h * self.data.beta - 3 * self.data.alpha
        return self.data.alpha / denom * u[k - 1][2] - 4 * self.data.alpha / denom * u[k - 1][1] \
               + 2 * self.h / denom * self.data.phi0(t)

    def _right_bound_a2p3(self, u, k, t):
        denom = 2 * self.h * self.data.delta + 3 * self.data.gamma
        return 4 * self.data.gamma / denom * u[k - 1][-2] - self.data.gamma / denom * u[k - 1][-3] \
               + 2 * self.h / denom * self.data.phil(t)

    def _left_bound_a2p2(self, u, k, t):
        n = self.data.c * self.h - 2 * self.data.a / self.h - self.h / self.tau ** 2 - self.data.d * self.h / (2 * self.tau) + \
            self.data.beta / self.data.alpha * (2 * self.data.a - self.data.b * self.h)
        return 1 / n * (- 2 * self.data.a / self.h * u[k][1] +
                        self.h / self.tau ** 2 * (u[k - 2][0] - 2 * u[k - 1][0]) +
                        -self.data.d * self.h / (2 * self.tau) * u[k - 2][0] +
                        -self.h * self.data.f() +
                        (2 * self.data.a - self.data.b * self.h) / self.data.alpha * self.data.phi0(t))

    def _right_bound_a2p2(self, u, k, t):
        n = -self.data.c * self.h + 2 * self.data.a / self.h + self.h / self.tau ** 2 + self.data.d * self.h / (2 * self.tau) + \
            self.data.delta / self.data.gamma * (2 * self.data.a + self.data.b * self.h)
        return 1 / n * (2 * self.data.a / self.h * u[k][-2] +
                        self.h / self.tau ** 2 * (2 * u[k - 1][-1] - u[k - 2][-1]) +
                        self.data.d * self.h / (2 * self.tau) * u[k - 2][-1] +
                        self.h * self.data.f() +
                        (2 * self.data.a + self.data.b * self.h) / self.data.gamma * self.data.phil(t))

    def _explicit_solve(self, N, K, T):
        u = np.zeros((K, N))

        for j in range(N):
            x = j * self.h
            u[0][j] = self.data.psi1(x)

            if self.data.approximation == 'p1':
                u[1][j] = self.data.psi1(x) + self.data.psi2(x) * self.tau + \
                    self.data.psi1_dir2(x) * self.tau ** 2 / 2
            elif self.data.approximation == 'p2':
                k = self.tau ** 2 / 2
                u[1][j] = (1 + self.data.c * k) * self.data.psi2(x) + \
                    self.data.a * k * self.data.psi1_dir2(x) + \
                    self.data.b * k * self.data.psi1_dir1(x) + \
                    (self.tau - self.data.d * k) * self.data.psi1(x) + \
                    k * self.data.f()

        if self.data.bound_type == 'a1p2':
            left_bound = self._left_bound_a1p2
            right_bound = self._right_bound_a1p2
        elif self.data.bound_type == 'a2p3':
            left_bound = self._left_bound_a2p3
            right_bound = self._right_bound_a2p3
        elif self.data.bound_type == 'a2p2':
            left_bound = self._left_bound_a2p2
            right_bound = self._right_bound_a2p2

        for k in range(2, K):
            t = k * self.tau
            for j in range(1, N - 1):
                u[k][j] = self.sigma * u[k - 1][j + 1] + \
                    (2 - 2 * self.sigma) * u[k - 1][j] + \
                    self.sigma * u[k - 1][j - 1] - \
                    u[k - 2][j]

            u[k][0] = left_bound(u, k, t)
            u[k][-1] = right_bound(u, k, t)

        return u
