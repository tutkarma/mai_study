import sys
import random

if __name__ == '__main__':
    inf = open("input", "w")
    out = open("output", "w")

    try:
        tests = int(sys.argv[1])
    except:
        tests = 10000

    opc = "*"
    ops = { '+': lambda a, b: a + b,
            '-': lambda a, b: a - b,
            '*': lambda a, b: a * b,
            '^': lambda a, b: a ** b,
            '/': lambda a, b: a / b,
            '<': lambda a, b: a < b,
            '>': lambda a, b: a > b,
            '=': lambda a, b: a == b }

    for i in range(tests):
        op_ = random.choice(opc)

        l = 500 if op_ is '^' else 10 ** 35
        r = 2000 if op_ is '^' else 10 ** 35

        a = random.randint(0, l)
        b = random.randint(0, r)

        res = ops[op_](a,b)
        if op_ in '<>=':
            res = str(res).lower()

        if op_ is '-' and a < b:
            res = "Error"

        if op_ is '/' and b == 0:
            res = "Error"

        inf.write(str(a) + "\n" + str(b) + "\n" + str(op_) + "\n")
        out.write(str(res) + "\n")
    inf.close()
    out.close()
