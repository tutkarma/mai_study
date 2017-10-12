#include "TList.h"
#include <iostream>
#include <cstdint>

template <class T, class F>
TList<T, F>::TList()
{
    head = nullptr;
    length = 0;
}

template <class T, class F>
void TList<T, F>::PushBack(const F& item)
{
    std::shared_ptr<TListItem<T>> tmp = this->head;
    TListItem *newelem = new TListItem<T>();
    std::shared_ptr<TListItem<T>> newElem(newelem);
    if (this->IsEmpty()) {
        tmp = newElem;
        tmp->next = nullptr;
        tmp->prev = nullptr;
    } else {
        while(tmp->next != nullptr)
            tmp = tmp->next;
        if (tmp->item->count >= 4) {
            tmp->next = newElem;
        }
    }
    tmp->item->Push(figure);
    ++length;
}


template <class T, class F>
uint32_t TList<T, F>::GetLength()
{
    return this->length;
}

template <class T, class F>
const bool TList<T, F>::IsEmpty() const
{
    return head == nullptr;
}

template <class T, class F>
void TList<T, F>::Pop()
{
    if (this->IsEmpty()) {
        std::cout << "List is empty." << std::endl;
    } else {
        std::shared_ptr<TListItem<T>> tmp = this->head;
        while(tmp->next != nullptr)
            tmp = tmp->next;
        tmp->item->Pop();
        if(tmp->item->IsEmpty()) {
            tmp = nullptr;
        }
    }
    --length;
}


template <class A, class B>
std::ostream& operator<<(std::ostream &os, const TList<A, B> &list)
{
    if (list.IsEmpty()) {
        os << "The list is empty." << std::endl;
        return os;
    }

    std::shared_ptr<TListItem<T>> cur(list.head);
    while(cur->GetNext() != nullptr) {
        cur->GetFigure()->Print();
        os << std::endl;
        cur = cur->GetNext();
    }
    cur->GetFigure()->Print();
    os << std::endl;
    /*for(int32_t i = 0; tmp; ++i) {
        os << "idx: " << i << "   ";
        tmp->GetFigure()->Print();
        os << std::endl;
        tmp = tmp->GetNext();
    }*/

    return os;
}

template <class T, class F>
void TList<T, F>::Del()
{
    while(!this->IsEmpty()) {
        this->Pop();
        --length;
    }
}

template <class T, class F>
TIterator<TListItem<T>,T> TList<T, F>::begin()
{
    return TIterator<TListItem<T>,T>(head);
}

template <class T, class F>
TIterator<TListItem<T>,T> TList<T, F>::end()
{
    return TIterator<TListItem<T>,T>(nullptr);
}

template <class T, class F>
TList<T, F>::~TList()
{
}

template <class T, class F>
void TList<T,F>::RemoveGreater(double value)
{
    std::shared_ptr<TListItem<T>> iter(this.head);
    while(iter->next != nullptr) {
        iter->item->RemGreater(figure);
        iter = iter->next;
    }
    iter->item->RemGreater(figure);
    this->RemoveEmpty();
}

template <class T, class F>
void TList<T,F>::RemoveLesser(double value)
{
    std::shared_ptr<TListItem<T>> iter(this.head);
    while(iter->next != nullptr) {
        iter->item->RemLesser(figure);
        iter = iter->next;
    }
    iter->item->RemLesser(figure);
    this->RemoveEmpty();
}


template <class T, class F>
void TList<T,F>::RemoveByType(Figure *figure)
{
    std::shared_ptr<TListItem<T>> iter(this.head);

    while(iter->next != nullptr) {
        iter->item->RemByType(figure);
        iter = iter->next;
    }
    iter->item->RemByType(figure);
    this->RemoveEmpty();
}


#include "TStack.h"
#include "figure.h"
template class TList<TStack<Figure>, Figure>;
template std::ostream& operator<<(std::ostream &out, const TList<TStack<Figure>, Figure> &obj);