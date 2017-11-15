#ifndef TLIST_H
#define TLIST_H

#include <cstdint>
#include <future>
#include <mutex>
#include <thread>
#include <functional>
#include "trapeze.h"
#include "rhomb.h"
#include "pentagon.h"
#include "TIterator.h"
#include "TListItem.h"

template <class T>
class TList
{
public:
    TList();
    void Push(const std::shared_ptr<T> &obj);
    void PushFirst(const std::shared_ptr<T> &obj);
    const bool IsEmpty() const;
    uint32_t GetLength();
    std::shared_ptr<T> Pop(int32_t ind);
    void Del();

    TIterator<TListItem<T>,T> begin();
    TIterator<TListItem<T>,T> end();

    void Sort();
    void ParSort();

    template <class A> friend std::ostream& operator<<(std::ostream &os, const TList<A> &list);
    virtual ~TList();

private:
    uint32_t length;
    std::shared_ptr<TListItem<T>> head;
    std::shared_ptr<TListItem<T>> PSort(std::shared_ptr <TListItem<T>>&);
    std::shared_ptr<TListItem<T>> PParSort(std::shared_ptr<TListItem<T>> &head);
    std::shared_ptr<TListItem<T>> Partition(std::shared_ptr<TListItem<T>>&);
    std::mutex mutex;

    void PushLast(const std::shared_ptr<T> &obj);
    void PushAtIndex(const std::shared_ptr<T> &obj, int32_t ind);
    std::shared_ptr<T> PopFirst();
    std::shared_ptr<T> PopLast();
    std::shared_ptr<T> PopAtIndex(int32_t ind);
};

#endif
