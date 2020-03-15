import numpy as np

from solver.labs.utils import tma


class EquationData:
    def __init__(self, params):
        self.a = params['a']
        self.b = params['b']
        self.c = params['c']
        self.l = params['l']
        self.f = params['f']
        self.alpha = params['alpha']
        self.beta = params['beta']
        self.gamma = params['gamma']
        self.delta = params['delta']
        self.psi = params['psi']
        self.phi0 = params['phi0']
        self.phil = params['phil']
        self.bound_type = params['bound_type']
        self.solution = params['solution']


class ParabolicSolver:
    def __init__(self, params, equation_type):
        self.data = EquationData(params)
        try:
            self.solve_func = getattr(self, f'_{equation_type}_solve')
        except:
            raise Exception("This type does not exist")

    def solve(self, N, K, T):
        self.h = self.data.l / N;
        self.tau = T / K;
        self.sigma = self.tau / (self.h ** 2)
        return self.solve_func(N, K, T)

    def solve_analytic(self, N, K, T):
        self.h = self.data.l / N;
        self.tau = T / K;
        self.sigma = self.tau / (self.h ** 2)
        u = np.zeros((K, N))
        for k in range(K):
            for j in range(N):
                u[k][j] = self.data.solution(j * self.h, k * self.tau)
        return u

    def _implicit_solve(self, N, K, T):
        a = np.zeros(N)
        b = np.zeros(N)
        c = np.zeros(N)
        d = np.zeros(N)
        u = np.zeros((K, N))

        for i in range(1, N - 1):
            u[0][i] = self.data.psi(i * self.h)
        u[0][-1] = 0

        for k in range(1, K):
            for j in range(1, N - 1):
                a[j] = self.sigma
                b[j] = -(1 + 2 * self.sigma)
                c[j] = self.sigma
                d[j] = -u[k - 1][j] - self.tau * self.data.f(j * self.h, k * self.tau)

            if self.data.bound_type == 'a1p1':
                a[0] = 0
                b[0] = -(1 + 2 * self.sigma)
                c[0] = self.sigma
                d[0] = -(u[k - 1][0] + self.sigma * self.data.phi0(k * self.tau))
                a[-1] = self.sigma
                b[-1] = -(1 + 2 * self.sigma)
                c[-1] = 0
                d[-1] = -(u[k - 1][-1] + self.sigma * self.data.phil(k * self.tau))
            elif self.data.bound_type == 'a1p2':
                a[0] = 0
                b[0] = -(1 + 2 * self.sigma)
                c[0] = self.sigma
                d[0] = -(u[k - 1][0] + self.sigma * self.data.phi0(k * self.tau)) - self.tau * self.data.f(0, k * self.tau)
                a[-1] = self.sigma
                b[-1] = -(1 + 2 * self.sigma)
                c[-1] = 0
                d[-1] = -(u[k - 1][-1] + self.sigma * self.data.phil(k * self.tau)) - self.tau * self.data.f((N - 1) * self.h, k * self.tau)
            elif self.data.bound_type == 'a1p3':
                a[0] = 0
                b[0] = -(1 + 2 * self.sigma)
                c[0] = self.sigma
                d[0] = -((1 - self.sigma) * u[k - 1][1] + self.sigma / 2 * u[k - 1][0]) - self.tau * f(0, k * self.tau) - self.sigma * self.data.phi0(k * self.tau)
                a[-1] = self.sigma
                b[-1] = -(1 + 2 * self.sigma)
                c[-1] = 0
                d[-1] = self.data.phil(k * self.tau) + self.data.f((N - 1) * self.h, k * self.tau) * self.h / (2 * self.tau) * u[k - 1][-1]

            u[k] = tma(a, b, c, d)

        return u

    def _explicit_solve(self, N, K, T):
        u = np.zeros((K, N))
        for i in range(1, N - 1):
            u[0][i] = self.data.psi(i * self.h)
        u[0][-1] = 0

        for k in range(1, K):
            u[k][0] = self.phi0(k * self.tau)
            for j in range(1, N - 1):
                u[k][j] = self.sigma * u[k - 1][j + 1] + \
                    (1 - 2 * self.sigma) * u[k - 1][j] + \
                    self.sigma * u[k - 1][j - 1] + \
                    self.tau * self.data.f(j * self.h)

            if self.data.bound_type == 'a1p1':
                u[k][-1] = u[k][-2] + self.data.phil(k * self.tau) * self.h
            elif self.data.bound_type == 'a1p2':
                u[k][-1] = (self.data.phil(k * self.tau) * 2 * self.h - u[k][-3] + 4 * u[k][-2]) / 3
            elif self.data.bound_type == 'a1p3':
                u[k][-1] = (self.data.phil(k * self.tau) + u[k][-2] / self.h + 2 * self.tau * u[k - 1][-1] / self.h) / (1 / self.h + 2 * self.tau / self.h)

        return u

    def _crank_nicholson_solve(self, N, K, T):
        theta = 0.5
        a = np.zeros(N)
        b = np.zeros(N)
        c = np.zeros(N)
        d = np.zeros(N)
        tmp_imp = np.zeros(N)
        u = np.zeros((K, N))
        for i in range(1, N - 1):
            u[0][j] = self.data.psi(i * self.h)
        u[0][-1] = 0

        for k in range(1, K):
            for j in range(1, N - 1):
                a[j] = self.sigma
                b[j] = -(1 + 2 * self.sigma)
                c[j] = self.sigma
                d[j] = -u[k - 1][j] - self.tau * self.data.f(j * self.h, k * self.tau)

            if self.data.bound_type == 'a1p1':
                a[0] = 0
                b[0] = -(1 + 2 * self.sigma)
                c[0] = self.sigma
                d[0] = -(u[k - 1][0] + self.sigma * self.data.phi0(k * self.tau))
                a[-1] = self.sigma
                b[-1] = -(1 + 2 * self.sigma)
                c[-1] = 0
                d[-1] = -(u[k - 1][-1] + self.sigma * self.data.phil(k * self.tau))
            elif self.data.bound_type == 'a1p2':
                a[0] = 0
                b[0] = -(1 + 2 * self.sigma)
                c[0] = self.sigma
                d[0] = -(u[k - 1][0] + self.sigma * self.data.phi0(k * self.tau)) - self.tau * self.data.f(0, k * self.tau)
                a[-1] = self.sigma
                b[-1] = -(1 + 2 * self.sigma)
                c[-1] = 0
                d[-1] = -(u[k - 1][-1] + self.sigma * self.data.phil(k * self.tau)) - self.tau * self.data.f((N - 1) * self.h, k * self.tau)
            elif self.data.bound_type == 'a1p3':
                a[0] = 0
                b[0] = -(1 + 2 * self.sigma)
                c[0] = self.sigma
                d[0] = -((1 - self.sigma) * u[k - 1][1] + self.sigma / 2 * u[k - 1][0]) - self.tau * f(0, k * self.tau) - self.sigma * self.data.phi0(k * self.tau)
                a[-1] = self.sigma
                b[-1] = -(1 + 2 * self.sigma)
                c[-1] = 0
                d[-1] = self.data.phil(k * self.tau) + self.data.f((N - 1) * self.h, k * self.tau) * self.h / (2 * self.tau) * u[k - 1][-1]

            tmp_imp = tma(a, b, c, d)

        tmp_exp = np.zeros(N)
        tmp_exp[0] = self.data.phi0(0)
        for j in range(1, N - 1):
            tmp_exp[j] = self.sigma * u[k - 1][j + 1] + \
                        (1 - 2 * self.sigma) * u [k - 1][j] + \
                        self.sigma * u[k - 1][j - 1] + self.tau * self.data.f(j * self.h, k * self.tau)
        tmp_exp[-1] = 0

        for j in range(N):
            u[k][j] = theta * tmp_imp[j] + (1 - theta) * tmp_exp[j]

        return u
