import json


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

