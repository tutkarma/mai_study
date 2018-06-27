from __future__ import print_function
from random import shuffle, randint

inf = open("input40", "w")
n = randint(40, 40)
m = randint(n, 45)
inf.write('{0} {1}\n'.format(m, n))

for i in range(m):
    data = [randint(0, 50) for _ in range(n + 1)]
    for item in data:
        inf.write('{0} '.format(item))
    inf.write('\n')

inf.close()