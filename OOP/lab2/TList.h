#ifndef TLIST_H
#define TLIST_H

#include <cstdint>
#include "trapeze.h"
#include "TListItem.h"

class TList
{
public:
    TList();
    void Push(Trapeze &obj);
    const bool IsEmpty() const;
    uint32_t GetLength();
    Trapeze Pop();
    friend std::ostream& operator<<(std::ostream &os, const TList &list);
    virtual ~TList();

private:
    uint32_t length;
    TListItem *head;

    void PushFirst(Trapeze &obj);
    void PushLast(Trapeze &obj);
    void PushAtIndex(Trapeze &obj, int32_t ind);
    Trapeze PopFirst();
    Trapeze PopLast();
    Trapeze PopAtIndex(int32_t ind);
};

#endif
