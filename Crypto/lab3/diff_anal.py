import random
import string
import logging

import bitarray
import matplotlib.pyplot as plt

import sha1


CNT_TESTS = 11


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


def mean(list_):
    return [int(sum(i)/len(i)) for i in zip(*list_)]


if __name__ == '__main__':
    logging.basicConfig(filename="analysis.log", level=logging.INFO)

    all_diffs = []
    cnt_rounds = [i for i in range(0, 81, 5)]
    for i in range(1, CNT_TESTS):
        logging.info("Test #{0}".format(i))
        input1 = get_random_string()
        input2 = change_one_bit(input1)

        logging.info("Input string:   {0}".format(input1))
        logging.info("Changed string: {0}".format(input2))

        diffs = []
        rounds = range(0, 81, 5)
        for i in rounds:
            logging.info("Count rounds: {0}".format(i))
            output1 = sha1.sha1(input1, i)
            output2 = sha1.sha1(input2, i)
            logging.info("Output original:  {0}".format(output1))
            logging.info("Output changed:   {0}".format(output2))
            res = bitcount(int(output1, 16) ^ int(output2, 16))
            diffs.append(res)
            logging.info("Count of different bits: {0}".format(res))
        logging.info("------------")
        all_diffs.append(diffs)

    mean_diffs = mean(all_diffs)
    for i, j in zip(cnt_rounds, mean_diffs):
        print("Count rounds: {0}".format(i))
        print("Count of different bits: {0}".format(j))
        print("------------")

    plt.bar(cnt_rounds, mean_diffs, align='center')
    plt.xlabel('Count rounds')
    plt.ylabel('Count of different bits')
    plt.show()