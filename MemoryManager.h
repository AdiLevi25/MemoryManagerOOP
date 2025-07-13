#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <iostream>
#include "Block.h"

class MemoryManager {

    protected:
        Block* m_memoryPool;      // Pointer to the first block
        int m_peakUsage;          // Maximum memory used at any point
        int m_totalSize;          // Total size of the memory pool
        int m_usedSize;           // Current used memory
        int m_failedAllocations;  // Count of failed allocation attempts

        void mergeBlock(Block* block);  // Merge adjacent free blocks

    public:

        MemoryManager(int poolSize = 1024); // Constructor
        virtual ~MemoryManager();           // Destructor


        // Split block if it's larger than needed
        bool splitBlock(Block* block, int size);  


        // Allocate memory block (to be implemented by subclasses)
        virtual void* allocate(int size) = 0;// Pure virtual allocation
        void deallocate(void* ptr);     // Free memory at given pointer


        /// --- Getters --- ///

        int getTotalMemory() const;        // Total pool size
        int getUsedMemory() const;         // Used memory
        int getFreeMemory() const;         // Free memory
        int getPeakUsage() const;          // Max used memory
        int getFailedAllocations() const;  // Failed allocations count
        const Block* getHeader() const;      // Return pointer to first block
        virtual const char* getAlgorithmName() const = 0;


        void reset(int poolSize);                // Reset the memory pool


        friend std::ostream& operator<<(std::ostream& os,
            const MemoryManager& mm); // Print state
};

#endif // MEMORY_MANAGER_H
