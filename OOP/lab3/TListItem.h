#ifndef TLISTITEM_H
#define TLISTITEM_H

#include <memory>
#include "trapeze.h"
#include "rhomb.h"
#include "pentagon.h"

class TListItem
{
public:
    TListItem(const std::shared_ptr<Figure> &obj);

    std::shared_ptr<Figure> GetFigure() const;
    std::shared_ptr<TListItem> GetNext();
    std::shared_ptr<TListItem> GetPrev();
    void SetNext(std::shared_ptr<TListItem> item);
    void SetPrev(std::shared_ptr<TListItem> item);
    friend std::ostream& operator<<(std::ostream &os, const TListItem &obj);

    virtual ~TListItem(){};

private:
    std::shared_ptr<Figure> item;
    std::shared_ptr<TListItem> next;
    std::shared_ptr<TListItem> prev;
};

#endif
