#ifndef WORST_FIT_ALLOCATOR_H
#define WORST_FIT_ALLOCATOR_H

#include "MemoryManager.h"

class WorstFitAllocator : public MemoryManager {

    public:
        // Constructor - initialize memory pool with given size
        WorstFitAllocator(int poolSize);

        // Allocate memory block using worst-fit algorithm
        void* allocate(int size);

        // Return the name of the allocation algorithm
        const char* getAlgorithmName() const;

    private:
        // Find the worst-fitting free block (largest block that fits)
        Block* findWorstFit(int size);
};



#endif // WORST_FIT_ALLOCATOR_H
