#include "MemorySimulator.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;


MemorySimulator::MemorySimulator(int iterations, int maxBlockSize) :
    m_iterations(iterations),
    m_numAllocations(0), m_numFailedAllocations(0) {
}


void MemorySimulator::runAllScenarios(MemoryManager* allocator) {
    srand(time(nullptr));
    string scenarios[] = { "Random Allocations" ,
                            "Increasing Size Allocations" ,
                            "Decreasing Size Allocations" ,
                            "Fragmentation Test",
                            "Burst Allocations",
                            "Mixed Overload" };

    int numScenarios = 6;
    for (int i = 0; i < numScenarios; i++) {
        m_numAllocations = 0;
        m_numFailedAllocations = 0;
        allocator->reset(allocator->getTotalMemory());
        runScenario(allocator, scenarios[i]);
    }
}

void MemorySimulator::runScenario(MemoryManager* allocator, const string& name) {

    cout << "\n--- Scenario: " << name << " (" << allocator->getAlgorithmName() << ") ---\n";
    if (name == "Random Allocations") {
        randomAllocations(allocator);
    }
    else if (name == "Increasing Size Allocations") {
        increasingSizeAllocations(allocator);
    }
    else if (name == "Decreasing Size Allocations") {
        decreasingSizeAllocations(allocator);
    }
    else if (name == "Fragmentation Test") {
        fragmentationTest(allocator);
    }
    else if (name == "Burst Allocations") {
        burstAllocations(allocator);
    }
    else if (name == "Mixed Overload") {
        mixedOverload(allocator);
    }
    printStatistics(allocator, name);
}

void MemorySimulator::randomAllocations(MemoryManager* allocator) {
    vector<void*> blocks;

    for (int i = 0; i < m_iterations; ++i) {
        if (blocks.empty() || rand() % 2 == 0) {
            m_numAllocations++;
            int size = 16 + rand() % 128;
            void* ptr = allocator->allocate(size);
            if (ptr) blocks.push_back(ptr);
            else m_numFailedAllocations++;
        }
        else {
            int idx = rand() % blocks.size();
            allocator->deallocate(blocks[idx]);
            blocks.erase(blocks.begin() + idx);
        }
    }
    for (void* ptr : blocks) allocator->deallocate(ptr);
}

void MemorySimulator::increasingSizeAllocations(MemoryManager* allocator) {
    vector<void*> blocks;
    m_numAllocations = m_iterations;
    for (int i = 4; i <= m_iterations * 4; i += 4) {
        void* ptr = allocator->allocate(i);
        if (ptr) blocks.push_back(ptr);
        else m_numFailedAllocations++;

    }
    for (void* ptr : blocks) allocator->deallocate(ptr);
}

void MemorySimulator::decreasingSizeAllocations(MemoryManager* allocator) {
    vector<void*> blocks;
    m_numAllocations = m_iterations;
    for (int i = m_iterations * 4; i >= 4; i -= 4) {
        void* ptr = allocator->allocate(i);
        if (ptr) blocks.push_back(ptr);
        else m_numFailedAllocations++;
    }
    for (void* ptr : blocks) allocator->deallocate(ptr);
}

void MemorySimulator::fragmentationTest(MemoryManager* allocator) {
    vector<void*> blocks;
    m_numAllocations = 0;
    for (int i = 0; i < m_iterations; ++i) {
        m_numAllocations++;
        void* ptr = allocator->allocate(64);
        if (ptr) blocks.push_back(ptr);
        else m_numFailedAllocations++;
    }
    for (size_t i = 0; i < blocks.size(); i += 2) {
        allocator->deallocate(blocks[i]); // deallocate every other
    }
    //allocator->defragmantation(); // attempt to merge
    for (size_t i = 1; i < blocks.size(); i += 2) {
        m_numAllocations++;
        if (!allocator->allocate(64))
            m_numFailedAllocations++;
    }
}

void MemorySimulator::burstAllocations(MemoryManager* allocator) {
    m_numAllocations = m_iterations;
    int minSize = 32;
    int maxSize = 64;
    vector<void*> blocks;
    for (int i = 0; i < m_numAllocations; ++i) {
        int size = minSize + rand() % (maxSize - minSize + 1);
        void* ptr = allocator->allocate(size);
        if (ptr) blocks.push_back(ptr);
        else m_numFailedAllocations++;
    }
    for (void* ptr : blocks) allocator->deallocate(ptr);
}

void MemorySimulator::mixedOverload(MemoryManager* allocator) {
    vector<void*> blocks;
    int minSize = 32;
    int maxSize = 64;
    for (int i = 0; i < m_iterations; ++i) {
        int size = (i % 2 == 0) ? minSize : maxSize;
        void* ptr = allocator->allocate(size);
        m_numAllocations++;
        if (ptr) blocks.push_back(ptr);
        else m_numFailedAllocations++;
        if (i % 5 == 0 && !blocks.empty()) {
            allocator->deallocate(blocks[0]);
            blocks.erase(blocks.begin());
        }
    }
    for (void* ptr : blocks) allocator->deallocate(ptr);
}

void MemorySimulator::printStatistics(MemoryManager* allocator, const string& scenario) {

    cout << "Failed Allocations: " << (double)m_numFailedAllocations / m_numAllocations * 100 << "%\n";
    cout << "Peak Usage        : " << allocator->getPeakUsage() << " bytes\n";
}


/*void MemorySimulator::printStatistics(MemoryManager* allocator, int iterations, int successfulAllocs) {
    double utilization = (double)allocator->getUsedMemory() / allocator->getTotalMemory() * 100;

    cout << "Total Iterations        : " << iterations << endl;
    cout << "Successful Allocations  : " << successfulAllocs << endl;
    cout << "Failed Allocations      : " << allocator->getFailedAllocations() << endl;
    cout << "Peak Memory Usage       : " << allocator->getPeakUsage() << " bytes" << endl;
    cout << "Final Used Memory       : " << allocator->getUsedMemory() << " bytes" << endl;
    cout << "Final Free Memory       : " << allocator->getFreeMemory() << " bytes" << endl;
    cout << "Memory Utilization      : " << utilization << " %" << endl << endl;
}
*/
