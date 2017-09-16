#ifndef TLISTITEM_H
#define TLISTITEM_H

#include <memory>
#include "trapeze.h"
#include "rhomb.h"
#include "pentagon.h"

template <class T>
class TListItem
{
public:
    TListItem(const std::shared_ptr<T> &obj);

    std::shared_ptr<T> GetFigure() const;
    std::shared_ptr<TListItem<T>> GetNext();
    std::shared_ptr<TListItem<T>> GetPrev();
    void SetNext(std::shared_ptr<TListItem<T>> item);
    void SetPrev(std::shared_ptr<TListItem<T>> item);
    template <class A> friend std::ostream& operator<<(std::ostream &os, const TListItem<A> &obj);

    virtual ~TListItem(){};

private:
    std::shared_ptr<T> item;
    std::shared_ptr<TListItem<T>> next;
    std::shared_ptr<TListItem<T>> prev;
};

#endif
