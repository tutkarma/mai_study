#include "TAllocator.h"

TAllocator::TAllocator(const size_t& size, const size_t& amountToAdd)
{
    sizeOfBlock = size;
    amount = amountToAdd;
    root->N = amount;
    root->sons = (TNTree*)malloc(sizeof(TNTree) * root->N);
    usedBlocks = (char*)malloc(sizeof(char) * sizeOfBlock * amount);

    for (size_t i = 0; i < amount; i++) {
        root->sons[i].data = malloc(sizeOfBlock);
        root->sons[i].data = usedBlocks + i * sizeOfBlock;
    }

    free = amountToAdd;
}

void* TAllocator::allocate()
{
    void* result = nullptr;

    if (free > 0) {
        result = root->sons[free - 1].data;
        free--;
    } else {
        throw std::runtime_error("TAllocator: out of memory");
    }
}

void TAllocator::deallocate(void* ptr)
{
    root->sons[free].data = ptr;
    free++;
}

TAllocator::~TAllocator()
{
    if (free < amount) {
        throw std::runtime_error("TAllocator: memory leak");
    }

    std::free(usedBlocks);
    for (size_t i = 0; i < root->N - 1; i++) {
        std::free(root->sons[i].data);
    }
    std::free(root->sons);
}