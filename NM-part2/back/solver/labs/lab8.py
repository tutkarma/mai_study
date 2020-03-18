import numpy as np

from solver.labs.utils import tma


class EquationData:
    def __init__(self, params):
        self.f = params['f']
        self.l1 = params['l1']
        self.l2 = params['l2']
        self.psi = params['psi']
        self.phi0 = params['phi0']
        self.phi1 = params['phi1']
        self.phi2 = params['phi2']
        self.phi3 = params['phi3']
        self.solution = params['solution']


class Parabolic2DSolver:
    def __init__(self, params, equation_type):
        self.data = EquationData(params)
        try:
            self.solve_func = getattr(self, f'_{equation_type}_solve')
        except:
            raise Exception("This type does not exist")

    def solve(self, N1, N2, K, T):
        self.tau = T / K
        self.h1 = self.data.l1 / N1;
        self.h2 = self.data.l2 / N2
        self.sigma = self.tau / (self.h1 ** 2)
        self.omega = self.tau / (self.h2 ** 2)
        return self.solve_func(N1, N2, K, T)

    def solve_analytic(self, N1, N2, K, T):
        self.tau = T / K
        self.h1 = self.data.l1 / N1
        self.sigma1 = self.tau / self.h1 ** 2
        self.h2 = self.data.l2 / N2
        self.sigma2 = self.tau / self.h2 ** 2

        u_x = np.zeros((K, N1))

        for k in range(K):
            for j in range(N1):
                u_x[k][j] = self.data.solution(j * self.h1, 0.1, k * self.tau)

        u_y = np.zeros((K, N2))
        for k in range(K):
            for i in range(N2):
                u_y[k][i] = self.data.solution(0.1, i * self.h2, k * self.tau)

        return {'grid_x': u_x.tolist(), 'grid_y': u_y.tolist()}

    def _alter_directions_solve(self, N1, N2, K, T):
        ax = np.zeros(N1)
        bx = np.zeros(N1)
        cx = np.zeros(N1)
        dx = np.zeros(N1)

        ay = np.zeros(N2)
        by = np.zeros(N2)
        cy = np.zeros(N2)
        dy = np.zeros(N2)

        for i in range(N1):
            ax[i] = self.sigma
            bx[i] = -(1 + 2 * self.sigma)
            cx[i] = self.sigma

        for j in range(N2):
            ay[j] = self.omega
            by[j] = -(1 + 2 * self.omega)
            cy[j] = self.omega

        u1 = np.zeros((N1, N2))
        u2 = np.zeros((N1, N2))
        u3 = np.zeros((N1, N2))
        ux = np.zeros(N1)
        uy = np.zeros(N2)

        results = []

        for i in range(N1):
            for j in range(N2):
                u1[i][j] = self.data.psi(i * self.h1, j * self.h2)

        for k in range(1, N1):
            tk1 = (k + 0.5) * self.h1
            tk2 = (k + 1) * self.h1

            for j in range(1, N2 - 1):
                yj = j * self.h2

                dx[0] = self.data.phi0(yj, tk2)
                dx[-1] = self.h1 * self.data.phi1(yj, tk2)

                for i in range(1, N1 - 1):
                    xi = i * self.h1
                    dx[i] = -self.omega * u1[i][j + 1] \
                            + (2 * self.omega - 1) * u1[i][j] \
                            - self.omega * u1[i][j - 1] \
                            - self.h1 / 2 * self.data.f(xi, yj, tk2)

                ux = tma(ax, bx, cx, dx)
                for i in range(N1):
                    u2[i][j] = ux[i]

            for i in range(N1):
                u2[i][0] = 0
                u2[i][-1] = 0

            for i in range(N1):
                u2[i][0] = self.data.phi2(i * self.h1, tk1)
                u2[i][-1] = u2[i][-2] + self.h2 * self.data.phi3(i * self.h1, tk1)


            for i in range(1, N1 - 1):
                xi = i * self.h1

                dy[0] = self.data.phi2(xi, tk2)
                dy[-1] = self.h2 * self.data.phi3(xi, tk2)

                for j in range(1, N2 - 1):
                    yj = j * self.h2
                    dy[j] = -self.sigma * u2[i + 1][j] \
                            + (2 * self.sigma - 1) * u2[i][j] \
                            - self.sigma * u2[i - 1][j] \
                            - self.h1 / 2 * self.data.f(xi, yj, tk2)

                uy = tma(ay, by, cy, dy)
                for j in range(N2):
                    u3[i][j] = uy[j]

            for j in range(N2):
                u3[0][j] = 0
                u3[-1][j] = 0

            for j in range(N2):
                u3[0][j] = self.data.phi0(j * self.h2, tk2)
                u3[-1][j] = u3[-2][j] + self.h1 * self.data.phi1(j * self.h2, tk2)

            results.append(np.copy(u3))

            u1, u3 = u3, u1

        return results[-2]

    def _fract_steps_solve(self, N1, N2, K, T):
        ax = np.zeros(N1)
        bx = np.zeros(N1)
        cx = np.zeros(N1)
        dx = np.zeros(N1)

        ay = np.zeros(N2)
        by = np.zeros(N2)
        cy = np.zeros(N2)
        dy = np.zeros(N2)

        for i in range(N1):
            ax[i] = self.sigma
            bx[i] = -(2 * self.sigma + 1)
            cx[i] = self.sigma

        for j in range(N2):
            ay[j] = self.omega
            by[j] = - (2 * self.omega + 1)
            cy[j] = self.omega

        u1 = np.zeros((N1, N2))
        u2 = np.zeros((N1, N2))
        u3 = np.zeros((N1, N2))
        ux = np.zeros(N1)
        uy = np.zeros(N2)

        results = []

        for i in range(N1):
            for j in range(N2):
                u1[i][j] = self.data.psi(i * self.h1, j * self.h2)

        for k in range(1, N1):
            tk1 = (k + 0.5) * self.h1
            tk2 = (k + 1) * self.h1

            for j in range(1, N2 - 1):
                yj = j * self.h2

                dx[0] = self.data.phi0(yj, tk2)
                dx[-1] = self.h1 * self.data.psi(yj, tk2)

                for i in range(1, N1 - 1):
                    xi = i * self.h1
                    dx[i] = -self.h1 / 2 * self.data.f(xi, yj, tk2)

                ux = tma(ax, bx, cx, dx)
                for i in range(N1):
                    u2[i][j] = ux[i]

            for i in range(N1):
                u2[i][0] = 0
                u2[i][-1] = 0

            for i in range(N1):
                u2[i][0] = self.data.phi2(i * self.h1, tk1)
                u2[i][-1] = u2[i][-2] + self.h2 * self.data.phi3(i * self.h1, tk1)

            for i in range(1, N1 - 1):
                xi = i * self.h1

                dy[0] = self.data.phi2(xi, tk2)
                dy[-1] = self.h2 * self.data.phi3(xi, tk2)

                for j in range(1, N2 - 1):
                    yj = j * self.h2
                    dy[j] = -self.h1 / 2 * self.data.f(xi, yj, tk2)

                uy = tma(ay, by, cy, dy)
                for j in range(N2):
                    u3[i][j] = uy[j]

            for j in range(N2):
                u3[0][j] = 0
                u3[-1][j] = 0

            for j in range(N2):
                u3[0][j] = self.data.phi0(j * self.h2, tk2)
                u3[-1][j] = u3[-2][j] + self.h1 * self.data.phi1(j * self.h2, tk2)

            results.append(np.copy(u3))

            u1, u3 = u3, u1

        return results[-2]
