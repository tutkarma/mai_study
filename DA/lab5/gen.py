#!usr/bin/python

from random import *
from string import ascii_letters

template = '{test}\n'

with open('input1kk', 'w') as f:
    for i in range(1, 1000000):
        if i == 1:
            f.write(template.format(**{'test': ''.join(choice('abcdef') for _ in range(1000))}))
        else:
            f.write(template.format(**{'test': ''.join(choice('abcdef') for _ in range(randint(1,10)))}))
