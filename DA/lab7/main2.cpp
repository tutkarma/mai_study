#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>

const size_t MAX = 1001;
long long PEN = std::numeric_limits<long long>::max();

void resSearch(int i, int j, long long mat[][MAX], int check, int m, long long sum)
{
    sum += mat[i][j];
    if (j < 0) {
        return;
    } else if (j == m) {
        return;
    }
    if (i + 1 == check) {
        if (PEN > sum)
            PEN = sum;
        return;
    }
    resSearch(i + 1, j - 1, mat, check, m, sum);
    resSearch(i + 1, j, mat, check, m, sum);
    resSearch(i + 1, j + 1, mat, check, m, sum);
}


int main(void)
{
    clock_t start = clock();
    int n = 0, m = 0;
    std::cin >> n >> m;
    long long init[MAX][MAX];


    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> init[i][j];
        }
    }

    std::vector<long long> res;
    for (int i = 0; i < m; ++i) {
        resSearch(0, i, init, n, m, 0);
    }

    std::cout <<  PEN << std::endl;

    clock_t end = clock();
    std::cout << "Naive time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;

    return 0;
}