#ifndef TTRIE_H
#define TTRIE_H

#include <vector>
#include <map>
#include <memory>
#include <string>

class TTrieNode
{
public:
    friend class TTrie;
    TTrieNode();
    ~TTrieNode() {};
private:
    std::map<unsigned long, TTrieNode *> to;
    TTrieNode *linkFail;
    std::vector<int> out;
};

class TTrie
{
public:
    TTrie();
    void Create(const std::vector<std::string> &);
    void Search(const std::vector<unsigned long>&, const int&, std::vector<std::pair<int, int> >&);
    ~TTrie() {};
private:
    TTrieNode *root;
    std::vector<int> lensPatterns;
    int withoutJoker; // amount subpattern without ?

    void CreateLinks();
};

#endif