#include "MemoryManager.h"
#include <iostream>
using namespace std;


// Constructor: initializes memory pool with a single free block
MemoryManager::MemoryManager(int poolSize)
    : m_peakUsage(0), m_totalSize(poolSize),
    m_usedSize(0), m_failedAllocations(0) {

    // Ensure pool size is large enough for at least one block
    if (poolSize < sizeof(Block)) {
        throw logic_error("Pool size too small to initialize memory.");
    }

    // Allocate memory pool
    m_memoryPool = (Block*)new char[poolSize];

    // Initialize the first block as free
    m_memoryPool->setSize(poolSize - sizeof(Block));
    m_memoryPool->setFree(true);
    m_memoryPool->setNext(nullptr);
}

// Destructor: releases the memory pool and clears pointer
MemoryManager::~MemoryManager() {
    delete[] m_memoryPool;
    m_memoryPool = nullptr;
}


// Splits a block into two if there's enough space for a new block
// Throws logic error if block is null
// Throws invalid argument if size is not positive
bool MemoryManager::splitBlock(Block* block, int size) {
    if (!block) {
        throw logic_error("Cannot split: block pointer is null.");
    }
    if (size <= 0) {
        throw invalid_argument("Cannot split: requested size must be positive.");
    }

    int remaining = block->getSize() - (size + sizeof(Block));

    // Not enough space to create a new block
    if (remaining < sizeof(Block)) {
        return false;
    }

    // Calculate address for the new free block
    Block* newBlock = (Block*)((char*)block + sizeof(Block) + size);

    // Initialize new free block
    newBlock->setSize(remaining);
    newBlock->setFree(true);
    newBlock->setNext(block->getNext());

    // Update current block as allocated
    block->setSize(size);
    block->setFree(false);
    block->setNext(newBlock);

    return true;
}


// Frees a memory block at the given pointer
// Does nothing if the pointer is null
// Throws std::out_of_range if the pointer is not part of the pool
void MemoryManager::deallocate(void* ptr) {

    if (!ptr)
        return;  // Ignore null pointer (no action needed)

    // Search for the block that matches the given data pointer
    Block* current = m_memoryPool;
    while (current != nullptr) {

        // Compute the start address of the data portion in the block
        void* dataStart = (void*)((char*)current + sizeof(Block));

        // Match found
        if (dataStart == ptr) {
            if (!current->isFree()) {
                // Update usage stats and mark block as free
                m_usedSize -= (current->getSize() + sizeof(Block));
                current->setFree(true);
                mergeBlock(current); // Try to merge with following free blocks
            }
            return; // Either way, stop searching
        }

        current = current->getNext();
    }

    // No matching block found in pool — invalid deallocation
    throw out_of_range("Cannot deallocate: pointer does not belong to memory pool.");
}


// Merges the given block with adjacent free blocks (forward only)
// Used during deallocation to reduce fragmentation
void MemoryManager::mergeBlock(Block* block) {
    // Continue merging while the next block exists and is free
    while (block && block->getNext()) {
        Block* next = block->getNext();

        // Merge current block with next if next is free
        if (next->isFree()) {
            int combinedSize = block->getSize() + sizeof(Block) + next->getSize();
            block->setSize(combinedSize);
            block->setNext(next->getNext());
            // Stay on the same block to check the new next block
        }
        else {
            break; // Stop merging if next block is not free
        }
    }
}


// Return total memory size
int MemoryManager::getTotalMemory() const {
    return m_totalSize;
}

// Return memory currently in use
int MemoryManager::getUsedMemory() const {
    return m_usedSize;
}

// Return amount of free memory
int MemoryManager::getFreeMemory() const {
    return m_totalSize - m_usedSize;
}

// Return peak memory usage
int MemoryManager::getPeakUsage() const {
    return m_peakUsage;
}

// Return number of failed allocations
int MemoryManager::getFailedAllocations() const {
    return m_failedAllocations;
}

// Return pointer to the first block (read-only)
const Block* MemoryManager::getHeader() const {
    return m_memoryPool;
}

// Return name of the memory allocation algorithm
const char* MemoryManager::getAlgorithmName() const {
    return "BaseMemoryManager"; // Default 
}

// Reset the memory pool with a new size, clearing all state and data
// Throws "invalid_argument" if the pool size is too small
void MemoryManager::reset(int poolSize) {
    if (poolSize < (int)sizeof(Block)) {
        throw logic_error("Reset failed: pool size too small to hold a block.");
    }

    // Free previously allocated memory
    delete[] m_memoryPool;
    m_memoryPool = nullptr;

    // Reset usage statistics
    m_totalSize = poolSize;
    m_usedSize = 0;
    m_peakUsage = 0;
    m_failedAllocations = 0;

    // Allocate new memory pool
    char* pool = new char[poolSize];
    m_memoryPool = (Block*)pool;

    // Initialize first block as free
    int usableSize = poolSize - sizeof(Block);
    m_memoryPool->setSize(usableSize);
    m_memoryPool->setFree(true);
    m_memoryPool->setNext(nullptr);
}

// Output memory manager status and block list
ostream& operator<<(ostream& os, const MemoryManager& mm) {
    // General memory usage statistics
    os << "Algorithm: " << mm.getAlgorithmName() << "\n";
    os << "Total Memory: " << mm.getTotalMemory() << "\n";
    os << "Used Memory: " << mm.getUsedMemory() << "\n";
    os << "Free Memory: " << mm.getFreeMemory() << "\n";
    os << "Peak Usage: " << mm.getPeakUsage() << "\n";
    os << "Failed Allocations: " << mm.getFailedAllocations() << "\n";

    // List of all memory blocks in the pool
    const Block* current = mm.getHeader();
    int index = 0;
    while (current != nullptr) {
        os << "Block " << index++ << ": size=" << current->getSize()
            << ", free=" << (current->isFree() ? "yes" : "no") << "\n";
        current = current->getNext();
    }

    return os;
}





