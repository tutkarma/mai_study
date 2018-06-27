#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>

const size_t MAX = 1000;

int main(void)
{
    clock_t start = clock();
    int n = 0, m = 0;
    std::cin >> n >> m;
    long long allPath[MAX][MAX];
    std::vector<long long> rowPrev(m + 2), rowCur(m);

    rowPrev[0] = std::numeric_limits<long long>::max();
    rowPrev[m + 1] = std::numeric_limits<long long>::max();

    for (int i = 0; i < m; ++i) {
        std::cin >> rowPrev[i + 1];
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> rowCur[j];
        }

        for (int j = 1; j <= m; ++j) {
            long long minCost = std::numeric_limits<long long>::max();
            if (rowPrev[j - 1] < minCost) {
                minCost = rowPrev[j - 1];
                allPath[i][j - 1] = j - 1;
            }
            if (rowPrev[j] < minCost) {
                minCost = rowPrev[j];
                allPath[i][j - 1] = j;
            }
            if (rowPrev[j + 1] < minCost) {
                minCost = rowPrev[j + 1];
                allPath[i][j - 1] = j + 1;
            }
            rowCur[j - 1] += minCost;
        }

        for (int j = 0; j < m; ++j) {
            rowPrev[j + 1] = rowCur[j];
        }
    }

    auto minEl = std::min_element(rowCur.begin(), rowCur.end());
    long long ans = *minEl;
    int end = std::distance(rowCur.begin(), minEl) + 1;

    std::vector<long long> path;
    for (int i = 0; i < n; ++i) {
        path.push_back(end);
        end = allPath[n - i - 1][end - 1];
    }
    clock_t endd = clock();

    std::cout << ans << std::endl;
    /*for (int i = 0; i < n; ++i) {
        i == 0 ? std::cout << "(" : std::cout << " (";
        std::cout << i + 1 << "," << path[n - i - 1] << ")";
    }
    std::cout << std::endl;*/
    std::cout << "DP time: " << (double)(endd - start) / CLOCKS_PER_SEC << " sec." << std::endl;

    return 0;
}