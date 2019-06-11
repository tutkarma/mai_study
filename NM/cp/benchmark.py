import matplotlib.pyplot as plt
import numpy as np


def count_proc():
    x = np.arange(5)
    time = [32.742, 30.516, 57.952, 100.024, 367.59]

    plt.title('Matrix 100x100')
    plt.xlabel('Count proc')
    plt.ylabel('Time (sec)')
    plt.bar(x, time)
    plt.xticks(x, ('3', '4', '11', '21', '41'))
    plt.show()


def performance():
    sz = [i for i in range(10, 110, 10)]
    jacobi_time = [1.614, 13.596, 26.25, 41.101, 59.926, 87.312, 112.712, 163.324, 199.235, 252.196]
    parallel_time = [0.0086, 0.0908, 0.2429, 1.1104, 1.3396, 4.5165, 8.0155, 14.4147, 23.2138, 31.225]

    plt.plot(sz, jacobi_time, color='b', label='Sequential method')
    plt.plot(sz, parallel_time, color='r', label='Parallel method')
    plt.legend()
    plt.title('Performance tests')
    plt.xlabel('Size of matrix')
    plt.ylabel('Time (sec)')
    plt.grid(True)
    plt.show()


if __name__ == '__main__':
    performance()
    count_proc()

