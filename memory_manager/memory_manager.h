#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

#include <climits>
#include <cstdlib>

namespace lab618
{
    template <class T>
    class CMemoryManager
    {
    private:
        struct block
        {
            // Массив данных блока
            T* pdata = 0;
            // Адрес следующего блока
            block* pnext = 0;
            // Первая свободная ячейка
            int firstFreeIndex = 0;
            // Число заполненных ячеек
            int usedCount = 0;

            void freeCell(T* p)
            {
                --usedCount;
                *reinterpret_cast<int*>(p) = firstFreeIndex;
                firstFreeIndex = p - pdata;
            }

            T* takeCell()
            {
                T* pFree = pdata + firstFreeIndex;
                firstFreeIndex = *reinterpret_cast<int*>(pdata + firstFreeIndex);
                ++usedCount;
                return pFree;
            }
        };

    public:
        class CException : public std::exception
        {
        public:
            CException() {}
            const char* what() const override
            {
                return "It is impossible to clear the data because the destructor was not called !";
            }
        };

    public:
        explicit CMemoryManager(int blockSize, bool isDeleteElementsOnDestruct = false)
            : m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct), m_blkSize(blockSize)
        {}

        virtual ~CMemoryManager()
        {
            clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject()
        {
            if (m_pCurrentBlk == 0 || m_pCurrentBlk->usedCount == m_blkSize)
            {
                m_pCurrentBlk = 0;
                for (block* pBlock = m_pBlocks; pBlock != 0; pBlock = pBlock->pnext)
                {
                    if (pBlock->usedCount != m_blkSize)
                    {
                        m_pCurrentBlk = pBlock;
                    }
                }

                if (m_pCurrentBlk == 0)
                {
                    m_pCurrentBlk = newBlock();
                }
            }


            T* pNewObject = m_pCurrentBlk->takeCell();
            new (pNewObject) T;

            return pNewObject;
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            for (block* pBlock = m_pBlocks; pBlock != 0; pBlock = pBlock->pnext)
            {
                if (pBlock->pdata <= p && p < pBlock->pdata + m_blkSize)
                {
                    p->~T();  // Не проверяет, что элемент действительно существует внутри менеджера
                    pBlock->freeCell(p);
                    return true;
                }
            }

            return false;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            block* pBlock = m_pBlocks;
            while (pBlock != 0)
            {
                block* pNextBlock = pBlock->pnext;
                if (m_isDeleteElementsOnDestruct)
                {
                    deleteBlock(pBlock);
                }
                else
                {
                    if (pBlock->usedCount == 0)
                    {
                        free(pBlock->pdata);
                    }
                    else
                    {
                        throw CException();
                    }
                }
                free(pBlock);
                pBlock = pNextBlock;
            }

            m_pBlocks = 0;
            m_pCurrentBlk = 0;
        }

    private:
        // Создать новый блок данных. применяется в newObject
        block* newBlock()
        {
            block* pNewBlock = reinterpret_cast<block*>(malloc(sizeof(block)));
            pNewBlock->pdata = reinterpret_cast<T*>(malloc(sizeof(T) * m_blkSize));
            pNewBlock->usedCount = 0;
            pNewBlock->firstFreeIndex = 0;
            pNewBlock->pnext = m_pBlocks;

            m_pBlocks = pNewBlock;

            for (int i = 0; i < m_blkSize; ++i)
            {
                *reinterpret_cast<int*>(pNewBlock->pdata + i) = i + 1;
            }

            return pNewBlock;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block* p)
        {
            bool* isFree = new bool[m_blkSize];
            for (int i = 0; i < m_blkSize; ++i)
            {
                isFree[i] = false;
            }

            int currentFreeIndex = p->firstFreeIndex;
            while (currentFreeIndex != m_blkSize)
            {
                isFree[currentFreeIndex] = true;
                currentFreeIndex = *reinterpret_cast<int*>(p->pdata + currentFreeIndex);
            }

            for (int i = 0; i < m_blkSize; ++i)
            {
                if (! isFree[i])
                {
                    (p->pdata[i]).~T();
                }
            }

            free(p->pdata);
            delete[] isFree;
        }

        // Размер блока
        int m_blkSize;
        // Начало списка блоков
        block* m_pBlocks = 0;
        // Текущий блок
        block* m_pCurrentBlk = 0;
        // Удалять ли элементы при освобождении
        bool m_isDeleteElementsOnDestruct;
    };
};  // namespace lab618

#endif  // #define MEMORY_MANAGER_HEAD_H_2021_02_18
