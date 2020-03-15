import json

import numpy as np
from werkzeug.exceptions import BadRequest
from flask import Blueprint, request, Response, jsonify
from flask_cors import CORS

from solver.labs.lab5 import ParabolicSolver


labs = Blueprint('labs', __name__)
CORS(labs)


@labs.route('/<int(min=5, max=8):lab_id>', methods=['POST'])
def get_solution(lab_id):
    data = request.get_json(force=True)

    # error = validate_task(data)
    # if error is not None:
    #     raise BadRequest(error)

    if lab_id == 5:
        resp = solve_lab5(data)
    elif lab_id == 6:
        resp = solve_lab6(data)
    elif lab_id == 7:
        resp = solve_lab7(data)
    elif lab_id ==8:
        resp = solve_lab8(data)


    return Response(
        json.dumps(resp),
        status=200
    )


def solve_lab5(data):
    equation_type = data['equation_type']
    N, K, T = int(data['N']), int(data['K']), int(data['T'])
    params = {
        'a': 1,
        'b': 0,
        'c': 0,
        'l': np.pi,
        'alpha': 1,
        'beta': 0,
        'gamma': 1,
        'delta': 0,
        'psi': np.sin,
        'f': lambda x, t: 0.5 * np.exp(-0.5 * t) * np.cos(x),
        'phi0': lambda t: np.exp(-0.5 * t),
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
    pass


def solve_lab7(data):
    pass


def solve_lab8(data):
    pass