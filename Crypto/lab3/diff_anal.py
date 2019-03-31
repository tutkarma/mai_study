import random
import string
import bitarray

import matplotlib.pyplot as plt

import sha1


def get_random_string(N=50):
    return ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(N))


def change_one_bit(msg):
    ba = bitarray.bitarray()
    ba.frombytes(msg.encode('ascii'))
    last_bit = ba[-1]
    new_last = bitarray.bitarray('0') if last_bit else bitarray.bitarray('1')
    ba = ba[:-1]
    ba += new_last
    return bitarray.bitarray(ba.tolist()).tobytes().decode('ascii')


def bitcount(n):
    return bin(n).count('1')


if __name__ == '__main__':
    input1 = get_random_string()
    input2 = change_one_bit(input1)

    cnt_rounds = []
    diffs = []
    rounds = range(0, 81, 5)
    for i in rounds:
        cnt_rounds.append(i)
        output1 = sha1.sha1(input1, i)
        output2 = sha1.sha1(input2, i)
        diffs.append(bitcount(int(output1, 16) ^ int(output2, 16)))

    print(cnt_rounds)
    print(diffs)
    plt.bar(cnt_rounds, diffs, align='center')
    plt.xlabel('Count rounds')
    plt.ylabel('Count of different bits')
    plt.show()