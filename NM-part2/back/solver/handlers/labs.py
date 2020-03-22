import json

import numpy as np
from werkzeug.exceptions import BadRequest
from flask import Blueprint, request, Response
from flask_cors import CORS

from solver.labs.lab5 import ParabolicSolver
from solver.labs.lab6 import HyperbolicSolver
from solver.labs.lab7 import EllipticSolver
from solver.labs.lab8 import Parabolic2DSolver


labs = Blueprint('labs', __name__)
CORS(labs)


@labs.route('/<int(min=5, max=8):lab_id>', methods=['POST'])
def get_solution(lab_id):
    data = request.get_json(force=True)

    if lab_id == 5:
        resp = solve_lab5(data)
    elif lab_id == 6:
        resp = solve_lab6(data)
    elif lab_id == 7:
        resp = solve_lab7(data)
    elif lab_id == 8:
        resp = solve_lab8(data)

    return Response(
        json.dumps(resp),
        status=200
    )


def solve_lab5(data):
    equation_type = data['equation_type']
    N, K, T = int(data['N']), int(data['K']), int(data['T'])

    params = {
        'l': np.pi,
        'psi': lambda x: np.sin(x),
        'f': lambda x, t: 0.5 * np.exp(-0.5 * t) * np.cos(x),
        'phi0': lambda t: -np.exp(-0.5 * t), # если брать значение из методички, то график строится неправильно
        'phil': lambda t: -np.exp(-0.5 * t),
        'solution': lambda x, t: np.exp(-0.5 * t) * np.sin(x),
        'bound_type': 'a1p2',
    }

    p1d7 = ParabolicSolver(params, equation_type)
    resp = {
        'numerical': p1d7.solve(N, K, T).tolist(),
        'analytic': p1d7.solve_analytic(N, K, T).tolist()
    }

    return resp


def solve_lab6(data):
    equation_type = data['equation_type']
    N, K, T = int(data['N']), int(data['K']), int(data['T'])

    params = {
        'a': 1,
        'b': 2,
        'c': -3,
        'd': 2,
        'l': np.pi / 2,
        'f': lambda: 0,
        'alpha': 1,
        'beta': 0,
        'gamma': 1,
        'delta': 0,
        'psi1': lambda x: np.exp(-x) * np.cos(x),
        'psi2': lambda x: -np.exp(-x) * np.cos(x),
        'psi1_dir1': lambda x: -np.exp(-x) * np.sin(x) - np.exp(-x) * np.cos(x),
        'psi1_dir2': lambda x:  2 * np.exp(-x) * np.sin(x),
        'phi0': lambda t: np.exp(-t) * np.cos(2 * t),
        'phil': lambda t: 0,
        'bound_type': 'a1p2',
        'approximation': 'p1',
        'solution': lambda x, t: np.exp(-t - x) * np.cos(x) * np.cos(2 * t),
    }

    h2d7 = HyperbolicSolver(params, equation_type)
    resp = {
        'numerical': h2d7.solve(N, K, T).tolist(),
        'analytic': h2d7.solve_analytic(N, K, T).tolist()
    }

    return resp


def solve_lab7(data):
    equation_type = data['equation_type']
    N, l, eps = int(data['N']), int(data['l']), float(data['eps'])

    params = {
        'phi0': lambda y: np.cos(y),
        'phi1': lambda y: 0,
        'phi2': lambda x: np.cos(x),
        'phi3': lambda x: 0,
        'solution': lambda x, y: np.cos(x) * np.cos(y),
    }

    e2d7 = EllipticSolver(params, equation_type)
    resp = {
        'numerical': e2d7.solve(N, l, eps).tolist(),
        'analytic': e2d7.solve_analytic(N, l, eps).tolist()
    }

    return resp


def solve_lab8(data):
    equation_type = data['equation_type']
    N1, N2, K, T = int(data['N1']), int(data['N2']), int(data['K']), int(data['T'])

    params = {
        'f': lambda x, y, t: -x * y * np.sin(t),
        'l1': 1,
        'l2': 1,
        'psi': lambda x, y: x * y,
        'phi0': lambda x, t: 0,
        'phi1': lambda x, t: x * np.cos(t),
        'phi2': lambda y, t: 0,
        'phi3': lambda y, t: y * np.cos(t),
        'solution': lambda x, y, t: x * y * np.cos(t)
    }

    p2d7 = Parabolic2DSolver(params, equation_type)
    resp = {
        'numerical': p2d7.solve(N1, N2, K, T).tolist(),
        'analytic': p2d7.solve_analytic(N1, N2, K, T)
    }

    return resp