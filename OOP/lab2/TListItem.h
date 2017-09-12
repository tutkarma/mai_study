#ifndef TLISTITEM_H
#define TLISTITEM_H

#include "trapeze.h"

class TListItem
{
public:
    TListItem(const Trapeze &obj);

    Trapeze GetFigure() const;
    TListItem* GetNext();
    TListItem* GetPrev();
    void SetNext(TListItem *item);
    void SetPrev(TListItem *item);

    virtual ~TListItem(){};

private:
    Trapeze item;
    TListItem *next;
    TListItem *prev;
};

#endif
