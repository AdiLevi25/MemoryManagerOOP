#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include "memoryManager.h"
#include <string>

class MemorySimulator {

    public:

        MemorySimulator(int iterations = 100, int maxBlockSize = 128);
        //void run(MemoryManager* allocator, int iterations = 100, int maxBlockSize = 128);
        void runAllScenarios(MemoryManager* allocator);

    private:

        void randomAllocations(MemoryManager* allocator);
        void increasingSizeAllocations(MemoryManager* allocator);
        void decreasingSizeAllocations(MemoryManager* allocator);
        void fragmentationTest(MemoryManager* allocator);
        void burstAllocations(MemoryManager* allocator);
        void mixedOverload(MemoryManager* allocator);

        void runScenario(MemoryManager* allocator, const std::string& name);
        void printStatistics(MemoryManager* allocator, const std::string& scenario);
        

        int m_iterations;
        int m_numAllocations;
        int m_numFailedAllocations;
};


#endif // MEMORY_SIMULATOR_H

