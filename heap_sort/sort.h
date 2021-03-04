#ifndef SORT_HEAD_H_2021_02_25
#define SORT_HEAD_H_2021_02_25

#include <algorithm>

namespace templates
{
    /** Определение типа функции сравнения элементов
    \return
    \li > 0  - pElem1 меньше pElem2
    \li 0   - элементы pElem1 и pElem2 равны
    \li < 0 - pElem1 больше pElem2
    */
    typedef int(CompareSortType)(const void *pElem1, const void *pElem2);

    void heapSort(void **ppArray, int length, CompareSortType pCompareFunc);

    class CHeapConvex
    {
    public:
        CHeapConvex(void **ppArray, int length, CompareSortType pCompareFunc)
            : m_ppArray(ppArray), m_length(length), m_comp(pCompareFunc)
        {
            buildHeap();
        };

        void *popMin()
        {
            void *result = m_ppArray[0];
            --m_length;

            if (m_length > 0)
            {
                m_ppArray[0] = m_ppArray[m_length];
                siftDown(0);
            }

            return result;
        }

    private:
        void **m_ppArray;
        int m_length;
        CompareSortType *m_comp;

        void buildHeap()
        {
            for (int i = m_length / 2; i >= 0; --i)
            {
                siftDown(i);
            }
        }

        void siftDown(int i)
        {
            while (true)
            {
                int new_i = i;
                int left = 2 * i + 1;
                int right = 2 * i + 2;

                if (left < m_length)
                {
                    if (m_comp(m_ppArray[left], m_ppArray[i]) > 0)
                    {
                        new_i = left;
                    }
                }

                if (right < m_length)
                {
                    if (m_comp(m_ppArray[right], m_ppArray[new_i]) > 0)
                    {
                        new_i = right;
                    }
                }

                if (new_i != i)
                {
                    std::swap(m_ppArray[new_i], m_ppArray[i]);
                    i = new_i;
                }
                else
                {
                    break;
                }
            }
        }

//        void siftUp(int i)
//        {
//            while (true)
//            {
//                int parent = (i - 1) / 2;
//                if (m_comp(m_ppArray[i], m_ppArray[parent]) > 0)
//                {
//                    std::swap(m_ppArray[i], m_ppArray[parent]);
//                    i = parent;
//                }
//                if (i != parent || i == 0)
//                {
//                    break;
//                }
//            }
//        }
    };

    template <class T>
    void heapSort(T **ppArray, int length, int(pCompareFunc)(const T *pElem1, const T *pElem2))
    {
        heapSort(reinterpret_cast<void **>(ppArray), length, (CompareSortType *)pCompareFunc);
    }
};  // namespace templates

#endif  // #define SORT_HEAD_H_2021_02_25
