import time

class Profiler(object):
    def __enter__(self):
        self._startTime = time.time()

    def __exit__(self, type, value, traceback):
        print("Python time: {:.6f} sec.".format(time.time() - self._startTime))

with Profiler() as p:
    while True:
        try:
            x = int(raw_input())
        except EOFError:
            break

        y = int(raw_input())
        s = raw_input()

        ops = { '+': lambda a, b: a + b,
                '-': lambda a, b: a - b,
                '*': lambda a, b: a * b,
                '^': lambda a, b: a ** b,
                '/': lambda a, b: a / b,
                '<': lambda a, b: a < b,
                '>': lambda a, b: a > b,
                '=': lambda a, b: a == b }

        res = ops[s](x,y)
        if s in '<>=':
            res = str(res).lower()

        if s in '-' and x < y:
            res = "Error"

        if s in '/' and y == 0:
            res = "Error"

        print(res)

