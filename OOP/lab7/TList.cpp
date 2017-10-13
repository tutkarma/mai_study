#include "TList.h"
#include <iostream>
#include <cstdint>
#include "TStack.h"

template <class T, class F>
TList<T, F>::TList()
{
    head = nullptr;
    length = 0;
}

template <class T, class F>
void TList<T, F>::PushBack(F &figure)
{
    std::shared_ptr<TListItem<T>> tmp = this->head;
    TListItem<T> *newelem = new TListItem<T>();
    std::shared_ptr<TListItem<T>> newElem(newelem);
    if (this->IsEmpty()) {
        tmp = newElem;
        tmp->GetNext() = nullptr;
        tmp->GetPrev() = nullptr;
    } else {
        while(tmp->GetNext() != nullptr)
            tmp = tmp->GetNext();
        if (tmp->GetFigure()->GetSize() >= 4) {
            tmp->GetNext() = newElem;
        }
    }
    tmp->GetFigure()->Push(figure);
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
        while(tmp->GetNext() != nullptr)
            tmp = tmp->GetNext();
        tmp->GetFigure()->Pop();
        if(tmp->GetFigure()->IsEmpty()) {
            tmp = nullptr;
        }
    }
    --length;
}


template <class T, class F>
std::ostream& operator<<(std::ostream &os, const TList<T, F> &list)
{
    if (list.IsEmpty()) {
        os << "The list is empty." << std::endl;
        return os;
    }

    std::shared_ptr<TListItem<T>> cur(list.head);
    while(cur->GetNext() != nullptr) {
        cur->GetFigure()->Top();
        os << std::endl;
        cur = cur->GetNext();
    }
    cur->GetFigure()->Top();
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
    std::shared_ptr<TListItem<T>> iter(this->head);
    while(iter->GetNext() != nullptr) {
        iter->GetFigure()->RemGreater(value);
        iter = iter->GetNext();
    }
    iter->GetFigure()->RemGreater(value);
    //this->RemoveEmpty(); //вроде как сдвинуть все во внутренних контейнерах к началу списка
}

template <class T, class F>
void TList<T,F>::RemoveLesser(double value)
{
    std::shared_ptr<TListItem<T>> iter(this->head);
    while(iter->GetNext() != nullptr) {
        iter->GetFigure()->RemLesser(value);
        iter = iter->GetNext();
    }
    iter->GetFigure()->RemLesser(value);
    //this->RemoveEmpty();
}


template <class T, class F>
void TList<T,F>::RemoveByType(const int& type)
{
    std::shared_ptr<TListItem<T>> iter(this->head);

    while(iter->GetNext() != nullptr) {
        iter->GetFigure()->RemByType(type);
        iter = iter->GetNext();
    }
    iter->GetFigure()->RemByType(type);
    //this->RemoveEmpty();
}


#include "TStack.h"
#include "figure.h"
template class TList<TStack<Figure>, Figure>;
template std::ostream& operator<<(std::ostream &out, const TList<TStack<Figure>, Figure> &obj);