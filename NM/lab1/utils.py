import json

import matrix

def read_matrix(filename, matrix, vector):
    with open(filename, 'r') as json_data:
        data = json.load(json_data)[0] # !
        matrix.size = data['size']
        matrix.data = data['A']
        vector.data = data['B']


def read_triagonal_matrix(filename, matrix, vector):
    with open(filename, 'r') as json_data:
        data = json.load(json_data)[0] # !
        matrix.size = data['size']
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