#include "TAllocationBlock.h"

TAllocationBlock::TAllocationBlock(int32_t size, int32_t count)
{
    _used_blocks = (Byte *)malloc(size * count);

    for(int32_t i = 0; i < count; ++i) {
        void *ptr = (void *)malloc(sizeof(void *));
        ptr = _used_blocks + i * size;
        _free_blocks.Push(ptr);
    }
}

void *TAllocationBlock::Allocate()
{
    if(!_free_blocks.IsEmpty()) {
        void *res = _free_blocks.Top();
        _free_blocks.Pop();
        return res;
    } else {
        throw std::bad_alloc();
    }
}

void TAllocationBlock::Deallocate(void *ptr)
{
    _free_blocks.Push(ptr);
}

bool TAllocationBlock::Empty()
{
    return _free_blocks.IsEmpty();
}

int32_t TAllocationBlock::Size()
{
    return _free_blocks.GetSize();
}

TAllocationBlock::~TAllocationBlock()
{
    while(!_free_blocks.IsEmpty()) {
        _free_blocks.Pop();
    }
    free(_used_blocks);
    //delete _free_blocks;
    //delete _used_blocks;
}