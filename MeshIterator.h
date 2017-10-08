#ifndef MESHITERATOR_H
#define MESHITERATOR_H
/*Edge and Face Iterators*/

#include <iostream>

template <class T>
class MeshIterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
	friend class Mesh;
private:
	typename list<T*>::iterator it;
	MeshIterator(typename list<T*>::iterator it) : it(it) {}
public:
	// Операции, необходимые для всех категорий итераторов.
	MeshIterator() = default;
	MeshIterator(const MeshIterator&) = default;
	MeshIterator& operator=(const MeshIterator&) = default;
	~MeshIterator() = default;
	reference operator*() const { return *(*it); }
	MeshIterator& operator++() { it++; return *this; }
	MeshIterator operator++(int) { auto old = *this; ++(*this); return old; }

	// Операции, необходимые для InputIterator.
	pointer operator->() const { return *it; }

	// Операции, необходимые для BidirectionalIterator.
	MeshIterator& operator--() { it--; return *this; }
	MeshIterator operator--(int) { auto old = *this; --(*this); return old; }

	// Операции, необходимые для всех категорий итераторов.
	void swap(MeshIterator& a, MeshIterator& b) { std::swap(a.it, b.it); }

	// Операции, необходимые для InputIterator.
	bool operator==(const MeshIterator& other) { return it == other.it; }
	bool operator!=(const MeshIterator& other) { return !(*this == other); }
};

#endif // MESHITERATOR_H