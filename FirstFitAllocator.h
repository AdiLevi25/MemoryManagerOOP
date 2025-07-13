#ifndef FIRST_FIT_ALLOCATOR_H
#define FIRST_FIT_ALLOCATOR_H

#include "MemoryManager.h"



class FirstFitAllocator : public MemoryManager {

    public:
        // Constructor - initialize memory pool with given size
        FirstFitAllocator(int poolSize);

        // Allocate memory block using first-fit algorithm
        void* allocate(int size);

        // Return the name of the allocation algorithm
        const char* getAlgorithmName() const;

    private:
        // Find the first free block that fits the requested size
        Block* findFirstFit(int size);
};


#endif // FIRST_FIT_ALLOCATOR_H
