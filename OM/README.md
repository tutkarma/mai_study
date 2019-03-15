### Методы оптимизации ###

Написанная на коленке прога, реализующая:
* классический метод
* метод градиентного спуска
* метод наискорейшего спуска (два разных выбора шага)
* метод градиентного спуска для плохо обусловленных функций
* метод покоординатного спуска
* метод Гаусса-Зейделя


### Использование ###

```
Syntax: main.py [--methods=#] [--max-iter=#] [--input=<filename>]
                [--output=<filename>]

  Flags:
    methods=#
        Numbers of methods to use. By default all method are used.

        1 -- Classical method
        2 -- Gradient descent method
        3 -- Quickest descent method with step type 1
        4 -- Quickest descent method with step type 3
        5 -- Gradient descent method for poorly conditioned function
        6 -- Coordinate descent method
        7 -- Gauss–Seidel method

    Example:
        --methods=1,3

    max-iter=#
        Maximum number of iterations. By default it is 10000.
    Example:
        --max-iter=5

    input=<filename>
        Read input from file.
    Example:
        --input=input.txt

    output=<filename>
        Write output to file.
    Example:
        --output=log.txt
```