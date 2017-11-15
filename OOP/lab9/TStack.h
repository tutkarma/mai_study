#ifndef TSTACK_H
#define TSTACK_H

#include <iostream>
#include <memory>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include "TStackItem.h"

template <class T>
class TStack
{
public:
    TStack();
    virtual ~TStack();

    void Push(const T &item);
    void Pop();
    T &Top();
    bool IsEmpty() const;
    uint32_t GetSize() const;
    //void Print();

    template <class A> friend std::ostream& operator<<(std::ostream &os, const TStack<A> &stack);

private:
    TStackItem<T> *head;
    uint32_t count;
};

#endif