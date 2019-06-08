import json

import numpy as np


def get_matrix(low=-1000, high=1000, sz=10):
    m = np.random.random_integers(low, high, size=(sz,sz))
    return (m + m.T).tolist()


if __name__ == '__main__':
    sizes = [i for i in range(10, 110, 10)]

    for i, s in enumerate(sizes):
        with open('performance_tests/{0:02d}_{1}.json'.format(i + 1, s), 'w') as f:
            data = [{
                        'eps': 0.001,
                        'matrix': get_matrix(sz=s)
                    }]
            json.dump(data, f, indent=4)

    for i in range(1, 11):
        with open('accuracy_tests/{0:02d}.json'.format(i), 'w') as f:
            data = [{
                        'eps': 0.001,
                        'matrix': get_matrix(low=-10, high=10)
                    }]
            json.dump(data, f, indent=4)
