#ifndef MSDMALLOC_MSDMALLOC_HPP
#define MSDMALLOC_MSDMALLOC_HPP

#include "MSDHashTable.hpp"

class MSDMalloc {
public:
    MSDHashTable msdHashTable;

    // Constructor and Deconstructor
    MSDMalloc();
    ~MSDMalloc();

    // "malloc"
    void* allocate(size_t bytesToAllocate);

    // "free"
    void deallocate(void* ptr);
};

#endif //MSDMALLOC_MSDMALLOC_HPP
