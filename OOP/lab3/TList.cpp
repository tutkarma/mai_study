#include "TList.h"
#include <iostream>
#include <cstdint>

TList::TList()
{
    head = nullptr;
    length = 0;
}

void TList::Push(std::shared_ptr<Figure> &obj) {
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

void TList::PushAtIndex(std::shared_ptr<Figure> &obj, int32_t ind)
{
    std::shared_ptr<TListItem> newItem = std::make_shared<TListItem>(obj);
    std::shared_ptr<TListItem> tmp = this->head;
    for(int32_t i = 1; i < ind; ++i){
        tmp = tmp->GetNext();
    }
    newItem->SetNext(tmp->GetNext());
    newItem->SetPrev(tmp);
    tmp->SetNext(newItem);
    tmp->GetNext()->SetPrev(newItem);
}

void TList::PushLast(std::shared_ptr<Figure> &obj)
{
    std::shared_ptr<TListItem> newItem = std::make_shared<TListItem>(obj);
    std::shared_ptr<TListItem> tmp = this->head;

    while (tmp->GetNext() != nullptr) {
        tmp = tmp->GetNext();
    }
    tmp->SetNext(newItem);
    newItem->SetPrev(tmp);
    newItem->SetNext(nullptr);
}

void TList::PushFirst(std::shared_ptr<Figure> &obj)
{
    std::shared_ptr<TListItem> newItem = std::make_shared<TListItem>(obj);
    std::shared_ptr<TListItem> oldHead = this->head;
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

std::shared_ptr<Figure> TList::Pop()
{
    int32_t ind = 0;
    std::cout << "Enter index = ";
    std::cin >> ind;
    std::shared_ptr<Figure> res;
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

std::shared_ptr<Figure> TList::PopAtIndex(int32_t ind)
{
    std::shared_ptr<TListItem> tmp = this->head;
    for(int32_t i = 0; i < ind - 1; ++i) {
        tmp = tmp->GetNext();
    }
    std::shared_ptr<TListItem> removed = tmp->GetNext();
    std::shared_ptr<Figure> res = removed->GetFigure();
    std::shared_ptr<TListItem> nextItem = removed->GetNext();
    tmp->SetNext(nextItem);
    nextItem->SetPrev(tmp);
    return res;
}

std::shared_ptr<Figure> TList::PopFirst()
{
    if (this->GetLength() == 1) {
        std::shared_ptr<Figure> res = this->head->GetFigure();
        this->head = nullptr;
        return res;
    }
    std::shared_ptr<TListItem> tmp = this->head;
    std::shared_ptr<Figure> res = tmp->GetFigure();
    this->head = this->head->GetNext();
    this->head->SetPrev(nullptr);
    return res;
}

std::shared_ptr<Figure> TList::PopLast()
{
    if (this->GetLength() == 1) {
        std::shared_ptr<Figure> res = this->head->GetFigure();
        this->head = nullptr;
        return res;
    }
    std::shared_ptr<TListItem> tmp = this->head;
    while(tmp->GetNext()->GetNext()) {
        tmp = tmp->GetNext();
    }
    std::shared_ptr<TListItem> removed = tmp->GetNext();
    std::shared_ptr<Figure>res = removed->GetFigure();
    tmp->SetNext(removed->GetNext());
    return res;
}


std::ostream& operator<<(std::ostream &os, const TList &list)
{
    if (list.IsEmpty()) {
        os << "The list is empty." << std::endl;
        return os;
    }

    std::shared_ptr<TListItem> tmp = list.head;
    for(int32_t i = 0; tmp; ++i) {
        os << "idx: " << i << "   ";
        tmp->GetFigure()->Print();
        os << std::endl;
        tmp = tmp->GetNext();
    }

    return os;
}

void TList::Del()
{
    while(!this->IsEmpty()) {
        this->PopFirst();
        --length;
    }
}

TList::~TList()
{
/*    while(!this->IsEmpty()) {
        this->PopFirst();
        --length;
    }*/
}

