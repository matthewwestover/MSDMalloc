#ifndef MSDMALLOC_MSDHASHTABLE_HPP
#define MSDMALLOC_MSDHASHTABLE_HPP

#include <utility>

class MSDHashTable {
    // Member Variables
    std::pair<void *, size_t> *tablePointer;
    size_t tableCapacity;
    size_t tableSize;
public:
    // Constructor and Deconstructor
    MSDHashTable();
    ~MSDHashTable();

    // Hash functions
    int hashValue(void *ptr);
    void grow();
    void insertNode(void *ptr, size_t neededSize);
    size_t deleteNode(void *ptr);
    void findAndInsert(void *ptr, size_t neededSize, int hash);
    size_t findAndDelete(const void *ptr, int hash);

    // Getters
    std::pair<void *, size_t> *getTable();
    size_t getCapacity();
    size_t getSize();
};


#endif //MSDMALLOC_MSDHASHTABLE_HPP
