#include <iostream>
#include <vector>
#include <algorithm>

bool Gauss(std::vector<std::vector<double> > &mat, int n, int &sum)
{
    for (int col = 0; col < n; ++col) {
        int minRow = -1;
        int minCost = 100;
        for (int row = col; row < n; ++row) {
            if (mat[row][col] != 0.0 && mat[row][n] < minCost) {
                minRow = row;
                minCost = mat[row][n];
            }
        }

        if (minRow == -1) {
            return false;
        }

        sum += minCost;
        mat[col].swap(mat[minRow]);

        for (int row = col + 1; row < n; ++row) {
            double c = mat[row][col] / mat[col][col];
            for (int i = col; i < n; ++i) {
                mat[row][i] -= mat[col][i] * c;
            }
        }
    }
    return true;
}

int main(void)
{
    clock_t start = clock();
    int m = 0, n = 0, cost = 0, numRow = 0;
    int bufCost = 0, minCost = std::numeric_limits<int>::max();
    std::cin >> m >> n;
    cost = n;
    numRow = n + 1;
    std::vector<std::vector<double> > elems, tmp;
    std::vector<int> res, answer;
    elems.resize(m);
    for (int i = 0; i < m; ++i) {
        elems[i].resize(n + 2);
    }
    tmp.resize(n);
    for (int i = 0; i < n; ++i) {
        tmp[i].resize(n + 2);
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n + 1; ++j) {
            std::cin >> elems[i][j];
        }
        elems[i][numRow] = i + 1;
    }

    std::vector<bool> v(m);
    std::fill(v.end() - n, v.end(), true);
    do {
        for (int i = 0; i < m; ++i) {
            if (v[i]) {
                tmp.push_back(elems[i]);
                res.push_back(i + 1);
            }
        }
        if (Gauss(tmp, n, bufCost) && bufCost < minCost) {
            answer = res;
            minCost = bufCost;
        }
        bufCost = 0;
        tmp.clear();
        res.clear();
    } while (std::next_permutation(v.begin(), v.end()));

    if (answer.empty()) {
        //std::cout << "-1" << std::endl;
    } else {
        for (int i = 0; i < answer.size(); ++i) {
            //std::cout << answer[i];
            if (i != answer.size() - 1) {
                //std::cout << " ";
            }
        }
        //std::cout << std::endl;
    }
    clock_t end = clock();
    std::cout << "Naive time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;

    return 0;
}