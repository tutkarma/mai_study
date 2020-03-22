import numpy as np

from solver.labs.utils import norm_inf, norm_inf_vec


class EquationData:
    def __init__(self, params):
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

    def solve_analytic(self, N, l, eps):
        self.h = l / N;
        u = np.zeros((N, N))
        for x in range(N):
            for y in range(N):
                u[x][y] = self.data.solution(x * self.h, y * self.h)
        return u

    def solve(self, N, l, eps):
        self.h = l / N
        A, b = self._get_equation_system(N, l)
        return self.solve_func(N, A, b, eps)

    def _leibmann_solve(self, N, A, b, eps):
        n = len(A)
        alpha, beta = self._find_equivalent_system(A, b)
        alpha_norm = norm_inf(alpha)
        x = np.zeros(n)
        x[:] = beta

        while True:
            next_x = np.zeros(n)
            for i in range(n):
                sum_ = 0
                for j in range(n):
                    sum_ += alpha[i][j] * x[j]
                next_x[i] = beta[i] + sum_
            diff_x = next_x - x
            if alpha_norm < 1:
                end_cond = alpha_norm / (1 - alpha_norm) * norm_inf_vec(diff_x)
            elif alpha_norm == 1:
                end_cond = norm_inf_vec(diff_x)
            else:
                break
            x = next_x
            if end_cond < eps:
                break
        u = self._vector_to_matrix(x, N - 1)

        return u

    def _seidel_solve(self, N, A, b, eps):
        n = len(A)
        alpha, beta = self._find_equivalent_system(A, b)
        x = np.zeros(n)
        x[:] = beta

        E = np.zeros((n, n))
        B = np.zeros((n, n))
        C = np.zeros((n, n))

        for i in range(n):
            for j in range(n):
                if i == j:
                    E[i][j] = 1
                    C[i][j] = alpha[i][j]
                elif i < j:
                    C[i][j] = alpha[i][j]
                else:
                    B[i][j] = alpha[i][j]

        alpha_norm = norm_inf(alpha)

        while True:
            next_x = np.zeros(n)

            for i in range(n):
                sum_ = 0
                for j in range(i):
                    sum_ += alpha[i][j] * next_x[j]
                for j in range(i, n):
                    sum_ += alpha[i][j] * x[j]
                next_x[i] = beta[i] + sum_

            diff_x = next_x - x
            if alpha_norm < 1:
                end_cond = alpha_norm / (1 - alpha_norm) * norm_inf_vec(diff_x)
            elif alpha_norm == 1:
                end_cond = norm_inf_vec(diff_x)
            else:
                break
            x = next_x
            if end_cond < eps:
                break

        u = self._vector_to_matrix(x, N - 1)

        return u

    def _get_equation_system(self, N, l):
        sz = N - 1
        A = np.zeros((sz * sz, sz * sz))
        b = np.zeros(sz * sz)
        for i in range(sz):
            for j in range(sz):
                A[i * sz + j][i * sz + j] -= 4

                if i + 1 == sz:
                    A[i * sz + j][i * sz + j] += 1
                else:
                    A[i * sz + j][(i + 1) * sz + j] += 1

                if i - 1 == -1:
                    b[i * sz + j] -= self.data.phi0(j * self.h)
                else:
                    A[i * sz + j][(i - 1) * sz + j] += 1

                if j + 1 == sz:
                    A[i * sz + j][i * sz + j] += 1
                else:
                    A[i * sz + j][i * sz + (j + 1)] += 1

                if j - 1 == -1:
                    b[i * sz + j] -= self.data.phi2(i * self.h)
                else:
                    A[i * sz + j][i * sz + (j - 1)] += 1

        return A, b

    def _find_equivalent_system(self, A, b):
        n = len(A)
        alpha = np.zeros((n, n))
        beta = np.zeros(n)
        for i in range(n):
            if A[i][i] == 0:
                swap_flag = False
                for j in range(i + 1, n):
                    if A[j][i] != 0:
                        A[i], A[j] = A[j], A[i]
                        b[i], b[j] = b[j], b[i]
                        swap_flag = True
                        break
                if not swap_flag:
                    continue

            beta[i] = b[i] / A[i][i]
            for j in range(n):
                if i == j:
                    alpha[i][j] = 0
                else:
                    alpha[i][j] = -A[i][j] / A[i][i]

        return alpha, beta

    def _vector_to_matrix(self, vec, sz):
        u = np.zeros((sz, sz))
        for i in range(sz):
            u[i] = vec[i * sz:(i + 1) * sz]
        return u
