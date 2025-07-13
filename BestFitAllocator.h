#ifndef BEST_FIT_ALLOCATOR_H
#define BEST_FIT_ALLOCATOR_H

#include "MemoryManager.h"

class BestFitAllocator : public MemoryManager {

    public:
        // Constructor: initializes memory pool with given size
        BestFitAllocator(int poolSize);

        // Allocates memory block of requested size using Best Fit algorithm
        void* allocate(int size);

        // Returns the name of this allocation algorithm
        const char* getAlgorithmName() const;

    private:
        // Finds the best fitting free block for the requested size
        Block* findBestFit(int size);
};


#endif // BEST_FIT_ALLOCATOR_H
