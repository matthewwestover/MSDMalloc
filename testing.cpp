#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "MSDHashTable.hpp"
#include "MSDMalloc.hpp"
#include <chrono>

TEST_CASE("Hash Table") {
    MSDHashTable hashTable;
    SECTION("Constructor") {
        int capacity = hashTable.getCapacity();
        CHECK(capacity == 256);
        CHECK(hashTable.getSize() == 0);
        for (int i = 0; i < capacity; i++) {
            CHECK(hashTable.getTable()[i].first == nullptr);
            CHECK(hashTable.getTable()[i].second == 0);
        }
    }
    SECTION("Hash generates same hash") {
        int number = 77;
        int *nptr = &number;
        int hash1 = hashTable.hashValue(nptr);
        int hash2 = hashTable.hashValue(nptr);
        CHECK(hash1 == hash2);
    }
    SECTION("Hash is unique for different objects") {
        int number1 = 77;
        int number2 = 77;
        int hash1 = hashTable.hashValue(&number1);
        int hash2 = hashTable.hashValue(&number2);
        CHECK(hash1 != hash2);
    }
    SECTION("Insertion") {
        int number = 77;
        hashTable.insertNode(&number, 12);
        int hash = hashTable.hashValue(&number);
        CHECK(hashTable.getSize() == 1);
        CHECK(hashTable.getTable()[hash].second == 12);
        int number2 = 89;
        hashTable.insertNode(&number2, 24);
        int hash2 = hashTable.hashValue(&number2);
        CHECK(hashTable.getSize() == 2);
        CHECK(hashTable.getTable()[hash2].second == 24);
    }
    SECTION("Deletion") {
        int number1 = 101;
        int number2 = 666;
        hashTable.insertNode(&number1, 57);
        hashTable.insertNode(&number2, 47);
        CHECK(hashTable.getSize() == 2);
        CHECK(hashTable.deleteNode(&number1) == 57);
        CHECK(hashTable.getSize() == 1);
    }
    SECTION("Grow") {
        int number = 77;
        hashTable.insertNode(&number, 23);
        CHECK(hashTable.getSize() == 1);
        CHECK(hashTable.getCapacity() == 256);
        hashTable.grow();
        CHECK(hashTable.getSize() == 1);
        CHECK(hashTable.getCapacity() == 512);
    }
}

TEST_CASE("MSDMalloc") {
    MSDMalloc msdMalloc;
    int numbers[5];
    msdMalloc.allocate(sizeof(numbers));
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;
    numbers[3] = 4;
    numbers[4] = 5;
    CHECK(msdMalloc.msdHashTable.getCapacity() == 256);
    CHECK(msdMalloc.msdHashTable.getSize() == 1);
    CHECK(numbers[0] == 1);
    CHECK(numbers[1] != 1);
    msdMalloc.deallocate(numbers);
    CHECK(msdMalloc.msdHashTable.getSize() == 0);
}

TEST_CASE("Benchmark Timing") {
    int TIMES_TO_RUN = 10000;
    MSDMalloc msdMalloc;
    double mallocTime = 0;
    double msdMallocTime = 0;

    for (int i = 0; i < TIMES_TO_RUN; i++) {
        int number1 = i;
        std::chrono::high_resolution_clock::time_point time1 = std::chrono::high_resolution_clock::now();
        msdMalloc.allocate(sizeof(number1));
        std::chrono::high_resolution_clock::time_point time2 = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
        msdMallocTime += duration;

        int number2 = i;
        std::chrono::high_resolution_clock::time_point time4 = std::chrono::high_resolution_clock::now();
        auto sysMalloc = malloc(sizeof(number2));
        std::chrono::high_resolution_clock::time_point time3 = std::chrono::high_resolution_clock::now();
        double duration3 = std::chrono::duration_cast<std::chrono::microseconds>(time4-time3).count();
    }
    mallocTime /= TIMES_TO_RUN;
    msdMallocTime /= TIMES_TO_RUN;
    std::cout << "malloc average run time: " << mallocTime << "microseconds across: " << TIMES_TO_RUN << " tests\n";
    std::cout << "MSDMalloc allocation average run time: " << msdMallocTime << "microseconds across: " << TIMES_TO_RUN << " tests\n\n";
}