#include <sort.h>
#include <cstring>

namespace templates
{
    void heapSort(void** ppArray, int length, CompareSortType pCompareFunc)
    {
        // создать кучу (либо вставкой, либо через массив за O(n)
        // много раз вытащить элемент из кучи
        // готово

        CHeapConvex heap(ppArray, length, pCompareFunc);
        void** sortedArray = new void*[length];
        for (int i = 0; i < length; ++i)
        {
            void* min = heap.popMin();
            sortedArray[i] = min;
        }

        std::memcpy(ppArray, sortedArray, length * sizeof(void*));
        delete[] sortedArray;
    }
}  // namespace templates
