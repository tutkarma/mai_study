#include "TList.h"
#include <iostream>
#include <cstdint>

template <class T>
TList<T>::TList()
{
    head = nullptr;
    length = 0;
}

template <class T>
void TList<T>::Push(std::shared_ptr<T> &obj)
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
    ++length;
}

template <class T>
void TList<T>::PushAtIndex(std::shared_ptr<T> &obj, int32_t ind)
{
    std::shared_ptr<TListItem<T>> newItem = std::make_shared<TListItem<T>>(obj);
    std::shared_ptr<TListItem<T>> tmp = this->head;
    for(int32_t i = 1; i < ind; ++i){
        tmp = tmp->GetNext();
    }
    newItem->SetNext(tmp->GetNext());
    newItem->SetPrev(tmp);
    tmp->SetNext(newItem);
    tmp->GetNext()->SetPrev(newItem);
}

template <class T>
void TList<T>::PushLast(std::shared_ptr<T> &obj)
{
    std::shared_ptr<TListItem<T>> newItem = std::make_shared<TListItem<T>>(obj);
    std::shared_ptr<TListItem<T>> tmp = this->head;

    while (tmp->GetNext() != nullptr) {
        tmp = tmp->GetNext();
    }
    tmp->SetNext(newItem);
    newItem->SetPrev(tmp);
    newItem->SetNext(nullptr);
}

template <class T>
void TList<T>::PushFirst(std::shared_ptr<T> &obj)
{
    std::shared_ptr<TListItem<T>> newItem = std::make_shared<TListItem<T>>(obj);
    std::shared_ptr<TListItem<T>> oldHead = this->head;
    this->head = newItem;
    if(oldHead != nullptr) {
        newItem->SetNext(oldHead);
        oldHead->SetPrev(newItem);
    }
}

template <class T>
uint32_t TList<T>::GetLength()
{
    return this->length;
}

template <class T>
const bool TList<T>::IsEmpty() const
{
    return head == nullptr;
}

template <class T>
std::shared_ptr<T> TList<T>::Pop()
{
    int32_t ind = 0;
    std::cout << "Enter index = ";
    std::cin >> ind;
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
    --length;
    return res;
}

template <class T>
std::shared_ptr<T> TList<T>::PopAtIndex(int32_t ind)
{
    std::shared_ptr<TListItem<T>> tmp = this->head;
    for(int32_t i = 0; i < ind - 1; ++i) {
        tmp = tmp->GetNext();
    }
    std::shared_ptr<TListItem<T>> removed = tmp->GetNext();
    std::shared_ptr<T> res = removed->GetFigure();
    std::shared_ptr<TListItem<T>> nextItem = removed->GetNext();
    tmp->SetNext(nextItem);
    nextItem->SetPrev(tmp);
    return res;
}

template <class T>
std::shared_ptr<T> TList<T>::PopFirst()
{
    if (this->GetLength() == 1) {
        std::shared_ptr<T> res = this->head->GetFigure();
        this->head = nullptr;
        return res;
    }
    std::shared_ptr<TListItem<T>> tmp = this->head;
    std::shared_ptr<T> res = tmp->GetFigure();
    this->head = this->head->GetNext();
    this->head->SetPrev(nullptr);
    return res;
}

template <class T>
std::shared_ptr<T> TList<T>::PopLast()
{
    if (this->GetLength() == 1) {
        std::shared_ptr<T> res = this->head->GetFigure();
        this->head = nullptr;
        return res;
    }
    std::shared_ptr<TListItem<T>> tmp = this->head;
    while(tmp->GetNext()->GetNext()) {
        tmp = tmp->GetNext();
    }
    std::shared_ptr<TListItem<T>> removed = tmp->GetNext();
    std::shared_ptr<T> res = removed->GetFigure();
    tmp->SetNext(removed->GetNext());
    return res;
}


template <class T>
std::ostream& operator<<(std::ostream &os, const TList<T> &list)
{
    if (list.IsEmpty()) {
        os << "The list is empty." << std::endl;
        return os;
    }

    std::shared_ptr<TListItem<T>> tmp = list.head;
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
    return TIterator<TListItem<T>,T>(head);
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

#include "figure.h"
template class TList<Figure>;
template std::ostream& operator<<(std::ostream &out, const TList<Figure> &obj);