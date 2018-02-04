#include <queue>
#include <iostream>

#include "TTrie.h"


TTrieNode::TTrieNode()
{
    linkFail = nullptr;
}

TTrie::TTrie()
{
    root = new TTrieNode();
    root->linkFail = root;
    withoutJoker = 0;
}


void TTrie::Create(const std::vector<std::string> &patterns)
{
    int patternNumber = 0;
    int patLen = patterns.size();
    lensPatterns.resize(patLen);
    int i;

    TTrieNode *curNode = root;
    for (i = 0; i < patLen; ++i) {
        if (patterns[i] == "?") {
            if (curNode != root) {
                curNode->out.push_back(patternNumber);
                lensPatterns[patternNumber] = i - patternNumber;
                ++withoutJoker;
                curNode = root;
            }
            patternNumber = i + 1;
            continue;
        }

        std::map<unsigned long, TTrieNode *>::iterator nodeToInsert = curNode->to.find(std::stoul(patterns[i]));

        if (nodeToInsert != curNode->to.end()) { //exist
            curNode = nodeToInsert->second;
        } else {
            TTrieNode *newNode = new TTrieNode();
            curNode->to[std::stoul(patterns[i])] = newNode;
            curNode = newNode;
        }
    }

    if (curNode != root) {
        curNode->out.push_back(patternNumber);
        lensPatterns[patternNumber] = i - patternNumber;
        ++withoutJoker;
    }

    CreateLinks();
}

void TTrie::CreateLinks()
{
    TTrieNode* curNode = root;
    std::queue<TTrieNode *> queue;
    queue.push(curNode);

    while (!queue.empty()) {
        curNode = queue.front();
        queue.pop();

        std::map<unsigned long, TTrieNode *>::iterator iter;
        for (iter = curNode->to.begin(); iter != curNode->to.end(); ++iter) {
            TTrieNode *child = iter->second;
            TTrieNode *parentFail = curNode->linkFail;
            unsigned long subPattern = iter->first;

            queue.push(child);

            while(1) {
                std::map<unsigned long, TTrieNode *>::iterator existingNode = parentFail->to.find(subPattern);

                if (existingNode != parentFail->to.end()) {
                    child->linkFail = (existingNode->second != child) ? existingNode->second : root;
                    child->out.insert(child->out.end(), child->linkFail->out.begin(), child->linkFail->out.end());
                    break;
                }

                if (parentFail == root) {
                    child->linkFail = root;
                    break;
                } else {
                    parentFail = parentFail->linkFail;
                }
            }
        }
    }
}


void TTrie::Search(const std::vector<unsigned long> &text, const int &patternLen, std::vector<std::pair<int, int> > &answer)
{
    int textLen = text.size();
    std::vector<int> entry(textLen, 0);
    TTrieNode *curNode = root;
    std::map<unsigned long, TTrieNode *>::iterator existingNode;
    int i;

    for (i = 0; i < textLen; ++i) {
        existingNode = curNode->to.find(text[i]);

        while (existingNode == curNode->to.end()) {
            curNode = curNode->linkFail;
            existingNode = curNode->to.find(text[i]);
            if (curNode == curNode->linkFail) {
                break;
            }
        }

        if (existingNode != curNode->to.end()) {
            curNode = existingNode->second;

            if (!curNode->out.empty()) {
                std::vector<int>::iterator iter;
                for (iter = curNode->out.begin(); iter != curNode->out.end(); ++iter) {
                    int patternId = i - lensPatterns[*iter] - *iter + 1;
                    if (patternId < 0) {
                        continue;
                    }
                    entry[patternId]++; // entrance
                }
            }
        }
    }

    for (i = 0; i < textLen; ++i) {
        if ((entry[i] == withoutJoker) && (i + patternLen <= textLen)) {
            std::cout << answer[i].first << ", " << answer[i].second << std::endl;
        }
    }
}