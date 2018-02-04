#include "TSuffixTree.h"

TSuffixTree::TSuffixTree(std::string str):text(str),root(new TNode(text.end(), text.end())), remainder(0)
{
    activeEdge = text.begin();
    activeNode = needSL = root->suffixLink = root;
    activeLen = 0;

    for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
        TreeExtend(it);
    }
}

TNode::TNode(std::string::iterator begin, std::string::iterator end): begin(begin), end(end), suffixLink(0) {}

void TSuffixTree::TreeDestroy(TNode *node)
{
    for (std::map<char, TNode *>::iterator it = node->to.begin(); it != node->to.end(); ++it) {
        TreeDestroy(it->second);
    }
    delete node;
}


void TSuffixTree::NodePrint(TNode *node, int dpth)
{
    for (int i = 0; i < dpth; ++i) {
        putchar('\t');
    }

    std::cout << "node represents: ";
    for (std::string::iterator str = node->begin; str != node->end; ++str) {
        std::cout << *str;
    }
    std:: cout << " node is " << node << " sufflink is " << node->suffixLink << std::endl;

    for (std::map<char, TNode *>::iterator it = node->to.begin(); it != node->to.end(); ++it) {
        NodePrint(it->second, dpth + 1);
    }
}

void TSuffixTree::TreePrint()
{
    NodePrint(root, 0);
}


void TSuffixTree::TreeExtend(std::string::iterator add)
{
    needSL = root;
    ++remainder;

    while (remainder) {
        if (!activeLen) {
            activeEdge = add;
        }
        std::map<char, TNode *>::iterator it = activeNode->to.find(*activeEdge);
        TNode *next = (it == activeNode->to.end()) ? NULL : it->second;
        if (!next) {
            TNode *leaf = new TNode(add, text.end());
            activeNode->to[*activeEdge] = leaf;
            SLAdd(activeNode);
        } else {
            if (WalkDown(add, next)) {
                continue;
            }

            if (*(next->begin + activeLen) == *add) {
                ++activeLen;
                SLAdd(activeNode);
                break;
            }

            TNode *split = new TNode(next->begin, next->begin + activeLen);
            TNode *leaf = new TNode(add, text.end());
            activeNode->to[*activeEdge] = split;

            split->to[*add] = leaf;
            next->begin += activeLen;
            split->to[*next->begin] = next;
            SLAdd(split);
        }
        --remainder;
        if (activeNode == root && activeLen) {
            --activeLen;
            activeEdge = add - remainder + 1;
        } else {
            activeNode = (activeNode->suffixLink) ? activeNode->suffixLink : root;
        }
    }
}

int TSuffixTree::EdgeLen(TNode *node, std::string::iterator pos)
{
    return min(node->end, pos + 1) - node->begin;
}

bool TSuffixTree::WalkDown(std::string::iterator cur_pos, TNode *node)
{
    if (activeLen >= EdgeLen(node, cur_pos)) {
        activeEdge += EdgeLen(node, cur_pos);
        activeLen -= EdgeLen(node, cur_pos);
        activeNode = node;
        return true;
    }
    return false;
}

void TSuffixTree::SLAdd(TNode *node)
{
    if (needSL != root) {
        needSL->suffixLink = node;
    }

    needSL = node;
}

void TSuffixTree::DFS(TNode *node, std::vector<int> &result, int deep)
{
    if (node->to.empty()) {
        result.push_back(text.size() - deep);
        return;
    }
    for (std::map<char, TNode *>::iterator it = node->to.begin(); it != node->to.end(); ++it) {
        int tmp = deep;
        tmp += it->second->end - it->second->begin;
        DFS(it->second, result, tmp);
    }
}

TSuffixArray::TSuffixArray(TSuffixTree tree):text(tree.text), array()
{
    tree.DFS(tree.root, array, 0);
    tree.TreeDestroy(tree.root);
}

std::vector<int> TSuffixArray::Find(std::string pattern)
{
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> range(array.begin(), array.end());
    for (int i = 0; i < pattern.size() && range.first != range.second; ++i) {
        range = equal_range(range.first, range.second, std::numeric_limits<int>::max(), [this, &pattern, &i] (int idx1, int idx2) -> bool {
            if (idx1 == std::numeric_limits<int>::max()) {
                return bool(pattern[i] < text[i + idx2]);
            } else {
                return bool(text[i + idx1] < pattern[i]);
            }
        });
    }

    std::vector<int> result(range.first, range.second);
    std::sort(result.begin(), result.end());

    return result;
}
