#include "WorstFitAllocator.h"
#include <stdexcept>

using namespace std;

// Constructor 
WorstFitAllocator::WorstFitAllocator(int poolSize)
    : MemoryManager(poolSize) {}

// Return the name of the allocation algorithm
const char* WorstFitAllocator::getAlgorithmName() const {
    return "Worst Fit";
}


// Allocate memory using Worst-Fit Algorithm
// Throws invalid_argument if requested size is non-positive
void* WorstFitAllocator::allocate(int size) {
    if (size <= 0) {
        throw invalid_argument("Requested allocation size must be positive.");
    }

    // Find the worst fitting free block (largest suitable block)
    Block* block = findWorstFit(size);
    if (!block) {
        // Increment failed allocation count if no suitable block found
        m_failedAllocations++;
        return nullptr;
    }

    bool didSplit = false;
    // Attempt to split block if enough space remains after allocation
    if (block->getSize() >= size + (int)sizeof(Block)) {
        didSplit = splitBlock(block, size);
    }

    // If splitting is not done, mark entire block as used
    if (!didSplit) {
        block->setFree(false);
    }

    // Calculate current used memory and update usage statistics
    int usedNow = didSplit ? (size + sizeof(Block)) 
        : (block->getSize() + sizeof(Block));
    m_usedSize += usedNow;

    if (m_usedSize > m_peakUsage) {
        m_peakUsage = m_usedSize;
    }

    // Return pointer to usable memory after block metadata
    return (void*)((char*)block + sizeof(Block));
}


// Find the worst (largest) fitting free block
Block* WorstFitAllocator::findWorstFit(int size) {

    Block* worstFit = nullptr;    
    Block* current = m_memoryPool;// The beginning of the memory pool

    // Iterate through all memory blocks in the pool
    while (current != nullptr) {
        // Check if the current block is free 
        //and large enough for requested allocation
        if (current->isFree() && current->getSize() >= size) {
            // If no worstFit found yet, or current block
            //is larger than the worstFit found so far
            if (!worstFit || current->getSize() > worstFit->getSize()) {
                worstFit = current;  // Update worstFit to current block
            }
        }
        current = current->getNext();  // Move to next block in the list
    }

    // Return the largest free block found
    return worstFit;
}

