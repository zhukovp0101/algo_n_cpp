#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <string>
#include <iostream>
#include <list>

#include "list.h"

const int ELEMENTS_COUNT = 100000;

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
//typedef lab618::CDualLinkedList<TestStruct> TestList;

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

static void generate(TestStruct *pts)
{
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

TEST_CASE("SingleList simple pushBack and erase")
{
    srand(time(0));

    TestSingleList single_list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushBack(ts);
    }

    CHECK(single_list.getSize() == ELEMENTS_COUNT);

    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it)
    {
        it.getLeaf();
        TestStruct ts = *it;
        single_list.erase(it);
    }

    CHECK(single_list.getSize() == 0);
}

TEST_CASE("SingleList") {
    srand(time(0));

    TestSingleList single_list;
    std::list<TestStruct> real_list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
      TestStruct ts;
      generate(&ts);
      single_list.pushBack(ts);
      real_list.push_back(ts);
    }

    CHECK(single_list.getSize() == real_list.size());

    int nErase = 100;
    for (int i = 0; i < nErase; ++i)
    {
        int number = rand() % (ELEMENTS_COUNT - i);
        if (i + 1 == nErase)
        {
            number = ELEMENTS_COUNT - i - 1;
        }

        auto singleListIter = single_list.begin();
        auto realListIter = real_list.begin();
        for (int j = 0; j < number; ++j)
        {
            ++singleListIter;
            ++realListIter;
        }
        single_list.erase(singleListIter);
        real_list.erase(realListIter);
    }

    CHECK(single_list.getSize() == real_list.size());

    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      TestStruct ts;
      generate(&ts);
      single_list.pushBack(ts);
      real_list.push_back(ts);
      single_list.pushFront(ts);
      real_list.push_front(ts);
    }

    while (!real_list.empty())
    {
        TestStruct realListTs = real_list.front(); real_list.pop_front();
        TestStruct singleListTs = single_list.popFront();
        CHECK(realListTs.key == singleListTs.key);
    }

    CHECK(single_list.getSize() == 0);
}
