#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext)
            {
                data = _data;
                pnext = _pnext;
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator()
                : m_pCurrent(0), m_pBegin(0)
            {
            }

            CIterator(leaf *p)
            {
                m_pBegin = 0;
                m_pCurrent = p;
            }

            CIterator(const CIterator &src) // = default ?;
            {
                m_pCurrent = src.m_pCurrent;
                m_pBegin = src.m_pBegin;
            }

            ~CIterator()
            {
            }

            bool operator != (const CIterator&  it) const
            {
                return (m_pBegin == it.m_pBegin) &&
                       (m_pCurrent == it.m_pCurrent);
            }

            void operator++()
            {
                if (m_pBegin != 0) {
                    m_pCurrent = m_pBegin;
                    m_pBegin = 0;
                }
                else
                {
                    if (isValid())
                    {
                        m_pCurrent = m_pCurrent->pnext;
                    }
                }
            }

            T& getData()
            {
                if (!isValid())
                {
                    throwError();
                }
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return getData();
            }

            leaf* getLeaf()
            {
              if (!isValid())
              {
                  throwError();
              }
              return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
                m_pBegin = 0;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pCurrent = 0;
                m_pBegin = p;
            }

            bool isValid()
            {
                return m_pCurrent != 0;
            }

        private:
            // храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;

            void throwError()
            {
                throw std::runtime_error("Iterator is not valid!");
            }
        };

    public:

        CSingleLinkedList(): m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CSingleLinkedList()
        {
            clear();
        }

        void pushBack(T& data)
        {
            leaf* pNewEnd = new leaf(data, 0);
            if (m_pEnd != 0)
            {
                m_pEnd->pnext = pNewEnd;
                m_pEnd = pNewEnd;
            }
            else
            {
              m_pEnd = pNewEnd;
              m_pBegin = m_pEnd;
            }
        }

        void pushFront(T& data)
        {
            leaf* pNewBegin = new leaf(data, m_pBegin);
            pNewBegin->pnext = m_pBegin;
            m_pBegin = pNewBegin;
            if (m_pEnd == 0)
            {
                m_pEnd = m_pBegin;
            }
        }

        T popFront()
        {
            if (m_pBegin == 0)
            {
                throw std::runtime_error("List is empty!");
            }
            T tmp = std::move(m_pBegin->data);
            leaf* new_begin = m_pBegin->pnext;
            delete m_pBegin;
            m_pBegin = new_begin;

            if (m_pBegin == 0)
            {
                m_pEnd = 0;
            }

            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
          leaf *pCurrent = m_pBegin;
          leaf *pPrev = 0;

          // todo: обработать случай, когда it не принадлежит списку
          while (pCurrent != it.getLeaf())
          {
              pPrev = pCurrent;
              pCurrent = pCurrent->pnext;
          }

          if (pCurrent == m_pBegin)
          {
              leaf* new_begin = m_pBegin->pnext;
              delete m_pBegin;
              m_pBegin = new_begin;
              if (m_pBegin == 0)
              {
                  m_pEnd = 0;
              }
          }
          else if (pCurrent == m_pEnd)
          {
              delete m_pEnd;
              m_pEnd = pPrev;
              if (m_pEnd != 0)
              {
                  m_pEnd->pnext = 0;
              }
              else
              {
                  m_pBegin = 0;
              }
          }
          else
          {
              pPrev->pnext = pCurrent->pnext;
              delete pCurrent;
          }

          if (pPrev != 0)
          {
              it.setLeaf(pPrev);
          }
          else
          {
              it.setLeafPreBegin(m_pBegin);
          }
        }

        int getSize()
        {
            size_t size = 0;
            for (leaf* pCurrent = m_pBegin; pCurrent != 0;
                 pCurrent = pCurrent->pnext)
            {
                ++size;
            }
            return size;
        }

        void clear()
        {
          leaf* pCurrent = m_pBegin;
          while (pCurrent != 0)
          {
              leaf* pNext = pCurrent->pnext;
              delete pCurrent;
              pCurrent = pNext;
          }
          m_pBegin = 0;
          m_pEnd = 0;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext): pnext(_pnext), pprev(_pprev), data(_data)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator(): m_pCurrent(0), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(leaf *p): m_pCurrent(p), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(const CIterator &src)
            {
                m_pCurrent = src.m_pCurrent;
                m_pBegin = src.m_pBegin;
                m_pEnd = src.m_pEnd;
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pCurrent = src.m_pCurrent;
                m_pBegin = src.m_pBegin;
                m_pEnd = src.m_pEnd;
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return (it.m_pCurrent == this->m_pCurrent) &&
                       (it.m_pBegin == this->m_pBegin) &&
                       (it.m_pEnd == this->m_pEnd);
            }

            void operator++()
            {
                if (m_pBegin != 0)
                {
                    m_pCurrent = m_pBegin;
                    m_pBegin = 0;
                }
                else
                {
                    if (isValid())
                    {
                        m_pCurrent = m_pCurrent->pnext;
                    }
                }
            }

            void operator--()
            {
                if (m_pEnd != 0)
                {
                    m_pCurrent = m_pEnd;
                    m_pEnd = 0;
                }
                else
                {
                    if (isValid())
                    {
                        m_pCurrent = m_pCurrent->pprev;
                    }
                }
            }

            T& getData()
            {
                if (!isValid())
                {
                    throwError();
                }
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return getData();
            }

            leaf *getLeaf()
            {
                if (!isValid())
                {
                    throwError();
                }
                return m_pCurrent;
            }

            void setLeaf(leaf *p)
            {
                m_pCurrent = p;
                m_pBegin = 0;
                m_pEnd = 0;
            }

            void setLeafPreBegin(leaf *p)
            {
                m_pCurrent = 0;
                m_pBegin = p;
                m_pEnd = 0;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pCurrent = 0;
                m_pBegin = 0;
                m_pEnd = p;
            }

            bool isValid()
            {
                return m_pCurrent != 0;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;

            void throwError()
            {
                throw std::runtime_error("Iterator is not valid!");
            }
        };

    public:

        CDualLinkedList(): m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CDualLinkedList()
        {
            clear();
        }

        void pushBack(T &data)
        {
            leaf *pNewEnd = new leaf(data, 0, 0);
            link(m_pEnd, pNewEnd);
            updateEnd(pNewEnd);
        }

        void pushFront(T &data)
        {
            leaf *pNewBegin = new leaf(data, 0, 0);
            link(pNewBegin, m_pBegin);
            updateBegin(pNewBegin);
        }

        T popFront()
        {
            if (m_pBegin == 0)
            {
              throw std::runtime_error("List is empty!");
            }
            T tmp = std::move(m_pBegin->data);

            leaf *newBegin = m_pBegin->pnext;
            unlink(m_pBegin);

            delete m_pBegin;
            updateBegin(newBegin);

            return tmp;
        }

        T popBack()
        {
            if (m_pEnd == 0)
            {
                throw std::runtime_error("List is empty!");
            }
            T tmp = std::move(m_pEnd->data);

            leaf *newEnd = m_pEnd->pprev;
            unlink(m_pEnd);

            delete m_pEnd;
            updateEnd(newEnd);

            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator &it)
        {
            leaf* pCurrent = it.getLeaf();

            bool isBegin = false;
            if (pCurrent == m_pBegin)
            {
                isBegin = true;
                leaf *newBegin = m_pBegin->pnext;
                updateBegin(newBegin);
            }
            else if (pCurrent == m_pEnd)
            {
                leaf *newEnd = m_pEnd->pprev;
                updateEnd(newEnd);
            }

            if (!isBegin)
            {
                it.setLeaf(pCurrent->pprev);
            }
            else
            {
                it.setLeafPreBegin(m_pBegin);
            }

            unlink(pCurrent);
            delete pCurrent;
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator &it)
        {
            leaf *pCurrent = it.getLeaf();

            bool isEnd = false;
            if (pCurrent == m_pEnd)
            {
                isEnd = true;
                leaf *newEnd = m_pEnd->pprev;
                updateEnd(newEnd);
            }
            else if (pCurrent == m_pBegin)
            {
                leaf *newBegin = m_pBegin->pnext;
                updateBegin(newBegin);
            }

            if (!isEnd)
            {
                it.setLeaf(pCurrent->pnext);
            }
            else
            {
                it.setLeafPostEnd(m_pEnd);
            }

            unlink(pCurrent);
            delete pCurrent;
        }

        int getSize()
        {
            size_t size = 0;
            for (leaf *pCurrent = m_pBegin; pCurrent != 0;
                 pCurrent = pCurrent->pnext)
            {
                ++size;
            }
            return size;
        }

        void clear()
        {
            leaf *pCurrent = m_pBegin;
            while (pCurrent != 0)
            {
                leaf *pNext = pCurrent->pnext;
                delete pCurrent;
                pCurrent = pNext;
            }
            m_pBegin = 0;
            m_pEnd = 0;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

        CIterator end()
        {
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin, *m_pEnd;

        void link(leaf* leaf_1, leaf* leaf_2)
        {
            if (leaf_1 != 0)
            {
                leaf_1->pnext = leaf_2;
            }
            if (leaf_2 != 0)
            {
                leaf_2->pprev = leaf_1;
            }
        }

        void unlink(leaf* leaf)
        {
            if (leaf == 0)
            {
                return;
            }

            link(leaf->pprev, leaf->pnext);
        }

        void updateEnd(leaf* newEnd)
        {
            m_pEnd = newEnd;
            if ((m_pBegin == 0) || (m_pEnd == 0))
            {
                m_pBegin = m_pEnd;
            }
        }

        void updateBegin(leaf *newBegin)
        {
            m_pBegin = newBegin;
            if ((m_pEnd == 0) || (m_pBegin == 0))
            {
                m_pEnd = m_pBegin;
            }
        }
    };
}
#endif //#ifndef TEMPLATES_LIST_2021_02_11
