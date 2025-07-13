#include "Block.h"
#include <iostream>
#include <stdexcept> // אקספשיין
using namespace std;

// Constructor
Block::Block(int size)
    : m_next(nullptr), m_isFree(true) {
    if (size < 0)
        throw invalid_argument("Block size cannot be negative.");
    m_size = size;
}


// ---- Setters ---- //


// Set the size of the memory block
// Throws exception if the given size is negative
void Block::setSize(int size) {
    if (size < 0) {
        throw invalid_argument("Block size cannot be negative.");
    }
    m_size = size;
}



// Set the free/used status of the block
void Block::setFree(bool state) {
    m_isFree = state;
}

// Set the pointer to the next block in the pool
// Throws exception if trying to point to itself
void Block::setNext(Block* next) {
    if (next == this) {
        throw invalid_argument("Block cannot point to itself.");
    }
    m_next = next;
}


// ---- Getters ---- //

// Get the size of the block
int Block::getSize() const {
    return m_size;
}

// Check if the block is free
bool Block::isFree() const {
    return m_isFree;
}

// Get the pointer to the next block
Block* Block::getNext() {
    return m_next;
}


// Get the pointer to the next block (Const)
const Block* Block::getNext() const {
    return m_next;
}

