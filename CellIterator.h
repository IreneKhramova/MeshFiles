#ifndef CELLITERATOR_H
#define CELLITERATOR_H

#include <iostream>

class CellIterator : public std::iterator<std::random_access_iterator_tag, Cell>
{
	friend class Mesh;
private:
	vector<Cell*>::iterator it;
	CellIterator(vector<Cell*>::iterator it) : it(it) {}
public:
	// ��������, ����������� ��� ���� ��������� ����������.
	CellIterator() = default;
	CellIterator(const CellIterator&) = default;
	CellIterator& operator=(const CellIterator&) = default;
	~CellIterator() = default;
	reference operator*() const { return *(*it); }
	CellIterator& operator++() { it++; return *this; }
	CellIterator operator++(int) { auto old = *this; ++(*this); return old; }

	// ��������, ����������� ��� InputIterator.
	pointer operator->() const { return *it; }

	// ��������, ����������� ��� BidirectionalIterator.
	CellIterator& operator--() { it--; return *this; }
	CellIterator operator--(int) { auto old = *this; --(*this); return old; }

	// ��������, ����������� ��� RandomAccessIterator.
	CellIterator& operator+=(difference_type n) { it += n; return *this; }
	CellIterator& operator-=(difference_type n) { return *this += -n; }
	reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }

	// ��������, ����������� ��� ���� ��������� ����������.
	void swap(CellIterator& a, CellIterator& b) { std::swap(a.it, b.it); }

	// ��������, ����������� ��� InputIterator.
	bool operator==(const CellIterator& other) { return (it == other.it); }
	bool operator!=(const CellIterator& other) { return !(*this == other); }

	// ��������, ����������� ��� RandomAccessIterator.
	bool operator<(const CellIterator& other) { return (it - other.it) < 0; }
	bool operator>(const CellIterator& other) { return (it - other.it) > 0; }
	bool operator<=(const CellIterator& other) { return !(*this > other); }
	bool operator>=(const CellIterator& other) { return !(*this < other); }
	CellIterator operator+(difference_type n) { return CellIterator(*this) += n; }
	friend CellIterator operator+(difference_type n, CellIterator it);
	CellIterator operator-(difference_type n) { return CellIterator(*this) -= n; }
	difference_type operator-(const CellIterator& other) { return it - other.it; }
};

CellIterator operator+(CellIterator::difference_type n, CellIterator it) { return it + n; }

#endif // CELLITERATOR_H