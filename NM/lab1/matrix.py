import copy
import json

from numpy.linalg import norm


class Vector:
    def __init__(self, sz=None):
        if sz is not None:
            self.data = [0] * sz
        else:
            self.data = []

    def __getitem__(self, idx):
        return self.data[idx]

    def __setitem__(self, idx, item):
        if idx >= len(self):
            self.data.extend(idx + 1)
        self.data[idx] = item

    def __len__(self):
        return len(self.data)

    def __str__(self):
        res = '\n'
        for i in self:
            res += '{0}\n'.format(i)
        return res

    def __add__(self, other):
        if isinstance(other, Matrix):
            other_vec = sum(other.data, [])
            added = [i + j for i, j in zip(self, other_vec)]
        elif isinstance(other, Vector):
            added = [i + j for i, j in zip(self, other.data)]
        return Vector.from_list(added)

    def __sub__(self, other):
        subbed = [i - j for i, j in zip(self, other)]
        return Vector.from_list(subbed)

    def append(self, item):
        self.data.append(item)

    def get_data(self):
        return [round(i, 4) for i in self.data]

    def norm(self):
        return max([abs(i) for i in self])

    @classmethod
    def from_list(cls, list_):
        vec = Vector()
        vec.data = list_
        return vec

    @classmethod
    def copy(cls, orig):
        vec = Vector()
        vec.data = copy.deepcopy(orig.data)
        return vec


class Matrix:
    def __init__(self, orig=None):
        if orig is None:
            self.non_copy_constructor()
        else:
            self.copy_constructor(orig)

    def non_copy_constructor(self):
        self.data = []

    def copy_constructor(self, orig):
        self.data = copy.deepcopy(orig.data)

    def __getitem__(self, idx):
        return self.data[idx]

    def __setitem__(self, idx, item):
        if idx >= len(self):
            self.data.extend(idx + 1)
        self.data[idx] = item

    def __len__(self):
        return len(self.data)

    def __str__(self):
        res = '\n'
        for i in range(len(self)):
            for j in range(len(self)):
                res += str(self.data[i][j]) + ' '
            res += '\n'
        return res

    def get_data(self):
        return [[round(j, 4) for j in i] for i in self.data]

    def multiply(self, other):
        if isinstance(other, Vector):
            b = other.data
            result = Vector.from_list([sum(ea * eb for ea, eb in zip(a, b)) for a in self])
        elif isinstance(other, Matrix):
            other_T = list(zip(*other))
            result = Matrix.from_list([[sum(ea * eb for ea, eb in zip(a, b)) for b in other_T] for a in self])
        return result

    def debug_print(self):
        for i in self.data:
            print(*i)

    def transpose(self):
        self.data = [list(i) for i in zip(*self.data)]

    def diag(self):
        v = Vector.from_list([self.data[i][i] for i in range(len(self))])
        return v

    def norm(self):
        return max([sum(list(map(abs, self.data[i]))) for i in range(len(self))])

    @classmethod
    def _make_matrix(cls, rows):
        mat = Matrix()
        mat.data = rows
        return mat

    @classmethod
    def zero(cls, sz):
        obj = Matrix()
        obj.data = [[0] * sz for _ in range(sz)]
        return obj

    @classmethod
    def identity(cls, sz):
        obj = Matrix()
        obj.data = [[1 if i == j else 0 for j in range(sz)] for i in range(sz)]
        return obj

    @classmethod
    def from_list(cls, list_of_lists):
        rows = list_of_lists[:]
        return cls._make_matrix(rows)

    @classmethod
    def triangular_from_matrix(cls, orig, type_tril=None):
        obj = Matrix(orig)
        if type_tril == 'lower':
            for i in range(len(orig)):
                for j in range(i, len(orig)):
                    obj[i][j] = 0
        elif type_tril == 'upper':
            for i in range(len(orig)):
                for j in range(0, i):
                    obj[i][j] = 0
        return obj



class TridiagonalMatrix:
    def __init__(self):
        self.a = []
        self.b = []
        self.c = []

    def __len__(self):
        return len(self.b)

    def __str__(self):
        res = '\n'
        res += str(self.b[0]) + ' ' + str(self.c[0]) + '\n'
        for i in range(1, len(self) - 1):
            res += str(self.a[i]) + ' ' + str(self.b[i]) + ' ' + str(self.c[i]) + '\n'
        res += str(self.a[-1]) + ' ' + str(self.b[-1]) +'\n'
        return res

    def debug_print(self, D):
        for i in range(len(self)):
            print("{0} {1} {2} {3}".format(self.a[i], 
                self.b[i], self.c[i], D[i]))