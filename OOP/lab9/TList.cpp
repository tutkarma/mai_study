#include "TList.h"
#include <iostream>
#include <cstdint>

template <class T>
TList<T>::TList()
{
    head = std::make_shared<TListItem<T>>();
    length = 0;
}

template <class T>
void TList<T>::Push(const std::shared_ptr<T> &obj)
{
    int32_t index = 0;
    std::cout << "Enter index = ";
    std::cin >> index;
    if (index > this->GetLength() - 1 || index < 0) {
        std::cerr << "This index doesn't exist\n";
        return;
    }
    if (index == 0) {
        this->PushFirst(obj);
    } else if (index == this->GetLength() - 1) {
        this->PushLast(obj);
    } else {
        this->PushAtIndex(obj, index);
    }
}

template <class T>
void TList<T>::PushAtIndex(const std::shared_ptr<T> &obj, int32_t ind)
{
    std::shared_ptr<TListItem<T>> newItem = std::make_shared<TListItem<T>>(obj);
    std::shared_ptr<TListItem<T>> tmp = this->head->GetNext();
    for(int32_t i = 1; i < ind; ++i){
        tmp = tmp->GetNext();
    }
    newItem->SetNext(tmp->GetNext());
    newItem->SetPrev(tmp);
    tmp->SetNext(newItem);
    tmp->GetNext()->SetPrev(newItem);
    ++length;
}

template <class T>
void TList<T>::PushLast(const std::shared_ptr<T> &obj)
{
    std::shared_ptr<TListItem<T>> newItem = std::make_shared<TListItem<T>>(obj);
    std::shared_ptr<TListItem<T>> tmp = this->head->GetNext();

    while (tmp->GetNext() != nullptr) {
        tmp = tmp->GetNext();
    }
    tmp->SetNext(newItem);
    newItem->SetPrev(tmp);
    newItem->SetNext(nullptr);
    ++length;
}

template <class T>
void TList<T>::PushFirst(const std::shared_ptr<T> &obj)
{
    std::shared_ptr<TListItem<T>> newItem = std::make_shared<TListItem<T>>(obj);
    std::shared_ptr<TListItem<T>> oldHead = this->head->GetNext();
    this->head->SetNext(newItem);
    if(oldHead != nullptr) {
        newItem->SetNext(oldHead);
        oldHead->SetPrev(newItem);
    }
    ++length;
}

template <class T>
uint32_t TList<T>::GetLength()
{
    return this->length;
}

template <class T>
const bool TList<T>::IsEmpty() const
{
    return this->length == 0;
}

template <class T>
std::shared_ptr<T> TList<T>::Pop(int32_t ind)
{
    //int32_t ind = 0;
    //std::cout << "Enter index = ";
    //std::cin >> ind;
    std::shared_ptr<T> res;
    if (ind > this->GetLength() - 1 || ind < 0 || this->IsEmpty()) {
        std::cout << "Change index" << std::endl;
        return res;
    }

    if (ind == 0) {
        res = this->PopFirst();
    } else if (ind == this->GetLength() - 1) {
        res = this->PopLast();
    } else {
        res = this->PopAtIndex(ind);
    }
    return res;
}

template <class T>
std::shared_ptr<T> TList<T>::PopAtIndex(int32_t ind)
{
    std::shared_ptr<TListItem<T>> tmp = this->head->GetNext();
    for(int32_t i = 0; i < ind - 1; ++i) {
        tmp = tmp->GetNext();
    }
    std::shared_ptr<TListItem<T>> removed = tmp->GetNext();
    std::shared_ptr<T> res = removed->GetFigure();
    std::shared_ptr<TListItem<T>> nextItem = removed->GetNext();
    tmp->SetNext(nextItem);
    nextItem->SetPrev(tmp);
    --length;
    return res;
}

template <class T>
std::shared_ptr<T> TList<T>::PopFirst()
{
    if (this->GetLength() == 1) {
        std::shared_ptr<T> res = this->head->GetNext()->GetFigure();
        this->head->SetNext(nullptr);
        return res;
    }
    std::shared_ptr<TListItem<T>> tmp = this->head->GetNext();
    std::shared_ptr<T> res = tmp->GetFigure();
    this->head->SetNext(this->head->GetNext()->GetNext());
    this->head->GetNext()->SetPrev(nullptr);
    --length;
    return res;
}

template <class T>
std::shared_ptr<T> TList<T>::PopLast()
{
    if (this->GetLength() == 1) {
        std::shared_ptr<T> res = this->head->GetNext()->GetFigure();
        this->head->SetNext(nullptr);
        return res;
    }
    std::shared_ptr<TListItem<T>> tmp = this->head->GetNext();
    while(tmp->GetNext()->GetNext()) {
        tmp = tmp->GetNext();
    }
    std::shared_ptr<TListItem<T>> removed = tmp->GetNext();
    std::shared_ptr<T> res = removed->GetFigure();
    tmp->SetNext(removed->GetNext());
    --length;
    return res;
}


template <class T>
std::ostream& operator<<(std::ostream &os, const TList<T> &list)
{
    if (list.IsEmpty()) {
        os << "The list is empty." << std::endl;
        return os;
    }

    std::shared_ptr<TListItem<T>> tmp = list.head->GetNext();
    for(int32_t i = 0; tmp; ++i) {
        os << "idx: " << i << "   ";
        tmp->GetFigure()->Print();
        os << std::endl;
        tmp = tmp->GetNext();
    }

    return os;
}

