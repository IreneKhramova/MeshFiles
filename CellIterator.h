#ifndef CELLITERATOR_H
#define CELLITERATOR_H

#include <iostream>

class CellIterator : public std::iterator<std::random_access_iterator_tag, Cell>
{
	friend class Mesh;
private:
	vector<Cell*> *t;
	int i;
	CellIterator(vector<Cell*> *t, int i) : t(t), i(i) {}
public:
	// Операции, необходимые для всех категорий итераторов.
	CellIterator() = default;
	CellIterator(const CellIterator&) = default;
	CellIterator& operator=(const CellIterator&) = default;
	~CellIterator() = default;
	reference operator*() const { return *(*t).at(i); }
	CellIterator& operator++() { i++; return *this; }
	CellIterator operator++(int) { auto old = *this; ++(*this); return old; }

	// Операции, необходимые для InputIterator.
	pointer operator->() const { return (*t).at(i); }

	// Операции, необходимые для BidirectionalIterator.
	CellIterator& operator--() { i--; return *this; }
	CellIterator operator--(int) { auto old = *this; --(*this); return old; }

	// Операции, необходимые для RandomAccessIterator.
	CellIterator& operator+=(difference_type n) { i += n; return *this; }
	CellIterator& operator-=(difference_type n) { return *this += -n; }
	reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }

	// Операции, необходимые для всех категорий итераторов.
	void swap(CellIterator& a, CellIterator& b) { std::swap(a.t, b.t); std::swap(a.i, b.i); }

	// Операции, необходимые для InputIterator.
	bool operator==(const CellIterator& other) { return ((t == other.t) && (i == other.i)); }
	bool operator!=(const CellIterator& other) { return !(*this == other); }

	// Операции, необходимые для RandomAccessIterator.
	bool operator<(const CellIterator& other) { return (i - other.i) < 0; } //t == other.t ?
	bool operator>(const CellIterator& other) { return (i - other.i) > 0; } //t == other.t ?
	bool operator<=(const CellIterator& other) { return !(*this > other); }
	bool operator>=(const CellIterator& other) { return !(*this < other); }
	CellIterator operator+(difference_type n) { return CellIterator(*this) += n; }
	friend CellIterator operator+(difference_type n, CellIterator it);
	CellIterator operator-(difference_type n) { return CellIterator(*this) -= n; }
	difference_type operator-(const CellIterator& other) { return i - other.i; }
};

CellIterator operator+(CellIterator::difference_type n, CellIterator it) { return it + n; }

#endif // CELLITERATOR_H