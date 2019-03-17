import copy


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

    def save_to_file(self, filename):
        with open(filename, 'w') as f:
            for i, el in enumerate(self.data):
                f.write('x{0} = {1}\n'.format(i + 1, el))


class Matrix:
    def __init__(self, orig=None):
        if orig is None:
            self.non_copy_constructor()
        else:
            self.copy_constructor(orig)

    def __getitem__(self, idx):
        return self.data[idx]

    def non_copy_constructor(self):
        self.size = 0
        self.data = []

    def copy_constructor(self, orig):
        self.size = orig.size
        self.data = copy.deepcopy(orig.data)

    def debug_print(self):
        for i in self.data:
            print(*i, sep=" ", end=" ")

    def transpose(self):
        self.data = [list(i) for i in zip(*self.data)]

    @classmethod
    def _make_matrix(cls, rows):
        mat = Matrix()
        mat.size = len(rows)
        mat.data = rows
        return mat

    @classmethod
    def identity(cls, sz):
        obj = Matrix()
        obj.size = sz
        obj.data = [[1 if i == j else 0 for j in range(sz)] for i in range(sz)]
        return obj

    @classmethod
    def from_list(cls, list_of_lists):
        rows = list_of_lists[:]
        return cls._make_matrix(rows)


class TridiagonalMatrix:
    def __init__(self):
        self.size = 0
        self.a = []
        self.b = []
        self.c = []

    def debug_print(self, D):
        for i in range(self.size):
            print("{0} {1} {2} {3}".format(self.a[i], 
                self.b[i], self.c[i], D[i]))