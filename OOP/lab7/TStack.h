#ifndef TSTACK_H
#define TSTACK_H

#include "figure.h"
#include <memory>
#include <iostream>

template <typename Q, typename O> class TStack
{
private:
    class Node {
    public:
        Q data;
        std::shared_ptr<Node> next;
        Node();
        Node(const O&);
        int itemsInNode;
    };

    std::shared_ptr<Node> head;
    int count;
public:
    TStack();

    void push(const O&);
    void print(); 
    void removeByType(const int&);
    void removeLesser(const double&);
};

#include "TStack.hpp"
#endif