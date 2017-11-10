#ifndef TNODE_H
#define TNODE_H
#include "TList.h"

class TNode
{
private:
    friend TList;
    Square keyS;
    Trap keyT;
    Pryam keyP;
    std::shared_ptr<TNode> next;
public:
    TNode();
    TNode(const Square&, const Trap&, const Pryam&);
    void GetNext() const;
    friend std::ostream& operator<< (std::ostream&, const TNode&);
};

#endif