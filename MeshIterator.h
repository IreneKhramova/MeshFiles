#ifndef MESHITERATOR_H
#define MESHITERATOR_H

#include <iostream>

class Cell;

template <class T>
class MeshIterator : public std::iterator<std::random_access_iterator_tag, T /* ��� �������� */>
{
	friend class Cell;
private:
	T* t;
	MeshIterator(T* t) : t(t) {}
public:
	// ��������, ����������� ��� ���� ��������� ����������.
	MeshIterator() = default;
	MeshIterator(const MeshIterator&) = default;
	MeshIterator& operator=(const MeshIterator&) = default;
	~MeshIterator() = default;
	reference operator*() const { return *t; }
	MeshIterator& operator++() { t++; return *this; }
	iterator operator++(int) { auto old = *this; ++(*this); return old; }

	// ��������, ����������� ��� InputIterator.
	pointer operator->() const { return t; }

	// ��������, ����������� ��� BidirectionalIterator.
	MeshIterator& operator--() { t--; return *this; }
	MeshIterator operator--(int) { auto old = *this; --(*this); return old; }

	// ��������, ����������� ��� RandomAccessIterator.
	reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }
	MeshIterator& operator+=(difference_type n) { t += n; return *this; }
	MeshIterator& operator-=(difference_type n) { return *this += -n; }

	// ��������, ����������� ��� ���� ��������� ����������.
	void swap(MeshIterator& a, MeshIterator& b) { std::swap(a.t, b.t); }

	// ��������, ����������� ��� InputIterator.
	bool operator==(const MeshIterator& other) { return t == other.t; }
	bool operator!=(const MeshIterator& other) { return t != other.t; }

	// ��������, ����������� ��� RandomAccessIterator.
	bool operator<(const MeshIterator& other) { return (t - other.t) < 0; }
	bool operator>(const MeshIterator& other) { return (t - other.t) > 0; }
	bool operator<=(const MeshIterator& other) { return !(*this > other); }
	bool operator>=(const MeshIterator& other) { return !(*this < other); }
	MeshIterator operator+(difference_type n) { return MeshIterator(*this) += n; }
	friend MeshIterator operator+(typename difference_type n, MeshIterator it);
	MeshIterator operator-(difference_type n) { return MeshIterator(*this) -= n; }
	difference_type operator-(const MeshIterator& other) { return t - other.t; }
};

template <class T>
MeshIterator<T> operator+(typename MeshIterator<T>::difference_type n, MeshIterator<T> it) { return it + n; }

#endif // MESHITERATOR_H