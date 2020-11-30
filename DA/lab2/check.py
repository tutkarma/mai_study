#!/usr/bin/env python
import argparse
import contextlib
import difflib
import io
import json
import os
import pathlib
import random
import shutil
import string
import subprocess
import sys
import tempfile
import time
from functools import partial

TIME_LIMIT = 160
MIN_VALUE = 0
MAX_VALUE = 2 ** 64 - 1
ONLY_USE_LOWERCASE = False
UNIQUE_LETTER_COUNT = 5
MIN_KEY_LEN = 1
MAX_KEY_LEN = 3


def generate_key():
    up = '' if ONLY_USE_LOWERCASE else string.ascii_uppercase[:UNIQUE_LETTER_COUNT]
    return ''.join(random.choices(string.ascii_lowercase[:UNIQUE_LETTER_COUNT] + up,
                                  k=random.randint(MIN_KEY_LEN, MAX_KEY_LEN)))


def generate_test(tmp, rows, percent, proba):
    ans = []
    percent *= rows
    percent = 2*int(percent)
    saved_files = set()
    for i in range(rows - percent):
        choice = random.choice(['+', '-', '?'])
        if random.random() > proba and percent > 0:
            name = f"db{len(saved_files)}"
            name = os.path.join(tmp, name)
            ans.append(f'! Save {name}')
            saved_files.add(name)
            percent -= 1
        if random.random() > proba and len(saved_files) > 0:
            ans.append(f'! Load {saved_files.pop()}')
        elif choice == '?':
            ans.append(generate_key())
        elif choice == '+':
            ans.append('+ ' + generate_key() + ' ' + str(random.randint(MIN_VALUE, MAX_VALUE)))
        else:
            ans.append('- ' + generate_key())

    for el in saved_files:
        ans.append(f'! Load {el}')
    for i in range(percent):
        name = f'db{i}'
        name = os.path.join(tmp, name)
        ans.append(f'! Save {name}')
        ans.append(f'! Load {name}')
    assert len(ans) == rows, f"{len(ans)} != {rows}"  # whether test is valid
    return ans


def solve_problem(test):
    d = {}
    for r in test:
        if r[0] == '!':
            cmd, filename = r[2:].split()
            cmd = cmd.lower()
            if cmd == 'load':
                with open(filename, 'r') as file:
                    d = json.load(file)
            elif cmd == "save":
                with open(filename, 'w') as file:
                    json.dump(d, file)
            print('OK')
        elif r[0] == '+':
            key, value = r[2:].split()
            if key in d:
                print('Exist')
            else:
                d[key] = int(value)
                print('OK')
        elif r[0] == '-':
            key = r[2:]
            if key not in d:
                print('NoSuchWord')
            else:
                del d[key]
                print('OK')
        elif r != '':
            if r in d:
                print('OK:', d[r])
            else:
                print('NoSuchWord')


def solver_func(args):
    with args.output as file:
        with contextlib.redirect_stdout(file):
            solve_problem(map(lambda x: x.strip(), args.input.readlines()))
    print(f'Output of solver now is in {args.output.name}')


def checker_func(args):
    tmp_dir = tempfile.mkdtemp()
    prev_dir = os.path.abspath(os.curdir)
    prog = args.prog.absolute().as_posix()
    os.chdir(tmp_dir)

    for i in range(args.tests_count):
        test = generate_test(os.curdir, args.rows_count, args.percent_load_save, args.probability_load_save)

        proc = subprocess.Popen([f'"{prog}"'], stdout=subprocess.PIPE, stdin=subprocess.PIPE,
                                stderr=subprocess.PIPE, shell=True)
        proc.stdin.write(('\n'.join(test)).encode())
        proc.stdin.close()

        now = time.time()

        test_myself = test.copy()
        test_myself = list(map(lambda x: x.replace('! Save db', '! Save dbother'), test_myself))
        f_myself_out = io.StringIO()
        with contextlib.redirect_stdout(f_myself_out):
            solve_problem(test_myself)

        f_myself_out.seek(0)
        f_myself_out_lines = f_myself_out.readlines()
        f_myself_out.close()

        proc.wait(max(now + args.time_limit - time.time(), 0))
        proc.terminate()
        if proc.poll() is None:
            proc.kill()
        if proc.returncode != 0:
            print('error in program occurred:')
            print('some error:', proc.returncode)
            print('stderr:', proc.stderr.read().decode())
            print(*test, sep='\n', file=open(os.path.join(prev_dir, args.out_testfile), 'w'))
            break
        out = list(map(lambda x: x.decode(), proc.stdout.readlines()))

        gen = list(difflib.unified_diff(out, f_myself_out_lines))

        if len(gen) > 0:
            print('Difference found within two outputs:')
            print(*gen)
            print(*gen, file=open(os.path.join(prev_dir, args.diff_filename)))
            print(*test, sep='\n', file=open(os.path.join(prev_dir, args.out_testfile), 'w'))
            break

    os.chdir(prev_dir)
    shutil.rmtree(tmp_dir)
    print('All outputs are match')


