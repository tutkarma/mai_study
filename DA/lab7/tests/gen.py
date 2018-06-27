from __future__ import print_function
from random import shuffle, randint

tests = 1
max_size = 1000
size = [1, max_size]
lim = [0, +1 * 10 ** 4]

inf = open("input30", "w")

def print_as_matrix(data, size):
    for idx, item in enumerate(data):
        inf.write('{0} '.format(item))
        if (idx + 1) % size == 0:
            inf.write("\n")


for i in range(1, 30 + 1): # kolvo strok
    inf.write('{0}'.format(i))
    data = [randint(lim[0], lim[1]) for _ in range(0, 30)]
    shuffle(data)
    print_as_matrix(data, 30) # stolb

#for i in range(tests / 2, 0, -1):
#    inf.write('{0}'.format(i))
#    data = [randint(lim[0], lim[1]) for _ in range(0, i * i)]
#    shuffle(data)
#    print_as_matrix(data, i)

inf.close()
