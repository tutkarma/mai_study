#ifndef TSUFTREE_H
#define TSUFTREE_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>

class TSuffixArray;

class TNode
{
public:
    std::map<char, TNode *> to;
    std::string::iterator begin, end;
    TNode *suffixLink;
    TNode(std::string::iterator begin, std::string::iterator end);
    ~TNode() {};
};

class TSuffixTree
{
public:
    TSuffixTree(std::string str);
    void TreePrint();
    ~TSuffixTree() {};
    friend TSuffixArray;

private:
    std::string text;
    TNode *root;
    int remainder;
    TNode *needSL, *activeNode;
    int activeLen;
    std::string::iterator activeEdge;

    void NodePrint(TNode *node, int dpth);
    int EdgeLen(TNode *node, std::string::iterator pos);
    void TreeDestroy(TNode *node);
    bool WalkDown(std::string::iterator cur_pos, TNode *node);
    void SLAdd(TNode *node);
    void TreeExtend(std::string::iterator add);
    void DFS(TNode *node, std::vector<int> &result, int deep);
};

class TSuffixArray
{
public:
    TSuffixArray(TSuffixTree tree);
    std::vector<int> Find(std::string pattern);
    ~TSuffixArray() {};
private:
    std::string text;
    std::vector<int> array;
};


#endif