def generate_func(args):
    test = generate_test(os.curdir, args.rows_count, args.percent_load_save, args.probability_load_save)
    print(*test, sep='\n', file=open(args.output, 'w'))
    print('Tests were written to', args.output)


def main():
    global MIN_KEY_LEN, MAX_KEY_LEN, ONLY_USE_LOWERCASE, UNIQUE_LETTER_COUNT
    parser = argparse.ArgumentParser(description='Helps to pass 2 laboratory work of diskran.\n',
                                     epilog='Test case is the one test file', )

    subparsers = parser.add_subparsers(required=True, dest='command_name')

    checker = subparsers.add_parser('check', description='Checks the given program for time limit and right answers')
    checker.add_argument('prog', type=pathlib.Path, help='A program to execute')
    checker.add_argument('-n', '--tests-count', default=1000, type=int, help='How many tests to do(default: 1000)')
    checker.add_argument('-N', '--rows-count', default=150, type=int,
                         help='Count of rows in one test file(default: 150)')
    checker.add_argument('-T', '--out-testfile', default='test.txt', type=pathlib.Path,
                         help='If error detected, '
                              'the test crashed program will be written to this file(default: test.txt)')
    checker.add_argument('-t', '--time-limit', default=TIME_LIMIT, type=int,
                         help=f'Max time program can run(default: {TIME_LIMIT})')
    parser.add_argument('-m', '--min-key-len', default=MIN_KEY_LEN, type=int,
                        help=f'Minimal key length(default: {MIN_KEY_LEN})')
    parser.add_argument('-M', '--max-key-len', default=MIN_KEY_LEN, type=int,
                        help=f'Maximal key length(default: {MAX_KEY_LEN})')
    parser.add_argument('-l', '--use-lowercase', action='store_true', default=False,
                        help='Use only lowercase(default: use all cases)')
    parser.add_argument('-U', '--count-unique-letters', default=UNIQUE_LETTER_COUNT, type=int,
                        help=f'Count of unique letters in keys(default: {UNIQUE_LETTER_COUNT})')

    dev_group = parser.add_argument_group(title='dev', description='The dev settings. '
                                                                   'Do not change them '
                                                                   'if you don\'t know what do they mean')
    dev_group.add_argument('-R', '--random-state', default=partial(time.time), type=int,
                           help='Random seed(do not change if don\'t know what is it)(default: current unix time)')
    dev_group.add_argument('-p', '--percent-load-save', default=0.01, type=float,
                           help='Count of percent(note: percents here are from 0 to 1)'
                                ' of rows with serialization(save/load)(default: 0.01)')
    dev_group.add_argument('-P', '--probability-load-save', default=0.8,
                           help='The probability of using save-load command(default: 0.8)')
    dev_group.add_argument('-d', '--diff-filename', default='diff.txt', type=pathlib.Path,
                           help='The filename where to save the difference of program and this script(default: diff.txt)')

    solver = subparsers.add_parser('solve')
    solver.add_argument('input', type=argparse.FileType('r'), help='File with tests')
    solver.add_argument('output', type=argparse.FileType('w'), help='File to output')

    generator = subparsers.add_parser('generate', description='Generate one test with given parameters')
    generator.add_argument('-N', '--rows-count', default=150, type=int,
                           help='Count of rows in one test file(default: 150)')
    generator.add_argument('output', default='test.txt', type=pathlib.Path,
                           help='file to write test')
    args = parser.parse_args(sys.argv[1:])

    MIN_KEY_LEN = args.min_key_len
    MAX_KEY_LEN = args.max_key_len
    ONLY_USE_LOWERCASE = args.use_lowercase
    UNIQUE_LETTER_COUNT = args.count_unique_letters

    random.seed(args.random_state)

    if args.command_name == 'checker':
        checker_func(args)
    elif args.command_name == 'solver':
        solver_func(args)
    elif args.command_name == 'generate':
        generate_func(args)

    print('Exiting...')


if __name__ == '__main__':
    main()
