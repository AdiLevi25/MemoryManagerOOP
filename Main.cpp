#include "FirstFitAllocator.h"
#include "BestFitAllocator.h"
#include "WorstFitAllocator.h"
#include "MemorySimulator.h"
#include "Block.h"
#include "MemoryManager.h"
#include <iostream>
#include <cassert>
#include <crtdbg.h> // For memory leak detection

using namespace std;


// TEST 1 -  for Block class
void testBlockClass() {
    cout << "==== Block Class Test ====\n";

    // Create block with size 128 and check default state
    Block b1(128);
    assert(b1.getSize() == 128);
    assert(b1.isFree() == true);
    assert(b1.getNext() == nullptr);

    // Change size, mark as used, and assign next block
    b1.setSize(256);
    b1.setFree(false);
    Block b2(64);
    b1.setNext(&b2);
    assert(b1.getSize() == 256);
    assert(b1.isFree() == false);
    assert(b1.getNext() == &b2);
    assert(b1.getNext()->getSize() == 64);

    // Create block with size 0 – should be valid
    Block b0(0);
    assert(b0.getSize() == 0);

    // Set next pointer to nullptr explicitly
    b1.setNext(nullptr);
    assert(b1.getNext() == nullptr);

    // Attempt to setSize with negative value (should throw exception)
    try {
        b1.setSize(-50);
        assert(false); // This line should not be reached
    }
    catch (const std::invalid_argument& e) {
        cout << "Caught expected exception (setSize): " << e.what() << endl;
    }

    // Attempt to construct block with negative size (should throw)
    try {
        Block b3(-100);
        assert(false);
    }
    catch (const std::invalid_argument& e) {
        cout << "Caught expected exception for negative size: " << e.what() << endl;
    }

    // Attempt to setNext to itself (should throw)
    try {
        b1.setNext(&b1);
        assert(false);
    }
    catch (const std::invalid_argument& e) {
        cout << "Caught expected exception for self-reference in setNext: " << e.what() << endl;
    }

    // Chain three blocks and verify the linkage
    Block b4(10), b5(20), b6(30);
    b4.setNext(&b5);
    b5.setNext(&b6);
    assert(b4.getNext()->getNext()->getSize() == 30);

    cout << "==== All Block Class Tests passed successfully ====\n\n";
}


// TEST 2 - for MemoryManager class (using FirstFit)
void testMemoryManager() {
    cout << "==== MemoryManager class Test ====\n" << endl;

    const int poolSize = 300;
    FirstFitAllocator mm(poolSize);

    cout << "Size of Block: " << sizeof(Block) << " bytes" << endl;

    // Initial state checks
    assert(mm.getTotalMemory() == poolSize);
    assert(mm.getUsedMemory() == 0);
    assert(mm.getFreeMemory() == poolSize);
    assert(mm.getPeakUsage() == 0);
    assert(mm.getFailedAllocations() == 0);

    const Block* head = mm.getHeader();
    assert(head != nullptr);
    assert(head->isFree());
    assert(head->getNext() == nullptr);
    assert(head->getSize() + sizeof(Block) == poolSize);

    cout << "--- START - Everything is free" << endl;
    cout << mm << endl;

    // Allocation 1 - 112 bytes
    void* p1 = mm.allocate(112);
    assert(p1 != nullptr);
    assert(mm.getUsedMemory() >= 112);

    cout << "--- AFTER ALLOCATION 1 (112 bytes)" << endl;
    cout << mm << endl;

    // Allocation 2 - 96 bytes
    void* p2 = mm.allocate(96);
    assert(p2 != nullptr);
    assert(mm.getUsedMemory() >= 112 + 96);

    cout << "--- AFTER ALLOCATION 2 (96 bytes)" << endl;
    cout << mm << endl;

    // Allocation 3 - 80 bytes – should fail
    void* p3 = mm.allocate(80);
    assert(p3 == nullptr);
    assert(mm.getFailedAllocations() == 1);

    cout << "--- ALLOCATION 3 FAILED as expected (80 bytes)" << endl;
    cout << mm << endl;

    // Deallocation of second block
    cout << "--- DEALLOCATING Block 2 (96 bytes)" << endl;
    mm.deallocate(p2);
    cout << mm << endl;

    // Deallocation of first block
    cout << "--- DEALLOCATING Block 1 (112 bytes)" << endl;
    mm.deallocate(p1);
    cout << mm << endl;

    // After automatic merge – expect one big free block
    const Block* after = mm.getHeader();
    assert(after->isFree());
    assert(after->getNext() == nullptr);
    assert(after->getSize() + sizeof(Block) == poolSize);
    assert(mm.getUsedMemory() == 0);

    cout << "======= Additional Edge Case Tests =======\n" << endl;
    // Deallocate nullptr – should do nothing
    mm.deallocate(nullptr);
    cout << "No crash on nullptr deallocation" << endl;
    // Try deallocation of pointer that does not belong to pool
    try {
        int x;
        mm.deallocate(&x); // not part of the memory pool
        assert(false); // should not get here
    }
    catch (const out_of_range& e) {
        cout << "Caught expected exception: " << e.what() << endl;
    }
    cout << "\n== Test reset with valid size ==" << endl;
    mm.reset(poolSize);
    assert(mm.getUsedMemory() == 0);
    assert(mm.getFailedAllocations() == 0);
    assert(mm.getHeader()->isFree());
    cout << mm << endl;

    // Test reset with too-small pool size (should throw)
    try {
        mm.reset(sizeof(Block) - 1);
        assert(false); // Should not reach here
    }
    catch (const logic_error& e) {
        cout << "Caught expected exception: " << e.what() << endl;
    }
    cout << "\n==== All MemoryManager Tests Passed Successfully ====\n\n";
}

