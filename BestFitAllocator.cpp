#include "BestFitAllocator.h"
#include <stdexcept>

using namespace std;

// Constructor - initializes base MemoryManager with pool size
BestFitAllocator::BestFitAllocator(int poolSize)
    : MemoryManager(poolSize) {}

// Returns the name of the allocation algorithm
const char* BestFitAllocator::getAlgorithmName() const {
    return "Best Fit";
}


// Allocates memory using the Best Fit strategy
// Throws invalid_argument if size is non-positive
void* BestFitAllocator::allocate(int size) {
    if (size <= 0) {
        throw invalid_argument("Requested allocation size must be positive.");
    }

    // Find the best fitting free block for the requested size
    Block* block = findBestFit(size);
    if (!block) {
        // Increment failed allocation count 
        m_failedAllocations++;
        return nullptr; //return nullptr if no suitable block found
    }

    bool didSplit = false;
    // Check if block can be split to fit requested size plus metadata
    if (block->getSize() >= size + (int)sizeof(Block)) {
        didSplit = splitBlock(block, size);
    }

    // If no split occurred, mark the entire block as used
    if (!didSplit) {
        block->setFree(false);
    }

    // Calculate used memory for this allocation and update statistics
    int usedNow = didSplit ? (size + sizeof(Block)) :
        (block->getSize() + sizeof(Block));
    m_usedSize += usedNow;

    if (m_usedSize > m_peakUsage) {
        m_peakUsage = m_usedSize;
    }

    // Return pointer to usable memory (after block metadata)
    return (void*)((char*)block + sizeof(Block));
}

// Searches the memory pool to find the smallest free block 
Block* BestFitAllocator::findBestFit(int size) {
    Block* bestFit = nullptr;
    Block* current = m_memoryPool;

    // Iterate through all blocks to find the smallest suitable free block
    while (current != nullptr) {
        if (current->isFree() && current->getSize() >= size) {
            if (!bestFit || current->getSize() < bestFit->getSize()) {
                bestFit = current;
            }
        }
        current = current->getNext();
    }

    return bestFit;
}



