import os
import argparse


CNT_ROUNDS = 80
BLOCK_SIZE = 512


class SHA1:
    def __init__(self, rounds):
        self.h = (0x67452301,
                  0xEFCDAB89,
                  0x98BADCFE,
                  0x10325476,
                  0xC3D2E1F0)
        self.rounds = rounds

    def update(self, msg):
        msg_bin = ''
        for i in range(len(msg)):
            msg_bin += '{0:08b}'.format(ord(msg[i]))

        len_msg = len(msg_bin)

        msg_bin += '1'
        while (len(msg_bin) % 512 != 448):
            msg_bin += '0'

        msg_bin += '{0:064b}'.format(len_msg)

        chunks = self.get_chunks(msg_bin)
        for chunk in chunks:
            self.process_chunk(chunk)

        return self

    def hexdigest(self):
        return '%08x%08x%08x%08x%08x' % self.h

    def get_chunks(self, msg):
        return [msg[i:i + BLOCK_SIZE] for i in range(0, len(msg), BLOCK_SIZE)]

    def process_chunk(self, chunk):
        h0, h1, h2, h3, h4 = (i for i in self.h)

        w = []
        for i in range(16):
            w.append(int(chunk[i * 32:i * 32 + 32], 2))
        for i in range(16, 80):
            w.append(self.rotl(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1))

        a, b, c, d, e = h0, h1, h2, h3, h4

        for i in range(self.rounds):
            if 0 <= i <= 19:
                f = (b & c) | ((~b) & d)
                k = 0x5A827999
            elif 20 <= i <= 39:
                f = b ^ c ^ d
                k = 0x6ED9EBA1
            elif 40 <= i <= 59:
                f = (b & c) | (b & d) | (c & d)
                k = 0x8F1BBCDC
            elif 60 <= i <= 79:
                f = b ^ c ^ d
                k = 0xCA62C1D6

            a, b, c, d, e = (self.rotl(a, 5) + f + e + k + w[i]) & 0xffffffff, \
                    a, self.rotl(b, 30), c, d

        h0 = (h0 + a) & 0xffffffff
        h1 = (h1 + b) & 0xffffffff
        h2 = (h2 + c) & 0xffffffff
        h3 = (h3 + d) & 0xffffffff
        h4 = (h4 + e) & 0xffffffff

        self.h = (h0, h1, h2, h3, h4)

    @staticmethod
    def rotl(n, k):
        return ((n << k) | (n >> (32 - k))) & 0xffffffff


def sha1(msg, rounds):
    return SHA1(rounds).update(msg).hexdigest()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--rounds', help='Count rounds (<= 80)')
    parser.add_argument('--input', required=True, help='Input file to hash')
    args = parser.parse_args()

    rounds = int(args.rounds) if args.rounds else CNT_ROUNDS
    filename = args.input
    if os.path.isfile(filename):
        with open(filename, "r") as f:
            text = f.read()
            print('sha1: ', sha1(text, rounds))
    else:
        print("Error, could not find " + filename + " file." )
