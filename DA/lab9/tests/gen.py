from __future__ import print_function
from random import randint

start = 1
end = 31
fromVrtx = 1
toVrtx = 1
weight = 10

inf = open("input30", "w")
inf.write('{0} {1}\n'.format(end, end * end))
for i in range(1, end * end + 1):
    fromVrtx = randint(start, end)
    toVrtx = randint(start, end)
    w = randint(1, weight)
    while fromVrtx >= toVrtx:
        fromVrtx = randint(start, end)
        toVrtx = randint(start, end)
    inf.write('{0} {1} {2}\n'.format(fromVrtx, toVrtx, w))
inf.close()
