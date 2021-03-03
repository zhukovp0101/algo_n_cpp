#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <iostream>
#include <list>
#include <string>

#include "memory_manager.h"

const int ELEMENTS_COUNT = 100000;
static size_t sNumber = 0;

template <size_t N>
struct TestStruct
{
    TestStruct()
    {
        ++sNumber;
    }
    ~TestStruct()
    {
        --sNumber;
    }
    std::string key;
    char arr[N];
};

template <size_t N>
using TestMemoryManager = lab618::CMemoryManager<TestStruct<N>>;

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

template <size_t N>
static void generate(TestStruct<N>* pts)
{
    pts->key = makeRandomString();
}

TEST_CASE("Memory Manager Test")
{
    srand(time(0));
    constexpr size_t N = 10;
    int steps = 10000;

    TestMemoryManager<N> memoryManager(100, true);

    std::vector<TestStruct<N>*> ptrs;
    std::vector<std::string> strings;

    for (int i = 0; i < steps; ++i)
    {
        TestStruct<N>* obj = memoryManager.newObject();
        std::string str = makeRandomString();
        obj->key = str;
        ptrs.push_back(obj);
        strings.push_back(str);
    }

    CHECK(sNumber == steps);

    for (int j = 0; j < 10; ++j)
    {
        for (int i = 0; i < steps / 2; ++i)
        {
            size_t j = rand() % ptrs.size();

            memoryManager.deleteObject(*(ptrs.begin() + j));
            ptrs.erase(ptrs.begin() + j);

            strings.erase(strings.begin() + j);
        }

        CHECK(sNumber == ptrs.size());

        for (int i = 0; i < ptrs.size(); ++i)
        {
            CHECK(ptrs[i]->key == strings[i]);
        }

        for (int i = 0; i < steps / 2; ++i)
        {
            TestStruct<N>* obj = memoryManager.newObject();
            std::string str = makeRandomString();
            obj->key = str;
            ptrs.push_back(obj);
            strings.push_back(str);
        }

        CHECK(sNumber == ptrs.size());
    }

    for (int i = 0; i < steps / 2; ++i)
    {
        size_t j = rand() % ptrs.size();

        memoryManager.deleteObject(*(ptrs.begin() + j));
        ptrs.erase(ptrs.begin() + j);

        strings.erase(strings.begin() + j);
    }

    memoryManager.clear();
    CHECK(sNumber == 0);
}