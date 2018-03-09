#ifndef MESHITERATOR_H
#define MESHITERATOR_H
/*Edge, Face and Cell Iterators*/

#include <iostream>

template <class T>
class MeshIterator : public std::iterator<std::random_access_iterator_tag, T>
{
    using base = std::iterator<std::random_access_iterator_tag, T>;
    using typename base::pointer;
    using typename base::reference;
    using typename base::difference_type;

    friend class Mesh;
private:
    typename vector<T*>::iterator it;
    MeshIterator(typename vector<T*>::iterator it) : it(it) {}
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

    // Операции, необходимые для RandomAccessIterator.
    MeshIterator& operator+=(difference_type n) { it += n; return *this; }
    MeshIterator& operator-=(difference_type n) { return *this += -n; }
    reference operator[](difference_type n) const { auto tmp = *this; tmp += n; return *tmp; }

    // Операции, необходимые для InputIterator.
    bool operator==(const MeshIterator& other) { return it == other.it; }
    bool operator!=(const MeshIterator& other) { return !(*this == other); }

    // Операции, необходимые для RandomAccessIterator.
    bool operator<(const MeshIterator& other) { return (it - other.it) < 0; }
    bool operator>(const MeshIterator& other) { return (it - other.it) > 0; }
    bool operator<=(const MeshIterator& other) { return !(*this > other); }
    bool operator>=(const MeshIterator& other) { return !(*this < other); }
    MeshIterator operator+(difference_type n) { return MeshIterator(*this) += n; }
    friend MeshIterator operator+(difference_type n, MeshIterator it);
    MeshIterator operator-(difference_type n) { return MeshIterator(*this) -= n; }
    difference_type operator-(const MeshIterator& other) { return it - other.it; }
};

template <class T>
MeshIterator<T> operator+(typename MeshIterator<T>::difference_type n, MeshIterator<T> it) { return it + n; }

#endif // MESHITERATOR_H