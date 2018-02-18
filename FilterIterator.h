#ifndef FILTERITERATOR_H
#define FILTERITERATOR_H

template <class Predicate, class Iterator>
class FilterIterator
{
public:
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::reference reference;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;

	FilterIterator() = default;
	FilterIterator(const FilterIterator&) = default;
	FilterIterator(Predicate f, Iterator begin, Iterator end, Iterator it) : m_pred(f), m_iter(it), m_end(end), m_begin(begin)
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
		return m_iter;
	}

	FilterIterator& operator--()
	{
		--m_iter;
		satisfyPredicateDecrement();
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
		while (m_iter != m_end && !m_pred(&(*m_iter)))
		{
			++m_iter;
		}
	}
	void satisfyPredicateDecrement()
	{
		while (m_iter != m_begin && !m_pred(&(*m_iter)))
		{
			--m_iter;
		}
	}
};

#endif // FILTERITERATOR_H