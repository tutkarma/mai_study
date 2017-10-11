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
    Byte *_used_blocks;
    TStack<void *>_free_blocks;
};

#endif /* TALLOCATIONBLOCK_H */