#ifndef BLOCK_H
#define BLOCK_H

class Block {
    public:
        Block(int size = 0);                 // Constructor
        void setSize(int size);              // Set block size
        void setFree(bool state);            // Set free/used status
        void setNext(Block* next);           // Set pointer to next block

        int getSize() const;                 // Get block size
        bool isFree() const;                 // Is the block free ?
        Block* getNext();                    // Get pointer to next block
        const Block* getNext() const;   //Get pointer to next block(const) 

    private:
        int m_size;       // Size of the memory block
        bool m_isFree;    // True if the block is free
        Block* m_next;    // Pointer to the next block 

        friend class MemoryManager;      // Allow MemoryManager full access
};

#endif // BLOCK_H