template <class T>
void TList<T>::Del()
{
    while(!this->IsEmpty()) {
        this->PopFirst();
        --length;
    }
}

template <class T>
TIterator<TListItem<T>,T> TList<T>::begin()
{
    return TIterator<TListItem<T>,T>(head->GetNext());
}

template <class T>
TIterator<TListItem<T>,T> TList<T>::end()
{
    return TIterator<TListItem<T>,T>(nullptr);
}

template <class T>
TList<T>::~TList()
{
}

template <class T>
std::shared_ptr<TListItem<T>> TList<T>::PSort(std::shared_ptr<TListItem<T>> &head)
{
    if (head == nullptr || head->GetNext() == nullptr) {
        return head;
    }

    std::shared_ptr<TListItem<T>> partitionedEl = Partition(head);
    std::shared_ptr<TListItem<T>> leftPartition = partitionedEl->GetNext();
    std::shared_ptr<TListItem<T>> rightPartition = head;

    partitionedEl->SetNext(nullptr);

    if (leftPartition == nullptr) {
        leftPartition = head;
        rightPartition = head->GetNext();
        head->SetNext(nullptr);
    }

    rightPartition = PSort(rightPartition);
    leftPartition = PSort(leftPartition);
    std::shared_ptr<TListItem<T>> iter = leftPartition;
    while (iter->GetNext() != nullptr) {
        iter = iter->GetNext();
    }

    iter->SetNext(rightPartition);

    return leftPartition;
}

template <class T>
std::shared_ptr<TListItem<T>> TList<T>::Partition(std::shared_ptr<TListItem<T>> &head)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (head->GetNext()->GetNext() = nullptr) {
        if (head->GetNext()->GetFigure()->Square() > head->GetFigure()->Square()) {
            return head->GetNext();
        } else {
            return head;
        }
    } else {
        std::shared_ptr<TListItem<T>> i = head->GetNext();
        std::shared_ptr<TListItem<T>> pivot = head;
        std::shared_ptr<TListItem<T>> lastElSwapped = (pivot->GetNext()->GetFigure()->Square() >= pivot->GetFigure()->Square()) ? pivot->GetNext() : pivot;

        while ((i != nullptr) && (i->GetNext() != nullptr)) {
            if (i->GetNext()->GetFigure()->Square() >= pivot->GetFigure()->Square()) {
                if (i->GetNext() == lastElSwapped->GetNext()) {
                    lastElSwapped = lastElSwapped->GetNext();
                } else {
                    std::shared_ptr<TListItem<T>> tmp = lastElSwapped->GetNext();
                    lastElSwapped->SetNext(i->GetNext());
                    i->SetNext(i->GetNext()->GetNext());
                    lastElSwapped = lastElSwapped->GetNext();
                    lastElSwapped->SetNext(tmp);
                }
            }
            i = i->GetNext();
        }
        return lastElSwapped;
    }
}

template <class T>
void TList<T>::Sort()
{
    if (head == nullptr)
        return;
    std::shared_ptr<TListItem<T>> tmp = head->GetNext();
    head->SetNext(PSort(tmp));
}

template <class T>
void TList<T>::ParSort()
{
    if (head == nullptr)
        return;
    std::shared_ptr<TListItem<T>> tmp = head->GetNext();
    head->SetNext(PParSort(tmp));
}

template <class T>
std::shared_ptr<TListItem<T>> TList<T>::PParSort(std::shared_ptr<TListItem<T>> &head)
{
    if (head == nullptr || head->GetNext() == nullptr) {
        return head;
    }

    std::shared_ptr<TListItem<T>> partitionedEl = Partition(head);
    std::shared_ptr<TListItem<T>> leftPartition = partitionedEl->GetNext();
    std::shared_ptr<TListItem<T>> rightPartition = head;

    partitionedEl->SetNext(nullptr);

    if (leftPartition == nullptr) {
        leftPartition = head;
        rightPartition = head->GetNext();
        head->SetNext(nullptr);
    }

    std::packaged_task<std::shared_ptr<TListItem<T>>(std::shared_ptr<TListItem<T>>&)>
        task1(std::bind(&TList<T>::PParSort, this, std::placeholders::_1));
    std::packaged_task<std::shared_ptr<TListItem<T>>(std::shared_ptr<TListItem<T>>&)>
        task2(std::bind(&TList<T>::PParSort, this, std::placeholders::_1));
    auto rightPartitionHandle = task1.get_future();
    auto leftPartitionHandle = task2.get_future();

    std::thread(std::move(task1), std::ref(rightPartition)).join();
    rightPartition = rightPartitionHandle.get();
    std::thread(std::move(task2), std::ref(leftPartition)).join();
    leftPartition = leftPartitionHandle.get();
    std::shared_ptr<TListItem<T>> iter = leftPartition;
    while (iter->GetNext() != nullptr) {
        iter = iter->GetNext();
    }

    iter->SetNext(rightPartition);
    return leftPartition;
}

#include "figure.h"
template class TList<Figure>;
template std::ostream& operator<<(std::ostream &out, const TList<Figure> &obj);