// TEST 3 - for FirstFitAllocator class
void testFirstFitAllocator() {
    cout << "==== FirstFitAllocator class Test ====\n" << endl;

    FirstFitAllocator allocator(512);

    // Allocation 1 - 100 bytes
    void* p1 = allocator.allocate(100);
    assert(p1 != nullptr);
    cout << "AFTER ALLOCATION 1 (100 bytes)" << endl;
    cout << allocator << endl;

    // Allocation 2 - 200 bytes
    void* p2 = allocator.allocate(200);
    assert(p2 != nullptr);
    cout << "AFTER ALLOCATION 2 (200 bytes)" << endl;
    cout << allocator << endl;

    // Allocation 3 - 300 bytes (should fail)
    void* p3 = allocator.allocate(300);
    assert(p3 == nullptr);
    cout << "ALLOCATION 3 FAILED as expected (300 bytes)" << endl;
    cout << allocator << endl;

    // Deallocate p1 and allocate 50 bytes into freed space
    allocator.deallocate(p1);
    void* p4 = allocator.allocate(50);
    assert(p4 != nullptr);
    cout << "AFTER ALLOCATION 4 (50 bytes into freed block)" << endl;
    cout << allocator << endl;

    // ===== First Fit Test in Real Situation =====
    void* p5 = allocator.allocate(30);
    assert(p5 != nullptr);
    cout << "AFTER ALLOCATION 5 (30 bytes into leftover block)" << endl;
    cout << allocator << endl;

    // Optional: Verify the block chosen was the leftover one (next after p4)
    Block* block5 = (Block*)((char*)p5 - sizeof(Block));
    assert(block5->getSize() >= 30);
    assert(block5->isFree() == false);

    // reset
    allocator.reset(512);
    cout << "AFTER RESET (full pool restored)" << endl;
    cout << allocator << endl;

    cout << "\n==== All FirstFitAllocator Tests Passed Successfully ====\n\n";
}


// TEST 4 - for BestFitAllocator class
void testBestFitAllocator() {
    cout << "==== BestFitAllocator class Test ====\n" << endl;

    BestFitAllocator allocator(512);

    // Allocation 1 - 100 bytes
    void* p1 = allocator.allocate(100);
    assert(p1 != nullptr);
    cout << "AFTER ALLOCATION 1 (100 bytes)" << endl;
    cout << allocator << endl;

    // Allocation 2 - 200 bytes
    void* p2 = allocator.allocate(200);
    assert(p2 != nullptr);
    cout << "AFTER ALLOCATION 2 (200 bytes)" << endl;
    cout << allocator << endl;

    // Allocation 3 - 300 bytes (should fail)
    void* p3 = allocator.allocate(300);
    assert(p3 == nullptr);
    cout << "ALLOCATION 3 FAILED as expected (300 bytes)" << endl;
    cout << allocator << endl;

    // Free first block and allocate 50 into it
    allocator.deallocate(p1);
    void* p4 = allocator.allocate(50);
    assert(p4 != nullptr);
    cout << "AFTER ALLOCATION 4 (50 bytes into freed block)" << endl;
    cout << allocator << endl;

    // Now two candidate free blocks exist: size≈34 and size=164
    // Best Fit should select the 34-byte block for 30 bytes
    void* p5 = allocator.allocate(30);
    assert(p5 != nullptr);
    cout << "AFTER ALLOCATION 5 (30 bytes into best fit block)" << endl;
    cout << allocator << endl;

    Block* block5 = (Block*)((char*)p5 - sizeof(Block));
    assert(block5->getSize() >= 30);
    assert(block5->isFree() == false);

    allocator.reset(512);
    cout << "AFTER RESET (full pool restored)" << endl;
    cout << allocator << endl;

    cout << "\n==== All BestFitAllocator Tests Passed Successfully ====\n\n";
}


