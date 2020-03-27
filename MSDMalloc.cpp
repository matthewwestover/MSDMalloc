#include "MSDMalloc.hpp"
#include <sys/mman.h>

MSDMalloc::MSDMalloc() {
}

MSDMalloc::~MSDMalloc() {
    auto table = msdHashTable.getTable();
    for (int i = 0; i < msdHashTable.getCapacity(); i++) {
        if (table[i].first != nullptr) {
            munmap(table[i].first, table[i].second);
        }
    }
}

void *MSDMalloc::allocate(size_t bytesToAllocate) {
    size_t pages = bytesToAllocate / 4096;
    // Round up
    if (bytesToAllocate % 4096 != 0) {
        pages++;
    }
    size_t finalBytes = pages * 4096;
    void *ptr = mmap(nullptr, finalBytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    msdHashTable.insertNode(ptr, finalBytes);
    return ptr;
}

void MSDMalloc::deallocate(void *ptr) {
    size_t sizeBytes = msdHashTable.deleteNode(ptr);
    munmap(ptr, sizeBytes);
}