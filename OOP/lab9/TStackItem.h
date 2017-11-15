#ifndef _TSTACKITEM_H_
#define _TSTACKITEM_H_

#include <iostream>
#include <memory>

template <class T>
class TStackItem
{
public:
    TStackItem(const T &val, TStackItem<T> *item);
    virtual ~TStackItem();

    void Push(const T &val);
    T &Pop() const;
    void SetNext(TStackItem<T> *item);
    TStackItem<T> &GetNext() const;

    T *value;
    TStackItem<T> *next;
};

#endif