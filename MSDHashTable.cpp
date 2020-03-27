#include "MSDHashTable.hpp"
#include <sys/mman.h>

MSDHashTable::MSDHashTable() {
    tablePointer = (std::pair<void*, size_t>*) mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    tableSize = 0;
    tableCapacity = 4096 / sizeof(tablePointer[0]);

    for (int x = 0; x < tableCapacity; x++) {
        tablePointer[x].first = nullptr;
        tablePointer[x].second = 0;
    }
}

MSDHashTable::~MSDHashTable() {
    for (int i = 0; i < tableCapacity; i++) {
        if (tablePointer[i].first != nullptr) {
            munmap(tablePointer[i].first, tablePointer[i].second);
        }
    }
    munmap(tablePointer, tableCapacity);
}

int MSDHashTable::hashValue(void *ptr) {
    auto hash = (uint64_t) ptr;
    hash = hash << 12;
    return (int) ((hash >> 12) % tableCapacity);
}

void MSDHashTable::grow() {
    size_t oldSize = tableCapacity * sizeof(tablePointer[0]);
    std::pair<void *, size_t>* oldTable = tablePointer;
    tableCapacity *= 2;

    // Make bigger table
    tablePointer = (std::pair<void*, size_t>*) mmap(nullptr, (tableCapacity * sizeof(tablePointer[0])), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

    // Copy oldTable to new bigger Table
    for (int i = 0; i < (tableCapacity / 2); i++) {
        if (oldTable[i].first != nullptr) {
            int hash = hashValue(oldTable[i].first);
            findAndInsert(oldTable[i].first, oldTable[i].second, hash);
        }
    }

    // Clear old Table
    munmap(oldTable, oldSize);
}

void MSDHashTable::findAndInsert(void *ptr, size_t neededSize, int hash) {
    while (tablePointer[hash].second != 0) {
        hash = (int) ((hash + 1) % tableCapacity);
    }
    tablePointer[hash].first = ptr;
    tablePointer[hash].second = neededSize;
}

size_t MSDHashTable::findAndDelete(const void *ptr, int hash) {
    while (tablePointer[hash].first != nullptr && tablePointer[hash].first != ptr) {
        hash = (int) ((hash + 1) % tableCapacity);
    }
    size_t neededSize = tablePointer[hash].second;
    tablePointer[hash].first = nullptr;
    tablePointer[hash].second = 0;
    return neededSize;
}

void MSDHashTable::insertNode(void *ptr, size_t neededSize) {
    // if current size is over 50% of the capacity, grow
    if (tableSize >= (tableCapacity * 0.5)) {
        grow();
    }
    int hash = hashValue(ptr);
    findAndInsert(ptr, neededSize, hash);
    tableSize++;
}

size_t MSDHashTable::deleteNode(void *ptr) {
    int hash = hashValue(ptr);
    size_t neededSize = findAndDelete(ptr, hash);
    tableSize--;
    return neededSize;
}

std::pair<void *, size_t> *MSDHashTable::getTable() {
    return tablePointer;
}

size_t MSDHashTable::getCapacity() {
    return tableCapacity;
}

size_t MSDHashTable::getSize() {
    return tableSize;
}