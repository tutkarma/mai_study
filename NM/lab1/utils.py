import json

import matrix

import numpy as np


def read_data(filename, need_args):
    init_dict = {}
    with open(filename, 'r') as json_data:
        data = json.load(json_data)[0] # !

        for arg in need_args:
            if arg not in data:
                raise ValueError('No "{0}" in given data'.format(arg))

            if arg == 'matrix':
                init_dict[arg] = matrix.Matrix.from_list(data[arg])
            elif arg == 'vector':
                init_dict[arg] = matrix.Vector.from_list(data[arg])
            else:
                init_dict[arg] = data[arg]

    return init_dict


def read_triagonal_matrix(filename, matrix, vector):
    with open(filename, 'r') as json_data:
        data = json.load(json_data)[0] # !
        matrix.a = [0] + data['A']
        matrix.b = data['B']
        matrix.c = data['C'] + [0]
        vector.data = data['D']


def save_to_file(filename, **kwargs):
    with open(filename, 'w') as f:
        json_data = {}
        for k, val in kwargs.items():
            if isinstance(val, matrix.Vector):
                json_data[k] = val.get_data()
            elif isinstance(val, matrix.Matrix):
                json_data[k] = val.get_data()
            else:
                json_data[k] = val
        json.dump(json_data, f, sort_keys=True, indent=4)


def complex_to_list(list_):
    list_without_complex = []
    for obj in list_:
        if isinstance(obj, (complex, np.complex)):
            list_without_complex.append([obj.real, obj.imag])
        else:
            list_without_complex.append(obj)
    return list_without_complex