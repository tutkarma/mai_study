import numpy as np
from numpy.linalg import solve, eig


def numpy_solve(matrix, vector, sol):
    a = np.array(matrix.get_data())
    b = np.array(vector.get_data())
    x = solve(a, b)
    print("My solution:")
    print(sol)
    print("Numpy solution:")
    print(x)


def numpy_eig(matrix, my_values):
    print("My eigenvalues:")
    print(my_values)

    a = np.array(matrix.get_data())
    eig_np = eig(a)
    print("Numpy eigenvalues:")
    print(eig_np[0].round(3))


def numpy_eigs(matrix, my_values, my_vectors):
    print("My eigenvalues:")
    print(my_values)
    print("My eigenvectors:")
    print(my_vectors)

    a = np.array(matrix.get_data())
    eig_np = eig(a)
    print("Numpy eigenvalues:")
    print(eig_np[0].round(3))
    print("Numpy eigenvectors:")
    print(eig_np[1].round(3))