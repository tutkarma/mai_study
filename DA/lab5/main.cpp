#include "TSuffixTree.h"
#include <string>
#include <iostream>
#include <ctime>

int main(void)
{
    std::string text, pattern;
    std::cin >> text;

    TSuffixTree tree(text + "$");
    TSuffixArray array(tree);

    for (int cntPattern = 1; std::cin >> text; ++cntPattern) {
        std::vector<int> result = array.Find(text);
        if (!result.empty()) {
            std::cout << cntPattern << ": ";
            for (int i = 0; i < result.size(); ++i) {
                std::cout << result[i] + 1;
                if (i < result.size() -  1) {
                    std::cout << ", ";
                }
            }
            std::cout << '\n';
        }
    }

    return 0;
}

