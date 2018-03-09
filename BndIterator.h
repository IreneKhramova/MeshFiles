#ifndef BNDITERATOR_H
#define BNDITERATOR_H

#include <iostream>

template <class T>
class BndIterator : public std::iterator<std::forward_iterator_tag, T>
{
    using base = std::iterator<std::forward_iterator_tag, T>;
    using typename base::pointer;
    using typename base::reference;

    friend class Mesh;
private:
    typename vector<T*>::iterator itT;
    typename vector<T*>::iterator itTEnd;
    map<std::string, vector<T*>> *m;
    vector<string>::iterator itS;
    vector<string>::iterator itSEnd;

    BndIterator(map<std::string, vector<T*>> *mBnd, vector<string>::iterator s, vector<string>::iterator sEnd, typename vector<T*>::iterator t)
    {
        m = mBnd;
        itS = s;
        itSEnd = sEnd;
        itT = t;

        if (itS != itSEnd)
        {
            itTEnd = (*m)[*itS].end();
        }
    }
public:
    BndIterator() = default;
    BndIterator(const BndIterator&) = default;
    BndIterator& operator=(const BndIterator&) = default;
    ~BndIterator() = default;
    reference operator*() const { return *(*itT); }

    BndIterator& operator++()
    {
        ++itT;
        if (itT == itTEnd)
        {
            ++itS;
            if (itS != itSEnd)
            {
                itT = (*m)[*itS].begin();
                itTEnd = (*m)[*itS].end();
            }
        }
        return *this;
    }

    BndIterator operator++(int) { auto old = *this; ++(*this); return old; }

    pointer operator->() const { return *itT; }

    bool operator==(const BndIterator& other)
    {
        if(itS == other.itS)
            return (itT == other.itT);
        return false;
    }
    bool operator!=(const BndIterator& other) { return !(*this == other); }
};

#endif // BNDITERATOR_H