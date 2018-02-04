#!usr/bin/python

from random import *
from string import ascii_letters

def get_random_pattern_key():
    return ''.join(choice('123456789') for i in range(randint(1,3)))

def get_random_text_key():
    return ''.join(choice('123456789') for i in range(randint(1,5)))

if __name__ == "__main__":
    pattern = ''
    text = ''
    keys = []
    test_file_name = "tests/{:02d}".format( 10 )
    with open( "{0}.t".format( test_file_name ), 'w' ) as output_file:
        for x in range(5):
            check = randint(1, 100000)
            if check % 2 == 0:
                key = '?'
            else:
                key = get_random_pattern_key()
                if key not in keys:
                    keys.append(key)
            pattern = pattern + ' ' + key
        output_file.write("{0}\n".format( pattern.lstrip() ))

        for i in range(10000): # kolvo strok v file
            if i % 1000 == 0:
                print 'Generate ', i
            text = ''
            for x in range(100): # kolvo chisel v str
                check = randint(1, 100000)
                if check % 2 == 0 and len(keys) > 0:
                    key = choice(keys)
                    text = text + ' ' + key
                else:
                    key = get_random_text_key()
                    text = text + ' ' + key
            output_file.write("{0}\n".format( text.lstrip() ))
