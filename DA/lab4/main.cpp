#include <iostream>
#include <sstream>

#include "TTrie.h"

int main(void)
{
    std::vector<std::pair<int, int> > answer;
    std::vector<std::string> pattern;
    std::vector<unsigned long> text;

    std::string input, token;
    TTrie trie;

    std::getline(std::cin, input);
    std::stringstream patternSS(input);

    while (patternSS >> token) {
        pattern.push_back(std::string(token));
    }

    int lineCnt = 1;
    int wordCnt = 1;

    while (std::getline(std::cin, input)) {
        std::stringstream textSS(input);

        while (textSS >> token) {
            text.push_back(std::stoul(token));
            answer.push_back(std::make_pair(lineCnt, wordCnt));
            ++wordCnt;
        }
        ++lineCnt;
        wordCnt = 1;
    }

    trie.Create(pattern);
    trie.Search(text, pattern.size(), answer);

    return 0;
}