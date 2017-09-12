#include "TListItem.h"
#include <iostream>

TListItem::TListItem(const Trapeze &obj)
{
    this->item = obj;
    this->next = nullptr;
    this->prev = nullptr;
}

Trapeze TListItem::GetFigure() const
{
    return this->item;
}

TListItem* TListItem::GetNext()
{
    return this->next;
}

TListItem* TListItem::GetPrev()
{
    return this->prev;
}

void TListItem::SetNext(TListItem *item)
{
    /*auto *oldNext = this->next;
    this->next = item;
    item->next = oldNext;*/
    this->next = item;
}

void TListItem::SetPrev(TListItem *item)
{
    /*auto *oldPrev = this->prev;
    this->prev = item;
    item->prev = oldPrev;*/
    this->prev = item;
}