// TEST 5 - for WorstFitAllocator class
void testWorstFitAllocator() {
    cout << "==== WorstFitAllocator class Test ====\n" << endl;

    WorstFitAllocator allocator(512);

    // Allocation 1 - 100 bytes
    void* p1 = allocator.allocate(100);
    assert(p1 != nullptr);
    cout << "AFTER ALLOCATION 1 (100 bytes)" << endl;
    cout << allocator << endl;

    // Allocation 2 - 200 bytes
    void* p2 = allocator.allocate(200);
    assert(p2 != nullptr);
    cout << "AFTER ALLOCATION 2 (200 bytes)" << endl;
    cout << allocator << endl;

    // Allocation 3 - 300 bytes (should fail)
    void* p3 = allocator.allocate(300);
    assert(p3 == nullptr);
    cout << "ALLOCATION 3 FAILED as expected (300 bytes)" << endl;
    cout << allocator << endl;

    // Free first block and allocate 50 into it
    allocator.deallocate(p1);
    void* p4 = allocator.allocate(50);
    assert(p4 != nullptr);
    cout << "AFTER ALLOCATION 4 (50 bytes into freed block)" << endl;
    cout << allocator << endl;

    // Now two candidate blocks exist: size≈34 and size=100
    // Worst Fit should choose the larger (100) for the next 30 bytes
    void* p5 = allocator.allocate(30);
    assert(p5 != nullptr);
    cout << "AFTER ALLOCATION 5 (30 bytes into worst fit block)" << endl;
    cout << allocator << endl;

    Block* block5 = (Block*)((char*)p5 - sizeof(Block));
    assert(block5->getSize() == 30); // Should be from the 100 block, not the 34
    assert(block5->isFree() == false);

    allocator.reset(512);
    cout << "AFTER RESET (full pool restored)" << endl;
    cout << allocator << endl;

    cout << "\n==== All WorstFitAllocator Tests Passed Successfully ====\n\n";
}



int main(void) {
    cout << "===== RUNNING ALL TESTS =====" << endl << endl;

    // Store initial memory state for global leak detection
    _CrtMemState memStart, memEnd, memDiff;
    _CrtMemCheckpoint(&memStart);

    try {
        // === TESTS FOR CLASSES ===
        
        //testBlockClass();           // Test 1 Block class
        //testMemoryManager();        // Test 2 MemoryManager class
        testFirstFitAllocator();    // Test 3 First-Fit class
        //testBestFitAllocator();     // Test 4 Best-Fit class
        //testWorstFitAllocator();    // Test 5 Worst-Fit class
        
        
        // === SIMULATOR TEST  ===
        cout << "\n===== RUNNING MEMORY SIMULATOR =====" << endl;

        const int poolSize = 2048;

        FirstFitAllocator firstFit(poolSize);
        BestFitAllocator bestFit(poolSize);
        WorstFitAllocator worstFit(poolSize);

        MemorySimulator simulator(100);
        simulator.runAllScenarios(&firstFit);
        simulator.runAllScenarios(&bestFit);
        simulator.runAllScenarios(&worstFit);
        

    }
    catch (const std::exception& e) {
        cout << "Exception caught during tests: " << e.what() << endl;
    }
    catch (...) {
        cout << "Unknown exception caught during tests." << endl;
    }

    // Check for global memory leaks
    _CrtMemCheckpoint(&memEnd);
    if (_CrtMemDifference(&memDiff, &memStart, &memEnd)) {
        cout << "\nMEMORY LEAK DETECTED during all tests!" << endl;
        _CrtDumpMemoryLeaks();
    }
    else {
        cout << "\nNo memory leaks detected across the project." << endl;
    }

    cout << "\n===== ALL TESTS & SIMULATION COMPLETE =====" << endl;

    return 0;
}


