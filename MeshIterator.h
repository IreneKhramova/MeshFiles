#ifndef MESHITERATOR_H
#define MESHITERATOR_H

#include <iostream>

class Cell;

template <class T>
class MeshIterator : public std::iterator<std::random_access_iterator_tag, T /* Тип элемента */>
{
	friend class Cell;
private:
	T* t;
	MeshIterator(T* t) : t(t) {}
public:
	// Операции, необходимые для всех категорий итераторов.
	MeshIterator() = default;
	MeshIterator(const MeshIterator&) = default;
	MeshIterator& operator=(const MeshIterator&) = default;
	~MeshIterator() = default;
	reference operator*() const { return *t; }
	MeshIterator& operator++() { t++; return *this; }
	iterator operator++(int) { auto old = *this; ++(*this); return old; }

	// Операции, необходимые для InputIterator.
	pointer operator->() const { return t; }

	// Операции, необходимые для BidirectionalIterator.
	MeshIterator& operator--() { t--; return *this; }
	MeshIterator operator--(int) { auto old = *this; --(*this); return old; }

	// Операции, необходимые для RandomAccessIterator.
	reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }
	MeshIterator& operator+=(difference_type n) { t += n; return *this; }
	MeshIterator& operator-=(difference_type n) { return *this += -n; }

	// Операции, необходимые для всех категорий итераторов.
	void swap(MeshIterator& a, MeshIterator& b) { std::swap(a.t, b.t); }

	// Операции, необходимые для InputIterator.
	bool operator==(const MeshIterator& other) { return t == other.t; }
	bool operator!=(const MeshIterator& other) { return t != other.t; }

	// Операции, необходимые для RandomAccessIterator.
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