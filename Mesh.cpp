#include <iostream>
#include <cmath>

using namespace std;

struct Point
{
	double x;
	double y;
	double z;

	double getX()
	{
		return x;
	}
};


///////////////////////
class Edge
{
private:
	Point* p[2];

public:
	void initEdge(Point* p1, Point* p2)
	{
		p[0] = p1;
		p[1] = p2;
	}

	double getlength()
	{
		return sqrt(pow(p[0]->x - p[1]->x, 2) + pow(p[0]->y - p[1]->y, 2) + pow(p[0]->z - p[1]->z, 2));
	}

	Point* getPoint(int i)
	{
		return p[i];
	}

	friend class Cell;
};


////////////////////////
class Face
{
private:
	Edge* e[4];
public:
	void initFace(Edge* e1, Edge* e2, Edge* e3, Edge* e4)
	{
		e[0] = e1;
		e[1] = e2;
		e[2] = e3;
		e[3] = e4;
	}

	Edge* getEdge(int i)
	{
		return e[i];
	}


	friend class Cell;
};

class Cell;

template <class T>
class MeshIterator : public std::iterator<std::random_access_iterator_tag, T /* Тип элемента */>
{
	friend class Cell;
private:
	T* t;
	//передаем из cell массив t-шек
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

///////////////////
class Cell
{
private:
	Edge* e;
	Face* f;
	Point* p;

public:
	Cell(Point* p)
	{
		this->p = p;
		e = new Edge[12];
		f = new Face[6];

		e[0].initEdge(&p[0], &p[1]);
		e[1].initEdge(&p[1], &p[2]);
		e[2].initEdge(&p[2], &p[3]);
		e[3].initEdge(&p[3], &p[0]);
		e[4].initEdge(&p[4], &p[5]);
		e[5].initEdge(&p[5], &p[6]);
		e[6].initEdge(&p[6], &p[7]);
		e[7].initEdge(&p[7], &p[4]);
		e[8].initEdge(&p[0], &p[4]);
		e[9].initEdge(&p[1], &p[5]);
		e[10].initEdge(&p[2], &p[6]);
		e[11].initEdge(&p[3], &p[7]);

		f[0].initFace(&e[0], &e[9], &e[4], &e[8]);
		f[1].initFace(&e[1], &e[10], &e[5], &e[9]);
		f[2].initFace(&e[2], &e[11], &e[6], &e[10]);
		f[3].initFace(&e[3], &e[8], &e[7], &e[11]);
		f[4].initFace(&e[0], &e[1], &e[2], &e[3]);
		f[5].initFace(&e[4], &e[5], &e[6], &e[7]);
	}

	typedef MeshIterator<Face> FaceIterator;
	typedef MeshIterator<Edge> EdgeIterator;
	typedef MeshIterator<Point> PointIterator;

	FaceIterator beginFace() { return FaceIterator(f); }
	FaceIterator endFace() { return FaceIterator(&f[6]); }

	EdgeIterator beginEdge() { return EdgeIterator(e); }
	EdgeIterator endEdge() { return EdgeIterator(&e[12]); }

	PointIterator beginPoint() { return PointIterator(p); }
	PointIterator endPoint() { return PointIterator(&p[9]); }

	~Cell()
	{
		delete[] e;
		delete[] f;
	}
};

int main()
{
	Point p[8];
	p[0].x = 0; p[0].y = 0; p[0].z = 0;
	p[1].x = 1/*0*/; p[1].y = 1; p[1].z = 0;
	p[2].x = 2/*1*/; p[2].y = 1; p[2].z = 0;
	p[3].x = 3/*1*/; p[3].y = 0; p[3].z = 0;
	p[4].x = 4/*0*/; p[4].y = 0; p[4].z = 1;
	p[5].x = 5/*0*/; p[5].y = 1; p[5].z = 1;
	p[6].x = 6/*1*/; p[6].y = 1; p[6].z = 1;
	p[7].x = 7/*1*/; p[7].y = 0; p[7].z = 1;
	//Единичный куб(координаты нужно вводить в определенном порядке)
	Cell cell(p);


	for (Cell::FaceIterator it = cell.beginFace(); it != cell.endFace(); it++)
		cout << (*it).getEdge(0)->getPoint(0)->getX() << " ";

	system("pause");
	return 0;
}