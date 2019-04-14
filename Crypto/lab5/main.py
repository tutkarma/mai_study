import random
import string
import getopt
import os
import sys

import urllib.request


CNT_RANDOM_TEXTS = 10
LEN_RANDOM_TEXT = 10 ** 6
CASES = 5


USAGE = """
Syntax: main.py [--cases=#]

  Flags:
    cases=#
        Numbers of cases to use. By default all cases are used.

        1 -- two meaningful texts in natural language
        2 -- meaningful text and text from random letters
        3 -- meaningful text and text from random words
        4 -- two texts from random letters
        5 -- two texts from random words

    Example:
        --cases=1,3

"""


def count_common_letters(text1, text2):
    cnt = 0
    for char1, char2 in zip(text1, text2):
        if char1 == char2:
            cnt += 1
    return cnt


def match_perc(text1, text2):
    return count_common_letters(text1, text2) / len(text1)


def gen_random_letters(n):
    text = ''
    while len(text) < n:
        len_word = random.randint(3, 10)
        word = ''.join(random.choice(string.ascii_letters) for _ in range(len_word))
        text += ' ' + word
    rem = len(text) - n
    if rem != 0:
        text = text[:-rem]
    return text


def gen_random_words(n):
    url = 'http://svnweb.freebsd.org/csrg/share/dict/words?view=co&content-type=text/plain'
    response = urllib.request.urlopen(url)
    words = response.read().decode()
    words = words.splitlines()
    text = ''
    while len(text) < n:
        text += ' ' + random.choice(words)
    rem = len(text) - n
    if rem != 0:
        text = text[:-rem]
    return text


def case1():
    print("Case #1: two meaningful texts in natural language.")
    url = 'http://www.gutenberg.org/files/4300/4300-0.txt'
    url2 = 'http://www.gutenberg.org/files/4280/4280-0.txt'
    response = urllib.request.urlopen(url)
    text1 = response.read().decode()
    response = urllib.request.urlopen(url2)
    text2 = response.read().decode()
    min_len = min(len(text1), len(text2))
    text1 = text1[:min_len]
    text2 = text2[:min_len]
    print("Text length: {0}".format(min_len))
    print("Match: {0}".format(match_perc(text1, text2)))


def case2():
    print("Case #2: meaningful text and text from random letters.")
    url = 'http://www.gutenberg.org/files/4300/4300-0.txt'
    response = urllib.request.urlopen(url)
    text1 = response.read().decode()
    s = 0
    for _ in range(CNT_RANDOM_TEXTS):
        text2 = gen_random_letters(len(text1))
        s += match_perc(text1, text2)
    s /= CNT_RANDOM_TEXTS
    print("Text length: {0}".format(len(text1)))
    print("Match: {0}".format(s))


def case3():
    print("Case #3: meaningful text and text from random words.")
    url = 'http://www.gutenberg.org/files/4300/4300-0.txt'
    response = urllib.request.urlopen(url)
    text1 = response.read().decode()
    s = 0
    for _ in range(CNT_RANDOM_TEXTS):
        text2 = gen_random_words(len(text1))
        s += match_perc(text1, text2)
    s /= CNT_RANDOM_TEXTS
    print("Text length: {0}".format(len(text1)))
    print("Match: {0}".format(s))


def case4():
    print("Case #4: two texts from random letters.")
    s = 0
    for _ in range(CNT_RANDOM_TEXTS):
        text1 = gen_random_letters(LEN_RANDOM_TEXT)
        text2 = gen_random_letters(LEN_RANDOM_TEXT)
        s += match_perc(text1, text2)
    s /= CNT_RANDOM_TEXTS
    print("Text length: {0}".format(LEN_RANDOM_TEXT))
    print("Match: {0}".format(s))


def case5():
    print("Case #5: two texts from random words.")
    s = 0
    for _ in range(CNT_RANDOM_TEXTS):
        text1 = gen_random_words(LEN_RANDOM_TEXT)
        text2 = gen_random_words(LEN_RANDOM_TEXT)
        s += match_perc(text1, text2)
    s /= CNT_RANDOM_TEXTS
    print("Text length: {0}".format(LEN_RANDOM_TEXT))
    print("Match: {0}".format(s))


def print_usage(message):
    print(USAGE)
    if message:
        sys.exit('\nFATAL ERROR: ' + message)
    else:
        sys.exit(1)


def parse_args(args):
    try:
        opts, args = getopt.getopt(args, '', ['help', 'cases='])
    except getopt.GetoptError:
        print_usage('Invalid arguments.')

    cases = [i for i in range(1, CASES + 1)]

    for (opt, val) in opts:
        if opt == '--help':
            print_usage(None)
        elif opt == '--cases':
            try:
                cases = set(map(int, val.split(',')))
            except ValueError:
                print_usage('Cases must be comma separated list.')

            for i in cases:
                if i not in range(1, CASES + 1):
                    print_usage('Incorrect cases')

    return cases


if __name__ == '__main__':
    cases = parse_args(sys.argv[1:])

    for i in cases:
        if i == 1:
            case1()
        elif i == 2:
            case2()
        elif i == 3:
            case3()
        elif i == 4:
            case4()
        elif i == 5:
            case5()
