import numpy as np

from solver.labs.utils import norm_inf


class EquationData:
    def __init__(self, params):
        self.x = params['x']
        self.y = params['y']
        self.a = params['a']
        self.b = params['b']
        self.c = params['c']
        self.d = params['d']
        self.phi0 = params['phi0']
        self.phi1 = params['phi1']
        self.phi2 = params['phi2']
        self.phi3 = params['phi3']
        self.solution = params['solution']


class EllipticSolver:
    def __init__(self, params, equation_type):
        self.data = EquationData(params)
        try:
            self.solve_func = getattr(self, f'_{equation_type}_solve')
        except:
            raise Exception("This type does not exist")

    def solve(self, N, l, eps):
        self.h = l / N;
        u1 = np.zeros((N, N))
        u2 = np.zeros((N, N))

        for j in range(0, N):
            y = j * self.h
            u1[0][j] = u2[0][j] = self.data.phi0(y)
            u1[-1][j] = u2[-1][j] = self.data.phi1(y)

        for i in range(0, N):
            x = i * self.h
            u1[i][0] = u2[i][0] = self.data.phi2(x)
            u1[i][-1] = u2[i][-1] = self.data.phi3(x)

        self.a = -(2 / self.h ** 2 + 2 * self.data.a / self.h ** 2 + self.data.d)
        self.b = self.data.b / (2 * self.h) - 1 / self.h ** 2
        self.c = -(self.data.b / (2 * self.h) + 1 / self.h ** 2)
        self.d = self.data.c / (2 * self.h) - self.data.a / self.h ** 2
        self.e = -(self.data.a / self.h ** 2 + self.data.c / (2 * self.h))

        return self.solve_func(N, l, eps, u1, u2)

    def solve_analytic(self, N, l, eps):
        self.h = l / N;
        u = np.zeros((N, N))
        for x in range(N):
            for y in range(N):
                u[x][y] = self.data.solution(x * self.h, y * self.h)
        return u


    def _simple_solve(self, N, l, eps, u1, u2):
        while norm_inf(np.abs(u2 - u1)) >= eps:
            for i in range(1, N - 1):
                for j in range(1, N - 1):
                    u2[i][j] = 1 / 4 * (u1[i + 1][j] + u1[i - 1][j] +
                                    u1[i][j + 1] + u1[i][j - 1])

            u1, u2 = u2, u1
        return u1


    def _leibmann_solve(self, N, l, eps, u1, u2):
        while norm_inf(np.abs(u2 - u1)) >= eps:
            for i in range(1, N - 1):
                for j in range(1, N - 1):
                    u2[i][j] = self.b * u1[i + 1][j] + self.c * u1[i - 1][j] + \
                           self.d * u1[i][j + 1] + self.e * u1[i][j - 1]
                    u2[i][j] /= self.a

            u1, u2 = u2, u1
        return u1

    def _seidel_solve(self, N, l, eps, u1, u2):
        while norm_inf(np.abs(u2 - u1)) >= eps:
            for i in range(1, N - 1):
                for j in range(1, N - 1):
                    u2[i][j] = self.b * u1[i + 1][j] + self.c * u2[i - 1][j] + \
                           self.d * u1[i][j + 1] + self.e * u2[i][j - 1]
                    u2[i][j] /= self.a
            u1, u2 = u2, u1
        return u1

    def _sor_solve(self, N, l, eps, u1, u2):
        omega = 1.5
        while norm_inf(np.abs(u2 - u1)) >= eps:
            for i in range(1, N - 1):
                for j in range(1, N - 1):
                    u2[i][j] = self.b * u1[i + 1][j] + self.c * u2[i - 1][j] + \
                           self.d * u1[i][j + 1] + self.e * u2[i][j - 1]
                    u2[i][j] /= self.a
                    u2[i][j] = omega * u2[i][j] + (1 - omega) * u1[i][j]
            u1, u2 = u2, u1
        return u1
