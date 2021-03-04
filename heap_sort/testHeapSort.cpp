#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>

#include "sort.h"

const int ELEMENTS_COUNT = 100000;

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

int TestStructCompare(const TestStruct* testStruct_1, const TestStruct* testStruct_2)
{
    if (testStruct_1->key < testStruct_2->key)
    {
        return 1;
    }

    if (testStruct_1->key > testStruct_2->key)
    {
        return -1;
    }

    return 0;
}

bool TestStructCompare2(TestStruct* const& testStruct_1, TestStruct* const& testStruct_2)
{
    if (testStruct_1->key < testStruct_2->key)
    {
        return true;
    }

    return false;
}

int TestStructCompareDescending(const TestStruct* testStruct_1, const TestStruct* testStruct_2)
{
    return -TestStructCompare(testStruct_1, testStruct_2);
}

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    for (int i = 0; i < length; ++i)
    {
        s += rand() % ('z' - 'a') + 'a';
    }
    return s;
}

static void generate(TestStruct* pts)
{
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

TEST_CASE("One element array")
{
    srand(time(0));

    TestStruct* testStruct = new TestStruct;
    generate(testStruct);
    TestStruct copyTestStruct = *testStruct;
    TestStruct** array = new TestStruct*[1];
    array[0] = testStruct;

    templates::heapSort(array, 1, TestStructCompare);

    CHECK(testStruct->key == copyTestStruct.key);

    delete testStruct;
    delete[] array;
}

TEST_CASE("Sort")
{
    srand(time(0));
    int size = 1000000;

    TestStruct* pStructs = new TestStruct[size];
    TestStruct* pStructsCopy = new TestStruct[size];
    for (int i = 0; i < size; ++i)
    {
        generate(pStructs + i);
        pStructsCopy[i] = pStructs[i];
    }

    TestStruct** ppStructs = new TestStruct*[size];
    for (int i = 0; i < size; ++i)
    {
        ppStructs[i] = pStructs + i;
    }

    TestStruct** ppStructsCopy = new TestStruct*[size];
    for (int i = 0; i < size; ++i)
    {
        ppStructsCopy[i] = pStructsCopy + i;
    }

    auto stdSortStart = std::chrono::steady_clock::now();
    std::sort(ppStructsCopy, ppStructsCopy + size, TestStructCompare2);
    auto stdSortEnd = std::chrono::steady_clock::now();
    MESSAGE("std::sort time: " << std::chrono::duration_cast<std::chrono::duration<double> >(
                                      stdSortEnd - stdSortStart)
                                      .count());

    auto heapSortStart = std::chrono::steady_clock::now();
    templates::heapSort(ppStructs, size, TestStructCompare);
    auto heapSortEnd = std::chrono::steady_clock::now();
    MESSAGE("heapSort time: " << std::chrono::duration_cast<std::chrono::duration<double> >(
                                     heapSortEnd - heapSortStart)
                                     .count());


    for (int i = 0; i < size; ++i)
    {
        CHECK(ppStructs[i]->key == ppStructsCopy[i]->key);
    }

    delete[] pStructs;
    delete[] pStructsCopy;
    delete[] ppStructs;
    delete[] ppStructsCopy;
}

TEST_CASE("Debug")
{
    srand(time(0));
    int size = 10;

    TestStruct pStructs[size];
    TestStruct* ppStructs[size];
    for (int i = 0; i < size; ++i)
    {
        generate(pStructs + i);
        ppStructs[i] = pStructs + i;
    }

    templates::heapSort(ppStructs, size, TestStructCompare);

    TestStruct pSortedStructs[size];
    for (int i = 0; i < size; ++i)
    {
        pSortedStructs[i] = *ppStructs[i];
    }

    std::cout << "end";
}