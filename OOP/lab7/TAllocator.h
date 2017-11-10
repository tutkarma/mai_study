#ifndef TALLOCATOR_H
#define TALLOCATOR_H

#include <cstdlib>
#include <stdexcept>

struct TNTree {
    TNTree* sons;
    void* data;
    size_t N;
};

class TAllocator
{
private:
    size_t free;
    size_t amount;
    size_t sizeOfBlock;

    TNTree* root;
    char* usedBlocks;
public:
    TAllocator(const size_t&, const size_t&);
    void* allocate();
    void deallocate(void*);
    ~TAllocator();
};

#endif