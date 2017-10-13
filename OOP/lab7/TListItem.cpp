#include "TListItem.h"
#include <iostream>

template <class T>
TListItem<T>::TListItem()
{
    item = std::shared_ptr<T>(new T);
    next = nullptr;
    prev = nullptr;
}

template <class T> TAllocationBlock
TListItem<T>::listitem_allocator(sizeof(TListItem<T>), 100);

template <class T>
std::shared_ptr<T> TListItem<T>::GetFigure() const
{
    return item;
}

template <class T>
std::shared_ptr<TListItem<T>> TListItem<T>::GetNext()
{
    return next;
}

template <class T>
std::shared_ptr<TListItem<T>> TListItem<T>::GetPrev()
{
    return prev;
}

template <class T>
void TListItem<T>::SetNext(std::shared_ptr<TListItem<T>> item)
{
    next = item;
}

template <class T>
void TListItem<T>::SetPrev(std::shared_ptr<TListItem<T>> item)
{
    prev = item;
}

template <class T>
std::ostream& operator<<(std::ostream &os, const TListItem<T> &obj)
{
    os << obj.item << std::endl;
    return os;
}

template <class T>
void *TListItem<T>::operator new(size_t size)
{
    return listitem_allocator.Allocate();
}

template <class T>
void TListItem<T>::operator delete(void *ptr)
{
    listitem_allocator.Deallocate(ptr);
}


#include "TStack.h"
#include "figure.h"
template class TListItem<TStack<Figure>>;
template std::ostream& operator<<(std::ostream &out, const TListItem<TStack<Figure>> &obj);