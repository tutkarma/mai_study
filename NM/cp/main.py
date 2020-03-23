import argparse
import json

from utils import save_to_file

from mpi4py import MPI
import numpy as np


def read_data(filename, need_args):
    init_dict = {}
    with open(filename, 'r') as json_data:
        data = json.load(json_data)[0] # !

        for arg in need_args:
            if arg not in data:
                raise ValueError('No "{0}" in given data'.format(arg))

            if arg == 'matrix':
                init_dict[arg] = np.array(data[arg], dtype=np.float64)
            else:
                init_dict[arg] = data[arg]

    return init_dict


def sign(n):
    return 1 if n > 0 else -1


def t(A):
    return np.sqrt(sum([A[i, j] ** 2 for i in range(A.shape[0]) 
        for j in range(i + 1, A.shape[0])]))


def indexes_max_elem(A):
    i_max = j_max = 0
    a_max = A[0, 0]
    for i in range(A.shape[0]):
        for j in range(i + 1, A.shape[0]):
            if abs(A[i, j]) > a_max:
                a_max = abs(A[i, j])
                i_max, j_max = i, j
    return i_max, j_max


def parallel_jacobi_rotate(comm, A, ind_j, ind_k):
    sz = A.shape[0]
    rank = comm.Get_rank()
    pool_size = comm.Get_size()
    c = s = 0.0
    j = k = 0
    row_j, row_k = np.zeros(sz), np.zeros(sz)
    if rank == 0:
        j, k = ind_j, ind_k

        if A[j, j] == A[k, k]:
            c = np.cos(np.pi / 4)
            s = np.sin(np.pi / 4)
        else:
            tau = (A[j, j] - A[k, k]) / (2 * A[j, k])
            t = sign(tau) / (abs(tau) + np.sqrt(1 + tau ** 2))
            c = 1 / np.sqrt(1 + t ** 2)
            s = c * t

        for i in range(sz):
            row_j[i] = A[j, i]
            row_k[i] = A[k, i]

    j = comm.bcast(j, root=0)
    k = comm.bcast(k, root=0)
    c = comm.bcast(c, root=0)
    s = comm.bcast(s, root=0)
    comm.Bcast(row_j, root=0)
    comm.Bcast(row_k, root=0)

    row_j_comm = comm.Create_group(comm.group.Incl([i for i in range(1, pool_size) if i % 2 == 1]))
    row_k_comm = comm.Create_group(comm.group.Incl([i for i in range(1, pool_size) if i % 2 == 0]))

    row_j_rank = row_j_size = -1
    row_j_new = np.zeros(sz)
    if MPI.COMM_NULL != row_j_comm:
        row_j_rank = row_j_comm.Get_rank()
        row_j_size = row_j_comm.Get_size()
        size = int(sz / row_j_size)
        row_j_part = np.zeros(size)
        row_k_part = np.zeros(size)
        row_j_new_part = np.zeros(size)

        row_j_comm.Scatter(row_j, row_j_part, root=0)
        row_j_comm.Scatter(row_k, row_k_part, root=0)

        for i in range(size):
            row_j_new_part[i] = c * row_j_part[i] + s * row_k_part[i]

        row_j_comm.Gather(row_j_new_part, row_j_new, root=0)
        if row_j_rank == 0:
            comm.Send([row_j_new, sz, MPI.FLOAT], dest=0, tag=0)
        row_j_comm.Free()

    row_k_rank = row_k_size = -1
    row_k_new = np.zeros(sz)
    if MPI.COMM_NULL != row_k_comm:
        row_k_rank = row_k_comm.Get_rank()
        row_k_size = row_k_comm.Get_size()
        size = int(sz / row_k_size)
        row_j_part = np.zeros(size)
        row_k_part = np.zeros(size)
        row_k_new_part = np.zeros(size)

        row_k_comm.Scatter(row_j, row_j_part, root=0)
        row_k_comm.Scatter(row_k, row_k_part, root=0)

        for i in range(size):
            row_k_new_part[i] = s * row_j_part[i] - c * row_k_part[i]

        row_k_comm.Gather(row_k_new_part, row_k_new, root=0)
        if row_k_rank == 0:
            comm.Send([row_k_new, sz, MPI.FLOAT], dest=0, tag=0)
        row_k_comm.Free()

    if rank == 0:
        status = MPI.Status()
        comm.Recv([row_j_new, sz, MPI.FLOAT], source=1, tag=0, status=status)
        comm.Recv([row_k_new, sz, MPI.FLOAT], source=2, tag=0, status=status)

        A[j, k] = A[k, j] = (c ** 2 - s ** 2) * row_j[k] + s * c * (row_k[k] - row_j[j])
        A[j, j] = c ** 2 * row_j[j] + 2 * s * c * row_j[k] + s ** 2 * row_k[k]
        A[k, k] = s ** 2 * row_j[j] - 2 * s * c * row_j[k] + c ** 2 * row_k[k]


        for i in range(sz):
            if i != j and i != k:
                A[j, i] = A[i, j] = row_j_new[i]
                A[k, i] = A[i, k] = row_k_new[i]

    return A


def jacobi_parallel(comm, A, eps):
    elapsed_time = 0
    i, j = indexes_max_elem(A)
    norm = t(A)
    rank = comm.Get_rank()
    eps = comm.bcast(eps, root=0)
    norm = comm.bcast(norm, root=0)

    k = 1
    while norm > eps:
        elapsed_time -= MPI.Wtime()
        A = parallel_jacobi_rotate(comm, A, j, i)
        if rank == 0:
            norm = t(A)
        elapsed_time += MPI.Wtime()
        norm = comm.bcast(norm, root=0)
        i, j = indexes_max_elem(A)
        k += 1

    return np.diag(A).tolist()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', required=True, help='Input file')
    parser.add_argument('--output', required=True, help='Output file')
    args = parser.parse_args()

    elapsed_time = 0
    need_args = ('matrix', 'eps')
    init_dict = read_data(args.input, need_args)
    A, eps = init_dict['matrix'], init_dict['eps']

    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()

    elapsed_time -= MPI.Wtime()
    eig = jacobi_parallel(comm, A, eps)
    elapsed_time += MPI.Wtime()

    if rank == 0:
        save_to_file(args.output, eigenvalues=eig)
        print("Dimension {0}, time elapsed {1} sec.\n".format(A.shape[0], elapsed_time))

    MPI.Finalize()