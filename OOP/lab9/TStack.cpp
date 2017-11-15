#include <iostream>
#include <memory>
#include "TStack.h"

template <class T>
TStack<T>::TStack()
{
    head = nullptr;
    count = 0;
}


template <class T>
void TStack<T>::Push(const T &item)
{
    TStackItem<T> *tmp = new TStackItem<T>(item, head);
    head = tmp;
    ++count;
}

template <class T>
bool TStack<T>::IsEmpty() const
{
    return !count;
}

template <class T>
uint32_t TStack<T>::GetSize() const
{
    return count;
}

template <class T>
void TStack<T>::Pop()
{
    if(head) {
        TStackItem<T> *tmp = &head->GetNext();
        delete head;
        head = tmp;
        --count;
    }
}

template <class T>
T &TStack<T>::Top()
{
    return head->Pop();
}

template <class T>
TStack<T>::~TStack()
{
    for(TStackItem<T> *tmp = head, *tmp2; tmp; tmp = tmp2) {
        tmp2 = &tmp->GetNext();
        delete tmp;
    }
}

/*template <class T>
void TStack<T>::Print()
{
    auto tmp = head;
    while (tmp) {
        auto future = std::async(std::launch::async, *(tmp->value));
        future.get();
        tmp = tmp->next;
    }
}*/

typedef unsigned char Byte;

template class TStack<void *>;
template class TStack<std::shared_ptr<std::function<void(void)>>>;