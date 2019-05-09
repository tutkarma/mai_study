#!/usr/bin/python

from random import *
from string import ascii_letters

template = '{key} {data}\n'

with open('input4', 'w') as f:
    for i in range(1, 10):
        if i % 10000 == 0:
            print i
        f.write(template.format(**{
            'key': ''.join(choice(ascii_letters) for _ in range(6)),
            'data': ''.join(choice(ascii_letters) for _ in range(randint(1, 100)))
}))
