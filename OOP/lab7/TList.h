#ifndef TLIST_H
#define TLIST_H

#include <cstdint>
#include "trapeze.h"
#include "rhomb.h"
#include "pentagon.h"
#include "TStack.h"
#include "TIterator.h"
#include "TListItem.h"

template <class T, class F> // T - container, F - figure
class TList
{
public:
    TList();
    virtual ~TList();

    void PushBack(F &figure);
    const bool IsEmpty() const;
    uint32_t GetLength();
    void Pop();
    void Del();

    TIterator<TListItem<T>,T> begin();
    TIterator<TListItem<T>,T> end();

    template <class A, class B>
    friend std::ostream& operator<<(std::ostream &os, const TList<A, B> &list);


    void RemoveByType(const int&);
    void RemoveGreater(double value);
    void RemoveLesser(double value);

private:
    uint32_t length;
    std::shared_ptr<TListItem<T>> head;

    void PopLast();
};

#endif
