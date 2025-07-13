#include "FirstFitAllocator.h"
#include <stdexcept>

using namespace std;


// Constructor - delegates to base class
FirstFitAllocator::FirstFitAllocator(int poolSize)
    : MemoryManager(poolSize) {}

// Return name of the algorithm
const char* FirstFitAllocator::getAlgorithmName() const {
    return "First Fit";
}


void* FirstFitAllocator::allocate(int size) {
    // Throw exception if requested size is zero or negative
    if (size <= 0) {
        throw invalid_argument("Requested allocation size must be positive.");
    }

    // Find the first free block large enough to fit 'size'
    Block* block = findFirstFit(size);
    if (!block) {
        // Increment failed allocation count and return nullptr if 
        //no suitable block found
        m_failedAllocations++;
        return nullptr;
    }

    bool didSplit = false;
    // Check if block can be split 
    if (block->getSize() >= size + (int)sizeof(Block)) {
        didSplit = splitBlock(block, size);
    }

    // If no split occurred, mark the entire block as used
    if (!didSplit) {
        block->setFree(false);
    }

    // Calculate used memory for this allocation
    int usedNow = didSplit ? (size + sizeof(Block)) :
        (block->getSize() + sizeof(Block));
    m_usedSize += usedNow;

    // Update peak memory usage if current usage exceeds previous peak
    if (m_usedSize > m_peakUsage) {
        m_peakUsage = m_usedSize;
    }

    // Return pointer to usable memory (after block metadata)
    return (void*)((char*)block + sizeof(Block));
}



// Find the first free block with size equal or larger than requested
Block* FirstFitAllocator::findFirstFit(int size) {
    Block* current = m_memoryPool;
    // Iterate through blocks until a suitable free block is found or list ends
    while (current != nullptr) {
        // Check if current block is free and large enough for allocation
        if (current->isFree() && current->getSize() >= size) {
            return current; // Return first fitting block found
        }
        current = current->getNext(); // Move to next block
    }
    return nullptr; // No suitable block found
}
