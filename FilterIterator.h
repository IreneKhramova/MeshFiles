#ifndef FILTERITERATOR_H
#define FILTERITERATOR_H

template <class Predicate, class Iterator, class T>
class FilterIterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
    using base = std::iterator<std::bidirectional_iterator_tag, T>;
    using typename base::pointer;
    using typename base::reference;

public:
    FilterIterator() = default;
    FilterIterator(const FilterIterator&) = default;
    FilterIterator(Predicate f, Iterator begin, Iterator end, Iterator it) : m_pred(f), m_begin(begin), m_end(end), m_iter(it)
    {
        satisfyPredicateIncrement();
    }
    FilterIterator& operator=(const FilterIterator&) = default;
    ~FilterIterator() = default;
    reference operator*() const { return *m_iter; }
    FilterIterator& operator++()
    {
        ++m_iter;
        satisfyPredicateIncrement();
        return *this;
    }
    FilterIterator operator++(int) { auto old = *this; ++(*this); return old; }
    pointer operator->() const
    {
        return &(*m_iter);
    }

    /* This is so slow */
    FilterIterator& operator--()
    {
        for (--m_iter; m_iter != m_begin; --m_iter)
        {
            if (m_pred(&(*m_iter)))
                break;
        }
        return *this;
    }
    FilterIterator operator--(int) { auto old = *this; --(*this); return old; }

    bool operator==(const FilterIterator& other) { return m_iter == other.m_iter; }
    bool operator!=(const FilterIterator& other) { return !(*this == other); }

private:
    Predicate m_pred;
    Iterator m_iter;
    Iterator m_end;
    Iterator m_begin;

    void satisfyPredicateIncrement()
    {
        for ( ; m_iter != m_end; ++m_iter)
        {
            if (m_pred(&(*m_iter)))
                break;
        }
    }
};

#endif // FILTERITERATOR_H