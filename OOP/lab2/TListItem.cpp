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
    this->next = item;
}

void TListItem::SetPrev(TListItem *item)
{
    this->prev = item;
}

std::ostream& operator<<(std::ostream &os, const TListItem &obj)
{
    os << "(" << obj.item << ")" << std::endl;
    return os;
}
