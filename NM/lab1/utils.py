def read_matrix(filename, matrix, vector):
    with open(filename, 'r') as f:
        matrix.size = int(f.readline())
        matrix.data = [[int(num) for num in line.split()] for _, line in zip(range(matrix.size), f)]
        vector.data = [int(i) for i in f.readline().split()]


def read_triagonal_matrix(filename, matrix, vector):
    with open(filename, 'r') as f:
        matrix.size = int(f.readline())
        for i in range(matrix.size):
            if i == 0:
                a, b, c = [0] + list(map(int, f.readline().split()))
            elif i == matrix.size - 1:
                a, b, c = list(map(int, f.readline().split())) + [0]
            else:
                a, b, c = list(map(int, f.readline().split()))
            matrix.a.append(a)
            matrix.b.append(b)
            matrix.c.append(c)
        vector.data = [int(i) for i in f.readline().split()]

