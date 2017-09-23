#ifndef TALLOCATIONBLOCK_H
#define TALLOCATIONBLOCK_H

#include <iostream>
#include <cstdlib>
#include "TStack.h"

typedef unsigned char Byte;

class TAllocationBlock
{
public:
    TAllocationBlock(int32_t size, int32_t count);
    void *Allocate();
    void Deallocate(void *ptr);
    bool Empty();
    int32_t Size();

    virtual ~TAllocationBlock();

private:
//    int32_t _size;
//    int32_t _count;

    Byte *_used_blocks;
    TStack<void *>_free_blocks;
//    void *mem;
//    int32_t _free_count;
};

#endif /* TALLOCATIONBLOCK_H */