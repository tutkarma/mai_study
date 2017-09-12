#include "TList.h"
#include <iostream>
#include <cstdint>

TList::TList()
{
    head = nullptr;
    length = 0;
}

void TList::Push(Trapeze &obj) {
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
        std::cout << "In push in func" << std::endl;
        this->PushLast(obj);
    } else {
        this->PushAtIndex(obj, index);
    }
    ++length;
}

void TList::PushAtIndex(Trapeze &obj, int32_t ind)
{
    std::cout << "In func in ind" << std::endl;
    auto *newItem = new TListItem(obj);
    auto *tmp = this->head;
    for(int32_t i = 0; i < ind; ++i){
        std::cout << "idu po listu" << std::endl;
        tmp = tmp->GetNext();
    }
    std::cout << "stavlu item" << std::endl;
    tmp->SetPrev(newItem);
    newItem->SetNext(tmp);
    tmp->GetPrev()->SetNext(newItem);
    newItem->SetPrev(tmp->GetPrev());

}

void TList::PushLast(Trapeze &obj)
{
    std::cout << "In push last" << std::endl;
    auto *newItem = new TListItem(obj);
    auto *tmp = this->head;

    while (tmp->GetNext() != nullptr) {
        tmp = tmp->GetNext();
    }
    tmp->SetNext(newItem);
    newItem->SetPrev(tmp);
}

void TList::PushFirst(Trapeze &obj)
{
    auto *newItem = new TListItem(obj);
    auto *oldHead = this->head;
    this->head = newItem;
    if(oldHead != nullptr) {
        newItem->SetNext(oldHead);
        oldHead->SetPrev(newItem);
    }
}

uint32_t TList::GetLength()
{
    return this->length;
}

const bool TList::IsEmpty() const
{
    return head == nullptr;
}

Trapeze TList::Pop()
{
    int32_t ind = 0;
    std::cout << "Enter index = ";
    std::cin >> ind;
    Trapeze res;
    if (ind > this->GetLength() || ind < 0 || this->IsEmpty()) {
        std::cout << "change index(pop)" << std::endl;
        return res;
    }

    if (ind == 0) {
        res = this->PopFirst();
    } else if (ind == this->GetLength()) {
        res = this->PopLast();
    } else {
        res = this->PopAtIndex(ind);
    }
    --length;
    return res;
}

Trapeze TList::PopAtIndex(int32_t ind)
{
    auto *tmp = this->head;
    for(int32_t i = 0; i < ind; ++i) {
        tmp = tmp->GetNext();
    }
    auto res = tmp->GetFigure();
    auto *nextItem = tmp->GetNext();
    auto *prevItem = tmp->GetPrev();
    nextItem->SetPrev(prevItem);
    prevItem->SetNext(nextItem);
    delete tmp;
    return res;
}

Trapeze TList::PopFirst()
{
    if (this->GetLength() == 1) {
        auto res = this->head->GetFigure();
        delete this->head;
        this->head = nullptr;
        return res;
    }
    auto *removed = this->head;
    auto res = removed->GetFigure();
    this->head = this->head->GetNext();
    this->head->SetPrev(nullptr);
    delete removed;
    return res;
}

Trapeze TList::PopLast()
{
    if (this->GetLength() == 1) {
        auto res = this->head->GetFigure();
        delete this->head;
        this->head = nullptr;
        return res;
    }

    auto *tmp = this->head;
    while (tmp->GetNext()) {
        tmp = tmp->GetNext();
    }
    auto res = tmp->GetFigure();
    tmp->GetPrev()->SetNext(nullptr);
    delete tmp;
    return res;
}


std::ostream& operator<<(std::ostream &os, const TList &list)
{
    if (list.IsEmpty()) {
        os << "The list is empty." << std::endl;
        return os;
    }


    auto *tmp = list.head;
    for(int32_t i = 0; tmp; ++i) {
        os << "idx: " << i << "   " << tmp->GetFigure() << std::endl;
        tmp = tmp->GetNext();
    }

    return os;
}

TList::~TList()
{
    auto *tmp = head;
    while (head) {
        head = head->GetNext();
        delete tmp;
        tmp = head;
    }
}

