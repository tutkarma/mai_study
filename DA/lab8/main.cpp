#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

int main(void)
{
    clock_t start = clock();
    bool hasSolution = true;
    int m = 0, n = 0, cost = 0, numRow = 0;
    std::cin >> m >> n;
    cost = n;
    numRow = n + 1;

    std::vector<int> res;
    std::vector<std::vector<double> > elems;
    elems.resize(m);
    for (int i = 0; i < m; ++i) {
        elems[i].resize(n + 2);
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n + 1; ++j) {
            std::cin >> elems[i][j];
        }
        elems[i][numRow] = i + 1;
    }

    for (int col = 0; col < n; ++col) {
        int minRow = -1;
        int minCost = 51;
        for (int row = col; row < m; ++row) {
            if (elems[row][col] != 0.0 && elems[row][cost] < minCost) {
                minRow = row;
                minCost = elems[row][cost];
            }
        }

        if (minRow == -1) {
            hasSolution = false;
            //std::cout << "-1" << std::endl;
            break;
        }

        elems[col].swap(elems[minRow]);
        res.push_back(elems[col][numRow]);
        for (int row = col + 1; row < m; ++row) {
            double c = elems[row][col] / elems[col][col];
            for (int i = col; i < n; ++i) {
                elems[row][i] -= elems[col][i] * c;
            }
        }
    }

    std::sort(res.begin(), res.end());
    if (hasSolution) {
        for (int i = 0; i < res.size(); ++i) {
            //std::cout << res[i];
            //if (i != res.size() - 1)
                //std::cout << " ";
        }
        //std::cout << std::endl;
    }
    clock_t end = clock();
    std::cout << "Greed time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;

    return 0;
}
