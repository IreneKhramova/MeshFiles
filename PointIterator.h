#ifndef POINTITERATOR_H
#define POINTITERATOR_H

#include <iostream>

class PointIterator : public std::iterator<std::random_access_iterator_tag, Point>
{
	friend class Mesh;
private:
	Point* t;
	PointIterator(Point* t) : t(t) {}
public:
	// ��������, ����������� ��� ���� ��������� ����������.
	PointIterator() = default;
	PointIterator(const PointIterator&) = default;
	PointIterator& operator=(const PointIterator&) = default;
	~PointIterator() = default;
	reference operator*() const { return *t; }
	PointIterator& operator++() { t++; return *this; }
	PointIterator operator++(int) { auto old = *this; ++(*this); return old; }

	// ��������, ����������� ��� InputIterator.
	pointer operator->() const { return t; }

	// ��������, ����������� ��� BidirectionalIterator.
	PointIterator& operator--() { t--; return *this; }
	PointIterator operator--(int) { auto old = *this; --(*this); return old; }

	// ��������, ����������� ��� RandomAccessIterator.
	reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }
	PointIterator& operator+=(difference_type n) { t += n; return *this; }
	PointIterator& operator-=(difference_type n) { return *this += -n; }

	// ��������, ����������� ��� ���� ��������� ����������.
	void swap(PointIterator& a, PointIterator& b) { std::swap(a.t, b.t); }

	// ��������, ����������� ��� InputIterator.
	bool operator==(const PointIterator& other) { return t == other.t; }
	bool operator!=(const PointIterator& other) { return !(*this == other); }

	// ��������, ����������� ��� RandomAccessIterator.
	bool operator<(const PointIterator& other) { return (t - other.t) < 0; }
	bool operator>(const PointIterator& other) { return (t - other.t) > 0; }
	bool operator<=(const PointIterator& other) { return !(*this > other); }
	bool operator>=(const PointIterator& other) { return !(*this < other); }
	PointIterator operator+(difference_type n) { return PointIterator(*this) += n; }
	friend PointIterator operator+(difference_type n, PointIterator it);
	PointIterator operator-(difference_type n) { return PointIterator(*this) -= n; }
	difference_type operator-(const PointIterator& other) { return t - other.t; }
};

PointIterator operator+(PointIterator::difference_type n, PointIterator it) { return it + n; }

#endif //POINTITERATOR_H